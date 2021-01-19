#pragma once

// The following filters in this file and the associated .cpp implementation file uses adapted code.
// Originally from http://www.exstrom.com/journal/sigproc/index.html
// Exstrom host code for various filters under the filter section of their website, and have released such code
// under the terms of the GNU General Public License (http://www.gnu.org/copyleft/gpl.html)

#define _USE_MATH_DEFINES
#include <math.h>

#include "Module.h"

using namespace std;

MakeModule(LowPassButter)
protected:
    int n;

    double* A;
    double* d1;
    double* d2;
    double* w0;
    double* w1;
    double* w2;

public:
    Input in_input;
    LowPassButterOutput out_output;

    LowPassButter(Input input, double cutoff, unsigned int strength);
    ~LowPassButter();
};

MakeModule(HighPassButter)
protected:
    int n;

    double* A;
    double* d1;
    double* d2;
    double* w0;
    double* w1;
    double* w2;
public:
    Input in_input;
    HighPassButterOutput out_output;

    HighPassButter(Input input, double cutoff, unsigned int strength);
    ~HighPassButter();
};

MakeModule(BandPassButter)
protected:
    int n;

    double* A;
    double* d1;
    double* d2;
    double* d3;
    double* d4;
    double* w0;
    double* w1;
    double* w2;
    double* w3;
    double* w4;

public:
    Input in_input;
    BandPassButterOutput out_output;

    BandPassButter(Input, double lowerCutoff, double upperCutoff, unsigned int strength);
    ~BandPassButter();
};

MakeModule(BandStopButter)
protected:
    int n;

    double r;
    double s;

    double* A;
    double* d1;
    double* d2;
    double* d3;
    double* d4;
    double* w0;
    double* w1;
    double* w2;
    double* w3;
    double* w4;

public:
    Input in_input;
    BandStopButterOutput out_output;

    BandStopButter(Input, double lowerCutoff, double upperCutoff, unsigned int strength);
    ~BandStopButter();
};