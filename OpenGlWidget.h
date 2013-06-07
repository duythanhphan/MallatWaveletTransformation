#ifndef __open_gl_widget_h__

#define __open_gl_widget_h__

#include <QKeyEvent>
#include <qapplication.h>
#include <qevent.h>
#include <qgl.h>
#include <Point.h>
#include <DiscreteFunction.h>

#define BASE_ZOOM -5
#define MAX_CONTROL_POINTS 10
#define MIN_CONTROL_POINTS 2
#define MAX_PATCHES        42
#define MIN_PATHES         3
#define MAX_SURFACE_POINTS 1024
#define COORD_SYSTEM_WIDTH 4
#define COORD_AXIS_LEN     16
#define MAX_ZOOM           24
#define MIN_ZOOM           2
#define TRANSLATION_FACTOR 0.1

class OpenGlWidget : public QGLWidget
{
    Q_OBJECT

private:
    bool showCoordinateSystem;
    bool showPoints;
    int zoom;
    int patches;
    int zoom_step;
    float angelx;
    float angely;
    int pointsInSurface;
    DiscreteFunction* functions;
    int functionsCount;
    float translationX;
    float translationY;
    // tmp variables
    int currx;
    int curry;
    int index;
    double tmpZ;
    int pressedButton;

public:
	OpenGlWidget(QWidget *parent=0,
      DiscreteFunction* _function=0,
      int _functinsCount = 0);
  ~OpenGlWidget();
	
protected:
	void initializeGL();
	void resizeGL( int width, int height );
	void paintGL();
  void keyPressEvent( QKeyEvent *e );
  void wheelEvent ( QWheelEvent * e );
  void mousePressEvent( QMouseEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void drawCoordinateSystem();
  void drawTriangles(Point* points, int count, float lineWidth, float red, float green, float blue);
  void drawFunctions();
  void drawFunction(DiscreteFunction& function);
  void drawCoordinateAxis(float x, float y, float red, float green, float blue);
  void getPointIndex(int x, int y);
  void initForGluProjection(double* model, double* proj, int* view);
  void initVariables();
  double getDist(Point point, double* model, double* proj, int* view, double* tmp, int x, int y);


protected slots:    
  void zoomin();
  void zoomout();
  void changeShowCoordinateSystem();
  void changeShowPoints();
  void addControlPoints();
  void removeControlPoints();
  void addPatches();
  void removePatches();
  void resetSlot();
};

#endif
