#include <OpenGlWidget.h>
#include <QKeyEvent>
#include <QMenuBar>
#include <QAction>
#include <qapplication.h>
#include <OpenGlWidget.h>
#include <MainWindow.h>
#include <DiscreteFunction.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) 
{
    Point* points = new Point[201];
    DiscreteFunction* function = new DiscreteFunction(points, 201);
    for (int i = -100; i < 101; i++) {
        points[i + 100].setX(i);
        points[i + 100].setY(0.05 * i * i);
    }

    this->gl = new OpenGlWidget(this, function);
    this->resize(800, 640);
    this->setWindowTitle("KGM Project 80307");
    this->setCentralWidget(gl);
    QMenuBar* menubar = menuBar();
    QMenu* file = menubar->addMenu("&File");
    createMenuItem("Reset", NULL, "Ctrl+N", file, SLOT(resetSlot()), gl);
    file->addSeparator();
    createMenuItem("Exit", NULL, "Esc", file, SLOT(closeSlot()), this);
    QMenu* edit = menubar->addMenu("&Edit");
    createMenuItem("Zoom in", NULL, "Ctrl+C", edit, SLOT(zoomin()), gl);
    createMenuItem("Zoom Out", NULL, "Ctrl+D", edit, SLOT(zoomout()), gl);
    edit->addSeparator();
    createMenuItem("Increase the number of control points", NULL, "Ctrl+A", edit, SLOT(addControlPoints()), gl);
    createMenuItem("Decrease the number of control points", NULL, "Ctrl+W", edit, SLOT(removeControlPoints()), gl);
    edit->addSeparator();
    createMenuItem("Increase the number of patches", NULL, "Ctrl+R", edit, SLOT(addPatches()), gl);
    createMenuItem("Decrease the number of patches", NULL, "Ctrl+T", edit, SLOT(removePatches()), gl);
    QMenu* view = menubar->addMenu("&View");
    createMenuItem("Change coordinate system visibility", NULL, "Ctrl+S", view, SLOT(changeShowCoordinateSystem()), gl);
    createMenuItem("Change points visibility", NULL, "Ctrl+P", view, SLOT(changeShowPoints()), gl);
}

void MainWindow::createMenuItem(QString label, QString iconLocation, QString shortCut, QWidget *addTo, const char * func, QWidget* signalTo) 
{
    QAction* tmp = new QAction(label, this);
    tmp->setShortcut(shortCut);
    connect(tmp, SIGNAL(triggered()), signalTo, func);
    addTo->addAction(tmp);
}

void MainWindow::closeSlot() 
{
    gl->close();
    this->close();
}
