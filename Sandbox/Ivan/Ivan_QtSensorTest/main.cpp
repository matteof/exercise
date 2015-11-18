#include "ivan_qtsensortest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Ivan_QtSensorTest w;
	w.show();
	return a.exec();
}
