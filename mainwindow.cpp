#include "mainwindow.h"

MainWindow::MainWindow(TSettings *settingsp, TServer *serverp, TClient *clientp)
{
	timer = new QTimer;
	settings = settingsp;
	server = serverp;
	client = clientp;
	setuptoolbar();
	setupwindow();
	setupstatus();
	connectui();
	selectedserver = new QString("");
	setTheme(settings->theme);
	timer->start(1000);
	client->updateSharedList();
}

void MainWindow::setuptoolbar()
{
	QSize iconsize = QSize(60, 32);
	Qt::ToolButtonStyle buttonstyle = Qt::ToolButtonTextUnderIcon;

	/* Elements definition */
	QToolBar *toolBar = new QToolBar;
	toolBar->setMovable(false);
	toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

	serversButton = new TToolButton(0);
	serversButton->setToolButtonStyle(buttonstyle);
	serversButton->setIconSize(iconsize);
	serversButton->setText("Servers");
	serversButton->setChecked(true);

	searchButton = new TToolButton(1);
	searchButton->setToolButtonStyle(buttonstyle);
	searchButton->setIconSize(iconsize);
	searchButton->setText("Search");

	sharedButton = new TToolButton(2);
	sharedButton->setToolButtonStyle(buttonstyle);
	sharedButton->setIconSize(iconsize);
	sharedButton->setText("Shared");

	optionsButton = new QToolButton;
	optionsButton->setToolButtonStyle(buttonstyle);
	optionsButton->setIconSize(iconsize);
	optionsButton->setText("Options");
	optionsButton->setCheckable(true);
	optionsButton->setAutoRaise(true);

	downloadsButton = new QToolButton;
	downloadsButton->setToolButtonStyle(buttonstyle);
	downloadsButton->setIconSize(iconsize);
	downloadsButton->setText("Downloads");
	downloadsButton->setCheckable(true);
	downloadsButton->setAutoRaise(true);
	downloadsButton->setChecked(true);

	upIcon = new QLabel;
	upLabel = new QLabel;
	upLabel->setText("Upload:");
	upSpeed = new QLabel;
	upSpeed->setFixedWidth(65);
	upSpeed->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	upSpeed->setText("0.0 B/s");

	downIcon = new QLabel;
	downLabel = new QLabel;
	downLabel->setText("Download:");
	downSpeed = new QLabel;
	downSpeed->setFixedWidth(65);
	downSpeed->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	downSpeed->setText("0.0 B/s");

	/* Layout definition */
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(serversButton);
	buttonsLayout->addWidget(searchButton);
	buttonsLayout->addWidget(sharedButton);
	buttonsLayout->setMargin(0);
	buttonsLayout->setSpacing(0);
	
	QWidget *layoutContainerLeft = new QWidget;
	layoutContainerLeft->setLayout(buttonsLayout);

	QGridLayout *infoLayout = new QGridLayout;
	infoLayout->addWidget(upIcon, 0, 0);
	infoLayout->addWidget(upLabel, 0, 1);
	infoLayout->addWidget(upSpeed, 0, 2);
	infoLayout->addWidget(downIcon, 1, 0);
	infoLayout->addWidget(downLabel, 1, 1);
	infoLayout->addWidget(downSpeed, 1, 2);
	infoLayout->setMargin(5);
	infoLayout->setSpacing(5);

	QHBoxLayout *barLayout = new QHBoxLayout;
	barLayout->addWidget(optionsButton);
	barLayout->addStretch();
	barLayout->addWidget(downloadsButton);
	barLayout->addSpacing(4);
	barLayout->addLayout(infoLayout);
	barLayout->setMargin(2);
	barLayout->setSpacing(0);

	QWidget *layoutContainerRight = new QWidget;
	layoutContainerRight->setLayout(barLayout);
	toolBar->addWidget(layoutContainerLeft);
	toolBar->addSeparator();
	toolBar->addWidget(layoutContainerRight);

	addToolBar(toolBar);
}

void MainWindow::setupwindow()
{
	/* Elements definition */
	serversPage = new ServersPage(client);

	searchPage = new SearchPage(client);

	sharedPage = new SharedPage(server);

	downloadsPage =	new	DownloadsPage(client);
	downloadsPage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	downloadsPage->setFixedWidth(230);

	pages = new QStackedWidget;
	pages->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	pages->addWidget(serversPage);
	pages->addWidget(searchPage);
	pages->addWidget(sharedPage);
	pages->setCurrentIndex(0);

	/* Layout definition */
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->setMargin(4);
	mainLayout->setSpacing(4);
	mainLayout->addWidget(pages);
	mainLayout->addWidget(downloadsPage);

	QWidget *centralWidget = new QWidget;
	centralWidget->setLayout(mainLayout);

	setCentralWidget(centralWidget);
	setWindowTitle("Tractor - Because the tractor pull more than the mule");
	setWindowIcon(QIcon(":/icons/tractor.png"));
	setWindowState(Qt::WindowMaximized);
	setMinimumWidth(700);
	setMinimumHeight(440);
}

void MainWindow::setupstatus()
{
	statusBar = new QStatusBar;
	statusBar->setSizeGripEnabled(false);
	statusBar->showMessage("Welcome to Tractor");
	setStatusBar(statusBar);
}

void MainWindow::connectui()
{
	connect(serversButton, SIGNAL(selected(int)), pages, SLOT(setCurrentIndex(int)));
	connect(searchButton, SIGNAL(selected(int)), pages, SLOT(setCurrentIndex(int)));
	connect(sharedButton, SIGNAL(selected(int)), pages, SLOT(setCurrentIndex(int)));
	connect(optionsButton, SIGNAL(clicked()), this, SLOT(modifySettings()));
	connect(downloadsButton, SIGNAL(toggled(bool)), downloadsPage, SLOT(setVisible(bool)));
	connect(serversPage, SIGNAL(serverSelected(QString)), this, SLOT(serverSelected(QString)));
	connect(client, SIGNAL(serverConnected()), this, SLOT(connectionSuccessful()));
	connect(client, SIGNAL(fileReceived(QString, QString, QString, QString, QString, QString)), searchPage, SLOT(addResult(QString, QString, QString, QString, QString, QString)));
	connect(timer, SIGNAL(timeout()), this, SLOT(updateSpeeds()));
}

void MainWindow::setTheme(QString theme)
{
	serversButton->setIcon(QIcon(":/icons/themes/" + theme + "/servers.png"));
	searchButton->setIcon(QIcon(":/icons/themes/" + theme + "/search.png"));
	sharedButton->setIcon(QIcon(":/icons/themes/" + theme + "/shared.png"));
	optionsButton->setIcon(QIcon(":/icons/themes/" + theme + "/settings.png"));
	downloadsButton->setIcon(QIcon(":/icons/themes/" + theme + "/downloads.png"));
	QPixmap up(":/icons/themes/" + theme + "/up.png/");
	up=up.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	upIcon->setPixmap(up);
	QPixmap down(":/icons/themes/" + theme + "/down.png");
	down=down.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	downIcon->setPixmap(down);
}

void MainWindow::modifySettings()
{
	OptionsWindow *optionsWindow = new OptionsWindow;
	optionsWindow->setModal(true);
	optionsWindow->connectionPage->portEdit->setText(QVariant(settings->port).toString());
	optionsWindow->connectionPage->maxconEdit->setText(QVariant(settings->maxconnections).toString());
	optionsWindow->directoriesPage->temporaryEdit->setText(settings->temporaryfiles);
	optionsWindow->directoriesPage->completedEdit->setText(settings->completedfiles);
	optionsWindow->directoriesPage->sharedEdit->setText(settings->sharedfiles);
	optionsWindow->interfacePage->themeSelector->setCurrentIndex(optionsWindow->interfacePage->themeSelector->findText(settings->theme));
	if(optionsWindow->exec() == QDialog::Accepted)
	{
		settings->port = optionsWindow->connectionPage->portEdit->text().toInt();
		settings->maxconnections = optionsWindow->connectionPage->maxconEdit->text().toInt();
		settings->temporaryfiles = optionsWindow->directoriesPage->temporaryEdit->text();
		settings->completedfiles = optionsWindow->directoriesPage->completedEdit->text();
		settings->sharedfiles = optionsWindow->directoriesPage->sharedEdit->text();
		settings->theme = optionsWindow->interfacePage->themeSelector->currentText();
		setTheme(settings->theme);
		settings->writeSettings();
	}
	delete optionsWindow;
	optionsButton->setChecked(false);
}

void MainWindow::showEvent(QString text)
{
	statusBar->showMessage(text);
}

void MainWindow::serverSelected(QString server)
{
	*selectedserver = server;
	showEvent("Connecting to " + server + "...");
}

void MainWindow::connectionSuccessful()
{
	showEvent("Connected to " + *selectedserver);
}

void MainWindow::updateSpeeds()
{
	float speed = 0;
	foreach(TTransfer *transfer, client->transferslist)
	{
		speed += transfer->getDownloadSpeed();
	}
	int unit = 0;
	while(speed >= 1024 && unit < 3)
	{
		speed /= 1024;
		unit++;
	}
	QString parsedSpeed;
	switch(unit)
	{
		case 0:
			parsedSpeed = parsedSpeed.setNum(speed, 'f', 1) + " B/s";
			break;
		case 1:
			parsedSpeed = parsedSpeed.setNum(speed, 'f', 1) + " KB/s";
			break;
		case 2:
			parsedSpeed = parsedSpeed.setNum(speed, 'f', 1) + " MB/s";
			break;
	}
	downSpeed->setText(parsedSpeed);
}
