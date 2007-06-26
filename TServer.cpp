#include "TServer.h"

TServer::TServer(quint16 port, int maxconn, QString shlipath)
{
	uid = 0;
	sharedlistpath = shlipath;
	setMaxConnection(maxconn);
	server.listen(QHostAddress::Any, port);
	start();
}

void TServer::run()
{
	exec();
}

void TServer::setMaxConnection(int maxconn)
{
	server.setMaxPendingConnections(maxconn);
}

void TServer::incomingConnection(int socketid)
{	
	TClientSocket *clientSocket = new TClientSocket(socketid, ++uid, sharedlistpath);
	connect(clientSocket, SIGNAL(deleteUpload(TClientSocket*)), this, SLOT(deleteUpload(TClientSocket*)));
	socketlist.append(clientSocket);
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
