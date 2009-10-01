#include "TerrainShaderPatchNode.h"

#include "DynamicGroupUpdateCallback.h"

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osg/Image>
#include <osg/Texture2D>

TerrainShaderPatchNode::TerrainShaderPatchNode()
{
	m_rootNode = new osg::Group;

	//���������������� �������� ���� �����
	InitTerrainNode();

	//����������� �������� ����
	m_rootNode->setDataVariance( osg::Object::DYNAMIC );
}

TerrainShaderPatchNode::~TerrainShaderPatchNode()
{

}

void TerrainShaderPatchNode::InitTerrainNode()
{
	//���������������� �������� ���� �����

	//�������� ������
	AddShader();

	//�������� �������� � ������ �����
	AddTextureHeightmap();

	//�������� �������� � ���������
	AddTextureIndex();

	//�������� ��������� ��������
	AddTexturePatches();

	//������ �������� ����� ����������
	m_rootNode->setUpdateCallback( new DynamicGroupUpdateCallback( m_unfVisPos.get()) );
}

void TerrainShaderPatchNode::AddTextureHeightmap()
{
	//�������� �������� � ������ �����
	osg::StateSet* state = m_rootNode->getOrCreateStateSet();

	// Load the texture image
	osg::ref_ptr<osg::Image> image0 =
		osgDB::readImageFile( "terHightmap.bmp" );

	// Attach the image in a Texture2D object
	osg::ref_ptr<osg::Texture2D> tex0 = new osg::Texture2D;
	tex0->setImage( image0.get() );

	tex0->setFilter( osg::Texture::MIN_FILTER,osg::Texture::LINEAR );
	tex0->setFilter( osg::Texture::MAG_FILTER,osg::Texture::LINEAR );
	tex0->setWrap( osg::Texture::WRAP_S , osg::Texture::REPEAT ); 
	tex0->setWrap( osg::Texture::WRAP_T , osg::Texture::REPEAT ); 

	// Attach the 2D texture attribute and enable GL_TEXTURE_2D,
	// both on texture unit 0.
	state->setTextureAttributeAndModes( 0, tex0.get() , osg::StateAttribute::ON );
}

void TerrainShaderPatchNode::AddTextureIndex()
{
	//�������� �������� � ���������
	osg::StateSet* state = m_rootNode->getOrCreateStateSet();

	// Load the texture image
	osg::ref_ptr<osg::Image> image0 =
		osgDB::readImageFile( "index.bmp" );

	// Attach the image in a Texture2D object
	osg::ref_ptr<osg::Texture2D> tex0 = new osg::Texture2D;
	tex0->setImage( image0.get() );

	tex0->setFilter( osg::Texture::MIN_FILTER,osg::Texture::NEAREST );
	tex0->setFilter( osg::Texture::MAG_FILTER,osg::Texture::NEAREST );
	tex0->setWrap( osg::Texture::WRAP_S , osg::Texture::CLAMP ); 
	tex0->setWrap( osg::Texture::WRAP_T , osg::Texture::CLAMP ); 

	// Attach the 2D texture attribute and enable GL_TEXTURE_2D,
	// both on texture unit 1.
	state->setTextureAttributeAndModes( 1 , tex0.get() , osg::StateAttribute::ON );
}

void TerrainShaderPatchNode::AddTexturePatches()
{
	//�������� ��������� ��������
	osg::StateSet* state = m_rootNode->getOrCreateStateSet();

	// Load the texture image
	osg::ref_ptr<osg::Image> image0 =
		osgDB::readImageFile( "big.bmp" );

	// Attach the image in a Texture2D object
	osg::ref_ptr<osg::Texture2D> tex0 = new osg::Texture2D;
	tex0->setImage( image0.get() );

	tex0->setFilter( osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR );
	tex0->setFilter( osg::Texture::MAG_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR );
	tex0->setWrap( osg::Texture::WRAP_S , osg::Texture::CLAMP ); 
	tex0->setWrap( osg::Texture::WRAP_T , osg::Texture::CLAMP ); 

	// Attach the 2D texture attribute and enable GL_TEXTURE_2D,
	// both on texture unit 2.
	state->setTextureAttributeAndModes( 2, tex0.get() , osg::StateAttribute::ON );
}

void TerrainShaderPatchNode::AddShader()
{
	//�������� ������

	//������������ ����� � ��������
	osg::StateSet* ss = m_rootNode->getOrCreateStateSet();

	//������� ��������� ���������
	osg::Program* program = new osg::Program;
	program->setName( "only_geom" );

	osg::Shader *VertObj = new osg::Shader( osg::Shader::VERTEX );
	osg::Shader *FragObj = new osg::Shader( osg::Shader::FRAGMENT );
	program->addShader( VertObj );
	program->addShader( FragObj );

	LoadShaderSource( VertObj , "glsl/only_geom.vert" );
	LoadShaderSource( FragObj , "glsl/only_geom.frag" );

	ss->setAttributeAndModes( program, osg::StateAttribute::ON );

	//��������� uniform'�
	SetupUniforms( ss );
}

void TerrainShaderPatchNode::SetupUniforms( osg::StateSet* ss )
{
	//��������� uniform'�

	/*
	//���������� uniform'� ��� ������� �������� �����
	ss->addUniform( new osg::Uniform( "posOffset" , osg::Vec3( 0.0f ,0.0f , 0.0f ) ) );

	//���������� uniform'�� ��� ������� ��������� � ��������������� ������
	ss->addUniform( new osg::Uniform( "colorP" , osg::Vec3( 0.0f ,0.0f , 0.0f ) ) );
	ss->addUniform( new osg::Uniform( "colorS" , osg::Vec3( 0.0f ,0.0f , 0.0f ) ) );

	//����������� ��������������� ���������
	ss->addUniform( new osg::Uniform( "fKofScale" , 1.0f ) );
*/
	//��������� �����������
	m_unfVisPos = new osg::Uniform( "posVis" , osg::Vec3( 0.0f ,0.0f , 0.0f ) );
	ss->addUniform( m_unfVisPos.get() );
	//���������� uniform'�� ��� ������ � ����������� ��������
	ss->addUniform( new osg::Uniform( "u_texture0" , 0 ) );
	ss->addUniform( new osg::Uniform( "u_texture1" , 1 ) );
	ss->addUniform( new osg::Uniform( "u_texture2" , 2 ) );
}

void TerrainShaderPatchNode::LoadShaderSource( osg::Shader* shader, const std::string& fileName )
{
	// load source from a file.
	std::string fqFileName = osgDB::findDataFile(fileName);
	if( fqFileName.length() != 0 )
	{
		shader->loadShaderSourceFromFile( fqFileName.c_str() );
	}
	else
	{
		osg::notify(osg::WARN) << "File \"" << fileName << "\" not found." << std::endl;
	}
}
