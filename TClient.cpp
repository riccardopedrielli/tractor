#include "TClient.h"
#include "TParser.h"
#include "TXml.h"
#include "info_structs.h"

TClient::TClient(QString shlipath, QString shpath, QString tmppath,
				 QString inpath, quint16 serverport)
{
	sharedlistpath = shlipath;
	sharepath = shpath;
	temppath = tmppath;
	incomingpath = inpath;
	srvport = serverport;

	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	connect(this, SIGNAL(connected()), this, SLOT(onConnect()));
	connect(this, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
}

bool TClient::isConnected()
{
	if(state() == QAbstractSocket::ConnectedState)
		return true;
	return false;
}

QStringList TClient::updateSharedList()
{
	QStringList duplist, idlist;
	idlist = shared.checkShareList(sharedlistpath);

	int dim = idlist.count();
	for (int i = 0; i < dim; i++)
		write(TParser::delFile(idlist.at(i)).toAscii());

	duplist = shared.updateShareList(sharepath, sharedlistpath);
	duplist = duplist + shared.updateShareList(incomingpath, sharedlistpath);
	return duplist;
}

void TClient::connectToServer(QString address, quint16 port)
{
	connectToHost(address,port,QIODevice::ReadWrite);
}

void TClient::onConnect()
{
	write(TParser::port(srvport).toAscii());
	emit serverConnected();
	QList<FileInfo> list = TXml::getFileList(sharedlistpath);

	int dim = list.count();
	for (int i = 0; i<dim; i++)
	{
		int complete = 0;

		if(list[i].completes == "yes")
			complete=1;

		write(TParser::addFile(list[i].fid,list[i].name,
					list[i].dim.toULongLong(),complete).toAscii());
	}
}

void TClient::onRead()
{
	QStringList cmdlist = TParser::splitCommands(readLine());
	int dim = cmdlist.count();

	for(int i=0; i < dim; i++)
	{
		QString cmdname = TParser::getCommand(cmdlist[i]);

		if (cmdname == "MSG")
		{
			QString message;
			if(!TParser::splitSendMsg(cmdlist[i],message))
				return;
			emit messageOfTheDay(message);
		}
		if (cmdname == "FILE")
		{
			QString name,dim,fid,complete,sources,sid;
			if(!TParser::splitSendFile(cmdlist[i],sid,name,dim,sources,complete,fid))
				return;
			emit fileReceived(fid,name,dim,sources,complete,sid);
		}
		if (cmdname == "IP")
		{
			QString fid, host, port;
			int dim;

			if(!TParser::splitSendIp(cmdlist[i],fid,host,port))
				return;

			dim = transferslist.count();
			for(int i=0; i < dim; i++)
			{
				if(fid == transferslist[i]->getId())
				{
					transferslist[i]->addHost(host, port.toUShort());
					break;
				}
			}
		}
	}
}

void TClient::onDisconnect()
{
	emit serverDisconnected();
}

void TClient::find(QString filename, quint64 sid)
{
	write(TParser::find(filename, sid).toAscii());
}

void TClient::getFile(QString name, QString fid, QString dim)
{
	TTransfer *transfer = new TTransfer(name, fid, dim.toULongLong(), temppath,
			incomingpath, sharedlistpath);

	connect(transfer, SIGNAL(deleteTransfer(TTransfer*)), this, SLOT(deleteTransfer(TTransfer*)));
	transferslist.append(transfer);
	emit newTransfer(transfer);
	write(TParser::getIp(fid).toAscii());
}

void TClient::deleteTransfer(TTransfer *transfer)
{
	int dim = transferslist.count();
	for( int i=0; i<dim; i++)
	{
		if(transferslist[i] == transfer)
		{
			transferslist.removeAt(i);
			break;
		}
	}
}
