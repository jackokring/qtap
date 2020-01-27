#ifndef CALCULUS_H
#define CALCULUS_H

#include <QtCore>
#include "../kringlicence.h"

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

protected:
    double h;//sampleStep
    double sigma = 0.0;
    uint64_t tick = 0;
};

#endif // CALCULUS_H
