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
    int StringToHex(QString qstr, QByteArray &data);
    char CharToHex(char ch);

    bool is_auto_move_to_bottom;
    bool is_port_opened;

protected:
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject *, QEvent *);

private slots:
    void slotOpenPort();
    void slotDisplayReceiveData(QVector<quint8>, bool);
    void slotDisplaySendData(const QByteArray &data);
    void slotSend();
    void slotClearReceive();
    void slotClearSend();
};

#endif // MAINWINDOW_H
