#include "ttoolbutton.h"

TToolButton::TToolButton(int id)
{
	setAutoExclusive(true);
	setCheckable(true);
	setAutoRaise(true);

	buttonid = id;
	connect(this, SIGNAL(toggled(bool)), this, SLOT(emitSelected(bool)));
}

void TToolButton::emitSelected(bool ok)
{
	if(ok)
		emit selected(buttonid);
}
