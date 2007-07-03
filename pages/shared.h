#ifndef SHARED_H
#define SHARED_H

#include <QtGui>
#include "TServer.h"

class SharedViewItem : public QObject, public QTreeWidgetItem
{
	Q_OBJECT

public:
	QList<TClientSocket *> socketList;

private slots:
	void newSpeed();
	void endUpload(TClientSocket *socket);
};

class SharedView : public QTreeWidget
{
	Q_OBJECT

public:
	SharedView(TServer *sp);

private:
	TServer *server;
	QList<SharedViewItem *> itemList;

public slots:
	void newUpload(TClientSocket *socket);
};

class SharedPage : public QWidget
{
	Q_OBJECT

public:
	SharedPage(TServer *sp);

private:
	TServer *server;
	SharedView *sharedView;
};

#endif //SHARED_H
