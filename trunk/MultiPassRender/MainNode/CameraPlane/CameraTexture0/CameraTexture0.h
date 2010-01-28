#ifndef _CAMERA_TEXTURE_0_H_
#define _CAMERA_TEXTURE_0_H_

#include <osg/Camera>
#include <osg/Texture2D>

class CameraTexture0
{
public:
	CameraTexture0();
	~CameraTexture0();

	//������������� ����-������
	void Init();

	//������� ���� � �������
	osg::ref_ptr< osg::Camera > GetCameraNode(){ return m_Camera.get(); }

	//������� ������� � ������� ���������� �����������
	osg::ref_ptr< osg::Texture2D > GetTexture(){ return m_Texture.get(); };

private:

	//������� ��������
	void CreateTexture();

	//�������� ������
	void CreateCamera();

	//��������� ���� ������
	void SetupCameraNode();

	//���� ������	
	osg::ref_ptr< osg::Camera > m_Camera;

	//���� � ������������ ���������
	osg::ref_ptr< osg::Texture2D > m_Texture; 
};

#endif	//_CAMERA_TEXTURE_0_H_