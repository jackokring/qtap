#include "calculus.h"

Calculus::Calculus() {

}

void Calculus::differential(double *input, double *output, double h) {
    //coefficients via http://web.media.mit.edu/~crtaylor/calculator.html
    //all done from prospective of sample input[0]
    double t = h;
    output[0] = input[0];
    output[1] = (105 * input[i - 8] - 960 * input[i - 7] +
            3920 * input[i - 6] - 9408 * input[i - 5] +
            14700 * input[i - 4] - 15680 * input[i - 3] +
            11760 * input[i - 2] - 6720 * input[i - 1] +
            2283 * input[i]) / (840 * t);
    t *= h;
    output[2] = (3267 * input[i - 8] - 29664 * input[i - 7] +
            120008 * input[i - 6] - 284256 * input[i - 5] +
            435330 * input[i - 4] - 448672 * input[i - 3] +
            312984 * input[i - 2] - 138528 * input[i - 1] +
            29531 * input[i]) / (5040 * t);
    t *= h;
    output[3] = (469 * input[i - 8] - 4216 * input[i - 7] +
            16830 * input[i - 6] - 39128 * input[i - 5] +
            58280 * input[i - 4] - 57384 * input[i - 3] +
            36706 * input[i - 2] - 13960 * input[i - 1] +
            2403 * input[i]) / (240 * t);
    t *= h;
    output[4] = (967 * input[i - 8] - 8576 * input[i - 7] +
            33636 * input[i - 6] - 76352 * input[i - 5] +
            109930 * input[i - 4] - 102912 * input[i - 3] +
            61156 * input[i - 2] - 21056 * input[i - 1] +
            3207 * input[i]) / (240 * t);
    t *= h;
    output[5] = (35 * input[i - 8] - 305 * input[i - 7] +
            1170 * input[i - 6] - 2581 * input[i - 5] +
            3580 * input[i - 4] - 3195 * input[i - 3] +
            1790 * input[i - 2] - 575 * input[i - 1] +
            81 * input[i])/(6 * t);
    t *= h;
    output[6] = (23 * input[i - 8] - 196 * input[i - 7] +
            732 * input[i - 6] - 1564 * input[i - 5] +
            2090 * input[i - 4] - 1788 * input[i - 3] +
            956 * input[i - 2] - 292 * input[i - 1] +
            39 * input[i]) / (4 * t);
    t *= h;
    output[7] = (7 * input[i - 8] - 58 * input[i - 7] +
            210 * input[i - 6] - 434 * input[i - 5] +
            560 * input[i - 4] - 462 * input[i - 3] +
            238 * input[i - 2] - 70 * input[i - 1] +
            9 * input[i]) / (2 * t);
    t *= h;
    output[8] = (1 * input[i - 8] - 8 * input[i - 7] +
            28 * input[i - 6] - 56 * input[i - 5] +
            70 * input[i - 4] - 56 * input[i - 3] +
            28 * input[i - 2] - 8 * input[i - 1] +
            1 * input[i])/(1 * t);
}
