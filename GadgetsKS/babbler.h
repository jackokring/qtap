#ifndef BABBLER_H
#define BABBLER_H

#include "../kringlicence.h"
#include <QtCore>

class Babbler {
public:
    /*==========================================================================
     * A FUNDEMETAL TEXT CLASS FOR AI?
     * -------------------------------
     *
     * Providing input control signals the Babbler produces output as time
     * is stepped. The method 'present' steps the time and control signal
     * current values and 'currentWords/currentLetters' provide a string
     * which has the initial letter of value plus some significant count
     * of expected continuation.
     *
     * This is part of more complex structure called a context multiplexer
     * GB 1905300.8 Pat. Pending. which can be used to apply context to AI
     * networks. A device for ...
     * =======================================================================*/
    Babbler();
    ~Babbler();

    //===========================================
    // INTERFACE
    //===========================================
    uint32_t present(double dX = 0.0, double dT = 0.0);
    QString currentLetters();
    QString currentWords();
    uint significantLetters();
    uint32_t future(double dX = 0.0, double dT = 1.0);
    uint32_t past(double dX = 0.0, double dT = 1.0);
    void loadCorpus(QString text);
    uint32_t mapUniform(uint32_t input);
    uint32_t mapInformational(uint32_t input);

protected:

};

#endif // BABBLER_H
