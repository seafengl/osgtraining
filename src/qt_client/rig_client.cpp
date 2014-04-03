#include "rig_client.h"

#include <QAction>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtCore>
#include <QtGui>
#include <QTimer>

#include "luaParseConf.h"

#include <Windows.h>

rig_client::rig_client(const QString& h, const QString& r, const QString& c, QWidget *parent)
	: host(h), req(r), conf(c), QDialog(parent), netMode(enTry2Connect), minerMode(enFirtMsg), prevMinerMode(enFirtMsg)
{
	iSwitchCoin=-1;
	luaParseConf luaConf;
	luaConf.parse(conf.toStdString().c_str(), &client);

	createActions();
	createTrayIcon();
	setIcon();
	createTimer();
	fillMap();

	connect(&tcpClient, SIGNAL(connected()),this, SLOT(connected()));
	connect(&tcpClient, SIGNAL(readyRead()),this, SLOT(readyRead()));
	connect(&tcpClient, SIGNAL(hostFound()),this, SLOT(hostFound()));

	if(client.bShowSysTray)
		trayIcon->show();
	else
		trayIcon->hide();
}

rig_client::~rig_client()
{

}

void rig_client::createActions()
{
	for(int i=0;i<client.vCoins.size();++i)
	{
		QIcon* icon=new QIcon(client.vCoins[i].sIcon.c_str());
		vIcon.push_back(icon);

		QAction* action=new QAction(*icon, client.vCoins[i].sCoin.c_str(), this);
		action->setCheckable(true);
		
		vAction.push_back(action);
		connect(action, SIGNAL(triggered()), this, SLOT(coinSelect()));
	}

	minimizeAction = new QAction(tr("Mi&nimize"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	restoreAction = new QAction(tr("&Restore"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void rig_client::coinSelect()
{
	int j=0;
	for(int i=0;i<vAction.size();++i)
	{
		bool b1=vAction[i]->isEnabled();
		bool b2=vAction[i]->isChecked();
		if ((b2)&&(b1))
		{
			j=i;
			vAction[i]->setEnabled(false);
			trayIcon->setIcon(*vIcon[i]);
			setWindowIcon(*vIcon[i]);
			break;
		}
	}
	for(int i=0;i<vAction.size();++i)
	{
		if (i!=j)
		{
			vAction[i]->setEnabled(true);
			vAction[i]->setChecked(false);
		}
	}

	//switch to J coin
	iSwitchCoin=j;
}

void rig_client::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	for(int i=0;i<vAction.size();++i)
		trayIconMenu->addAction(vAction[i]);

	trayIconMenu->addSeparator();
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
}

void rig_client::setIcon()
{
	iconDef = new QIcon("icons/money.png");
	trayIcon->setIcon(*iconDef);
	setWindowIcon(*iconDef);
	trayIcon->setToolTip(tr("0"));
}

void rig_client::closeEvent(QCloseEvent *event)
{
	if (trayIcon->isVisible())
	{
		hide();
		event->ignore();
	}
}

void rig_client::createTimer()
{
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
	timer->start(1000);
}

void rig_client::timerTick()
{
	if (iSwitchCoin>-1)
	{
		if (minerMode==enFirtMsg)
		{
			//start miner
			startMiner(iSwitchCoin);

			iSwitchCoin=-1;

			return;
		}
		else
		{
			minerMode=enQuit;
		}
	}

	if (netMode==enTry2Connect)
		minerMode=enFirtMsg;

	netMode=enTry2Connect;
	tcpClient.connectToHost(client.sHost.c_str(), client.iPort, QAbstractSocket::ReadWrite, QAbstractSocket::IPv4Protocol);
}

void rig_client::connected()
{
	if (minerMode==enFirtMsg)
		minerMode=(eMinerMode)((int)minerMode+1);
	
	std::string sReq=mode2Str[minerMode];
	tcpClient.write(sReq.c_str());

	if (minerMode==enQuit)
		minerMode=enFirtMsg;
	else
	{
		minerMode=(eMinerMode)((int)minerMode+1);
		if(minerMode==enQuit)
			minerMode=enSummary;
	}	
}

void rig_client::readyRead()
{
	QString sR= tcpClient.readAll();
	setWindowTitle(sR);
	tcpClient.disconnectFromHost();
}

void rig_client::hostFound()
{
	netMode=enConnectSuccess;
}

void rig_client::startMiner(int i)
{
	//start miner
	STARTUPINFOW si;

	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	std::string sFull=" walletpassphrase ";

	//if(!CreateProcess(sCrypto.c_str(), (LPSTR)sFull.c_str(), NULL, NULL,FALSE, 0,NULL,NULL,&si,&pi))
	{
		
	}
}

void rig_client::fillMap()
{
	mode2Str[enFirtMsg]="error";
	mode2Str[enSummary]="summary";
	mode2Str[enPools]="pools";
	mode2Str[enCoin]="coin";
	mode2Str[enQuit]="quit";
}