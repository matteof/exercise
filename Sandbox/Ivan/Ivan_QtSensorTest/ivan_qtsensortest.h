#ifndef IVAN_QTSENSORTEST_H
#define IVAN_QTSENSORTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_ivan_qtsensortest.h"

#include <QCompass>
#include <QGyroscope>
#include <QtPositioning/QGeoPositionInfoSource>


class Ivan_QtSensorTest : public QMainWindow, public Ui::Ivan_QtSensorTestClass
{
	Q_OBJECT

public:
	Ivan_QtSensorTest( QWidget *parent = 0 );
	~Ivan_QtSensorTest();

public slots:
	void		CompassUpdate();
	void		CompassError( int );

	void		GyroscopeUpdate();
	void		GyroscopeError( int );

	void		PositionUpdate( const QGeoPositionInfo &info );	
	void		PositionError( QGeoPositionInfoSource::Error positioningError );

private:

	QCompass*					mCompass;
	QGyroscope*					mGyroscope;
	QGeoPositionInfoSource*	mPosSrc;
};

#endif // IVAN_QTSENSORTEST_H
