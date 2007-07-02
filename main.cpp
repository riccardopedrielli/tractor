#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QDir dir;
	QString path = dir.homePath() + "/.tractor/";
	dir.mkpath(path);
	TSettings *settings = new TSettings(path);
	TServerThread *serverThread = new TServerThread(settings->port, settings->maxconnections, path + "sharedlist.xml");
	//TClientThread *clientThread = new TClientThread(path + "sharedlist.xml", settings->sharedfiles, settings->temporaryfiles, settings->completedfiles, settings->port);
	TClient *client = new TClient(path + "sharedlist.xml", settings->sharedfiles, settings->temporaryfiles, settings->completedfiles, settings->port);
	MainWindow *mainWindow = new MainWindow(settings, serverThread->server, client /*clientThread->client*/);
	mainWindow->show();
	return app.exec();
}
