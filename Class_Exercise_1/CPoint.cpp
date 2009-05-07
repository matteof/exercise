// system includes
#include <iostream>

// project includes
#include "CPoint.h"


// --------------------------------------------------------------------------
//	 CPoint
// --------------------------------------------------------------------------
CPoint::CPoint( double inX, double inY )
: mX(inX)
, mY(inY)
{

}

// --------------------------------------------------------------------------
//	 ~CPoint
// --------------------------------------------------------------------------
CPoint::~CPoint()
{}


// --------------------------------------------------------------------------
//	 SetCoord
// --------------------------------------------------------------------------
void
CPoint::SetCoord( double inX, double inY )
{
	mX = inX;
	mY = inY;
}

// --------------------------------------------------------------------------
//	 GetCoord
// --------------------------------------------------------------------------
void
CPoint::GetCoord( double& outX, double& outY )
{
	outX = mX;
	outY = mY;
}

// --------------------------------------------------------------------------
//	 Draw
// --------------------------------------------------------------------------
void
CPoint::Draw()
{
	std::cout << "(" << mX << "," << mY << ")";
}


