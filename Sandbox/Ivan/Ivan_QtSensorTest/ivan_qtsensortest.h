#ifndef IVAN_QTSENSORTEST_H
#define IVAN_QTSENSORTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_ivan_qtsensortest.h"

#include <QCompass>
#include <QGyroscope>
#include <QtPositioning/QGeoPositionInfoSource>
#include <QAction>



class Ivan_QtSensorTest : public QMainWindow, public Ui::Ivan_QtSensorTestClass
{
	Q_OBJECT

public:
	Ivan_QtSensorTest( QWidget *parent = 0 );
	~Ivan_QtSensorTest();

public slots:
	void		StartAll();

	void		CompassUpdate();
	void		CompassError( int );

	void		GyroscopeUpdate();
	void		GyroscopeError( int );

	void		positionUpdated(const QGeoPositionInfo &info);	
	void		PositionError( QGeoPositionInfoSource::Error positioningError );
	void		PositionUpdateTimeout();

private:

	QCompass*					mCompass;
	QGyroscope*					mGyroscope;
	QGeoPositionInfoSource*	mPosSrc;
};

#endif // IVAN_QTSENSORTEST_H
