#include "servers.h"

ServerView::ServerView()
{
	menu = new QMenu;
	connectAction = new QAction(menu);
	deleteAction = new QAction(menu); 
	menu->addAction(connectAction);
	menu->addAction(deleteAction);
	
	QFileIconProvider *iconProvider = new QFileIconProvider;

	QTreeWidgetItem *header = new QTreeWidgetItem;
	header->setIcon(0, iconProvider->icon(QFileIconProvider::Computer));
	header->setText(0, "   Server");
	header->setIcon(1, iconProvider->icon(QFileIconProvider::Network));
	header->setText(1, "   Address");
	header->setIcon(2, iconProvider->icon(QFileIconProvider::Folder));
	header->setText(2, "   Description");

	setHeaderItem(header);
	setColumnWidth(0, 200);
	setColumnWidth(1, 200);
	setColumnWidth(2, 200);
	setSortingEnabled(true);
	setRootIsDecorated(false);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	sortItems(0, Qt::AscendingOrder);
}

void ServerView::contextMenuEvent(QContextMenuEvent *event)
{
	QTreeWidgetItem *item = itemAt(event->pos());
	if(!item)
	{
		return;
	}
	connectAction->setText((item->text(0).isEmpty()) ? "Connect to this server" : "Connect to " + item->text(0));
	deleteAction->setText("Delete this server");
	QAction *action = menu->exec(event->globalPos());
	if(action == deleteAction)
	{
		emit deleteServer(item);
	}
	else if(action == connectAction)
	{
		emit connectToServer(item);
	}
}

ServersPage::ServersPage(TClient *cp)
{
	client = cp;

	/* Elements definition */
	iconProvider = new QFileIconProvider;

	serverView = new ServerView;

	QLabel *nameLabel = new QLabel("Name:");
	QLabel *ipLabel = new QLabel("IP:");
	QLabel *portLabel = new QLabel("Port:");
	QLabel *descLabel = new QLabel("Desc:");
	nameEdit = new QLineEdit;
	nameEdit->setMaximumWidth(nameEdit->sizeHint().width()*2);
	ipEdit = new QLineEdit;
	ipEdit->setMaximumWidth(ipEdit->sizeHint().width());
	portEdit = new QLineEdit;
	portEdit->setMaximumWidth(portEdit->sizeHint().width()/2);
	descEdit = new QLineEdit;
	addButton = new QPushButton("Add server");

	/* Layout definition */
	QHBoxLayout *newServerLayout = new QHBoxLayout;
	newServerLayout->addWidget(nameLabel);
	newServerLayout->addWidget(nameEdit);
	newServerLayout->addWidget(ipLabel);
	newServerLayout->addWidget(ipEdit);
	newServerLayout->addWidget(portLabel);
	newServerLayout->addWidget(portEdit);
	newServerLayout->addWidget(descLabel);
	newServerLayout->addWidget(descEdit);
	newServerLayout->addWidget(addButton);
	QGroupBox *addGroup = new QGroupBox("New server");;
	addGroup->setLayout(newServerLayout);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(serverView);
	mainLayout->addWidget(addGroup);

	setLayout(mainLayout);

	/* Connections */
	connect(addButton, SIGNAL(clicked()), this, SLOT(addServer()));
	connect(serverView, SIGNAL(deleteServer(QTreeWidgetItem *)), this, SLOT(deleteServer(QTreeWidgetItem *)));
	connect(serverView, SIGNAL(connectToServer(QTreeWidgetItem *)), this, SLOT(connectToServer(QTreeWidgetItem *)));
	connect(serverView, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(connectToServer(QTreeWidgetItem *)));

	/* Load server list */
	serverFile = new QFile(QDir::homePath() + "/.tractor/server.list");
	serverFile->open(QIODevice::ReadOnly | QIODevice::Text);
	while(!serverFile->atEnd())
	{
		 QString serverString = serverFile->readLine();
		 QStringList serverAttr = serverString.split(";");
		 Server server;
		 server.name = serverAttr.at(0);
		 server.ip = serverAttr.at(1);
		 server.port = serverAttr.at(2);
		 server.desc = serverAttr.at(3);
		 server.desc.chop(1);
		 serverList.append(server);
	}
	serverFile->close();

	int n = serverList.count();
	for(int i=0; i<n; i++)
	{
		QTreeWidgetItem *temp = new QTreeWidgetItem(serverView);
		temp->setIcon(0, iconProvider->icon(QFileIconProvider::Computer));
		temp->setText(0, serverList.at(i).name);
		temp->setText(1, serverList.at(i).ip + " : " + serverList.at(i).port);
		temp->setText(2, serverList.at(i).desc);
		temp->setSizeHint(0, QSize(temp->sizeHint(0).width(), 18));
		serverViewItems.append(temp);
	}
}

void ServersPage::writeServerList()
{
	serverFile->open(QIODevice::WriteOnly | QIODevice::Text);
	int n = serverList.count();
	for(int i=0; i<n; i++)
	{
		QString serverString = serverList.at(i).name + ";" + serverList.at(i).ip + ";" + serverList.at(i).port + ";" + serverList.at(i).desc + "\n";
		serverFile->write(serverString.toAscii());
	}
	serverFile->close();
}

void ServersPage::addServer()
{
	if(ipEdit->text()=="" || portEdit->text()=="")
	{
		QMessageBox::information(this, "Missing informations", "IP and Port are required.");
		return;
	}
	Server newServer;
	newServer.name = nameEdit->text();
	newServer.ip = ipEdit->text();
	newServer.port = portEdit->text();
	newServer.desc = descEdit->text();
	int n = serverList.count();
	for(int i=0; i<n; i++)
	{
		if(serverList.at(i).ip==newServer.ip && serverList.at(i).port==newServer.port)
		{
			QMessageBox::information(this, "Existing server", "There is already a server with the same IP and Port.");
			return;
		}
	}
	nameEdit->clear();
	ipEdit->clear();
	portEdit->clear();
	descEdit->clear();
	serverList.append(newServer);
	QTreeWidgetItem *temp = new QTreeWidgetItem(serverView);
	temp->setIcon(0, iconProvider->icon(QFileIconProvider::Computer));
	temp->setText(0, newServer.name);
	temp->setText(1, newServer.ip + " : " + newServer.port);
	temp->setText(2, newServer.desc);
	temp->setSizeHint(0, QSize(temp->sizeHint(0).width(), 18));
	serverViewItems.append(temp);
	writeServerList();
}

void ServersPage::deleteServer(QTreeWidgetItem *item)
{
	int pos = serverViewItems.indexOf(item);
	serverViewItems.removeAt(pos);
	serverList.removeAt(pos);
	delete item;
	writeServerList();
}

void ServersPage::connectToServer(QTreeWidgetItem *item)
{
	int pos = serverViewItems.indexOf(item);
	emit serverSelected((serverList.at(pos).name.isEmpty()) ? serverList.at(pos).ip : serverList.at(pos).name);
	client->connectToServer(serverList.at(pos).ip, serverList.at(pos).port.toShort());
	item->setForeground(0, QBrush(QColor(255, 0, 0)));
	item->setForeground(1, QBrush(QColor(255, 0, 0)));
	item->setForeground(2, QBrush(QColor(255, 0, 0)));
}
