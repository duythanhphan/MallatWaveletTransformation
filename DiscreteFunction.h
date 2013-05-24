#ifndef __discrete_function_h__

#define __discrete_function_h__

class DiscreteFunction 
{
private:
    Point* points;
    int count;
public:
    DiscreteFunction(Point* _points, int _count);
    ~DiscreteFunction();
    int getCount();
    Point* getPoints();
    Point getPoint(int index);
};

#endif
