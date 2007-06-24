#ifndef TSERVER_H
#define TSERVER_H

#include <QtNetwork>
#include "TClientSocket.h"

class TServer : public QTcpServer
{
	Q_OBJECT

private:
	void incomingConnection(int socketid);
	int uid;
	QString sharedlistpath;

public:
	QList<TClientSocket*> socketlist;
	
	TServer(quint16 port, int maxconn, QString shlipath);
	void setMaxConnection(int maxconn);

private slots:
	void deleteUpload(TClientSocket *socket);

};

#endif //TSERVER_H
