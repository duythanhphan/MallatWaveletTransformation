#ifndef __main_window_h__

#define __main_window_h__

#include <QMenuItem>
#include <QMainWindow>
#include <qapplication.h>
#include <qgl.h>
#include <OpenGlWidget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    OpenGlWidget* gl;
    void createMenuItem(QString label, QString iconLocation, QString shortCut, QWidget *addTo, const char * func, QWidget* signalTo);

public:
    MainWindow(QWidget *parent=0);

protected slots:
    void closeSlot();
};

#endif
