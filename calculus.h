#ifndef CALCULUS_H
#define CALCULUS_H


class Calculus {
public:
    Calculus();
    /*============================================================
     * SIGNAL PROCESSING
     * =========================================================*/
    //8 point differential uses input[i-8] to input[i]
    //outputs to output[1] to output[8]
    //so in effect can use a ring buffer
    void differential(double *input, double *output, int i, double h = 1.0);
};

#endif // CALCULUS_H
