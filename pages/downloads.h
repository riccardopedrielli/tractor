#ifndef DOWNLOADS_H
#define DOWNLOADS_H

#include <QtGui>
#include "TClient.h"

class Download : public QObject, public QListWidgetItem
{
	Q_OBJECT

public:
	Download(TTransfer *tp);
	TTransfer *transfer;
	quint64 totalsize;
	quint64 size;

private:
	QString name;
	QString fid;
	quint64 speed;

private slots:
	void newSpeed(quint64 newspeed);
	void newSize(quint64 newsize);
};

class DownloadsPage : public QListWidget
{
	Q_OBJECT

public:
	DownloadsPage(TClient *cp);

private:
	QMenu *menu;
	QAction *removeAction;
	TClient *client;
	void contextMenuEvent(QContextMenuEvent *event);

private slots:
	void addTransfer(TTransfer *transfer);
};

#endif //DOWNLOADS_H
