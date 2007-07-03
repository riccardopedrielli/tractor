#include "shared.h"
#include "info_structs.h"
#include "TXml.h"

void SharedViewItem::newSpeed()
{
	float speed = 0;
	foreach(TClientSocket *socket, socketList)
	{
		speed += socket->getUploadSpeed();
	}
	int unit = 0;
	while(speed >= 1024 && unit < 3)
	{
		speed /= 1024;
		unit++;
	}
	QString parsedSpeed;
	switch(unit)
	{
		case 0:
			parsedSpeed = parsedSpeed.setNum(speed, 'f', 1) + " B/s   ";
			break;
		case 1:
			parsedSpeed = parsedSpeed.setNum(speed, 'f', 1) + " KB/s   ";
			break;
		case 2:
			parsedSpeed = parsedSpeed.setNum(speed, 'f', 1) + " MB/s   ";
			break;
	}
	setText(3, parsedSpeed);
}

void SharedViewItem::endUpload(TClientSocket *sp)
{
	foreach(TClientSocket *socket, socketList)
	{
		if(socket == sp)
		{
			socketList.removeAt(socketList.indexOf(socket));
			setText(2, QVariant(text(2).toInt() - 1).toString());
			break;
		}
	}
}

SharedView::SharedView(TServer *sp)
{
	server = sp;
	
	QTreeWidgetItem *header = new QTreeWidgetItem;
	header->setText(0, "File");
	header->setText(1, "Size");
	//header->setText(2, "Uploads");
	//header->setText(3, "Upload speed");
	header->setText(2, "File ID");

	setHeaderItem(header);
	setColumnWidth(0, 350);
	setColumnWidth(1, 100);
	//setColumnWidth(2, 100);
	//setColumnWidth(3, 120);
	setColumnWidth(2, 200);
	setSortingEnabled(true);
	setRootIsDecorated(false);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::NoSelection);
	sortItems(0, Qt::AscendingOrder);

	QList<FileInfo> fileList = TXml::getFileList(QDir::homePath() + "/.tractor/sharedlist.xml");

	foreach(FileInfo file, fileList)
	{
			QString dim = file.dim;
			float size = dim.toFloat();
			int unit = 0;
			while(size >= 1024 && unit < 5)
			{
				size /= 1024;
				unit++;
			}
			switch(unit)
			{
				case 0:
					dim = dim.setNum(size, 'f', 2) + " B";
					break;
				case 1:
					dim = dim.setNum(size, 'f', 2) + " KB";
					break;
				case 2:
					dim = dim.setNum(size, 'f', 2) + " MB";
					break;
				case 3:
					dim = dim.setNum(size, 'f', 2) + " GB";
					break;
				case 4:
					dim = dim.setNum(size, 'f', 2) + " TB";
					break;
			}

			SharedViewItem *newItem = new SharedViewItem;
			newItem->setText(0, file.name);
			newItem->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
			newItem->setText(1, dim);
			newItem->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
			//newItem->setText(2, "0");
			//newItem->setTextAlignment(3, Qt::AlignRight | Qt::AlignVCenter);
			//newItem->setText(3, "0.0 B/s   ");
			newItem->setText(2, file.fid.toUpper());
			newItem->setSizeHint(0, QSize(newItem->sizeHint(0).width(), 18));
			addTopLevelItem(newItem);
			itemList.append(newItem);
	}
}

void SharedView::newUpload(TClientSocket *socket)
{
/*	foreach(SharedViewItem *item, itemList)
	{
		if(socket->getId() == item->text(4).toLower())
		{
			item->socketList.append(socket);
			connect(socket, SIGNAL(newSpeed()), item, SLOT(newSpeed()));
			connect(socket, SIGNAL(endUpload(TClientSocket *)), item, SLOT(endUpload(TClientSocket *)));
			item->setText(2, QVariant(item->text(2).toInt() + 1).toString());
			break;
		}
	}*/
}

SharedPage::SharedPage(TServer *sp)
{
	server = sp;

	/* Elements definition */
	sharedView = new SharedView(server);

	/* Layout definition */
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(sharedView);

	setLayout(mainLayout);
	
	/* Connection */
	connect(server, SIGNAL(newUpload(TClientSocket *)), sharedView, SLOT(newUpload(TClientSocket *)));
}
