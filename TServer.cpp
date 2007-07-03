#include "TServer.h"

#include <QMessageBox>

TServer::TServer(quint16 port, int maxconn, QString shlipath)
{
	uid = 0;
	sharedlistpath = shlipath;
	setMaxConnection(maxconn);
	listen(QHostAddress::Any, port);
}

void TServer::setMaxConnection(int maxconn)
{
	setMaxPendingConnections(maxconn);
}

void TServer::incomingConnection(int socketid)
{
	TClientSocketThread *clientSocketThread = new TClientSocketThread(socketid, ++uid, sharedlistpath, this);
	connect(clientSocketThread, SIGNAL(socketCreated(TClientSocket *)), this, SLOT(socketCreated(TClientSocket *)));
	clientSocketThread->start();
}

void TServer::deleteUpload(TClientSocket *socket)
{
	int dim = socketlist.count();
	for (int i = 0; i < dim;i++)
	{
		if(socketlist[i] == socket)
		{
			socketlist.removeAt(i);
			break;
		}
	}
}

void TServer::socketCreated(TClientSocket *socket)
{
	socketlist.append(socket);
	emit newUpload(socket);
}

TServerThread::TServerThread(quint16 pport, int pmaxconn, QString pshlipath)
{
	port = pport;
	maxconn = pmaxconn;
	shlipath = pshlipath;
	start();
}

void TServerThread::run()
{
	server = new TServer(port, maxconn, shlipath);
	exec();
}
