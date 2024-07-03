#include "Urysohn.h"
#include <cstdlib> 
#include <iostream>

Urysohn::Urysohn(double* xmin, double* xmax, double targetMin, double targetMax, int* layers, int len) {
	_length = len;
	double ymin = targetMin / _length;
	double ymax = targetMax / _length;
	_univariateList = new Univariate*[_length];
    for (int i = 0; i < _length; ++i)
	{
		_univariateList[i] = new Univariate(xmin[i], xmax[i], ymin, ymax, layers[i]);
	}
}

Urysohn::~Urysohn() {
	ReleaseObjects();
}

void Urysohn::ReleaseObjects() {
	for (int i = 0; i < _length; ++i) {
		if (NULL != _univariateList[i]) {
			delete _univariateList[i];
			_univariateList[i] = NULL;
		}
	}
	if (NULL != _univariateList) {
		delete[] *_univariateList;
		_univariateList = NULL;
	}
}

Urysohn::Urysohn(Urysohn& uri) {
	ReleaseObjects();
	_length = uri._length;
	_univariateList = new Univariate*[_length];
	for (int i = 0; i < _length; ++i)
	{
		_univariateList[i] = new Univariate(*uri._univariateList[i]);
	}
}

void Urysohn::UpdateUsingInput(double delta, double* inputs, double mu) {
	delta /= _length;
	for (int i = 0; i < _length; ++i)
	{
		_univariateList[i]->UpdateUsingInput(inputs[i], delta, mu);
	}
}

void Urysohn::UpdateUsingMemory(double delta, double mu) {
	delta /= _length;
	for (int i = 0; i < _length; ++i)
	{
		_univariateList[i]->UpdateUsingMemory(delta, mu);
	}
}

double Urysohn::GetValueUsingInput(double* inputs) {
	double f = 0.0;
	for (int i = 0; i < _length; ++i)
	{
		f += _univariateList[i]->GetFunctionUsingInput(inputs[i]);
	}
	return f;
}
