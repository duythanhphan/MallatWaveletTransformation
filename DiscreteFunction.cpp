#include <Point.h>
#include <DiscreteFunction.h>

DiscreteFunction::DiscreteFunction() {
}

DiscreteFunction::DiscreteFunction(Point* _points, int _count) {
    points = _points;
    count = _count;
}

DiscreteFunction::~DiscreteFunction() {
    delete[] points;
}

int DiscreteFunction::getCount() {
    return count;
}

Point* DiscreteFunction::getPoints() {
    return points;
}

Point DiscreteFunction::getPoint(int index) {
    return points[index];
}

void DiscreteFunction::setPoints(Point* _points) {
    points = _points;
}

void DiscreteFunction::setPointsCount(int _count) {
    count = _count;
}
