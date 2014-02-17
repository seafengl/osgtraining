#include <iostream>
#include <string>

#include <io.h>
#include <fcntl.h>
#include <conio.h>

#include "logic/PassGen.h"
#include "logic/BtcdLauncher.h"

std::wstring wConf;
std::wstring wAutosave;
std::wstring wSuccess;
std::wstring wPhrase;
std::wstring wVocab;
std::wstring wCrypto(L"bitcoind.exe");

PassGen m_PassGen;
BtcdLauncher m_BtcdLauncher;

//����������� ������� �� �������
bool KeyPressDetect();

int main(int argc, char** argv)
{
	//��� ����������� ������� � �������
	_setmode(_fileno(stdout), _O_WTEXT);
	
	//��� �������
	for(int i=1; i<argc; i++)
	{
		if (strcmp(argv[i], "-config") == 0)
		{
			i++; if (i >= argc) break;
			std::string sConf = argv[i];
			wConf=std::wstring(sConf.begin(), sConf.end());
			continue;
		}
		else if (strcmp(argv[i], "-autosave") == 0)
		{
			i++; if (i >= argc) break;
			std::string sSave = argv[i];
			wAutosave=std::wstring(sSave.begin(), sSave.end());
			continue;
		}
		else if (strcmp(argv[i], "-success") == 0)
		{
			i++; if (i >= argc) break;
			std::string sConf = argv[i];
			wSuccess=std::wstring(sConf.begin(), sConf.end());
			continue;
		}
		else if (strcmp(argv[i], "-phrase") == 0)
		{
			i++; if (i >= argc) break;
			std::string sConf = argv[i];
			wPhrase=std::wstring(sConf.begin(), sConf.end());
			continue;
		}
		else if (strcmp(argv[i], "-vocab") == 0)
		{
			i++; if (i >= argc) break;
			std::string sConf = argv[i];
			wVocab=std::wstring(sConf.begin(), sConf.end());
			continue;
		}
		else if (strcmp(argv[i], "-crypto") == 0)
		{
			i++; if (i >= argc) break;
			std::string sConf = argv[i];
			wCrypto=std::wstring(sConf.begin(), sConf.end());
			continue;
		}
	}

	m_BtcdLauncher.Init(wCrypto);
	
	if (!wVocab.empty())
		m_PassGen.InitVocab(wVocab);
	else
		m_PassGen.Init(wConf, wAutosave, wPhrase);

	bool bStop = false;
	pswTry tryStatus=pswNext;
	int iCount=0;
	while(!bStop)
	{
		if (tryStatus==pswNext)
		{
			//generate next password
			std::wstring sPass=m_PassGen.GenNextPass();

			if (!sPass.empty())
				tryStatus=m_BtcdLauncher.Process(sPass);
			else
			{
				std::wcout<<"End of vocab."<<std::endl;
				tryStatus=pswError;
			}
		}

		if (tryStatus!=pswNext)
			bStop=true;

		if (tryStatus==pswTryAgain)
			bStop=false;

		//����������� ������� �� �������
		bStop = bStop||KeyPressDetect();
		
		if (tryStatus!=pswTryAgain)
			++iCount;
	}

	switch (tryStatus)
	{
	case pswTry::pswSuccess:
		m_PassGen.SaveSuccessPass(wSuccess);
		break;
	case pswTry::pswError:
		std::wcout<<"Error lunching program."<<std::endl;
		break;
	}

	bool bSave=false;
	if (iCount>1)
		bSave=true;
	m_PassGen.Close(bSave);
}

bool KeyPressDetect()
{
	//����������� ������� �� �������
	bool bStop = false;
	
	char cT('_');
	if (kbhit())
		cT = getch();

	if ( cT == 27 )
		bStop = true;

	return bStop;
}