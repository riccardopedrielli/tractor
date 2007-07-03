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
	float dim = totalsize;
	int unit = 0;
	while(dim >= 1024 && unit < 5)
	{
		dim /= 1024;
		unit++;
	}
	QString parsedTotalSize;
	switch(unit)
	{
		case 0:
			parsedTotalSize = parsedTotalSize.setNum(dim, 'f', 2) + " B";
			break;
		case 1:
			parsedTotalSize = parsedTotalSize.setNum(dim, 'f', 2) + " KB";
			break;
		case 2:
			parsedTotalSize = parsedTotalSize.setNum(dim, 'f', 2) + " MB";
			break;
		case 3:
			parsedTotalSize = parsedTotalSize.setNum(dim, 'f', 2) + " GB";
			break;
		case 4:
			parsedTotalSize = parsedTotalSize.setNum(dim, 'f', 2) + " TB";
			break;
	}

	dim = size;
	unit = 0;
	while(dim >= 1024 && unit < 5)
	{
		dim /= 1024;
		unit++;
	}
	QString parsedSize;
	switch(unit)
	{
		case 0:
			parsedSize = parsedSize.setNum(dim, 'f', 2) + " B";
			break;
		case 1:
			parsedSize = parsedSize.setNum(dim, 'f', 2) + " KB";
			break;
		case 2:
			parsedSize = parsedSize.setNum(dim, 'f', 2) + " MB";
			break;
		case 3:
			parsedSize = parsedSize.setNum(dim, 'f', 2) + " GB";
			break;
		case 4:
			parsedSize = parsedSize.setNum(dim, 'f', 2) + " TB";
			break;
	}

	dim = speed;
	unit = 0;
	while(dim >= 1024 && unit < 3)
	{
		dim /= 1024;
		unit++;
	}
	QString parsedSpeed;
	switch(unit)
	{
		case 0:
			parsedSpeed = parsedSpeed.setNum(dim, 'f', 1) + " B/s";
			break;
		case 1:
			parsedSpeed = parsedSpeed.setNum(dim, 'f', 1) + " KB/s";
			break;
		case 2:
			parsedSpeed = parsedSpeed.setNum(dim, 'f', 1) + " MB/s";
			break;
	}
	
	QString progress = QVariant(size * 100 / totalsize).toString() + "%";

	setToolTip("<font size=4><table width=600><tr><td colspan=2><b>" + name + "</b><hr></td></tr><tr><td>Size: " + parsedTotalSize + "</td><td>Progress: " + progress + "</td></tr><tr><td>Downloaded: " + parsedSize + "</td><td>Download speed: " + parsedSpeed + "</td></tr><tr><td colspan=2><hr></td></tr><tr><td colspan=2>ID: " + fid.toUpper() + "</td></tr></table></font>");
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
