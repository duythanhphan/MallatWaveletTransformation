#include "MallatWaveletTransformation.h"

MallatWaveletTransformation::MallatWaveletTransformation(
        DiscreteFunction* function) {
    setOriginalFunction(function);
}

DiscreteFunction* MallatWaveletTransformation::getOriginalFunction() {
    return originalFunction;
}

DiscreteFunction* MallatWaveletTransformation::getTransformedFunction() {
    if (originalFunction == NULL) {
        return NULL;
    }
    
    if (cachedTransformedFunction == NULL) {
        int pointsCount = originalFunction->getCount();

        Point* points = originalFunction->getPoints();

        Point* newPoints = new Point[pointsCount];

        for (int i = 0; i < pointsCount; i++) {
            newPoints[i] = points[i];
        }

        cachedTransformedFunction = new DiscreteFunction(newPoints, pointsCount);

        for (int i = 0; i < pointsCount; i++) {
            newPoints[i].setY(newPoints[i].getY() * 10);
        }
        // calculate;
    }

    return cachedTransformedFunction;
}

void MallatWaveletTransformation::setOriginalFunction(
        DiscreteFunction* function) {
    originalFunction = function;

    if (cachedTransformedFunction != NULL) {
        delete cachedTransformedFunction;
    }
}

DiscreteFunction* MallatWaveletTransformation::getFunctions() {
    if (originalFunction == NULL) {
        return NULL;
    }

    DiscreteFunction* functions = new DiscreteFunction[2];

    functions[0].setPoints(originalFunction->getPoints());
    functions[0].setPointsCount(originalFunction->getCount());

    DiscreteFunction* second = getTransformedFunction();

    functions[1].setPoints(second->getPoints());
    functions[1].setPointsCount(second->getCount());

    return functions;
}
