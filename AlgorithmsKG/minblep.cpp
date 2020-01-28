// http://www.experimentalscene.com/articles/minbleps.php
// MinBLEP Generation Code
// By Daniel Werner
// This Code Is Public Domain

#include <math.h>

#define PI 3.14159265358979f

// SINC Function
inline double SINC(double x)
{
    double pix;

    if (x == 0.0f)
        return 1.0f;
    else {
        pix = PI * x;
        return sinf(pix) / pix;
    }
}

// Generate Blackman Window
inline void BlackmanWindow(int n, double *w)
{
    int m = n - 1;
    int i;
    double f1, f2, fm;

    fm = (double) m;
    for (i = 0; i <= m; i++) {
        f1 = (2.0f * PI * (double) i) / fm;
        f2 = 2.0f * f1;
        w[i] = 0.42f - (0.5f * cosf(f1)) + (0.08f * cosf(f2));
    }
}

// Discrete Fourier Transform
void DFT(int n, double *realTime, double *imagTime, double *realFreq, double *imagFreq)
{
    int k, i;
    double sr, si, p;

    for (k = 0; k < n; k++) {
        realFreq[k] = 0.0f;
        imagFreq[k] = 0.0f;
    }

    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++) {
            p = (2.0f * PI * (double) (k * i)) / n;
            sr = cosf(p);
            si = -sinf(p);
            realFreq[k] += (realTime[i] * sr) - (imagTime[i] * si);
            imagFreq[k] += (realTime[i] * si) + (imagTime[i] * sr);
        }
}

// Inverse Discrete Fourier Transform
void InverseDFT(int n, double *realTime, double *imagTime, double *realFreq, double *imagFreq)
{
    int k, i;
    double sr, si, p;

    for (k = 0; k < n; k++) {
        realTime[k] = 0.0f;
        imagTime[k] = 0.0f;
    }

    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            p = (2.0f * PI * (double) (k * i)) / n;
            sr = cosf(p);
            si = -sinf(p);
            realTime[k] += (realFreq[i] * sr) + (imagFreq[i] * si);
            imagTime[k] += (realFreq[i] * si) - (imagFreq[i] * sr);
        }
        realTime[k] /= n;
        imagTime[k] /= n;
    }
}

// Complex Absolute Value
inline double cabs(double x, double y)
{
    return sqrtf((x * x) + (y * y));
}

// Complex Exponential
inline void cexp(double x, double y, double *zx, double *zy)
{
    double expx;

    expx = expf(x);
    *zx = expx * cosf(y);
    *zy = expx * sinf(y);
}

// Compute Real Cepstrum Of Signal
void RealCepstrum(int n, double *signal, double *realCepstrum)
{
    double *realTime, *imagTime, *realFreq, *imagFreq;
    int i;

    realTime = new double[n];
    imagTime = new double[n];
    realFreq = new double[n];
    imagFreq = new double[n];

    // Compose Complex FFT Input

    for (i = 0; i < n; i++) {
        realTime[i] = signal[i];
        imagTime[i] = 0.0f;
    }

    // Perform DFT

    DFT(n, realTime, imagTime, realFreq, imagFreq);

    // Calculate Log Of Absolute Value

    for (i = 0; i < n; i++) {
        realFreq[i] = logf(cabs(realFreq[i], imagFreq[i]));
        imagFreq[i] = 0.0f;
    }

    // Perform Inverse FFT

    InverseDFT(n, realTime, imagTime, realFreq, imagFreq);

    // Output Real Part Of FFT
    for (i = 0; i < n; i++)
        realCepstrum[i] = realTime[i];

    delete realTime;
    delete imagTime;
    delete realFreq;
    delete imagFreq;
}

// Compute Minimum Phase Reconstruction Of Signal
void MinimumPhase(int n, double *realCepstrum, double *minimumPhase)
{
    int i, nd2;
    double *realTime, *imagTime, *realFreq, *imagFreq;

    nd2 = n / 2;
    realTime = new double[n];
    imagTime = new double[n];
    realFreq = new double[n];
    imagFreq = new double[n];

    if ((n % 2) == 1) {
        realTime[0] = realCepstrum[0];
        for (i = 1; i < nd2; i++)
            realTime[i] = 2.0f * realCepstrum[i];
        for (i = nd2; i < n; i++)
            realTime[i] = 0.0f;
    }
    else {
        realTime[0] = realCepstrum[0];
        for (i = 1; i < nd2; i++)
            realTime[i] = 2.0f * realCepstrum[i];
        realTime[nd2] = realCepstrum[nd2];
        for (i = nd2 + 1; i < n; i++)
            realTime[i] = 0.0f;
    }

    for (i = 0; i < n; i++)
        imagTime[i] = 0.0f;

    DFT(n, realTime, imagTime, realFreq, imagFreq);

    for (i = 0; i < n; i++)
        cexp(realFreq[i], imagFreq[i], &realFreq[i], &imagFreq[i]);

    InverseDFT(n, realTime, imagTime, realFreq, imagFreq);

    for (i = 0; i < n; i++)
        minimumPhase[i] = realTime[i];

    delete realTime;
    delete imagTime;
    delete realFreq;
    delete imagFreq;
}

// Generate MinBLEP And Return It In An Array Of doubleing Point Values
double *GenerateMinBLEP(int zeroCrossings, int overSampling)
{
    int i, n;
    double r, a, b;
    double *buffer1, *buffer2, *minBLEP;

    n = (zeroCrossings * 2 * overSampling) + 1;

    buffer1 = new double[n];
    buffer2 = new double[n];

    // Generate Sinc

    const double bandlimit = 0.9f;
    a = bandlimit * (double) -zeroCrossings;
    b = -a;
    for (i = 0; i < n; i++) {
        r = ((double) i) / ((double) (n - 1));
        buffer1[i] = SINC(a + (r * (b - a)));
    }

    // Window Sinc

    BlackmanWindow(n, buffer2);
    for (i = 0; i < n; i++)
        buffer1[i] *= buffer2[i];

    // Minimum Phase Reconstruction

    RealCepstrum(n, buffer1, buffer2);
    MinimumPhase(n, buffer2, buffer1);

    // Integrate Into MinBLEP

    minBLEP = new double[n];
    a = 0.0f;
    for (i = 0; i < n; i++) {
        a += buffer1[i];
        minBLEP[i] = a;
    }

    // Normalize
    a = minBLEP[n - 1];
    a = 1.0f / a;
    for (i = 0; i < n; i++)
        minBLEP[i] *= a;

    delete buffer1;
    delete buffer2;
    return minBLEP;
}
