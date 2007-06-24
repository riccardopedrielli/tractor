#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>
#include "pages/options_pages.h"

class OptionsWindow : public QDialog
{
	Q_OBJECT

public:
	OptionsWindow();
	ConnectionPage *connectionPage;
	DirectoriesPage *directoriesPage;
	InterfacePage *interfacePage;

private:
	QListWidget *menu;
	QStackedWidget *pages;

private slots:
	void changePage(QListWidgetItem *current, QListWidgetItem *previous);
};

#endif //SETTINGS_H
