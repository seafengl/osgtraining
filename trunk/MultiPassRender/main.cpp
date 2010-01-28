#include "binDef.h"
#include "CameraUpdateCallback.h"
#include "KeyboardHandler.h"
#include "binEvents.h"
#include "MainNode.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <iostream>

int main()
{
	binEvents ff;

	// Create a Viewer.
	osgViewer::Viewer viewer;

	// add the stats handler
	//viewer.addEventHandler( new osgViewer::StatsHandler );

	osg::ref_ptr< KeyboardHandler > keyboard = new KeyboardHandler;

	// add the pick handler
	viewer.addEventHandler( keyboard.get() );

	//������ ����
	//viewer.setUpViewInWindow(10, 10, WIN_W, WIN_H );
	viewer.setUpViewOnSingleScreen( 0 );

	viewer.getCamera()->setClearColor(osg::Vec4( 0.1f , 0.2f , 0.3f , 1.0f ));

	//��������� ������
	viewer.getCamera()->setProjectionMatrixAsFrustum( -HALF_SIZE , HALF_SIZE 
		, -HALF_SIZE * WIN_H / WIN_W , HALF_SIZE * WIN_H / WIN_W , 1.0 , 2970.0 );

//////////////////////////////////////////////////////////////////////////
	//������� ����, � ������� ������ ��������� �����
	MainNode m_MainNode;

	m_MainNode.Init();

	//�������� � ����� �������� ����
	viewer.setSceneData( m_MainNode.GetNode().get() );
//////////////////////////////////////////////////////////////////////////

	osg::StateSet* stateNode = viewer.getCamera()->getOrCreateStateSet();

	//��������� ���������
	stateNode->setMode( GL_LIGHTING , osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

	viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR); 

	float fY = 0.0f;

	viewer.getCamera()->setUpdateCallback( new CameraUpdateCallback() );

	// Display, and main loop.
	while (!viewer.done())
	{
		viewer.frame();
	}
}