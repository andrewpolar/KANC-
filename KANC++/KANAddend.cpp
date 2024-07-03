#include "KANAddend.h"

KANAddend::KANAddend(double* xmin, double* xmax, double targetMin, double targetMax,
    int inner, int outer, double muInner, double muOuter, int number_of_inputs) {

    _muInner = muInner;
    _muInner = muInner;
    _muOuter = muOuter;
    _targetMin = targetMin;
    _targetMax = targetMax;
    _lastInnerValue = 0.0;

    int* interior_structure = new int[number_of_inputs];
    for (int i = 0; i < number_of_inputs; i++)
    {
        interior_structure[i] = inner;
    }
    _u = new Urysohn(xmin, xmax, _targetMin, _targetMax, interior_structure, number_of_inputs);
    _univariate = new Univariate(_targetMin, _targetMax, _targetMin, _targetMax, outer);
    delete[] interior_structure;
}

void KANAddend::ReleaseObjects() {
    if (NULL != _u) {
        delete _u;
        _u = NULL;
    }
    if (NULL != _univariate) {
        delete _univariate;
        _univariate = NULL;
    }
}

KANAddend::~KANAddend() {
    ReleaseObjects();
}

KANAddend::KANAddend(KANAddend& addend) {
    ReleaseObjects();
    _muInner = addend._muInner;
    _muOuter = addend._muOuter;
    _targetMin = addend._targetMin;
    _targetMax = addend._targetMax;
    _lastInnerValue = addend._lastInnerValue;
    _univariate = new Univariate(*addend._univariate);
    _u = new Urysohn(*addend._u);
}

void KANAddend::UpdateUsingMemory(double diff) {
    double derrivative = _univariate->GetDerivative(_lastInnerValue);
    _u->UpdateUsingMemory(diff * derrivative, _muInner);
    _univariate->UpdateUsingMemory(diff, _muOuter);
}

void KANAddend::UpdateUsingInput(double* input, double diff) {
    double value = _u->GetValueUsingInput(input);
    double derrivative = _univariate->GetDerivative(value);
    _u->UpdateUsingInput(diff * derrivative, input, _muInner);
    _univariate->UpdateUsingInput(value, diff, _muOuter);
}

double KANAddend::ComputeUsingInput(double* input) {
    _lastInnerValue = _u->GetValueUsingInput(input);
    return _univariate->GetFunctionUsingInput(_lastInnerValue);
}