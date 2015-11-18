#ifndef IVAN_QTSENSORTEST_H
#define IVAN_QTSENSORTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_ivan_qtsensortest.h"

#include <QCompass>
#include <QCompassReading>

class Ivan_QtSensorTest : public QMainWindow
{
	Q_OBJECT

public:
	Ivan_QtSensorTest(QWidget *parent = 0);
	~Ivan_QtSensorTest();

public slots:
    void update();
    void error(int);

private:
	Ui::Ivan_QtSensorTestClass ui;

    QCompass *compass;
    QCompassReading *compass_reading;
};

#endif // IVAN_QTSENSORTEST_H
