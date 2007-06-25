#ifndef SHARED_H
#define SHARED_H

#include <QtGui>
#include "TClient.h"

class SharedPage : public QFrame
{
	Q_OBJECT

public:
	SharedPage(TClient *cp);

private:
	TClient *client;
};

#endif //SHARED_H
