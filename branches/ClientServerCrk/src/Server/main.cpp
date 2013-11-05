#include <iostream>
#include <string>

#include <io.h>
#include <fcntl.h>

#include "Core/Lua/Lua_vm.h"
#include "ServerLogic.h"

std::string sAddress("127.0.0.1");
std::wstring wRule;
std::wstring wAutosave;
std::wstring wSuccess;
std::wstring wPhrase;

int iPort=9999;

int main(int argc, char **argv)
{
	//��� ����������� ������� � �������
	_setmode(_fileno(stdout), _O_TEXT);	//_O_WTEXT	_O_TEXT

	//��� �������
	for(int i=1; i<argc; i++)
	{
		if (strcmp(argv[i], "-config") == 0)
		{
			i++; if (i >= argc) break;
			Lua_vm lvm(argv[i]);
			sAddress=lvm.get_string("address");
			iPort=lvm.get_int("port");
			continue;
		}
		else if (strcmp(argv[i], "-rule") == 0)
		{
			i++; if (i >= argc) break;
			std::string sConf = argv[i];
			wRule=std::wstring(sConf.begin(), sConf.end());
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

	}
//////////////////////////////////////////////////////////////////////////
	ServerLogic m_ServerLogic;

	m_ServerLogic.Init(iPort, sAddress);
	m_ServerLogic.InitPassLogic(wRule, wAutosave, wPhrase);
	m_ServerLogic.Process();
	m_ServerLogic.Close();
}