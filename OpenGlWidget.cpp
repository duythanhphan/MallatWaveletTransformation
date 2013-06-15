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
    float colors[][3] = {
        {0.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 0.25f},
        {0.25f, 1.0f, 0.25f},
        {0.0f, 1.0f, 0.0f}};
    for(int i = 0; i < functionsCount; i++) {
      int colorIndex = i & 3;
      if (showFunction[i]) {
          drawFunction(functions[i], colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
      }
    }
}

void OpenGlWidget::drawFunction(DiscreteFunction& function, float red, float green, float blue) {
    glEnable( GL_LINE_SMOOTH );                                                

    glEnable(GL_BLEND);                                                        
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                         

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);                                    
    glFlush();                                                                  
    glColor3f(red, green, blue);
    glLineWidth ( 2 );

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < function.getCount(); i++) {
        glVertex2f(function.getPoint(i).getX(), function.getPoint(i).getY());
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
    } else {
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
            int zoomFactor = (zoom / 5 > 0) ? zoom / 5 : 1;

            translationX += zoomFactor * TRANSLATION_FACTOR * dx;
            translationY -= zoomFactor * TRANSLATION_FACTOR * dy;
        }
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
    showCoordinateSystem = true;
    translationX = 0.0f;
    translationY = 0.0f;
}

// finds the distance from 3d point projected on the screen to the place where the click occured
double OpenGlWidget::getDist(Point point, double* model, double* proj, int* view, double* tmp, int x, int y)
{
    //gluProject ( point.getX(), point.getY(), point.getZ(), model, proj, view, tmp, tmp+1, tmp+2 );
    return (tmp[0] - x) * (tmp[0] - x) + (tmp[1] - y) * (tmp[1] - y);
}


void OpenGlWidget::setVisible(int* show, int length) {
    if (length < functionsCount) {
        std::cerr << "Set visible invoked with too small show array\n";
    }

    this->showFunction = show;
}

void OpenGlWidget::changeFunctionVisibility() {
    showFunction[0] = !showFunction[0];
    updateGL();
}

void OpenGlWidget::changeCoeficientsFunctionVisibility() {
    showFunction[1] = !showFunction[1];
    updateGL();
}

void OpenGlWidget::changeCalculatedFunctionVisibility() {
    showFunction[2] = !showFunction[2];
    updateGL();
}

void OpenGlWidget::changeFunctionDifferenceVisibility() {
    showFunction[3] = !showFunction[3];
    updateGL();
}
