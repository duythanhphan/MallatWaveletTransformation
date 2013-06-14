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
    MallatWaveletTransformation mallatWaveletTransformation;

    Point* points = new Point[2048];
    Point* points2 = new Point[2048];
    int sign = -1;

    for (int i = 0; i < 2048; i+=4) {
        if (i % 4 == 0) {
            sign *= -1;
        }
        
        points2[i].setX(i);
        points2[i].setY(0);

        points2[i + 1].setX(i + 1);
        points2[i + 1].setY(sign * 100);

        points2[i + 2].setX(i + 2);
        points2[i + 2].setY(sign * 200);

        points2[i + 3].setX(i + 3);
        points2[i + 3].setY(sign * 100);

    }

    mallatWaveletTransformation.setOriginalFunction(new DiscreteFunction(points2, 2048));

    this->gl = new OpenGlWidget(this, mallatWaveletTransformation.getFunctions(), 2);
    this->resize(800, 640);

    this->setWindowTitle("Mallat wavelet transformation");
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
