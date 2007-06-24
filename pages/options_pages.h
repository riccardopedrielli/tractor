#ifndef SETTINGS_PAGES_H
#define SETTINGS_PAGES_H

#include <QtGui>

class ConnectionPage : public QFrame
{
	Q_OBJECT

public:
	ConnectionPage();
	QLineEdit *uploadEdit;
	QLineEdit *downloadEdit;
	QLineEdit *portEdit;
	QLineEdit *maxconEdit;
};

class DirectoriesPage : public QFrame
{
	Q_OBJECT

public:
	DirectoriesPage();
	QLineEdit *temporaryEdit;
	QLineEdit *completedEdit;
	QLineEdit *sharedEdit;

private:
	QPushButton *temporaryButton;
	QPushButton *completedButton;
	QPushButton *sharedButton;

private slots:
	void temporaryDirectoryEdit();
	void completedDirectoryEdit();
	void sharedDirectoryEdit();
};

class InterfacePage : public QFrame
{
	Q_OBJECT

public:
	InterfacePage();
	QComboBox *themeSelector;
};

#endif //SETTINGS_PAGES_H
