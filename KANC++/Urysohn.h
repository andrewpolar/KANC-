#pragma once
#include "Univariate.h"

class Urysohn
{
public:
	int _length;
	Univariate** _univariateList = NULL;
	Urysohn(double* xmin, double* xmax, double targetMin, double targetMax, int* layers, int len);
	~Urysohn();
	Urysohn(Urysohn& uri);
	void UpdateUsingInput(double delta, double* inputs, double mu);
	void UpdateUsingMemory(double delta, double mu);
	double GetValueUsingInput(double* inputs);
private:
	void ReleaseObjects();
};

