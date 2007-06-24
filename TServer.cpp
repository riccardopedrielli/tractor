#include "TServer.h"

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
