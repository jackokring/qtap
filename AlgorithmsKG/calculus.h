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
    //The 0th differential one step in the future from input[-8] to input[0]
    //can then store it in input[1] and ...
    double future(double *input);
    //A sum using the residual to gain higher precision
    static double sum(double *coeff, double *inputBegin, double *inputEnd, int step = 1);
    //Setting some important timing factors
    void atTick(uint64_t now);
    void setSigma(double sigmaValue);
    void next();
    //Apply an exponetial decay of -sigma*t to the input history
    //splitDistribute uses expm1 for x.expm1 + x = x.exp
    void expDecay(double *inputBegin, double *inputEnd, double *output,
                  int step = 1, bool splitDistribute = false);
    //A cumulative sum for series acceleration input
    static void cumSum(double *inputBegin, double *inputEnd, double *output, int step = 1);
    //Some series acceleration routines
    static bool seriesAccel(double *inputBegin, double *inputEnd,
                            double *output, int step = 1, bool outsToo = false);
    static double seriesAccelLim(double *inputBegin, double *inputEnd, int step = 1);
    static double seriesAccelLim2(double *inputBegin, double *inputEnd,
                                  int step = 1, uint nest = 1);
    //Premultiplication by a coefficient set
    static void preMul(double *coeff, double *inputBegin, double *inputEnd, double *output, int step = 1);
    //Map a simple function over an input (with step stride)
    static void map(double fn(double), double *inputBegin, double *inputEnd, double *output, int step = 1);
    //Entropy self information for example, and the a sum gives total entropy
    static void entropy(double *inputBegin, double *inputEnd, double *output, int step = 1);
    //integral pre-multiply input[0] to input[8] GB 1905339.6 Pat. Pending.
    //A device for doing integral estimation ...
    //So applying this to the output of 'differential(double *input, double *output)'
    //and then doing a series acceleration provides an integral estimate
    //with no constant integration error offset
    //This makes it particularly suited to feed forward systems and
    //estimations where the whole function is not known but just the differential series
    //The 'end point' integral maybe bianry searched on end point for solution fits
    //This would not be possible as with something like the Simpson's rule
    //N.B. An offset in the Y axis is a rectangle to unsigned integrals
    void integralPreMul(double *input);
    //There are other forms of this as many series for 'end points' exist
    //There is the pole form, where before differential, the function is
    //divided by time (x)
    //Then fact and xacc variables must be updated before (*input++)
    //There is a very complicated one involving logs and is best seen
    //as a linear part and an entropic part but needs many series accelerations

    //Calculates inline differentials for the future predicted sample
    //The returned value is the difference in the predicted 8th order differential
    //estimated based on the 'new' sample as extra knowledge minus the
    //sample as part of a moving window, and such represents the change
    //in the 8th differential by 'excluding information from the past'
    //This then is a stability measure as integration of differentials
    //leads to offset errors becoming offset drifts and offset accelerations etc ...
    //How much memory does a time series sequence need?
    double differential9(double *input);

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
