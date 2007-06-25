#ifndef SERVERS_H
#define SERVERS_H

#include <QtGui>
#include "TClient.h"

class ServerView : public QTreeWidget
{
	Q_OBJECT

public:
	ServerView();
	QAction *connectAction;
	QAction *deleteAction;

private:
	QMenu *menu;
	void contextMenuEvent(QContextMenuEvent *event);

signals:
	void deleteServer(QTreeWidgetItem *item);
	void connectToServer(QTreeWidgetItem *item);
};

class ServersPage : public QWidget
{
	Q_OBJECT

public:
	ServersPage(TClient *cp);

private:
	TClient *client;
	QFileIconProvider *iconProvider;
	ServerView *serverView;
	QList<QTreeWidgetItem *> serverViewItems;
	QLineEdit *nameEdit;
	QLineEdit *ipEdit;
	QLineEdit *portEdit;
	QLineEdit *descEdit;
	QPushButton *addButton;
	struct Server
	{
		QString name;
		QString ip;
		QString port;
		QString desc;
	};
	QList<Server> serverList;
	QFile *serverFile;

	void writeServerList();

signals:
	void serverSelected(QString server);
	void connectionRequest(QString ip, quint16 port);

private slots:
	void addServer();
	void deleteServer(QTreeWidgetItem *item);
	void connectToServer(QTreeWidgetItem *item);
};

#endif //SERVERS_H
