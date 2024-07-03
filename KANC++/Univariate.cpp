#include "Univariate.h"
#include <cstdlib> 

Univariate::Univariate(double xmin, double xmax, double ymin, double ymax, int points) {
    _points = points;
    _xmin = xmin;
    _xmax = xmax;
    _ymin = ymin;
    _ymax = ymax;
    SetLimits();
    SetRandomFunction();
}

Univariate::~Univariate() {
    if (NULL != _y) {
        delete[] _y;
        _y = NULL;
    }
}

Univariate::Univariate(Univariate& uni) {
    _points = uni._points;
    _xmin = uni._xmin;
    _xmax = uni._xmax;
    _ymin = uni._ymin;
    _ymax = uni._ymax;
    _deltax = (_xmax - _xmin) / (_points - 1);
    if (nullptr != _y) delete _y;
    _y = new double[_points];
    for (int i = 0; i < _points; i++)
    {
        _y[i] = uni._y[i];
    }
    _lastLeftIndex = uni._lastLeftIndex;
    _lastLeftOffset = uni._lastLeftOffset;
}

void Univariate::SetLimits() {
    double range = _xmax - _xmin;
    _xmin -= 0.01 * range;
    _xmax += 0.01 * range;
    _deltax = (_xmax - _xmin) / (_points - 1);
}

void Univariate::SetRandomFunction() {
    _y = new double[_points];
    for (int i = 0; i < _points; ++i)
    {
        _y[i] = rand() % 100;
    }
    double min = _y[0];
    double max = _y[0];
    for (int i = 0; i < _points; ++i) {
        if (_y[i] < min) min = _y[i];
        if (_y[i] > max) max = _y[i];
    }
    if (min == max) max = min + 1.0;
    for (int i = 0; i < _points; ++i)
    {
        _y[i] = (_y[i] - min) / (max - min) * (_ymax - _ymin) + _ymin;
    }
}

void Univariate::FitDefinition(double x) {
    if (x < _xmin) {
        _xmin = x;
        SetLimits();
    }
    if (x > _xmax) {
        _xmax = x;
        SetLimits();
    }
}

double Univariate::GetDerivative(double x) {
    int low = (int)((x - _xmin) / _deltax);
    return (_y[low + 1] - _y[low]) / _deltax;
}

void Univariate::UpdateUsingInput(double x, double delta, double mu) {
    FitDefinition(x);
    delta *= mu;
    double offset = (x - _xmin) / _deltax;
    int left = (int)(offset);
    double leftx = offset - left;
    _y[left + 1] += delta * leftx;
    _y[left] += delta * (1.0 - leftx);
}

void Univariate::UpdateUsingMemory(double delta, double mu) {
    delta *= mu;
    _y[_lastLeftIndex + 1] += delta * _lastLeftOffset;
    _y[_lastLeftIndex] += delta * (1.0 - _lastLeftOffset);
}

double Univariate::GetFunctionUsingInput(double x) {
    FitDefinition(x);
    double offset = (x - _xmin) / _deltax;
    int leftIndex = (int)(offset);
    double leftOffset = offset - leftIndex;
    _lastLeftIndex = leftIndex;
    _lastLeftOffset = leftOffset;
    return _y[leftIndex] + (_y[leftIndex + 1] - _y[leftIndex]) * leftOffset;
}