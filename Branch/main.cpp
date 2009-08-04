#include "xmlRoot/xmlRoot.h"

#include "BranchXML.h"

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

int main()
{
	xmlRoot::Instance().Init( "tree.xml" );

	// Create a Viewer.
	osgViewer::Viewer viewer;

	osg::ref_ptr< BranchXML > branch = new BranchXML;

	viewer.setSceneData( branch->getRootNode().get() );

	// Display, and main loop.
	return viewer.run();
}