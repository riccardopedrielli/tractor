#ifndef TCLIENTSOCKET_H
#define TCLIENTSOCKET_H

#include <QtNetwork>

class TClientSocket : public QThread
{
	Q_OBJECT

private:
	QTcpSocket socket;
	
	QString name,
			id,
			host,
			sharedlistpath;

	QTimer uploadtimer;

	quint64 upspeed,
			bytesend;

	int uid;
	
	void getFile(QString command);

protected:
    void run();

public:
	TClientSocket(int socketid, int upid, QString shlipath);
	int getUploadId(){return uid;};
	QString getName(){return name;};
	QString getId(){return id;};
	QString getHost(){return socket.peerName();};
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

#endif //TCLIENTSOCKET_H
