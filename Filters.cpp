#include "Filters.h"
#include "SoundConstants.h"

LowPassButter::LowPassButter(Input input, double cutoff, unsigned int order) : in_input(input), out_output(0.0) {
    n = order;
    double a = tan(M_PI * cutoff / SOUND_FREQUENCY);
    double a2 = a * a;

    A = new double[n];
    d1 = new double[n];
    d2 = new double[n];
    w0 = new double[n] {0.0};
    w1 = new double[n] {0.0};
    w2 = new double[n] {0.0};

    a *= 2.0;
    a2 += 1.0;
    double k = (4.0 - 2.0 * a2);
    double l = M_PI / (4.0 * n);
    double m = l * 2.0;

    for (int i = 0; i < n; ++i) {
        double ar = a * sin(i * m + l);
        double is = 1.0 / (ar + a2);
        A[i] = (a2 - 1.0) * is;
        d1[i] = k * is;
        d2[i] = (ar - a2) * is;
    }
}
LowPassButter::~LowPassButter() {
    delete[] A;
    delete[] d1;
    delete[] d2;
    delete[] w0;
    delete[] w1;
    delete[] w2;
}
void LowPassButter::CalculateState() {
    double x = *in_input;
    for (int i = 0; i < n; ++i) {
        w0[i] = d1[i] * w1[i] + d2[i] * w2[i] + x;
        x = A[i] * (w0[i] + 2.0 * w1[i] + w2[i]);
    }
    double* t = w2;
    w2 = w1;
    w1 = w0;
    w0 = t;
    out_output.backValue = x;
}
void LowPassButter::PresentState() {
    out_output.PresentValue();
}

HighPassButter::HighPassButter(Input input, double cutoff, unsigned int order) : in_input(input), out_output(0.0) {
    n = order;
    double a = tan(M_PI * cutoff / SOUND_FREQUENCY);
    double a2 = a * a;

    A = new double[n];
    d1 = new double[n];
    d2 = new double[n];
    w0 = new double[n] {0.0};
    w1 = new double[n] {0.0};
    w2 = new double[n] {0.0};

    a *= 2.0;
    a2 += 1.0;
    double k = 4.0 - 2.0 * a2;
    double l = M_PI / (4.0 * n);
    double m = l * 2.0;

    for (int i = 0; i < n; ++i) {
        double ar = a * sin(i * m + l);
        double is = 1.0 / (ar + a2);
        A[i] = is;
        d1[i] = k * is;
        d2[i] = (ar - a2) * is;
    }
}
HighPassButter::~HighPassButter() {
    delete[] A;
    delete[] d1;
    delete[] d2;
    delete[] w0;
    delete[] w1;
    delete[] w2;
}
void HighPassButter::CalculateState() {
    double x = *in_input;
    for (int i = 0; i < n; ++i) {
        w0[i] = d1[i] * w1[i] + d2[i] * w2[i] + x;
        x = A[i] * (w0[i] - 2.0 * w1[i] + w2[i]);
    }
    double* t = w2;
    w2 = w1;
    w1 = w0;
    w0 = t;
    out_output.backValue = x;
}
void HighPassButter::PresentState() {
    out_output.PresentValue();
}

BandPassButter::BandPassButter(Input input, double lowerCutoff, double upperCutoff, unsigned int order) : in_input(input), out_output(0.0) {
    n = order;
    double a = cos(M_PI * (upperCutoff + lowerCutoff) / SOUND_FREQUENCY) / cos(M_PI * (upperCutoff - lowerCutoff) / SOUND_FREQUENCY);
    double a2 = a * a;
    double b = tan(M_PI * (upperCutoff - lowerCutoff) / SOUND_FREQUENCY);
    double b2 = b * b;

    A = new double[n];
    d1 = new double[n];
    d2 = new double[n];
    d3 = new double[n];
    d4 = new double[n];
    w0 = new double[n] {0.0};
    w1 = new double[n] {0.0};
    w2 = new double[n] {0.0};
    w3 = new double[n] {0.0};
    w4 = new double[n] {0.0};

    a *= 2.0;
    b *= 2.0;
    b2 += 1.0;
    double k = 2.0 * b2 - 4.0 * a2 - 4.0;
    double l = M_PI / (4.0 * n);
    double m = l * 2.0;

    for (int i = 0; i < n; ++i) {
        double br = b * sin(i * m + l);
        double is = 1.0 / (b2 + br);
        A[i] = (b2 - 1.0) * is;
        d1[i] = a * (2.0 + br) * is;
        d2[i] = k * is;
        d3[i] = a * (2.0 - br) * is;
        d4[i] = (br - b2) * is;
    }
}
BandPassButter::~BandPassButter() {
    delete[] A;
    delete[] d1;
    delete[] d2;
    delete[] d3;
    delete[] d4;
    delete[] w0;
    delete[] w1;
    delete[] w2;
    delete[] w3;
    delete[] w4;
}
void BandPassButter::CalculateState() {
    double x = *in_input;
    for (int i = 0; i < n; ++i) {
        w0[i] = d1[i] * w1[i] + d2[i] * w2[i] + d3[i] * w3[i] + d4[i] * w4[i] + x;
        x = A[i] * (w0[i] - 2.0 * w2[i] + w4[i]);
    }
    double* t = w4;
    w4 = w3;
    w3 = w2;
    w2 = w1;
    w1 = w0;
    w0 = t;
    out_output.backValue = x;
}
void BandPassButter::PresentState() {
    out_output.PresentValue();
}

BandStopButter::BandStopButter(Input input, double lowerCutoff, double upperCutoff, unsigned int order) : in_input(input), out_output(0.0) {
    n = order;
    double a = cos(M_PI * (upperCutoff + lowerCutoff) / SOUND_FREQUENCY) / cos(M_PI * (upperCutoff - lowerCutoff) / SOUND_FREQUENCY);
    double a2 = a * a;
    double b = tan(M_PI * (upperCutoff - lowerCutoff) / SOUND_FREQUENCY);
    double b2 = b * b;

    A = new double[n];
    d1 = new double[n];
    d2 = new double[n];
    d3 = new double[n];
    d4 = new double[n];
    w0 = new double[n] {0.0};
    w1 = new double[n] {0.0};
    w2 = new double[n] {0.0};
    w3 = new double[n] {0.0};
    w4 = new double[n] {0.0};

    a *= 2.0;
    b *= 2.0;
    b2 += 1.0;
    double k = 2.0 * b2 - 4.0 * a2 - 4.0;
    double l = M_PI / (4.0 * n);
    double m = l * 2.0;

    for (int i = 0; i < n; ++i) {
        double br = b * sin(i * m + l);
        double is = 1.0 / (b2 + br);
        A[i] = is;
        d1[i] = a * (2.0 + br) * is;
        d2[i] = k * is;
        d3[i] = a * (2.0 - br) * is;
        d4[i] = (br - b2) * is;
    }

    r = -2.0 * a;
    s = 4.0 * a2 + 2.0;
}
BandStopButter::~BandStopButter() {
    delete[] A;
    delete[] d1;
    delete[] d2;
    delete[] d3;
    delete[] d4;
    delete[] w0;
    delete[] w1;
    delete[] w2;
    delete[] w3;
    delete[] w4;
}
void BandStopButter::CalculateState() {
    double x = *in_input;
    for (int i = 0; i < n; ++i) {
        w0[i] =
            d1[i] * w1[i] +
            d2[i] * w2[i] +
            d3[i] * w3[i] +
            d4[i] * w4[i] +
            x;
        x = A[i] * (w0[i] + r * w1[i] + s * w2[i] + r * w3[i] + w4[i]);
    }
    double* t = w4;
    w4 = w3;
    w3 = w2;
    w2 = w1;
    w1 = w0;
    w0 = t;
    out_output.backValue = x;
}
void BandStopButter::PresentState() {
    out_output.PresentValue();
}