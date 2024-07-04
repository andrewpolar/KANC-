//Concept: Andrew Polar and Mike Poluektov
//Developer Andrew Polar

//License
//In case if end user finds the way of making a profit by using this code and earns
//billions of US dollars and meet developer bagging change in the street near McDonalds,
//he or she is not in obligation to buy him a sandwich.

//Symmetricity
//In case developer became rich and famous by publishing this code and meet misfortunate
//end user who went bankrupt by using this code, he is also not in obligation to buy
//end user a sandwich.

//Publications:
//https://www.sciencedirect.com/science/article/abs/pii/S0016003220301149
//https://www.sciencedirect.com/science/article/abs/pii/S0952197620303742
//https://arxiv.org/abs/2305.08194

//Formula4 is area of triangles as a function of vertices.
//Formula3 is Mike's formula.

#include <iostream>
#include <thread>
#include "DataHolder.h"
#include "KANAddend.h"

void ShowMatrix(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%4.1f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void ShowVector(double* t, int N) {
    int cnt = 0;
    for (int i = 0; i < N; ++i) {
        printf("%5.2f ", t[i]);
        if (++cnt >= 10) {
            printf("\n");
            cnt = 0;
        }
    }
}

void FindMinMax(double* xmin, double* xmax, double& targetMin, double& targetMax, 
    double** matrix, double* target, int nRows, int nCols) {

    for (int i = 0; i < nCols; ++i) {
        xmin[i] = DBL_MAX;
        xmax[i] = -DBL_MIN;
    }

    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            if (matrix[i][j] < xmin[j]) xmin[j] = matrix[i][j];
            if (matrix[i][j] > xmax[j]) xmax[j] = matrix[i][j];
        }
    }

    targetMin = DBL_MAX;
    targetMax = -DBL_MIN;
    for (int j = 0; j < nRows; ++j) {
        if (target[j] < targetMin) targetMin = target[j];
        if (target[j] > targetMax) targetMax = target[j];
    }
}

void Training(KANAddend** addends, double** matrix, double* target, 
    int nRows, int nModels, double targetMin, double targetMax, int nEpochs, 
    int marginStart, int margineEnd, double sensitivity) {
    double* residualError = new double[nRows] {0.0};
    int end = nEpochs - margineEnd;
    for (int epoch = 0; epoch < nEpochs; ++epoch)
    {
        double error2 = 0.0;
        int cnt = 0;
        for (int i = 0; i < nRows; ++i)
        {
            if (epoch >= marginStart && epoch < end && residualError[i] < sensitivity) continue;
            double residual = target[i];
            for (int j = 0; j < nModels; ++j)
            {
                residual -= addends[j]->ComputeUsingInput(matrix[i]);
            }
            for (int j = 0; j < nModels; ++j)
            {
                //this method reuses properties computed in ComputeUsingInput
                addends[j]->UpdateUsingMemory(residual);

                //next method updates independently without reusing properties computed
                //by ComputeUsingInput
                //addends[j]->UpdateUsingInput(f4->inputs[i], residual);
            }
            error2 += residual * residual;
            residualError[i] = fabs(residual);
            ++cnt;
        }
        if (0 == cnt) error2 = 0.0;
        else {
            error2 /= cnt;
            error2 = sqrt(error2);
            error2 /= (targetMax - targetMin);
        }
        printf("Training step %d, relative RMSE %4.4f\n", epoch, error2);
    }
    delete[] residualError;
}

int main() {

    int nRecords = 10000;
    Formula3* f3 = new Formula3();
    f3->GenerateData(nRecords);

    //ShowMatrix(f4->inputs, f4->_N, f4->nInputs);
    //ShowVector(f4->target, nRecords);

    clock_t start_encoding = clock();

    double* xmin = new double[f3->nInputs];
    double* xmax = new double[f3->nInputs];
    double targetMin;
    double targetMax;

    FindMinMax(xmin, xmax, targetMin, targetMax, f3->inputs, f3->target, f3->_N, f3->nInputs);
    //printf("Target min %4.2f, Target max %4.2f\n", targetMin, targetMax);
    //for (int j = 0; j < f3->nInputs; ++j) {
    //    printf("%4.2f %4.2f\n", xmin[j], xmax[j]);
    //}

    srand((unsigned int)time(NULL));
    int nModels = 11;
    double zmin = targetMin / nModels;
    double zmax = targetMax / nModels;
    KANAddend** addends = new KANAddend*[nModels];
    for (int i = 0; i < nModels; ++i) {
        addends[i] = new KANAddend(xmin, xmax, zmin, zmax, 6, 12, 0.1, 0.01, f3->nInputs);
    }

    std::thread th(Training, addends, f3->inputs, f3->target, nRecords, nModels, targetMin, targetMax, 36, 6, 6, 0.06);
    th.join();

    clock_t end_encoding = clock();
    printf("Time for training %2.3f sec.\n", (double)(end_encoding - start_encoding) / CLOCKS_PER_SEC);

    //Initialization for Formula4
    //int nModels = 32;
    //double zmin = targetMin / nModels;
    //double zmax = targetMax / nModels;
    //KANAddend** addends = new KANAddend*[nModels];
    //for (int i = 0; i < nModels; ++i)
    //{
    //    addends[i] = new KANAddend(xmin, xmax, zmin, zmax, 4, 20, 0.01, 0.01, f4->nInputs);
    //}
    //std::thread th(Training, addends, f3->inputs, f3->target, nRecords, nModels, targetMin, targetMax, 46, 6, 10, 120.0);

    //// Object copy test //////
    KANAddend** addendsCopy = new KANAddend*[nModels];
    for (int i = 0; i < nModels; ++i) {
        addendsCopy[i] = new KANAddend(*addends[i]);
    }

    double error = 0.0;
    double error3 = 0.0;
    int NTests = 1000;
    for (int i = 0; i < NTests; ++i)
    {
        double* test_input = new double[f3->nInputs];
        f3->GetInput(test_input);
        double test_target = f3->GetTarget(test_input);

        double model1 = 0.0;
        for (int j = 0; j < nModels; ++j)
        {
            model1 += addends[j]->ComputeUsingInput(test_input);
        }

        double model2 = 0.0;
        for (int j = 0; j < nModels; ++j)
        {
            model2 += addendsCopy[j]->ComputeUsingInput(test_input);
        }

        error += (test_target - model1) * (test_target - model1);
        error3 += (test_target - model2) * (test_target - model2);
    }
    error /= NTests;
    error = sqrt(error);
    error /= (targetMax - targetMin);
    error3 /= NTests;
    error3 = sqrt(error3);
    error3 /= (targetMax - targetMin);
    printf("\nRelative RMSE for unseen data %f, RMSE for copy object %f\n", error, error3);

    for (int i = 0; i < nModels; ++i) {
        delete addends[i];
    }
    delete[] *addends;

    for (int i = 0; i < nModels; ++i) {
        delete addendsCopy[i];
    }
    delete[] *addendsCopy;

    delete[] xmin;
    delete[] xmax;
}

