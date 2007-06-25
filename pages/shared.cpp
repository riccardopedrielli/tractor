#include "shared.h"

SharedPage::SharedPage(TClient *cp)
{
	client = cp;
	setFrameShape(QFrame::StyledPanel);
}
