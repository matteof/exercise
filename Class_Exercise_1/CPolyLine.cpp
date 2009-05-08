// system includes
#include <iostream>

// project includes
#include "CPolyLine.h"



// --------------------------------------------------------------------------
//	 CPolyLine
// --------------------------------------------------------------------------
CPolyLine::CPolyLine()
: mBoundRect( CPoint(0,0), CPoint(0,0) )
{

}

// --------------------------------------------------------------------------
//	 ~CPolyLine
// --------------------------------------------------------------------------
CPolyLine::~CPolyLine()
{}



// --------------------------------------------------------------------------
//	 AddPoint
// --------------------------------------------------------------------------
void
CPolyLine::AddPoint( const CPoint& inP )
{
	mVertices.push_back( inP );
	if ( GetVertexCount() == 1)
		mBoundRect.SetExtremes( inP, inP );
	else
	{
		if  ( !( mBoundRect.Inside( inP )) )
		{
			CPoint mP( 	min ( mBoundRect.GetLT().GetX(), inP.GetX() ),
						min ( mBoundRect.GetLT().GetY(), inP.GetY() ) );
					
			CPoint mQ( 	max ( mBoundRect.GetRB().GetX(), inP.GetX() ),
						max ( mBoundRect.GetRB().GetY(), inP.GetY() ) );
						
			mBoundRect.SetExtremes( mP, mQ ) ;
		}
	
	}
}

// --------------------------------------------------------------------------
//	 Draw
// --------------------------------------------------------------------------
void
CPolyLine::Draw()
{
	vector< CPoint >::iterator  myIter;
	//double x, y;

	for ( myIter=mVertices.begin(); myIter<(mVertices.end()-1); ++myIter)
	{
		myIter->Draw();
		cout << "-";
	}

	myIter->Draw();
	cout << "\n";
}






