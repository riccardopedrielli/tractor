#include "TClientSocket.h"
#include "TServer.h"
#include "TParser.h"
#include "TXml.h"

const int BUFFER_SIZE = 1024; //regolare il buffer se non va bene

TClientSocket::TClientSocket(int socketid, int upid, QString shlipath, QObject *pparent)
{
	parent = pparent;
	setSocketDescriptor(socketid);
	uid = upid;
	sharedlistpath = shlipath;

	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	connect(this, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnect()));	
	connect(&uploadtimer, SIGNAL(timeout()), this, SLOT(upSpeed()));	
}

void TClientSocket::onRead()
{	
	QStringList cmdlist = TParser::splitCommands(readLine());	
	QString cmdname = TParser::getCommand(cmdlist[0]);
	if (cmdname == "GETFILE")	
		getFile(cmdlist[0]);
	
	disconnectFromHost();
}

void TClientSocket::onDisconnect()
{
	emit endUpload(this);
	emit deleteUpload(this);
	disconnect(this, 0, 0, 0);
	((QThread *)parent)->quit();
}

void TClientSocket::getFile(QString command)
{
	QString fid, byte, xname, xid, xlastmod, xcomplete, xpath;

	if(!TParser::splitGetFile(command,fid,byte))
		return;

	TXml::getFileInfo(sharedlistpath, TAG_ID, fid, xname, xid, 
			xlastmod, xcomplete, xpath);

	name = xname;
	id = xid;

	QFile file(xpath);
	if(!file.open(QIODevice::ReadOnly))
		return;

	if(byte.toULongLong()>0)
		if(file.seek(byte.toULongLong()))		
			return;

	uploadtimer.start(1000);
	
	while(!file.atEnd())
	{		
		QByteArray line = file.read(BUFFER_SIZE);
		bytesend = write(line);
		waitForBytesWritten();
	}

	uploadtimer.stop();
	file.close();
}

void TClientSocket::upSpeed()
{
	upspeed = bytesend;
	bytesend = 0;
	emit newSpeed();
}

TClientSocketThread::TClientSocketThread(int psocketid, int pupid, QString pshlipath, QObject *pparent)
{
	parent = pparent;
	socketid = psocketid;
	upid = pupid;
	shlipath = pshlipath;
}

void TClientSocketThread::run()
{
	socket = new TClientSocket(socketid, upid, shlipath, this);
	connect(socket, SIGNAL(deleteUpload(TClientSocket*)), (TServer *)parent, SLOT(deleteUpload(TClientSocket*)));
	emit socketCreated(socket);
	exec();
}
