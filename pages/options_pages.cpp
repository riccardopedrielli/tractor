#include "options_pages.h"

ConnectionPage::ConnectionPage()
{
	setAutoFillBackground(true);
	setBackgroundRole(QPalette::Base);
	setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

	QHBoxLayout *uploadLayout = new QHBoxLayout;
	QGroupBox *uploadBox = new QGroupBox("Upload limit");
	uploadBox->setHidden(true);
	uploadBox->setLayout(uploadLayout);
	QLabel *uploadLabel = new QLabel("KB/s");
	uploadEdit = new QLineEdit;
	uploadLayout->addWidget(uploadEdit);
	uploadLayout->addWidget(uploadLabel);

	QHBoxLayout *downloadLayout = new QHBoxLayout;
	QGroupBox *downloadBox = new QGroupBox("Download limit");
	downloadBox->setHidden(true);
	downloadBox->setLayout(downloadLayout);
	QLabel *downloadLabel = new QLabel("KB/s");
	downloadEdit = new QLineEdit;
	downloadLayout->addWidget(downloadEdit);
	downloadLayout->addWidget(downloadLabel);

	QHBoxLayout *bandwidthLayout = new QHBoxLayout;
	bandwidthLayout->addWidget(uploadBox);
	bandwidthLayout->addWidget(downloadBox);

	QHBoxLayout *portLayout = new QHBoxLayout;
	QGroupBox *portBox = new QGroupBox("Server port");
	portBox->setLayout(portLayout);
	portEdit = new QLineEdit;
	portLayout->addWidget(portEdit);

	QHBoxLayout *maxconLayout = new QHBoxLayout;
	QGroupBox *maxconBox = new QGroupBox("Max connections");
	maxconBox->setLayout(maxconLayout);
	maxconEdit = new QLineEdit;
	maxconLayout->addWidget(maxconEdit);

	QHBoxLayout *connectionLayout = new QHBoxLayout;
	connectionLayout->addWidget(portBox);
	connectionLayout->addWidget(maxconBox);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(bandwidthLayout);
	mainLayout->addLayout(connectionLayout);
	mainLayout->addStretch();
	setLayout(mainLayout);
}

DirectoriesPage::DirectoriesPage()
{
	setAutoFillBackground(true);
	setBackgroundRole(QPalette::Base);
	setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

	QHBoxLayout *temporaryLayout = new QHBoxLayout;
	QGroupBox *temporaryBox = new QGroupBox("Temporary files");
	temporaryBox->setLayout(temporaryLayout);
	temporaryEdit = new QLineEdit;
	temporaryButton = new QPushButton("Browse...");
	temporaryLayout->addWidget(temporaryEdit);
	temporaryLayout->addWidget(temporaryButton);

	QHBoxLayout *completedLayout = new QHBoxLayout;
	QGroupBox *completedBox = new QGroupBox("Completed files");
	completedBox->setLayout(completedLayout);
	completedEdit = new QLineEdit;
	completedButton = new QPushButton("Browse...");
	completedLayout->addWidget(completedEdit);
	completedLayout->addWidget(completedButton);

	QHBoxLayout *sharedLayout = new QHBoxLayout;
	QGroupBox *sharedBox = new QGroupBox("Shared files");
	sharedBox->setLayout(sharedLayout);
	sharedEdit = new QLineEdit;
	sharedButton = new QPushButton("Browse...");
	sharedLayout->addWidget(sharedEdit);
	sharedLayout->addWidget(sharedButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(temporaryBox);
	mainLayout->addWidget(completedBox);
	mainLayout->addWidget(sharedBox);
	mainLayout->addStretch();
	setLayout(mainLayout);
	
	connect(temporaryButton, SIGNAL(clicked()), this, SLOT(temporaryDirectoryEdit()));
	connect(completedButton, SIGNAL(clicked()), this, SLOT(completedDirectoryEdit()));
	connect(sharedButton, SIGNAL(clicked()), this, SLOT(sharedDirectoryEdit()));
}

void DirectoriesPage::temporaryDirectoryEdit()
{
	QString newdir = QFileDialog::getExistingDirectory(this, "Temporary files directory", temporaryEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(!newdir.isEmpty())
		temporaryEdit->setText(newdir);
}

void DirectoriesPage::completedDirectoryEdit()
{
	QString newdir = QFileDialog::getExistingDirectory(this, "Completed files directory", completedEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(!newdir.isEmpty())
		completedEdit->setText(newdir);
}

void DirectoriesPage::sharedDirectoryEdit()
{
	QString newdir = QFileDialog::getExistingDirectory(this, "Shared files directory", sharedEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(!newdir.isEmpty())
		sharedEdit->setText(newdir);
}

InterfacePage::InterfacePage()
{
	setAutoFillBackground(true);
	setBackgroundRole(QPalette::Base);
	setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

	QHBoxLayout *themeLayout = new QHBoxLayout;
	QGroupBox *themeBox = new QGroupBox("Theme");
	themeBox->setLayout(themeLayout);

	themeSelector = new QComboBox;
	themeSelector->addItem(QIcon(":/icons/themes/eternalblue/downloads.png"), "eternalblue");
	themeSelector->addItem(QIcon(":/icons/themes/folders/downloads.png"), "folders");
	themeSelector->addItem(QIcon(":/icons/themes/greyblue/downloads.png"), "greyblue");
	themeSelector->addItem(QIcon(":/icons/themes/kids/downloads.png"), "kids");
	themeSelector->addItem(QIcon(":/icons/themes/newblue/downloads.png"), "newblue");
	themeSelector->addItem(QIcon(":/icons/themes/snow/downloads.png"), "snow");
	themeSelector->addItem(QIcon(":/icons/themes/stylish/downloads.png"), "stylish");
	themeSelector->addItem(QIcon(":/icons/themes/whiteblock/downloads.png"), "whiteblock");

	themeLayout->addWidget(themeSelector);

	QHBoxLayout *styleLayout = new QHBoxLayout;
	QGroupBox *styleBox = new QGroupBox("Style");
	styleBox->setLayout(styleLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(themeBox);
	mainLayout->addStretch();
	setLayout(mainLayout);
}
