#include "osgTestPattern.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>

#include <iostream>

int main()
{
	// Create a Viewer.
	osgViewer::Viewer viewer;

	// add the stats handler
	viewer.addEventHandler( new osgViewer::StatsHandler );

	//��������� ������
	//viewer.getCamera()->setProjectionMatrixAsPerspective( 45.0, 600.0/ 1024.0 , 1.0 , 35000.0 );
	viewer.getCamera()->setProjectionMatrixAsOrtho( 0 , 640 , 0 , 480 , 1 , 10000 );

	viewer.setUpViewInWindow(40, 40, 640, 480 );

	osgTestPattern node;
	viewer.setSceneData( node.GetNode().get() );

	viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR); 

	osg::Matrix mtTr;
	float fY = 0.0f;
/*
	// Display, and main loop.
	while (!viewer.done())
	{
		mtTr.makeTranslate( 0, fY , 0 );

		//������ ���� ��������
		viewer.getCamera()->setViewMatrix( mtTr );
		
		viewer.frame();

		fY += 0.01;

		std::cout << fY << " ";
	}
*/
	viewer.run();
}