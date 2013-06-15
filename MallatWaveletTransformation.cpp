#include <iostream>

#include "MallatWaveletTransformation.h"

MallatWaveletTransformation::MallatWaveletTransformation(
        DiscreteFunction* function) {
    cachedTransformedFunction = NULL;
    functionsDifferance = NULL;
    calculatedOriginalFunction = NULL;
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

void MallatWaveletTransformation::forwardTransformation() {
    std::cout << "Calculating\n";

    int pointsCount = originalFunction->getCount();

    Point* points = originalFunction->getPoints();

    Point* newPoints = new Point[pointsCount];

    for (int i = 0; i < pointsCount; i++) {
        newPoints[i] = points[i];
    }

    cachedTransformedFunction = new DiscreteFunction(newPoints, pointsCount);

    int length = pointsCount;
    double values[pointsCount];

    while (length > 1) {
        int half = length >> 1;           

        for (int i = 0; i < half; i++) {
            values[i] = newPoints[2 * i].getY() * basis[0][0]
                + newPoints[2 * i + 1].getY() * basis[0][1];
            values[i + half] = newPoints[2 * i].getY() * basis[1][0]
                + newPoints[2 * i + 1].getY() * basis[1][1];

            std::cout << newPoints[2 * i].getX() << " " << newPoints[2 * i].getY()
                << " " << newPoints[2 * i + 1].getX() << " "
                << newPoints[2 * i + 1].getY() << " " << values[i] << " " <<
                values[i + half] << std::endl;
        }          

        for (int i = 0; i < length; i++) {
            newPoints[i].setY(values[i]);
        }

        length >>= 1;
    }

    std::cout << "\nFinished\n--------------------------\n\n";
}

void MallatWaveletTransformation::reverseTransformation() {
    std::cout << "Calculating reverse Mallat transformation\n";

    DiscreteFunction* transformedFunction = getTransformedFunction();

    int pointsCount = transformedFunction->getCount();

    Point* points = transformedFunction->getPoints();

    Point* newPoints = new Point[pointsCount];

    for (int i = 0; i < pointsCount; i++) {
        newPoints[i] = points[i];
    }

    calculatedOriginalFunction = new DiscreteFunction(newPoints, pointsCount);

    int length = 2;
    double values[pointsCount];

    while(length <= pointsCount) {
        int half = length >> 1;
        
        for (int i = 0; i < half; i++) {
            values[i << 1] = newPoints[i].getY() * basis[0][0]
                + newPoints[i + half].getY() * basis[0][1];

            values[(i << 1) + 1] = newPoints[i].getY() * basis[1][0]
                + newPoints[i + half].getY() * basis[1][1];

            std::cout << newPoints[i].getX() << " " << newPoints[i].getY()
                << " " << newPoints[i + half].getX() << " "
                << newPoints[i + half].getY() << " " <<  values[2 * i] << " " <<
                values[2 * i + 1] << std::endl;
        }

        for (int i = 0; i < length; i++) {
            newPoints[i].setY(values[i]);
        }

        length <<= 1;
    }
}

DiscreteFunction* MallatWaveletTransformation::getTransformedFunction() {
    if (originalFunction == NULL) {
        return NULL;
    }
    
    if (cachedTransformedFunction == NULL) {
        forwardTransformation();
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

    DiscreteFunction* functions = new DiscreteFunction[4];

    functions[0].setPoints(originalFunction->getPoints());
    functions[0].setPointsCount(originalFunction->getCount());

    DiscreteFunction* second = getTransformedFunction();

    functions[1].setPoints(second->getPoints());
    functions[1].setPointsCount(second->getCount());

    DiscreteFunction* third = getCalculatedOriginalFunction();

    functions[2].setPoints(third->getPoints());
    functions[2].setPointsCount(third->getCount());

    DiscreteFunction* forth = getFunctionsDifferance();

    functions[3].setPoints(forth->getPoints());
    functions[3].setPointsCount(forth->getCount());

    return functions;
}

DiscreteFunction* MallatWaveletTransformation::getFunctionsDifferance() {
    if (originalFunction == NULL) {
        return NULL;
    }

    if (functionsDifferance == NULL) {
        calculateDifferance();
    }

    return functionsDifferance;
}

DiscreteFunction* MallatWaveletTransformation::getCalculatedOriginalFunction() {
    if (originalFunction == NULL) {
        return NULL;
    }

    if (calculatedOriginalFunction == NULL) {
        reverseTransformation();
    }

    return calculatedOriginalFunction;
}

void MallatWaveletTransformation::calculateDifferance() {
    Point* originalPoints = originalFunction->getPoints();
    int pointsCount = originalFunction->getCount();

    Point* calculatedPoints = getCalculatedOriginalFunction()->getPoints();
    
    Point* result = new Point[pointsCount];

    std::cout << "\n\n\n------------------------------------------------\n";

    for (int i = 0; i < pointsCount; i++) {
        result->setX(originalPoints[i].getX());
        result->setY(1e15 * (originalPoints[i].getY() - calculatedPoints[i].getY()));

        std::cout << result->getX() << " " << result->getY() << std::endl;
    }

    functionsDifferance = new DiscreteFunction(result, pointsCount);
}
