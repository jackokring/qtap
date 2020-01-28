#include "calculus.h"
#include <math.h>
#include "minblep.cpp"

//WARNING WILL ROBINSON may contain traces of GB 1905339.6 Pat. Pending.

Calculus::Calculus(double sampleStep) {
    h = sampleStep;
}

void Calculus::differential(double *input, double *output) {
    //coefficients via http://web.media.mit.edu/~crtaylor/calculator.html
    //all done from prospective of sample input[0]
    double t = h;
    output[0] = input[0];
    double co1[] = { 105, -960, 3920, -9408, 14700, -15680, 11760, -6720, 2283 };
    output[1] = sum(co1, input - 8, input) / (840 * t);
    t *= h;
    double co2[] = { 3267, -29664, 120008, -284256, 435330, -448672, 312984, -138528, 29531 };
    output[2] = sum(co2, input - 8, input) / (5040 * t);
    t *= h;
    double co3[] = { 469, -4216, 16830, -39128, 58280, -57384, 36706, -13960, 2403 };
    output[3] = sum(co3, input - 8, input) / (240 * t);
    t *= h;
    double co4[] = { 967, -8576, 33636, -76352, 109930, -102912, 61156, -21056, 3207 };
    output[4] = sum(co4, input - 8, input) / (240 * t);
    t *= h;
    double co5[] = { 35, -305, 1170, -2581, 3580, -3195, 1790, -575, 81 };
    output[5] = sum(co5, input - 8, input) / (6 * t);
    t *= h;
    double co6[] = { 23, -196, 732, -1564, 2090, -1788, 956, -292, 39 };
    output[6] = sum(co6, input - 8, input) / (4 * t);
    t *= h;
    double co7[] = { 7, -58, 210, -434, 560, -462, 238, -70, 9 };
    output[7] = sum(co7, input - 8, input) / (2 * t);
    t *= h;
    double co8[] = { 1, -8, 28, -56, 70, -56, 28, -8, 1 };
    output[8] = sum(co8, input - 8, input) / (1 * t);
}

double Calculus::sum(double *coeff, double *inputBegin, double *inputEnd, int step) {
    volatile double residual = 0.0;
    double add = 0.0;
    double temp;
    for(; inputBegin <= inputEnd; inputBegin += step) {
        temp = (*(coeff++)) * (*inputBegin);
        double test = add + (temp + residual);
        residual = (temp + residual) - (test - add);
        add = test;
    }
    return add;
}

void Calculus::atTick(uint64_t now) {
    tick = now;
}

void Calculus::setSigma(double sigmaValue) {
    sigma = sigmaValue;
}

void Calculus::next() {
    tick++;
}

void Calculus::expDecay(double *inputBegin, double *inputEnd, double *output,
                        int step, bool splitDistribute) {//from now
    uint64_t tt = tick - (inputEnd - inputBegin) / step;//work out sampled start time
    for(; inputBegin <= inputEnd; inputBegin += step) {
        (*(output++)) = (*inputBegin) * expm1(-(sigma * h * tt)) +
                (splitDistribute ? 0.0 :(*inputBegin));
        tt += 1;
    }
}

void Calculus::cumSum(double *inputBegin, double *inputEnd, double *output, int step) {
    volatile double residual = 0.0;
    double add = 0.0;
    double temp;
    for(; inputBegin <= inputEnd; inputBegin += step) {
        temp = (*inputBegin);
        double test = add + (temp + residual);
        residual = (temp + residual) - (test - add);
        (*(output++)) = test;
    }
}

/*===================================================================
 * THREE BELOW ACCELERATIONS WORK WITH CUMULATIVE SUMS OF SERIES
 * ================================================================*/
bool Calculus::seriesAccel(double *inputBegin, double *inputEnd,
                           double *output, int step, bool outsToo) {
    if(inputBegin == inputEnd) {
        *output = *inputBegin;
        return true;//convergence extra not possible
    }
    inputBegin += step;
    inputEnd -= step;
    double temp;
    double nm1;
    double np1;
    double temp2;
    if(inputBegin > inputEnd) {
        *output = (*(inputBegin - 1) + *inputBegin) / 2.0;
        return true;//convergence extra not possible
    }
    bool cov = (inputBegin == inputEnd);
    for(; inputBegin <= inputEnd; inputBegin += step) {
        //Shank's method
        nm1 = *(inputBegin - step);
        np1 = *(inputBegin + step);
        temp = temp2 = (np1 - *inputBegin);
        temp *= temp;
        temp2 -= (*inputBegin - nm1);
        if(temp2 == 0.0) {
            temp = 0.0;//pass through as no delta
        } else {
            temp /= temp2;
        }
        *output = np1 - temp;
        output += outsToo ? step : 1;//step outs for replacements
    }
    return cov;
}

double Calculus::seriesAccelLim(double *inputBegin, double *inputEnd, int step) {
    while(!seriesAccel(inputBegin, inputEnd, inputBegin, step, true)) {//overwrite
        inputEnd -= 2;//two off end
    }
    return *inputBegin;
}

//for an application test on sets of convergents as a series which has same limit?
double Calculus::seriesAccelLim2(double *inputBegin, double *inputEnd,
                                 int step, uint nest) {
    if(nest > 1) {
        seriesAccelLim(inputEnd, inputBegin, step *= -2);
        return seriesAccelLim2(inputEnd, inputBegin, step, --nest);
    }
    return seriesAccelLim(inputEnd, inputBegin, step *= -2);
}

void Calculus::preMul(double *coeff, double *inputBegin, double *inputEnd, double *output, int step) {
    for(; inputBegin <= inputEnd; inputBegin += step) {
        (*(output++)) = *inputBegin * *(coeff++);//pre multiply by coeeficients
    }
}

Blep::Blep(uint zeros, uint oversample) {
    scales = GenerateMinBLEP(zeros, oversample);
    max = (zeros * 2 * oversample) + 1;
    array = new double[max]{};
    residual = new double[max]{};
}

Blep::~Blep() {
    delete [] residual;
    delete [] array;
}

double Blep::out(uint sampleInc) {//allows undersampling
    double val = array[index];
    array[index] = 0.0;//reset
    index += sampleInc;
    index %= max;//limit
    return val;
}

void Blep::in(double value) {
    value += residual[indexw];
    for(uint i = 0; i < max - 1 /* the left */; ++i) {
        array[(i + indexw) % max] += value * scales[i];
    }
    residual[(15 + indexw) % max] = value * scales[max - 1];//last
    ++indexw;
    indexw %= max;
}
