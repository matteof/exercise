#include "ivan_qtsensortest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);

	QString aa = QCoreApplication::applicationDirPath();

	QCoreApplication::addLibraryPath( aa );


	Ivan_QtSensorTest w;
	w.show();
	return a.exec();
}
