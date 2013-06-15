#include <QKeyEvent>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>

#include <iostream>
#include <fstream>

#include <MainWindow.h>
#include <DiscreteFunction.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) 
{
    int functionsCount = 4;
    showFunction = new int[functionsCount];
    for (int i = 0; i < functionsCount; i++) {
        showFunction[i] = 1;
    }

    this->gl = new OpenGlWidget(this);
    this->gl->setVisible(showFunction, functionsCount);

    this->resize(800, 640);

    this->setWindowTitle("Mallat wavelet transformation");
    this->setCentralWidget(gl);

    QMenuBar* menubar = menuBar();
    QMenu* file = menubar->addMenu("&File");

    createMenuItem("Open file", "Ctrl+O", file, SLOT(importSlot()), this);
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

    connect(this,
            SIGNAL(functionsChanged(DiscreteFunction*, int)),
            gl,
            SLOT(updateFunctions(DiscreteFunction*, int)));

    //emit functionsChanged(mallatWaveletTransformation.getFunctions(), functionsCount);
}

void MainWindow::createMenuItem(QString label, QString shortCut, QWidget *addTo, const char * func, QWidget* signalTo) 
{
    QAction* tmp = new QAction(label, this);
    tmp->setShortcut(shortCut);
    connect(tmp, SIGNAL(triggered()), signalTo, func);
    addTo->addAction(tmp);
}

void MainWindow::importSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"),
            "",
            tr("Files (*.*)"));
    std::cout << fileName.toStdString() << std::endl;

    //QFile file(fileName);

    //QTextStream in(&file);

    std::ifstream in(fileName.toStdString().c_str());

    int pointsCount;

    in >> pointsCount;

    std::cout << pointsCount << std::endl;

    Point* points = new Point[pointsCount];

    for (int i = 0; i < pointsCount; i++) {
        points[i].setX(i);

        double tmp;
        in >> tmp;

        points[i].setY(tmp);

        std::cout << points[i].getX() << " " << points[i].getY() << std::endl;
    }

    DiscreteFunction* originalFuntion = new DiscreteFunction(points, pointsCount);

    MallatWaveletTransformation mallatWaveletTransformation = 
        MallatWaveletTransformation();

    mallatWaveletTransformation.setOriginalFunction(originalFuntion);

    emit functionsChanged(mallatWaveletTransformation.getFunctions(), 4);
}

void MainWindow::closeSlot() 
{
    gl->close();
    this->close();
}
