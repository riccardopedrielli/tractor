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

SearchView::SearchView()
{
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
	setColumnWidth(0, 200);
	setColumnWidth(1, 150);
	setColumnWidth(2, 100);
	setColumnWidth(3, 100);
	setColumnWidth(4, 200);
	setSortingEnabled(true);
	setRootIsDecorated(false);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	sortItems(0, Qt::AscendingOrder);
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
		emit addToDownloads(item);
	}
}

SearchPage::SearchPage()
{
	/* Elements definition */
	searchEdit = new QLineEdit;
	searchEdit->setFixedWidth(searchEdit->sizeHint().width()*2);
	searchButton = new QPushButton("Search");

	searchTab = new SearchTab;
	searchTab->addTab("asd");
	searchTab->addTab("lol");
	searchTab->addTab("wtf");

	SearchView *asdView = new SearchView;
	SearchView *lolView = new SearchView;
	SearchView *wtfView = new SearchView;
	
	QTreeWidgetItem *asd1 = new QTreeWidgetItem(asdView);
	asd1->setText(0, "asd1");
	QTreeWidgetItem *asd2 = new QTreeWidgetItem(asdView);
	asd2->setText(0, "asd2");
	QTreeWidgetItem *asd3 = new QTreeWidgetItem(asdView);
	asd3->setText(0, "asd3");
	QTreeWidgetItem *lol1 = new QTreeWidgetItem(lolView);
	lol1->setText(0, "lol1");
	QTreeWidgetItem *lol2 = new QTreeWidgetItem(lolView);
	lol2->setText(0, "lol2");
	QTreeWidgetItem *wtf1 = new QTreeWidgetItem(wtfView);
	wtf1->setText(0, "wtf1");
	QTreeWidgetItem *wtf2 = new QTreeWidgetItem(wtfView);
	wtf2->setText(0, "wtf2");
	QTreeWidgetItem *wtf3 = new QTreeWidgetItem(wtfView);
	wtf3->setText(0, "wtf3");
	QTreeWidgetItem *wtf4 = new QTreeWidgetItem(wtfView);
	wtf4->setText(0, "wtf4");

	searchPages = new QStackedWidget;
	searchPages->addWidget(asdView);
	searchPages->addWidget(lolView);
	searchPages->addWidget(wtfView);

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
}

void SearchPage::deletePage()
{
	SearchView *oldSearch = (SearchView *)searchPages->currentWidget();
	searchPages->removeWidget(searchPages->currentWidget());
	delete oldSearch;
	searchTab->removeTab(searchTab->currentIndex());
	if(searchTab->count() == 0)
	{
		SearchView *emptySearch = new SearchView;
		searchPages->addWidget(emptySearch);
	}
}

void SearchPage::startNewSearch()
{
	if(searchEdit->text() == "")
	{
		QMessageBox::information(this, "Empty search", "You have to write something to search.");
		return;
	}
	if(searchTab->count() > 0)
	{
		SearchView *newSearch = new SearchView;
		searchPages->addWidget(newSearch);
	}
	searchTab->addTab(searchEdit->text());
}
