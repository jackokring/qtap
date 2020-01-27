#include "calculus.h"
#include <math.h>

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
    uint64_t tt = tick;
    for(; inputBegin <= inputEnd; inputBegin += step) {
        (*(output++)) = (*inputBegin) * expm1(-(sigma * h * tt)) +
                (splitDistribute ? 0.0 :(*inputBegin));
        tt += 1;
    }
}

void cumSum(double *inputBegin, double *inputEnd, double *output, int step = 1) {
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
