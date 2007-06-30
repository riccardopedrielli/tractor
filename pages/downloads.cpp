#include "downloads.h"

Download::Download(TTransfer *tp)
{
	transfer = tp;
	
	name = transfer->getName();
	fid = transfer->getId();
	totalsize = transfer->getTotDim();
	size = 0;
	speed = 0;
	
	setToolTip("File: <b>" + name + "</b>\nSize: " + totalsize + "\nDownloaded: " + size + "\nSpeed: " + speed + "\nId: " + fid);

	setText(name);
	
	connect(transfer, SIGNAL(newSpeed(quint64)), this, SLOT(newSpeed(quint64)));
	connect(transfer, SIGNAL(newSize(quint64)), this, SLOT(newSize(quint64)));
}

void Download::newSpeed(quint64 newspeed)
{
	speed = newspeed;
	setToolTip("File: <b>" + name + "</b>\nSize: " + totalsize + "\nDownloaded: " + size + "\nSpeed: " + speed + "\nId: " + fid);
}

void Download::newSize(quint64 newsize)
{
	size = newsize;
	setToolTip("File: <b>" + name + "</b>\nSize: " + totalsize + "\nDownloaded: " + size + "\nSpeed: " + speed + "\nId: " + fid);
	if(size == totalsize)
		setForeground(QBrush(QColor(255, 0, 0)));
}

DownloadsPage::DownloadsPage(TClient *cp)
{
	menu = new QMenu;
	removeAction = new QAction("Remove", menu);
	menu->addAction(removeAction);

	client = cp;
	connect(client, SIGNAL(newTransfer(TTransfer *)), this, SLOT(addTransfer(TTransfer *)));
}

void DownloadsPage::contextMenuEvent(QContextMenuEvent *event)
{
	Download *item = (Download *)itemAt(event->pos());
	if(!item)
	{
		return;
	}
	QAction *action = menu->exec(event->globalPos());
	if(action == removeAction)
	{
		if(item->size != item->totalsize)
			item->transfer->remove();
		removeItemWidget(item);
		delete item;
	}
}

void DownloadsPage::addTransfer(TTransfer *transfer)
{
	Download *download = new Download(transfer);
	addItem(download);
}
