#include <iostream>

#include "MallatWaveletTransformation.h"

MallatWaveletTransformation::MallatWaveletTransformation(
        DiscreteFunction* function) {
    cachedTransformedFunction = NULL;
    setOriginalFunction(function);

    SQRT2 = sqrt(2);

    basis[0][0] = 1 / SQRT2;
    basis[0][1] = 1 / SQRT2;
    basis[1][0] = 1 / SQRT2;
    basis[1][1] = -1 / SQRT2;
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

        std::cout << "Calculating\n";

        int length = pointsCount;
        double values[pointsCount];

        while (length > 1) {
            int half = length >> 1;           

            for (int i = 0; i < half; i++) {
                values[i] = newPoints[i].getY() * basis[0][0]
                        + newPoints[i + 1].getY() * basis[0][1];
                values[i + half] = newPoints[i].getY() * basis[1][0]
                        + newPoints[i + 1].getY() * basis[1][1];

                std::cout << newPoints[i].getX() << " " << newPoints[i].getY() << " " <<
                        newPoints[i + 1].getX() << " " << newPoints[i + 1].getY() << 
                        values[i] << " " << values[i + half] << std::endl;
            }          

            for (int i = 0; i < length; i++) {
                newPoints[i].setY(values[i]);
                std::cout << values[i] << std::endl;
            }

            length >>= 1;
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
