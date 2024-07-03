#pragma once

class Formula4
{
public:
	~Formula4();
	double** inputs;
	double* target;
	void GenerateData(int N);
	int _N;
	const int nInputs = 6;
	void GetInput(double* input);
	double GetTarget(double* input);
private:
	double Function(double x1, double y1, double x2, double y2, double x3, double y3);
	double GetValue();
};

