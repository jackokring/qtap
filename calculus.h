#ifndef CALCULUS_H
#define CALCULUS_H

#include "kringlicence.h"

class Calculus {
public:
    Calculus();
    /*============================================================
     * SIGNAL PROCESSING
     * =========================================================*/
    //9 point differential uses input[-8] to input[0]
    //outputs to output[0] to output[8]
    //so in effect can use a ring buffer
    void differential(double *input, double *output, double h = 1.0);
};

#endif // CALCULUS_H
