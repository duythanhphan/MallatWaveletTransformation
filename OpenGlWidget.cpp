#include <OpenGlWidget.h>
#include <QKeyEvent>
#include <QMenuBar>
#include <QAction>
#include <qapplication.h>
#include <iostream>
#include <GL/glu.h>
#include <Qt>

using namespace std;

// initialize variables
OpenGlWidget::OpenGlWidget(QWidget *parent, DiscreteFunction* _functions, int _functinsCount) : QGLWidget(parent) {
    functions = _functions;
    functionsCount = _functinsCount;

    initVariables();
    setMouseTracking(false);
    pressedButton = Qt::NoButton;
}

// free variables
OpenGlWidget::~OpenGlWidget() {
    //delete controlPoints;
    //delete surfacePoints;
}

// pass some basic GL settings
void OpenGlWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);                                                       
                                                                                   
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                                          
    glEnable ( GL_DEPTH_TEST );                                                    
    // enables anti-aliasing                                                       
    glEnable ( GL_POINT_SMOOTH );                                                  
    glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);                                      
    glFlush ();
}

// handles resize events
void OpenGlWidget::resizeGL(int width, int height)
{
    height = height ? height : 1;

    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// handles repaint events
void OpenGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();

    glTranslatef(translationX, translationY, BASE_ZOOM * zoom);

    drawCoordinateSystem();
    drawFunctions();
}

void OpenGlWidget::drawFunctions() {
    for(int i = 0; i < functionsCount; i++) {
      drawFunction(functions[i]);
    }
}

void OpenGlWidget::drawFunction(DiscreteFunction& function, float red, float green, float blue) {
    glEnable ( GL_LINE_SMOOTH );                                                

    glEnable (GL_BLEND);                                                        
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                         

    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);                                    
    glFlush();                                                                  
    glColor3f (1.0f, 1.0f, 1.0f);
    glLineWidth ( 2 );

    glBegin (GL_LINE_STRIP);

    for (int i = 0; i < function.getCount(); i++) {
        glVertex2f(10 * function.getPoint(i).getX(), 10 * function.getPoint(i).getY());
    }
    
    glEnd ();
}

// handles key board events
void OpenGlWidget::keyPressEvent( QKeyEvent *e )
{
    switch( e->key() )
    {
        case Qt::Key_Escape:
            close();
    }
}

// handles mouse press events
void OpenGlWidget::mousePressEvent( QMouseEvent * e )
{
    cout << e->x() << " " << e->y() << endl;
    if(e->button() == Qt::LeftButton) {
        currx = e->x();
        curry = e->y();
        e->accept();
        pressedButton = Qt::LeftButton;
    }
    //else if(e->button() == Qt::RightButton) {
        //getPointIndex(e->x(), e-> y());
        //pressedButton = Qt::RightButton;
    //}
    else {
        currx = -1;
        curry = -1;
        pressedButton = Qt::NoButton;
    }
}

// handles move move events
void OpenGlWidget::mouseMoveEvent( QMouseEvent * e ) 
{
    if(currx >= 0 && curry >= 0) {
        int newx = e->x();
        int newy = e->y();
        if(pressedButton == Qt::LeftButton) {
            // we want to move the camera -> compute the translation
            int dx = newx - currx;
            int dy = newy - curry;
            currx = newx;
            curry = newy;
            translationX += (zoom / 5) * TRANSLATION_FACTOR * dx;
            translationY -= (zoom / 5) * TRANSLATION_FACTOR * dy;
        }
        //else if(pressedButton == Qt::RightButton) {
            //// we want to move a point -> compute the new coordinates
            //double model[16];
            //double proj[16];
            //int view[4];
            //initForGluProjection(model, proj, view);
            //newy = view[3] - newy;
            //double x, y, z;
            ////gluUnProject ( newx, newy, tmpZ, model, proj, view, &x, &y, &z );
            //controlPoints[index].setCoordinates(x, y, z);
            //initSurfacePoints();
        //} 
    }

    e->accept();
    updateGL();
}

// handles mouse wheel event
void OpenGlWidget::wheelEvent( QWheelEvent * e ) 
{
    int delta = e->delta();
    if(delta > 0) {
        zoomin();
    }
    else {
        zoomout();
    }
    e->accept();
}

// zooms the scene
void OpenGlWidget::zoomin() 
{
    if(zoom / 2 > MIN_ZOOM) {
        zoom /= 2;
        updateGL();
    }
}

// zooms out the scene
void OpenGlWidget::zoomout() 
{
    if(zoom * 1.5 < MAX_ZOOM) {
        zoom *= 1.5;
        updateGL();
    }
}

// makes coordinate system visibility be !showCoordinateSystem
void OpenGlWidget::changeShowCoordinateSystem() 
{
    showCoordinateSystem = !showCoordinateSystem;
    updateGL();
}

// makes control points visibility be !showPoints
void OpenGlWidget::changeShowPoints() 
{
    showPoints = !showPoints;
    updateGL();
}

// adds control points and reinitialize them
void OpenGlWidget::addControlPoints() 
{
}

// removes control points and reinitialize them
void OpenGlWidget::removeControlPoints()
{
}

// adds patches and recomputes the surface
void OpenGlWidget::addPatches() 
{
}

// removes patches and recomputes the surface
void OpenGlWidget::removePatches()
{
}

// resets the scene
void OpenGlWidget::resetSlot()
{
   initVariables();
   updateGL();
}

// draws the coordinate system
void OpenGlWidget::drawCoordinateSystem() 
{
    if(!showCoordinateSystem) {
        return;
    }

    glLineWidth ( COORD_SYSTEM_WIDTH );

    drawCoordinateAxis(100* COORD_AXIS_LEN, 0.0, 1.0, 0.0, 0.0);
    drawCoordinateAxis(0.0, 100 * COORD_AXIS_LEN, 0.0, 1.0, 0.0);
}

// given a set of points draws triangles
void OpenGlWidget::drawTriangles(Point* points, int count, float lineWidth, float red, float green, float blue) 
{
    //int i, j;
    //int k, l, m;
    //int curr = 0;
    //int n = (count + 1) * (count + 2) / 2;

    //glEnable ( GL_LINE_SMOOTH );

    //glEnable (GL_BLEND);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glFlush();
    //glColor3f ( red, green, blue);
    //glLineWidth ( lineWidth );
    //// make it clearer
    //for (i = 0; i < count; ++i) {
        //for (j = 0; j <= i; ++j, ++curr) {
            //k = curr; l = curr+i+1; m = curr+i+2;
            //glBegin ( GL_LINE_LOOP );
                //glVertex3d ( points[k].getX(), points[k].getY(), points[k].getZ() );
                //glVertex3d ( points[l].getX(), points[l].getY(), points[l].getZ() );
                //glVertex3d ( points[m].getX(), points[m].getY(), points[m].getZ() );
            //glEnd ();           
        //}
    //}

    //glColor3f ( red, green, blue );
    //glPointSize ( lineWidth * 5 );
    //glBegin ( GL_POINTS );
    //for (i = 0; i < n; i++) {
        //glVertex3d ( points[i].getX(), points[i].getY(), points[i].getZ() );
    //}
    //glEnd ();
}

// finds the point that is closes to the position where the click event occured
void OpenGlWidget::getPointIndex (int x, int y) {
    //double model[16];
    //double proj[16];
    //int view[4];
    //int i;
    //int n = (points + 1) * (points + 2) / 2;
    //double min = -1, dist;
    //double tmp[3];
    
    //initForGluProjection(model, proj, view);
    //y = view[3] - y;

    //min = getDist(controlPoints[0], model, proj, view, tmp, x, y);
    //tmpZ = tmp[2];
    //index = 0;

    //for (i = 1; i < n; ++i) {
        //dist = getDist(controlPoints[i], model, proj, view, tmp, x, y);
        //if ( dist < min || (dist == min && tmp[2] < tmpZ) ) {
            //min = dist;
            //index = i;
            //tmpZ = tmp[2];
        //}
    //}
}

// draws only one of the coordinate axis
void OpenGlWidget::drawCoordinateAxis(float x, float y, float red, float green, float blue) 
{
    glColor3f (red, green, blue);
    glBegin (GL_LINE_STRIP);
        glVertex2f ( -x, -y );
        glVertex2f (  x,  y);
    glEnd ();

}

// initialize some variables for glu projection
void OpenGlWidget::initForGluProjection(double* model, double* proj, int* view) 
{
    glLoadIdentity();
    glTranslatef(0.0f, 0.0, BASE_ZOOM * zoom);

    glGetDoublev ( GL_MODELVIEW_MATRIX, model );
    glGetDoublev ( GL_PROJECTION_MATRIX, proj );
    glGetIntegerv ( GL_VIEWPORT, view );
}

// initialize some variablels
void OpenGlWidget::initVariables() 
{
    zoom_step = 2;
    zoom = 10;
    showCoordinateSystem = showPoints = true;
    translationX = 0.0f;
    translationY = 0.0f;
}

// finds the distance from 3d point projected on the screen to the place where the click occured
double OpenGlWidget::getDist(Point point, double* model, double* proj, int* view, double* tmp, int x, int y)
{
    //gluProject ( point.getX(), point.getY(), point.getZ(), model, proj, view, tmp, tmp+1, tmp+2 );
    return (tmp[0] - x) * (tmp[0] - x) + (tmp[1] - y) * (tmp[1] - y);
}

