#include "ivan_qtsensortest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QString aa;
	//QString aa = "D:/DEVEL/Builder/Sandbox/Ivan/Ivan_QtSensorTest/Win32/Debug";
	//QCoreApplication::addLibraryPath( aa );

	QApplication a(argc, argv);

	aa = QCoreApplication::applicationDirPath();
	QCoreApplication::addLibraryPath( aa );


	Ivan_QtSensorTest w;
	w.show();
	return a.exec();
}
