#ifndef mallat_wavelet_transformation_h__

#define mallat_wavelet_transformation_h__

#include <cstddef>
#include <cmath>

#include "DiscreteFunction.h"


class MallatWaveletTransformation
{

private:
    DiscreteFunction* originalFunction; 
    DiscreteFunction* cachedTransformedFunction;
    DiscreteFunction* calculatedOriginalFunction;
    DiscreteFunction* functionsDifferance;

    double basis[2][2];
    double SQRT2;

    void forwardTransformation();

    void reverseTransformation();

public:
    MallatWaveletTransformation(DiscreteFunction* function = NULL);

    void setOriginalFunction(DiscreteFunction* function);

    DiscreteFunction* getOriginalFunction();

    DiscreteFunction* getTransformedFunction();

    DiscreteFunction* getFunctions();

    DiscreteFunction* getFunctionsDifferance();

    DiscreteFunction* getCalculatedOriginalFunction();
};

#endif
