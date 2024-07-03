#include "DataHolder.h"
#include <math.h>
#include <ctime>
#include <cstdlib> 

Formula4::~Formula4() {
    if (NULL != target) delete target;
    if (NULL != inputs) {
        for (int i = 0; i < _N; ++i) {
            if (NULL != inputs[i]) {
                delete inputs[i];
            }
        }
        delete[] inputs;
    }
}

double Formula4::Function(double x1, double y1, double x2, double y2, double x3, double y3) {
    double X1 = fabs(x1 - x2);
    double Y1 = fabs(y1 - y2);
    double X2 = fabs(x1 - x3);
    double Y2 = fabs(y1 - y3);
    double X3 = fabs(x2 - x3);
    double Y3 = fabs(y2 - y3);

    double a = sqrt(X1 * X1 + Y1 * Y1);
    double b = sqrt(X2 * X2 + Y2 * Y2);
    double c = sqrt(X3 * X3 + Y3 * Y3);
    double s = (a + b + c) / 2.0;
    double rtn = sqrt(s * (s - a) * (s - b) * (s - c));
    return rtn;
}

double Formula4::GetValue() {
    double v = rand() % 1000 / 1000.0 * 100.0;
    return v;
}

void Formula4::GetInput(double* input)
{
    double x1 = GetValue();
    double x2 = GetValue();
    double x3 = GetValue();
    double y1 = GetValue();
    double y2 = GetValue();
    double y3 = GetValue();

    input[0] = x1;
    input[1] = y1;
    input[2] = x2;
    input[3] = y2;
    input[4] = x3;
    input[5] = y3;
}

double Formula4::GetTarget(double* input) {
    return Function(input[0], input[1], input[2], input[3], input[4], input[5]);
}

void Formula4::GenerateData(int N) {
    _N = N;
    inputs = new double*[N];
    for (int i = 0; i < N; ++i) {
        inputs[i] = new double[6];
    }
    target = new double[N];

    int counter = 0;
    while (true) {
        GetInput(inputs[counter]);
        target[counter] = Function(inputs[counter][0], inputs[counter][1], inputs[counter][2],
            inputs[counter][3], inputs[counter][4], inputs[counter][5]);
        if (++counter >= N) break;
    }
}
    
