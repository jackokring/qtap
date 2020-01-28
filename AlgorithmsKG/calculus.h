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
    //a sum using the residual to gain higher precision
    static double sum(double *coeff, double *inputBegin, double *inputEnd, int step = 1);
    //setting some important timing factors
    void atTick(uint64_t now);
    void setSigma(double sigmaValue);
    void next();
    void expDecay(double *inputBegin, double *inputEnd, double *output,
                  int step = 1, bool splitDistribute = false);//from now
    static void cumSum(double *inputBegin, double *inputEnd, double *output, int step = 1);
    static bool seriesAccel(double *inputBegin, double *inputEnd,
                            double *output, int step = 1, bool outsToo = false);
    static double seriesAccelLim(double *inputBegin, double *inputEnd, int step = 1);
    static double seriesAccelLim2(double *inputBegin, double *inputEnd,
                                  int step = 1, uint nest = 1);
    static void preMul(double *coeff, double *inputBegin, double *inputEnd, double *output, int step = 1);
    static void map(double fn(double), double *inputBegin, double *inputEnd, double *output, int step = 1);
    static void entropy(double *inputBegin, double *inputEnd, double *output, int step = 1);

protected:
    double h;//sampleStep
    double sigma = 0.0;
    uint64_t tick = 0;
};

class Blep {
public:
    Blep(uint zeros = 1, uint oversample = 8);//length
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
