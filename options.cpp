#include "options.h"

OptionsWindow::OptionsWindow()
{
	/* Elements definition */
	menu = new QListWidget;
	menu->setIconSize(QSize(48, 48));
	menu->setFixedWidth(160);
	menu->setSelectionBehavior(QAbstractItemView::SelectRows);

	QListWidgetItem *connectionButton = new QListWidgetItem(menu);
	connectionButton->setIcon(QIcon(":/icons/options/connection.png"));
	connectionButton->setText("Connection");
	connectionButton->setTextAlignment(Qt::AlignVCenter);
	connectionButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *foldersButton = new QListWidgetItem(menu);
	foldersButton->setIcon(QIcon(":/icons/options/directories.png"));
	foldersButton->setText("Directories");
	foldersButton->setTextAlignment(Qt::AlignVCenter);
	foldersButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *interfaceButton = new QListWidgetItem(menu);
	interfaceButton->setIcon(QIcon(":/icons/options/interface.png"));
	interfaceButton->setText("Interface");
	interfaceButton->setTextAlignment(Qt::AlignVCenter);
	interfaceButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	menu->setCurrentRow(0);

	connectionPage = new ConnectionPage;

	directoriesPage = new DirectoriesPage;

	interfacePage = new InterfacePage;

	pages = new QStackedWidget;
	pages->addWidget(connectionPage);
	pages->addWidget(directoriesPage);
	pages->addWidget(interfacePage);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	/* Layout definition */
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setMargin(4);
	mainLayout->setSpacing(4);
	mainLayout->addWidget(menu, 0, 0);
	mainLayout->addWidget(pages, 0, 1);
	mainLayout->addWidget(buttonBox, 1, 0, 1, 2);
	setLayout(mainLayout);
	setFixedSize(sizeHint());
	setWindowTitle("Tractor options");

	/* Connections */
	connect(menu, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void OptionsWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	if(!current)
		current = previous;
	pages->setCurrentIndex(menu->row(current));
}
