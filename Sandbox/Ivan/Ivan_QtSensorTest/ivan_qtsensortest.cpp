#include "ivan_qtsensortest.h"

#include <QSensor>
#include <QDateTime>
#include <QCompassReading>
#include <QDataStream>


Ivan_QtSensorTest::Ivan_QtSensorTest(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);


	// global information
	QList<QByteArray> sensorList = QSensor::sensorTypes();
	mGlobalInitView->append( "Sensor list length: " + QString::number( sensorList.size() ) );

	foreach( QByteArray sensorName, sensorList ) 
		mGlobalInitView->append( "Sensor: " + sensorName );

	mGlobalInitView->append( "" );


	// global information
	QStringList posSources =  QGeoPositionInfoSource::availableSources();
	mGlobalInitView->append( "available position sources: " + QString::number( posSources.size() ) );

	foreach( QString posSrc, posSources ) 
		mGlobalInitView->append( "Pos src: " + posSrc );

	mGlobalInitView->append( "" );


	// compass
	mCompassInitView->append( "Initialization..." );

	mCompass = new QCompass( this );
	mCompass->setDataRate( 2 );
	connect( mCompass, &QCompass::readingChanged, this, &Ivan_QtSensorTest::CompassUpdate );
	connect( mCompass, &QCompass::sensorError, this, &Ivan_QtSensorTest::CompassError );
	mCompass->start();

	mCompassInitView->append( "QCompass isBusy: " + QString::number(mCompass->isBusy()) );
	mCompassInitView->append( "QCompass isConnectedToBackend: " + QString::number(mCompass->isConnectedToBackend()) );
	mCompassInitView->append( "QCompass isActive: " + QString::number(mCompass->isActive()) );


	// Gyroscope
	mGyroscopeInitView->append( "Initialization..." );

	mGyroscope = new QGyroscope( this );
	mGyroscope->setDataRate( 2 );
	connect( mGyroscope, &QGyroscope::readingChanged, this, &Ivan_QtSensorTest::GyroscopeUpdate );
	connect( mGyroscope, &QGyroscope::sensorError, this, &Ivan_QtSensorTest::GyroscopeError );
	mGyroscope->start();

	mGyroscopeInitView->append( "QGyroscope isBusy: " + QString::number(mGyroscope->isBusy()) );
	mGyroscopeInitView->append( "QGyroscope isConnectedToBackend: " + QString::number(mGyroscope->isConnectedToBackend()) );
	mGyroscopeInitView->append( "QGyroscope isActive: " + QString::number(mGyroscope->isActive()) );


	// Positioning
	mPositionInitView->append( "Initialization..." );

	mPosSrc = QGeoPositionInfoSource::createDefaultSource( this );
	if( !mPosSrc )
	{
		mPositionInitView->append( "createDefaultSource fails" );
	}
	else
	{
		connect( mPosSrc, &QGeoPositionInfoSource::positionUpdated, this, &Ivan_QtSensorTest::PositionUpdate );
		connect( mPosSrc, SIGNAL( error( QGeoPositionInfoSource::Error ) ), this, SLOT( PositionError( QGeoPositionInfoSource::Error ) ) );
		mPosSrc->setUpdateInterval( 1000 );
		mPosSrc->startUpdates();

		mPositionInitView->append( "sourceName: " + mPosSrc->sourceName() );
		mPositionInitView->append( "supportedPositioningMethods: " + QString::number(mPosSrc->supportedPositioningMethods(), 16) );
	}
}

Ivan_QtSensorTest::~Ivan_QtSensorTest()
{
}

void Ivan_QtSensorTest::CompassUpdate()
{
	mCompassStatusView->clear();

	QCompassReading* compass_reading = mCompass->reading();
	if( compass_reading == 0 )
	{
		mCompassStatusView->append( "Compass: UNAVAILABLE" );
		return;
	}

	QString text_compass = 
		"timestamp = " + QString::number( compass_reading->timestamp() ) + "\n"
		"azimuth = " + QString::number( compass_reading->azimuth() ) + "\n"
		"calibration level = " + QString::number( compass_reading->calibrationLevel() ) + "\n";

	mCompassStatusView->append( text_compass );	
}

void Ivan_QtSensorTest::CompassError( int error ) 
{
	mCompassStatusView->clear();
	mCompassStatusView->append( "Compass error num: " + QString::number(error) );	
}

void Ivan_QtSensorTest::GyroscopeUpdate()
{
	mGyroscopeStatusView->clear();

	QGyroscopeReading* gyroscope_reading = mGyroscope->reading();
	if( gyroscope_reading == 0 )
	{
		mGyroscopeStatusView->append( "Gyroscope: UNAVAILABLE" );
		return;
	}

	QString text_Gyroscope = 
		"timestamp = " + QString::number( gyroscope_reading->timestamp() ) + "\n"
		"x = " + QString::number( gyroscope_reading->x() ) + " deg/sec\n"
		"y = " + QString::number( gyroscope_reading->y() ) + " deg/sec\n"
		"z = " + QString::number( gyroscope_reading->z() ) + " deg/sec\n";

	mGyroscopeStatusView->append( text_Gyroscope );	
}

void Ivan_QtSensorTest::GyroscopeError( int error ) 
{
	mGyroscopeStatusView->clear();
	mGyroscopeStatusView->append( "Gyroscope error num: " + QString::number(error) );	
}

void Ivan_QtSensorTest::PositionUpdate( const QGeoPositionInfo &info )
{
	mPositionStatusView->clear();

	QByteArray ba;
	QDataStream ds( &ba, QIODevice::WriteOnly );

	ds << info;

	mPositionStatusView->append( QString(ba) );	
}

void Ivan_QtSensorTest::PositionError( QGeoPositionInfoSource::Error positioningError )
{
	mPositionStatusView->clear();
	// \todo
	mPositionStatusView->append( "Position error: " + QString::number(positioningError) );	
}
