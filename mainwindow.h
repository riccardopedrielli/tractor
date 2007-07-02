#ifndef	CLIENTWINDOW_H
#define	CLIENTWINDOW_H

#include <QtGui>
#include "TClient.h"
#include "TServer.h"
#include "TSettings.h"
#include "options.h"
#include "ttoolbutton.h"
#include "pages/servers.h"
#include "pages/search.h"
#include "pages/shared.h"
#include "pages/downloads.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(TSettings *settingsp, TServer *serverp, TClient *clientp);

private:
	TSettings *settings;
	TServer *server;
	TClient *client;
	QString *selectedserver;
	QTimer *timer;

	/* ToolBar elements */
	TToolButton *serversButton;
	TToolButton *searchButton;
	TToolButton *sharedButton;
	QToolButton *optionsButton;
	QToolButton *downloadsButton;
	QLabel *upIcon;
	QLabel *upLabel;
	QLabel *upSpeed;
	QLabel *downIcon;
	QLabel *downLabel;
	QLabel *downSpeed;

	/* Window elements */
	QStackedWidget *pages;
	ServersPage *serversPage;
	SearchPage *searchPage;
	SharedPage *sharedPage;
	DownloadsPage *downloadsPage;
	QStatusBar	*statusBar;

	/* Interface setup methods */
	void setuptoolbar();
	void setupwindow();
	void setupstatus();
	void connectui();
	void setTheme(QString theme);

private slots:
	void modifySettings();
	void showEvent(QString text);
	void serverSelected(QString server);
	void connectionSuccessful();
	void updateSpeeds();
};

#endif //CLIENTWINDOW_H
