#ifndef CALCULUS_H
#define CALCULUS_H

#include <QtCore>
#include "../kringlicence.h"

double entropic(double x);

class Calculus {
public:
    Calculus(double sampleStep = 1.0);
    /*============================================================
     * SIGNAL PROCESSING
     * =========================================================*/
    //9 point differential uses input[-8] to input[0]
    //outputs to output[0] to output[8]
    //so in effect can use a ring buffers
    void differential(double *input, double *output);
    //the 0th differential one step in the future from input[-8] to input[0]
    //can then store it in input[1] and ...
    double future(double *input);
    //a sum using the residual to gain higher precision
    static double sum(double *coeff, double *inputBegin, double *inputEnd, int step = 1);
    //setting some important timing factors
    void atTick(uint64_t now);
    void setSigma(double sigmaValue);
    void next();
    //apply an exponetial decay of -sigma*t to the input history
    //splitDistribute uses expm1 for x.expm1 + x = x.exp
    void expDecay(double *inputBegin, double *inputEnd, double *output,
                  int step = 1, bool splitDistribute = false);
    //a cumulative sum for series acceleration input
    static void cumSum(double *inputBegin, double *inputEnd, double *output, int step = 1);
    //some series acceleration routines
    static bool seriesAccel(double *inputBegin, double *inputEnd,
                            double *output, int step = 1, bool outsToo = false);
    static double seriesAccelLim(double *inputBegin, double *inputEnd, int step = 1);
    static double seriesAccelLim2(double *inputBegin, double *inputEnd,
                                  int step = 1, uint nest = 1);
    //premultiplication by a coefficient set
    static void preMul(double *coeff, double *inputBegin, double *inputEnd, double *output, int step = 1);
    //map a simple function over an input (with step stride)
    static void map(double fn(double), double *inputBegin, double *inputEnd, double *output, int step = 1);
    //entropy self information for example, and the a sum gives total entropy
    static void entropy(double *inputBegin, double *inputEnd, double *output, int step = 1);
    //integral pre-multiply input[0] to input[8] GB 1905339.6 Pat. Pending.
    //a device for doing integral estimation ...
    //so applying this to the output of 'differential(double *input, double *output)'
    //and then doing a series acceleration provides an integral estimate
    //with no constant integration error offset
    //this makes it particularly suited to feed forward systems and
    //estimations where the whole function is not known but just the differential series
    //the 'end point' integral maybe bianry searched on end point for solution fits
    //this would not be possible as with something like the Simpson's rule
    void integralPreMul(double *input);

protected:
    double h;//sampleStep
    double sigma = 0.0;
    uint64_t tick = 0;
};

class Blep {
    //a MinBLEP unit step band limiter for spectrum alias reduction
public:
    Blep(uint zeros = 1, uint oversample = 8, uint trunc = 1);//length
    ~Blep();

    double out(uint sampleInc = 1);
    void in(double value);

protected:
    double *array;
    double *residual;
    uint index = 0;
    uint indexw = 0;
    uint max;
    double *scales;
};

class DBuffer {//Saves a lot of % operations
    //exactly^^
public:
    DBuffer(uint size = 16, uint over = 16);//length
    ~DBuffer();

    double *outAddress(double *address, int step = 1);
    void fixBuffer(int step = 1);//positive motion
    double *useAddress(double *address, int step = 1);

protected:
    double *array;
    uint max;
    uint limit;
};

#endif // CALCULUS_H
