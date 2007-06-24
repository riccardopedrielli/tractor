#include "TClientSocket.h"
#include "TServer.h"
#include "TParser.h"
#include "TXml.h"

const int BUFFER_SIZE = 1024; //regolare il buffer se non va bene

TClientSocket::TClientSocket(int socketid, int upid, QString shlipath)
{
	socket.setSocketDescriptor(socketid);
	uid = upid;
	sharedlistpath = shlipath;

	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	connect(&socket, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(&socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));	
	connect(&uploadtimer,SIGNAL(timeout()), this, SLOT(upSpeed()));	

	start();
}

void TClientSocket::run()
{	
	exec();
}

void TClientSocket::onRead()
{	
	QStringList cmdlist = TParser::splitCommands(socket.readLine());	
	QString cmdname = TParser::getCommand(cmdlist[0]);

	if (cmdname == "GETFILE")	
		getFile(cmdlist[0]);
	
	socket.disconnectFromHost();
}

void TClientSocket::onDisconnect()
{
	emit endUpload(uid);
	emit deleteUpload(this);
	quit();
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

	emit newUpload(uid);

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
		bytesend = socket.write(line);
		socket.waitForBytesWritten();
	}

	uploadtimer.stop();
	file.close();
}

void TClientSocket::upSpeed()
{
	upspeed = bytesend;
	bytesend = 0;
}

