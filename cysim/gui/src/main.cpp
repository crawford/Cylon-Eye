#include <QtGui/QApplication>
#include <QFile>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	if(argc != 2) {
		qCritical("Usage: CylonEyeSim config_file_name");
		return -1;
	}
	if(!QFile::exists(a.arguments()[1])) {
		qCritical("Error: specified config file not found");
		return -2;
	}
	if(!QFile::exists("CmdProc")) {
		qCritical("Error: command processor ('CmdProc.class') not found");
		return -3;
	}

	MainWindow w;
	w.show();
    return a.exec();
}
