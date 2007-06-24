#ifndef SEARCH_H
#define SEARCH_H

#include <QtGui>

class SearchTab : public QTabBar
{
	Q_OBJECT

public:
	SearchTab();

private:
	QMenu *menu;
	QAction *closeAction;
	void contextMenuEvent(QContextMenuEvent *event);

signals:
	void deletePage();
};

class SearchView : public QTreeWidget
{
	Q_OBJECT

public:
	SearchView();

private:
	QMenu *menu;
	QAction *downloadAction;
	void contextMenuEvent(QContextMenuEvent *event);

signals:
	void addToDownloads(QTreeWidgetItem *item);
};

class SearchPage : public QWidget
{
	Q_OBJECT

public:
	SearchPage();

private:
	QLineEdit *searchEdit;
	QPushButton *searchButton;
	SearchTab *searchTab;
	QStackedWidget *searchPages;

private slots:
	void deletePage();
	void startNewSearch();
};

#endif //SEARCH_H
