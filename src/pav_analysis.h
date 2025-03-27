#ifndef PAV_ANALYSIS_H
#define PAV_ANALYSIS_H
#define M_PI 3.14159265358979323846

void compute_hamming_window(float *w, int N);
float compute_power(const float *x, unsigned int N);
float compute_am(const float *x, unsigned int N);
float compute_zcr(const float *x, unsigned int N, float fm);

float compute_power_hamming(const float *x, unsigned int N, float fm);
#endif	/* PAV_ANALYSIS_H	*/
