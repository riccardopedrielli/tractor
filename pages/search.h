#ifndef SEARCH_H
#define SEARCH_H

#include <QtGui>
#include "TClient.h"

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
	int id;
	SearchView(TClient *cp, int sid);

private:
	TClient *client;
	QMenu *menu;
	QAction *downloadAction;
	void contextMenuEvent(QContextMenuEvent *event);

private slots:
	void addToDownloads(QTreeWidgetItem *item);
};

class SearchPage : public QWidget
{
	Q_OBJECT

public:
	SearchPage(TClient *cp);

private:
	int sid;
	TClient *client;
	QLineEdit *searchEdit;
	QPushButton *searchButton;
	SearchTab *searchTab;
	QStackedWidget *searchPages;

private slots:
	void deletePage();
	void startNewSearch();
	void addResult(QString fid, QString name, QString dim, QString sources, QString complete, QString sid);
};

#endif //SEARCH_H
