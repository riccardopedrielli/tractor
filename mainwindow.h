#ifndef	CLIENTWINDOW_H
#define	CLIENTWINDOW_H

#include <QtGui>
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
	MainWindow(TSettings *settingsPointer);

private:
	TSettings *settings;

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
	OptionsWindow *optionsWindow;

	/* Interface setup methods */
	void setuptoolbar();
	void setupwindow();
	void setupstatus();
	void connectui();
	void setTheme(QString theme);

private slots:
	void about();
	void notImplemented();
	void modifySettings();
	void eventToStatusBar(QString text);
};

#endif //CLIENTWINDOW_H
