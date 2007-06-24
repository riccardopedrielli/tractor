#ifndef	TTOOLBUTTON_H
#define	TTOOLBUTTON_H

#include <QToolButton>

class TToolButton : public QToolButton
{
	Q_OBJECT

public:
	TToolButton(int id);

private:
	int buttonid;

private slots:
	void emitSelected(bool ok);

signals:
	void selected(int buttonid);
};

#endif //TTOOLBUTTON_H
