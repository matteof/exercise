#include "ivan_qtsensortest.h"

#include <QSensor>
#include <QDateTime>
#include <QCompassReading>
#include <QDataStream>
#include <QtPositioning/QGeoSatelliteInfoSource>
#include <QMutexLocker>
#include <QThread>
#include <QDebug>
#include <QtGlobal>


Ivan_QtSensorTest::Ivan_QtSensorTest(QWidget *parent)
:	QMainWindow(parent)
,	mPosCount( 0 )
//,  mPosLogMutex( QMutex::Recursive )
,  mInPositionUpdated(false)
{
	setupUi(this);

	setWindowTitle( "113" );

	QAction* mStartAll = new QAction( QString("start"), this );
	connect( mStartAll, &QAction::triggered, this, &Ivan_QtSensorTest::StartAll );

	mToolBar->addAction( mStartAll );
}

void	Ivan_QtSensorTest::StartAll()
{
	mPreferredPositioningMethodFrame->setDisabled( true );

	// global information
	QList<QByteArray> sensorList = QSensor::sensorTypes();
	mGlobalInitView->append( "Sensor list length: " + QString::number( sensorList.size() ) );

	foreach( QByteArray sensorName, sensorList ) 
		mGlobalInitView->append( "Sensor: " + sensorName );

	mGlobalInitView->append( "" );


	QStringList posSources =  QGeoPositionInfoSource::availableSources();
	mGlobalInitView->append( "QGeoPositionInfoSource::availableSources: " + QString::number( posSources.size() ) );

	foreach( QString posSrc, posSources ) 
		mGlobalInitView->append( "Pos src: " + posSrc );

	mGlobalInitView->append( "" );


	posSources =  QGeoSatelliteInfoSource::availableSources();
	mGlobalInitView->append( "QGeoSatelliteInfoSource::availableSources: " + QString::number( posSources.size() ) );

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
	mPositionView->append( "*** Initialization start" );

	mPosSrc = QGeoPositionInfoSource::createDefaultSource( this );

	LogPosSrcInfo();

	if( mPosSrc )
	{
		connect( mPosSrc, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT(PositionError(QGeoPositionInfoSource::Error)) );
		connect( mPosSrc, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(PositionUpdated(QGeoPositionInfo)) );
		connect( mPosSrc, SIGNAL(updateTimeout()), this, SLOT(PositionUpdateTimeout()) );

		if( mPreferredPositioningMethodCombo->currentText() == "AllPositioningMethods" )
			mPosSrc->setPreferredPositioningMethods( QGeoPositionInfoSource::AllPositioningMethods );
		else if( mPreferredPositioningMethodCombo->currentText() == "SatellitePositioningMethods" )
			mPosSrc->setPreferredPositioningMethods( QGeoPositionInfoSource::SatellitePositioningMethods );
		else if( mPreferredPositioningMethodCombo->currentText() == "NonSatellitePositioningMethods" )
			mPosSrc->setPreferredPositioningMethods( QGeoPositionInfoSource::NonSatellitePositioningMethods );
		else if( mPreferredPositioningMethodCombo->currentText() == "NoPositioningMethods" )
			mPosSrc->setPreferredPositioningMethods( QGeoPositionInfoSource::NoPositioningMethods );
	
		//mPosSrc->setUpdateInterval( 1000 );
		

		mPosSrc->startUpdates();
		mPosSrc->requestUpdate(1);
	}

	LogPosSrcInfo();

	mPositionView->append( "*** Initialization end" );
	mPositionView->append( "" );


	// compass
	mRotationInitView->append( "Initialization..." );

	mRotation = new QRotationSensor( this );
	mRotation->setDataRate( 2 );
	connect( mRotation, &QCompass::readingChanged, this, &Ivan_QtSensorTest::RotationUpdate );
	connect( mRotation, &QCompass::sensorError, this, &Ivan_QtSensorTest::RotationError );
	mRotation->start();

	mRotationInitView->append( "QRotation isBusy: " + QString::number(mRotation->isBusy()) );
	mRotationInitView->append( "QRotation isConnectedToBackend: " + QString::number(mRotation->isConnectedToBackend()) );
	mRotationInitView->append( "QRotation isActive: " + QString::number(mRotation->isActive()) );
}

Ivan_QtSensorTest::~Ivan_QtSensorTest()
{
}

void Ivan_QtSensorTest::paintEvent(QPaintEvent * event)
{
	//QMutexLocker a( &mPosLogMutex );
	qDebug() << __FUNCTION__ << "b - posCount" << mPosCount << "- thread" << (unsigned)(QThread::currentThread());

	QMainWindow::paintEvent( event );

	qDebug() << __FUNCTION__ << "e - posCount" << mPosCount << "- thread" << (unsigned)(QThread::currentThread());
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

void Ivan_QtSensorTest::PositionUpdated(const QGeoPositionInfo &info)
{
	//QMutexLocker a( &mPosLogMutex );
	++mPosCount;
	int thePC = mPosCount;
	qDebug() << __FUNCTION__ << "b - posCount" << thePC << "- thread" << (unsigned)(QThread::currentThread());
	if( mInPositionUpdated )
		return;
	mInPositionUpdated = true;

	mPositionView->clear();
   LogPosSrcInfo();

	QGeoCoordinate  thePos =  info.coordinate();
	mPositionView->append( QString("QGeoCoordinate: %1 %2 %3 \n Accuracy h %4 v %5\n")
		.arg( thePos.isValid() )
		.arg( thePos.latitude(), 0, 'g', 16 )
		.arg( thePos.longitude(), 0, 'g', 16 ) 
		.arg( info.attribute(QGeoPositionInfo::HorizontalAccuracy) )
		.arg( info.attribute(QGeoPositionInfo::VerticalAccuracy) )
		);

	mPositionView->append( "" );

	qDebug() << __FUNCTION__ << "e - posCount" << thePC << "- thread" << (unsigned)(QThread::currentThread());
	mInPositionUpdated = false;
}

void Ivan_QtSensorTest::PositionError( QGeoPositionInfoSource::Error positioningError )
{
	//QMutexLocker a( &mPosLogMutex );
	++mPosCount;
	int thePC = mPosCount;
	qDebug() << __FUNCTION__ << "b - posCount" << thePC << "- thread" << (unsigned)(QThread::currentThread());

	LogPosSrcInfo();

	mPositionView->append( "Position error: " + PosErrorToString(positioningError) );
	mPositionView->append( "" );

	qDebug() << __FUNCTION__ << "e - posCount" << thePC << "- thread" << (unsigned)(QThread::currentThread());
}

void Ivan_QtSensorTest::PositionUpdateTimeout()
{
	//QMutexLocker a( &mPosLogMutex );
	++mPosCount;
	int thePC = mPosCount;
	qDebug() << __FUNCTION__ << "b - posCount" << thePC << "- thread" << (unsigned)(QThread::currentThread());

	LogPosSrcInfo();

	mPositionView->append( "PositionUpdateTimeout" );
	mPositionView->append( "" );

	qDebug() << __FUNCTION__ << "e - posCount" << thePC << "- thread" << (unsigned)(QThread::currentThread());
}

void Ivan_QtSensorTest::RotationUpdate()
{
	mRotationStatusView->clear();

	QRotationReading* reading = mRotation->reading();
	if( reading == 0 )
	{
		mCompassStatusView->append( "Rotation: UNAVAILABLE" );
		return;
	}

	QString text = 
		"timestamp = " + QString::number( reading->timestamp() ) + "\n"
		"x = " + QString::number( reading->x() ) + " deg\n"
		"y = " + QString::number( reading->y() ) + " deg\n"
		"z = " + QString::number( reading->z() ) + " deg\n";

	mRotationStatusView->append( text );	
}

void Ivan_QtSensorTest::RotationError( int error ) 
{
	mRotationStatusView->clear();
	mRotationStatusView->append( "Rotation error num: " + QString::number(error) );	
}


void Ivan_QtSensorTest::LogPosSrcInfo() 
{
	mPositionView->append( "*** LOG COUNT: " + QString::number( mPosCount ) );
	mPositionView->append( "therad id: " + QString::number( (unsigned)(QThread::currentThread()) ) );

	mPositionView->append( "mPosSrc ptr: " + QString::number( (unsigned)(mPosSrc), 16 ) );

	if( !mPosSrc )
		return;

	mPositionView->append( "error: " + PosErrorToString(mPosSrc->error()) );

	QGeoCoordinate  thePos = mPosSrc->lastKnownPosition().coordinate();
	mPositionView->append( QString( "lastKnownPosition : %1 %2 %3" ).arg( thePos.isValid() ).arg( thePos.latitude() ).arg( thePos.longitude() ) );
	mPositionView->append( QString( "lastKnownPosition: " ) + thePos.toString() );

	mPositionView->append( "minimumUpdateInterval: " + QString::number( mPosSrc->minimumUpdateInterval() ) );

	QString posMethodsStr = PosMethodToString( mPosSrc->preferredPositioningMethods() );
	mPositionView->append( "preferredPositioningMethods: " + posMethodsStr + " " + QString::number( mPosSrc->preferredPositioningMethods(), 16 ) );

	mPositionView->append( "sourceName: " + mPosSrc->sourceName() );

	posMethodsStr = PosMethodToString( mPosSrc->supportedPositioningMethods() );
	mPositionView->append( "supportedPositioningMethods: " + posMethodsStr + " " + QString::number( mPosSrc->supportedPositioningMethods(), 16 ) );

	mPositionView->append( "updateInterval: " + QString::number( mPosSrc->updateInterval() ) );

	mPositionView->append( "" );
}

QString	Ivan_QtSensorTest::PosErrorToString( QGeoPositionInfoSource::Error inVal ) const
{
	switch( inVal )
	{
	case QGeoPositionInfoSource::AccessError:				return QString( "AccessError" );				
	case QGeoPositionInfoSource::ClosedError:				return QString( "ClosedError" );				
	case QGeoPositionInfoSource::NoError:					return QString( "NoError" );					
	case QGeoPositionInfoSource::UnknownSourceError:	return QString( "UnknownSourceError" );	
	default:															return QString( "eh?" );						
	}
}

QString	Ivan_QtSensorTest::PosMethodToString( QGeoPositionInfoSource::PositioningMethods inVal ) const
{
	switch( inVal )
	{
	case QGeoPositionInfoSource::NoPositioningMethods:					return QString( "NoPositioningMethods" );				
	case QGeoPositionInfoSource::SatellitePositioningMethods:		return QString( "SatellitePositioningMethods" );	
	case QGeoPositionInfoSource::NonSatellitePositioningMethods:	return QString( "NonSatellitePositioningMethods" );
	case QGeoPositionInfoSource::AllPositioningMethods:				return QString( "AllPositioningMethods" );			
	default:																			return QString( "eh?" );									
	}
}
