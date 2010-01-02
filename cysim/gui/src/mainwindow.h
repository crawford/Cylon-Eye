#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QDialog>
#include <QGraphicsScene>
#include <QProcess>
#include "panel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QDialog {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	QGraphicsScene _scene;
	QProcess *cmdProc;
	QList<Panel*> *panels;

	bool parseConfig();
	bool startCmdProc();

private slots:
	void readCmdProc();
	void readCmdProcError();
};

#endif // MAINWINDOW_H
