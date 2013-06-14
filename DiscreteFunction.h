#ifndef __discrete_function_h__

#define __discrete_function_h__

#include "Point.h"

class DiscreteFunction 
{
private:
    Point* points;
    int count;

public:
    DiscreteFunction();
    DiscreteFunction(Point* _points, int _count);
    ~DiscreteFunction();
    int getCount();
    Point* getPoints();
    Point getPoint(int index);
    void setPoints(Point* _points);
    void setPointsCount(int _count);
};

#endif
