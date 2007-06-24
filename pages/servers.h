#ifndef SERVERS_H
#define SERVERS_H

#include <QtGui>

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
	ServersPage();

private:
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
	void newEvent(QString text);

private slots:
	void addServer();
	void deleteServer(QTreeWidgetItem *item);
	void connectToServer(QTreeWidgetItem *item);
};

#endif //SERVERS_H
