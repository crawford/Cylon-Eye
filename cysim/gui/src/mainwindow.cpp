#include <QDomDocument>
#include <QFile>
#include <QDomDocument>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QDialog(parent), ui(new Ui::MainWindow) {
	panels = new QList<Panel*>();

	ui->setupUi(this);
	ui->grvPanels->setRenderHints(QPainter::Antialiasing);
	ui->grvPanels->setScene(&_scene);
	ui->grvPanels->setDragMode(QGraphicsView::NoDrag);

	parseConfig();
	startCmdProc();
}

MainWindow::~MainWindow() {
	delete ui;
	cmdProc->kill();
}

bool MainWindow::parseConfig() {
	QDomDocument doc;
	QFile file(qApp->arguments()[1]);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QString errorMsg;
	int errorLine;
	int errorCol;
	if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorCol)) {
		file.close();
		qDebug() << "Error Parsing (Line" << errorLine << ", Col" << errorCol << "): " << errorMsg;
		return false;
	}
	file.close();

	QDomElement docElem = doc.documentElement();
	QDomNode node = docElem.firstChild();

	bool ok;
	unsigned long long zid;
	short pid;
	short x;
	short y;

	while(!node.isNull()) {
		QDomElement elm = node.toElement(); // try to convert the node to an element
		node = node.nextSibling();

		if(elm.isNull()) continue;
		if(elm.tagName() != "panel") continue;

		zid = elm.attribute("zid", "0000000000000000").toLongLong(&ok, 16);
		if(!ok) continue;

		pid = elm.attribute("pid", "0").toShort(&ok, 10);
		if(!ok) continue;

		x = elm.attribute("x", "0").toShort(&ok, 10);
		if(!ok) continue;

		y = elm.attribute("y", "0").toShort(&ok, 10);
		if(!ok) continue;

		Panel *panel = new Panel(zid, pid);
		panel->setFlag(QGraphicsItem::ItemIsMovable, false);
		panel->setPos(x*panel->boundingRect().width(), y*panel->boundingRect().height());
		_scene.addItem(panel);
		panels->append(panel);
	}
	return true;
}

bool MainWindow::startCmdProc() {
	cmdProc = new QProcess();
	cmdProc->start("java.exe CmdProc");
	if(!cmdProc->waitForStarted(5000)) {
		qDebug() << cmdProc->errorString();
		return false;
	}

	connect(cmdProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readCmdProc()));
	return true;
}

void MainWindow::readCmdProc() {
	QString line;
	QStringList list;
	bool ok;
	unsigned long long zid;
	short pid;
	Panel *panel = 0;

	while(cmdProc->canReadLine()) {
		qDebug() << "Parsing data";
		line = cmdProc->readLine();
		qDebug() << line;

		//Parse input line into zid, pid, data, and led
		list = line.split('-');
		zid = (list[0]).toLongLong(&ok, 16);
		qDebug() << "zid: " << list[0];

		pid = (list[1]).toShort(&ok, 16);
		qDebug() << "pid: " << list[1];

		qDebug() << "data: " << list[2];

		//Find the panel object we need to modify
		for(short i = 0; i < panels->count(); i++) {
			if(panels->at(i)->getZID() == zid &&
			   panels->at(i)->getPID() == pid) {
				panel = panels->at(i);
				break;
			}
		}

		if(!panel) {
			qDebug() << "Could not find panel";
			continue;
		}

		for(short i = 0; i < 16; i++) {
			panel->setIntensity(i, list[2].mid(i, 1).toShort(&ok, 10));
		}

		panel->setAuxLED(list[3] != "0");

		panel->update();
	}
}
