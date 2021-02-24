#pragma once

// The following filters in this file and the associated .cpp implementation file uses adapted code.
// Originally from http://www.exstrom.com/journal/sigproc/index.html
// Exstrom host code for various filters under the filter section of their website, and have released such code
// under the terms of the GNU General Public License (http://www.gnu.org/copyleft/gpl.html)

#define _USE_MATH_DEFINES
#include <math.h>

#include "Module.h"
#include "Sound.h"

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
    Input<double> in_input;
    LowPassButterOutput<double> out_output;

    LowPassButter(Input<double> input, double cutoff, unsigned int strength, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
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
    Input<double> in_input;
    HighPassButterOutput<double> out_output;

    HighPassButter(Input<double> input, double cutoff, unsigned int strength, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
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
    Input<double> in_input;
    BandPassButterOutput<double> out_output;

    BandPassButter(Input<double>, double lowerCutoff, double upperCutoff, unsigned int strength, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
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
    Input<double> in_input;
    BandStopButterOutput<double> out_output;

    BandStopButter(Input<double>, double lowerCutoff, double upperCutoff, unsigned int strength, ModuleRegistry& registry = ModuleRegistry::globalRegistry);
    ~BandStopButter();
};