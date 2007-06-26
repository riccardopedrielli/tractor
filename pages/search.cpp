#include "search.h"

SearchTab::SearchTab()
{
	menu = new QMenu;
	closeAction = new QAction("Close", menu);
	menu->addAction(closeAction);
}

void SearchTab::contextMenuEvent(QContextMenuEvent *event)
{
	QAction *action = menu->exec(event->globalPos());
	if(action == closeAction)
	{
		emit deletePage();
	}
}

SearchView::SearchView(TClient *cp, int sid)
{
	client = cp;
	id = sid;

	menu = new QMenu;
	downloadAction = new QAction("Download", menu);
	menu->addAction(downloadAction);

	QTreeWidgetItem *header = new QTreeWidgetItem;
	header->setText(0, "Name");
	header->setText(1, "Size");
	header->setText(2, "Sources");
	header->setText(3, "Completes");
	header->setText(4, "File ID");

	setHeaderItem(header);
	setColumnWidth(0, 250);
	setColumnWidth(1, 60);
	setColumnWidth(2, 80);
	setColumnWidth(3, 80);
	setColumnWidth(4, 200);
	setSortingEnabled(true);
	setRootIsDecorated(false);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	sortItems(0, Qt::AscendingOrder);

	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(addToDownloads(QTreeWidgetItem *)));
}

void SearchView::contextMenuEvent(QContextMenuEvent *event)
{
	QTreeWidgetItem *item = itemAt(event->pos());
	if(!item)
	{
		return;
	}
	QAction *action = menu->exec(event->globalPos());
	if(action == downloadAction)
	{
		addToDownloads(item);
	}
}

void SearchView::addToDownloads(QTreeWidgetItem *item)
{
	QMessageBox::information(this, "Debug information", "getFile(" + item->text(0) + ", " + item->text(4).toLower() + ", " + item->data(1, Qt::ToolTipRole).toString() + ")");
	client->getFile(item->text(0), item->text(4).toLower(), item->data(1, Qt::ToolTipRole).toString());
}

SearchPage::SearchPage(TClient *cp)
{
	sid = 0;
	client = cp;

	/* Elements definition */
	searchEdit = new QLineEdit;
	searchEdit->setFixedWidth(searchEdit->sizeHint().width()*2);
	searchButton = new QPushButton("Search");

	searchTab = new SearchTab;

	searchPages = new QStackedWidget;
	SearchView *emptySearch = new SearchView(client, sid);
	searchPages->addWidget(emptySearch);

	/* Layout definition */
	QHBoxLayout *searchLayout = new QHBoxLayout;
	searchLayout->addWidget(searchEdit);
	searchLayout->addSpacing(5);
	searchLayout->addWidget(searchButton);
	searchLayout->addStretch();

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->addLayout(searchLayout);
	mainLayout->addSpacing(8);
	mainLayout->addWidget(searchTab);
	mainLayout->addWidget(searchPages);
	setLayout(mainLayout);

	/* Connections */
	connect(searchTab, SIGNAL(currentChanged(int)), searchPages, SLOT(setCurrentIndex(int)));
	connect(searchTab, SIGNAL(deletePage()), this, SLOT(deletePage()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(startNewSearch()));
	connect(searchEdit, SIGNAL(returnPressed()), this, SLOT(startNewSearch()));
}

void SearchPage::deletePage()
{
	SearchView *oldSearch = (SearchView *)searchPages->currentWidget();
	searchPages->removeWidget(searchPages->currentWidget());
	delete oldSearch;
	searchTab->removeTab(searchTab->currentIndex());
	if(searchTab->count() == 0)
	{
		SearchView *emptySearch = new SearchView(client, sid);
		searchPages->addWidget(emptySearch);
	}
}

void SearchPage::startNewSearch()
{
	if(!client->isConnected())
	{
		QMessageBox::information(this, "Not connected", "You are not connected to any server.\nYou have to be connected to a server in order to do a search.");
		return;
	}
	if(searchEdit->text() == "")
	{
		QMessageBox::information(this, "Empty search", "You have to write something to search.");
		return;
	}
	if(searchTab->count() > 0)
	{
		SearchView *newSearch = new SearchView(client, sid);
		searchPages->addWidget(newSearch);
	}
	searchTab->addTab(searchEdit->text());
	searchTab->setCurrentIndex(searchTab->count()-1);
	client->find(searchEdit->text(), sid);
	sid++;
}

void SearchPage::addResult(QString fid, QString name, QString dim, QString sources, QString complete, QString sid)
{
	int n = searchPages->count();
	for(int i=0; i<n; i++)
	{
		if(((SearchView *)searchPages->widget(i))->id == sid.toInt())
		{
			QString bytes = dim;
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
			complete = QVariant(complete.toInt()*100/sources.toInt()).toString() + "%";
			fid = fid.toUpper();
			QTreeWidgetItem *newResult = new QTreeWidgetItem;
			newResult->setText(0, name);
			newResult->setTextAlignment(1, Qt::AlignRight | Qt::AlignVCenter);
			newResult->setText(1, dim);
			newResult->setData(1, Qt::ToolTipRole, bytes);
			newResult->setTextAlignment(2, Qt::AlignRight | Qt::AlignVCenter);
			newResult->setText(2, sources);
			newResult->setTextAlignment(3, Qt::AlignRight | Qt::AlignVCenter);
			newResult->setText(3, complete);
			newResult->setText(4, fid);
			newResult->setSizeHint(0, QSize(newResult->sizeHint(0).width(), 18));
			((SearchView *)searchPages->widget(i))->addTopLevelItem(newResult);
			break;
		}
	}
}
