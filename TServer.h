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

class TServerThread : public QThread
{
	Q_OBJECT

private:
	quint16 port;
	int maxconn;
	QString shlipath;

protected:
    void run();

public:
	TServer *server;
	TServerThread(quint16 port, int maxconn, QString shlipath);
};

#endif //TSERVER_H
