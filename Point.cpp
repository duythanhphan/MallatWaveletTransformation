#include <Point.h>
#include <cmath>

#define square(x) (x * x)

Point::Point(double x, double y, double z) 
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point() 
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}

void Point::setX(double x) 
{
    this->x = x;
}

void Point::setY(double y) 
{
    this->y = y;
}

void Point::setZ(double z) 
{
    this->z = z;
}

double Point::getX() 
{
    return x;
}

double Point::getY() 
{
    return y;
}

double Point::getZ() 
{
    return z;
}

void Point::setCoordinates(double x, double y, double z) 
{
    this->setX(x);
    this->setY(y);
    this->setZ(z);
}


double Point::getDistanceTo(Point other) 
{
    return sqrt(getDoubledDistanceTo(other));   
}

double Point::getDoubledDistanceTo(Point other) 
{
    return square(getX() - other.getX()) +
        square(getY() - other.getY()) + 
        square(getZ() - other.getZ());
}
