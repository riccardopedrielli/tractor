#ifndef TCLIENTSOCKET_H
#define TCLIENTSOCKET_H

#include <QtNetwork>

class TClientSocket : public QTcpSocket
{
	Q_OBJECT

private:
	QObject *parent;
	QString name,
			id,
			host,
			sharedlistpath;

	QTimer uploadtimer;

	quint64 upspeed,
			bytesend;

	int uid;
	
	void getFile(QString command);

public:
	TClientSocket(int socketid, int upid, QString shlipath, QObject *pparent);
	int getUploadId(){return uid;};
	QString getName(){return name;};
	QString getId(){return id;};
	QString getHost(){return peerName();};
	quint64 getUploadSpeed(){return upspeed;};	

signals:
	void newUpload(int uid);
	void endUpload(int uid);
	void deleteUpload(TClientSocket *clientsocket);

private slots:
	void onRead();
	void onDisconnect();
	void upSpeed();
};

class TClientSocketThread : public QThread
{
	Q_OBJECT

private:
	QObject *parent;
	int socketid;
	int upid;
	QString shlipath;

protected:
    void run();

public:
	TClientSocket *socket;
	TClientSocketThread(int psocketid, int pupid, QString pshlipath, QObject *pparent);
};

#endif //TCLIENTSOCKET_H
