#include <QByteArray>

#include "TTransfer.h"
#include "TParser.h"
#include "TXml.h"
#include "TClient.h"

TTransfer::TTransfer(QString filename, QString fileid, quint64 filedim, 
			QString tmppath, QString inpath, QString shlipath)
{
	name = filename;
	id = fileid; 
	totdim = filedim;
	hostindex = 0;
	bytesrecived = 0;
	downloadspeed = 0;
	sharedlistpath = shlipath;
	incomingpath = inpath;

	file.setFileName( tmppath + "/" + filename +".part");
	file.open(QIODevice::Append);
	
	QFileInfo fileinfo(file);

	TXml::writeFileInfo(
		sharedlistpath,
		fileinfo.fileName(),
		id,
		QString::number(totdim),
		fileinfo.lastModified().toString(Qt::ISODate),
		QString("no"),
		fileinfo.absoluteFilePath()
	);
	
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	connect(&socket, SIGNAL(connected()), this, SLOT(onConnect()));
	connect(&socket, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(&socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
	connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	connect(&iptimer, SIGNAL(timeout()), this, SLOT(tryToConnect()));
	connect(&downloadtimer,SIGNAL(timeout()), this, SLOT(downSpeed()));
	
	start();
}

void TTransfer::addHost(QString newhost, quint16 port)
{
	hostInfo *hostinfo = new hostInfo;
	hostinfo->host = newhost;
	hostinfo->port = port;
	hostlist.append(hostinfo);
	iptimer.start(WAITING_TIME);
}

void TTransfer::onConnect()
{
	iptimer.stop();
	downloadtimer.start(1000);
	socket.write(TParser::getFile(id,file.size()).toAscii());
}

void TTransfer::onRead()
{
	QByteArray line = socket.readAll();
	bytesrecived += line.size();
	file.write(line);
	emit newSize(file.size());
}

void TTransfer::onDisconnect()
{
	downloadtimer.stop();
	downloadspeed = 0;

	if (totdim == file.size())
	{
		QString tmp = "";
		TXml::setFileInfo(sharedlistpath,TAG_ID,id,TAG_COMPLETE,QString("yes"));

		for(int i = 1; !file.rename(incomingpath+"/"+tmp+name); i++)			
			tmp="("+QString::number(i)+")";

		remove();
		return;
	}
	
	iptimer.start(WAITING_TIME);
}

void TTransfer::error(QAbstractSocket::SocketError socketerror)
{
	downloadtimer.stop();
	downloadspeed = 0;
	delete hostlist[hostindex - 1];
	hostlist.removeAt(hostindex - 1);
}

void TTransfer::tryToConnect()
{
	iptimer.stop();
	int count = hostlist.count();

	if (!count)
		return;

	if (hostindex == count)
		hostindex = 0;
	
	socket.connectToHost(hostlist[hostindex]->host, hostlist[hostindex]->port, QIODevice::ReadWrite);	
	hostindex++;
	iptimer.start(WAITING_TIME);
}

void TTransfer::downSpeed()
{
	downloadspeed = bytesrecived;
	bytesrecived = 0;
	emit newSpeed(downloadspeed);
}

TTransfer::~TTransfer()
{
	int dim = hostlist.count();
	file.close();
	for (int i=0;i<dim;i++)
		delete hostlist[i];
}

void TTransfer::run()
{
	exec();
}

void TTransfer::remove()
{
	if(socket.state()>QAbstractSocket::UnconnectedState)
	{
		socket.disconnectFromHost();
		socket.waitForDisconnected();
	}
	emit deleteTransfer(this);
	file.close();
	quit();
}

/*TTransferThread::TTransferThread(QString pfilename, QString pfileid, quint64 pfiledim, 
		QString ptmppath, QString pinpath, QString pshlipath)
{
	filename = pfilename;
	fileid = pfileid;
	filedim = pfiledim;
	tmppath = ptmppath;
	inpath = pinpath;
	shlipath = pshlipath;
	start();
}

void TTransferThread::run()
{
	transfer = new TTransfer(filename, fileid, filedim, tmppath,
				inpath, shlipath);
	exec();
}*/
