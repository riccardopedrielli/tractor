#ifndef TTRANSFER_H
#define TTRANSFER_H

#include <QtNetwork>
#include <QString>
#include <QFile>

#define WAITING_TIME 5000

class TTransfer : public QThread
{
	Q_OBJECT

private:
	struct hostInfo{
		QString host;
		quint16 port;
	};
	
	QTimer iptimer, 
		   downloadtimer;

	QString name,
			id,
			sharedlistpath,
			incomingpath;
	
	quint64	bytesrecived,
			downloadspeed;

	qint64 totdim;
	QTcpSocket client;	
	int hostindex;
	QList<hostInfo*> hostlist;	
	QFile file;

protected:
    void run();

public:	
	void remove();
	QString getName(){return name;};
	QString getId(){return id;};
	qint64 getTotDim(){return totdim;};
	quint64 getCurDim(){return file.size();};
	quint64 getDownloadSpeed(){return downloadspeed;};
	void addHost(QString newhost, quint16 port);
	TTransfer(QString filename, QString fileid, quint64 filedim, 
		QString tmppath, QString inpath, QString shlipath);
	~TTransfer();

signals:
	void deleteTransfer(TTransfer *transfer);
	void newSpeed(quint64 speed);
	void newSize(quint64 size);

private slots:
	void onRead();
	void onConnect();
	void onDisconnect();
	void error(QAbstractSocket::SocketError socketerror);
	void tryToConnect();
	void downSpeed();
};
#endif //TTRANSFER_H
