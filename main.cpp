#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
		QDir dir;
	QString path = dir.homePath() + "/.tractor/";
	dir.mkpath(path);
	TSettings *settings = new TSettings(path);
	TServer *server = new TServer(settings->port, settings->maxconnections, path + "sharedlist.xml");
	TClient *client = new TClient(path + "sharedlist.xml", settings->sharedfiles, settings->temporaryfiles, settings->completedfiles, settings->port);
	MainWindow *mainWindow = new MainWindow(settings, server, client);
	mainWindow->show();
	return app.exec();
}
