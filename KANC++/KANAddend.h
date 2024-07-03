#pragma once
#include "Urysohn.h"
#include "Univariate.h"

class KANAddend
{
public:
    double _targetMin;
    double _targetMax;
    double _muOuter;
    double _muInner;
    KANAddend(double* xmin, double* xmax, double targetMin, double targetMax,
        int inner, int outer, double muInner, double muOuter, int number_of_inputs);
    KANAddend(KANAddend& obj);
    ~KANAddend();
    void UpdateUsingMemory(double diff);
    void UpdateUsingInput(double* input, double diff);
    double ComputeUsingInput(double* input);
private:
    void ReleaseObjects();
    double _lastInnerValue;
    Urysohn* _u = NULL;
    Univariate* _univariate = NULL;
};

