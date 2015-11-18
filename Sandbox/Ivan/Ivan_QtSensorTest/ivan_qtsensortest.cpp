#include "ivan_qtsensortest.h"

#include <QSensor>
#include <QDateTime>
#include <QMessageBox>


Ivan_QtSensorTest::Ivan_QtSensorTest(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QList<QByteArray> sensorList = QSensor::sensorTypes();
	ui.init->append( "Sensor list length: " + QString::number( sensorList.size() ).toUtf8() );

	foreach( QByteArray sensorName, sensorList ) 
	{
		ui.init->append( "Sensor: " + sensorName );
	}



	///////////////
	ui.init->append( "Initialization..." );

	compass = new QCompass( this );
	connect( compass, SIGNAL( readingChanged() ), this, SLOT( update() ) );
	connect( compass, SIGNAL( sensorError( int ) ), this, SLOT( error( int ) ) );
	compass->setDataRate( 1 );
	compass->start();
	if( compass->isBusy() ) {
		ui.init->append( "QCompass is busy..." );
	}
	if( compass->isConnectedToBackend() ) {
		ui.init->append( "QCompass is connected to backend..." );
	}
	if( compass->isActive() ) {
		ui.init->append( "QCompass isActive..." );
	}

	ui.init->append( "Waiting for sensors..." );
}

Ivan_QtSensorTest::~Ivan_QtSensorTest()
{
	delete compass;
}

void Ivan_QtSensorTest::update()
{
    QString text_compass;

    ui.textEdit->clear();

   // accel_reading = accel->reading();

    compass_reading = compass->reading();
    if(compass_reading != 0) {
        text_compass = QDateTime::currentDateTime().toString() +
                + "\nCompass:  azimuth = " + QString::number(compass_reading->azimuth());
                + "\ncalibration level = " + QString::number(compass_reading->calibrationLevel());

        ui.textEdit->append(text_compass);
    }
    else {
        text_compass = "\nCompass: UNAVAILABLE";
        ui.textEdit->append(text_compass);
    }
}


void Ivan_QtSensorTest::error(int erreur) {
    QMessageBox::critical(this, "Erreur", "Erreur num : " + QString::number(erreur).toUtf8());
}
