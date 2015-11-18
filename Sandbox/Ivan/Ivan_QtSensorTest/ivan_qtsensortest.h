#ifndef IVAN_QTSENSORTEST_H
#define IVAN_QTSENSORTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_ivan_qtsensortest.h"

class Ivan_QtSensorTest : public QMainWindow
{
	Q_OBJECT

public:
	Ivan_QtSensorTest(QWidget *parent = 0);
	~Ivan_QtSensorTest();

private:
	Ui::Ivan_QtSensorTestClass ui;
};

#endif // IVAN_QTSENSORTEST_H
