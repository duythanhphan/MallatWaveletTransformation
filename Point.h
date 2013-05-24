#ifndef __point_h__

#define __point_h__

class Point 
{
private:
    double x;
    double y;
    double z;
public:
    Point(double x, double y, double z);
    Point();
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    double getX();
    double getY();
    double getZ();
    void setCoordinates(double x, double y, double z);
    double getDistanceTo(Point other);
    double getDoubledDistanceTo(Point other);
};

#endif
