#ifndef TCLIENT_H
#define TCLIENT_H

#include <QtNetwork>
#include <QString>
#include "TTransfer.h"
#include "TShared.h"

class TClient : public QTcpSocket
{
	Q_OBJECT

private:
	TShared shared;

	//general settings
	quint16 srvport;
	QString sharedlistpath,
			sharepath,
			temppath,
			incomingpath;

public:
	QList<TTransfer*> transferslist;

	TClient(QString shlipath, QString shpath, QString tmppath,
				 QString inpath, quint16 serverport);
	void connectToServer(QString address, quint16 port);
	void find(QString filename, quint64 sid);
	void getFile(QString name, QString fid, QString dim);
	bool isConnected();
	QStringList updateSharedList();

signals:
	void fileReceived(QString fid, QString name, QString dim, QString sources,
		QString complete, QString sid);
	void serverConnected();
	void serverDisconnected();
	void messageOfTheDay(QString message);
	void connectionError(QString error);
	void newTransfer(TTransfer *transfer);

private slots:
	void onRead();
	void onConnect();
	void onDisconnect();
	void deleteTransfer(TTransfer *transfer);
};

#endif //TCLENT_H
