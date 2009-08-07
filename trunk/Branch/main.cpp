#include "xmlRoot/xmlRoot.h"

#include "Tree.h"

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

int main()
{
	xmlRoot::Instance().Init( "tree.xml" );

	// Create a Viewer.
	osgViewer::Viewer viewer;

	osg::ref_ptr< Tree > tree = new Tree;

	viewer.setSceneData( tree->getRootNode().get() );
	
	viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR); 

	// Display, and main loop.
	return viewer.run();
}