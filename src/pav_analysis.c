#include <math.h>
#include "pav_analysis.h"


float compute_power(const float *x, unsigned int N) {
    
    float sum = 0.0;
    for (unsigned int n = 0; n < N; n++) {
        sum += x[n] * x[n];
    }
    return 10.0 * log10f(sum / N); //AIXO ESTA EN DBs PER AIXO ES NEGATIU
}

float compute_am(const float *x, unsigned int N) {
    float sum = 0.0;
    for (unsigned int n = 0; n < N; n++) {
        sum += fabsf(x[n]);
    }
    return sum / N;
}   

float compute_zcr(const float *x, unsigned int N, float fm) {
    int crossings = 0;
    for (unsigned int n = 1; n < N; n++) {
        if ((x[n] >= 0 && x[n-1] < 0) || (x[n] < 0 && x[n-1] >= 0)) {
            crossings++;
        }
    }
    return (fm / 2.0f) * ((float)crossings / (N - 1));
}
float compute_power_hamming(const float *x, unsigned int N, float fm) {
    float sum = 0.0f;
    float sum_w2 = 0.0f;

    // Aplicar la ventana de Hamming al único bloque de longitud N
    for (unsigned int n = 0; n < N; n++) {
        float w = 0.54f - 0.46f * cosf((2.0f * 3.14159 * n) / (N - 1));
        sum += (x[n] * w) * (x[n] * w);
        sum_w2 += w * w;
    }
    
    if (sum_w2 == 0.0f) {
        return 2; // or another appropriate value
    }
    return 10.0f * log10f(sum / sum_w2);
}
