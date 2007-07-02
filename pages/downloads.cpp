#include "downloads.h"

Download::Download(TTransfer *tp)
{
	transfer = tp;
	
	name = transfer->getName();
	fid = transfer->getId();
	totalsize = transfer->getTotDim();
	size = 0;
	speed = 0;
	
	updateToolTip();

	setText(name);
	
	connect(transfer, SIGNAL(newSpeed(quint64)), this, SLOT(newSpeed(quint64)));
	connect(transfer, SIGNAL(newSize(quint64)), this, SLOT(newSize(quint64)));
}

void Download::updateToolTip()
{
	setToolTip("File: <b>" + name + "</b><br>Size: " + QVariant(totalsize).toString() + "<br>Downloaded: " + QVariant(size).toString() + "<br>Speed: " + QVariant(speed).toString() + "<br>Id: " + fid);
}

void Download::newSpeed(quint64 newspeed)
{
	speed = newspeed;
	updateToolTip();
}

void Download::newSize(quint64 newsize)
{
	size = newsize;
	updateToolTip();
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
