#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "mycom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MyCom *serial_port_;

private:
    Ui::MainWindow *ui;
    bool is_port_opened;
    int StringToHex(QString qstr, QByteArray &data);
    char CharToHex(char ch);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void slotOpenPort();
    void slotDisplayReceiveData(QVector<quint8>, bool);
    void slotSend();
};

#endif // MAINWINDOW_H
