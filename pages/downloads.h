#ifndef DOWNLOADS_H
#define DOWNLOADS_H

#include <QtGui>
#include "TClient.h"

class DownloadsPage : public QListWidget
{
	Q_OBJECT

public:
	DownloadsPage(TClient *cp);

private:
	TClient *client;
};

#endif //DOWNLOADS_H
