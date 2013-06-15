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
        points2[i + 1].setY(i * sign * 1);

        points2[i + 2].setX(i + 2);
        points2[i + 2].setY(i * sign * 2);

        points2[i + 3].setX(i + 3);
        points2[i + 3].setY(i * sign * 1);

    }

    mallatWaveletTransformation.setOriginalFunction(new DiscreteFunction(points2, 2048));

    mallatWaveletTransformation.getFunctionsDifferance();

    int functionsCount = 4;
    showFunction = new int[functionsCount];
    for (int i = 0; i < functionsCount; i++) {
        showFunction[i] = 1;
    }

    this->gl = new OpenGlWidget(this, mallatWaveletTransformation.getFunctions(), functionsCount);
    this->gl->setVisible(showFunction, functionsCount);

    this->resize(800, 640);

    this->setWindowTitle("Mallat wavelet transformation");
    this->setCentralWidget(gl);

    QMenuBar* menubar = menuBar();
    QMenu* file = menubar->addMenu("&File");
    createMenuItem("Reset view", "Ctrl+R", file, SLOT(resetSlot()), gl);
    file->addSeparator();
    createMenuItem("Exit", "Esc", file, SLOT(closeSlot()), this);
    QMenu* view = menubar->addMenu("&View");
    createMenuItem("Zoom in", "Ctrl++", view, SLOT(zoomin()), gl);
    createMenuItem("Zoom Out", "Ctrl+-", view, SLOT(zoomout()), gl);
    view->addSeparator();

    createMenuItem("Change coordinate system visibility",
            "Ctrl+S",
            view,
            SLOT(changeShowCoordinateSystem()),
            gl);
    createMenuItem("Change function visibility",
            "Ctrl+F",
            view,
            SLOT(changeFunctionVisibility()),
            gl);
    createMenuItem("Change coeficients visibility",
            "Ctrl+C",
            view,
            SLOT(changeCoeficientsFunctionVisibility()),
            gl);
    createMenuItem("Change calculated function visibility",
            "Ctrl+A",
            view,
            SLOT(changeCalculatedFunctionVisibility()),
            gl);
    createMenuItem("Change functions difference visibility",
            "Ctrl+D",
            view,
            SLOT(changeFunctionDifferenceVisibility()),
            gl);
}

void MainWindow::createMenuItem(QString label, QString shortCut, QWidget *addTo, const char * func, QWidget* signalTo) 
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
