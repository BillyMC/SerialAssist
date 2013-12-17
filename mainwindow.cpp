#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , is_port_opened(0)
{
    ui->setupUi(this);
    setWindowTitle(tr("Serial Assist V1.0"));
    resize(800, 600);

    serial_port_ = new MyCom(this);

    QStringList comname;
#ifdef LINUX
    for (int i = 0; i <= 10; ++i)
        comname << "ttyS" + QString::number(i);
#else
    for (int i = 1; i <= 10; ++i)
        comname << "COM" + QString::number(i);
#endif
    ui->cobox_ComName->insertItems(0, comname);

    QStringList baudrate;
    baudrate << "4800" << "9600" << "14400" << "19200" << "115200";
    ui->cobox_BaudRate->insertItems(0, baudrate);
    ui->cobox_BaudRate->setCurrentIndex(4);

    QStringList parity;
    parity << tr("NONE") << tr("ODD") << tr("EVEN");
    ui->cobox_Parity->insertItems(0, parity);

    QStringList databits;
    databits << "5" << "6" << "7" << "8";
    ui->cobox_DataBits->insertItems(0, databits);
    ui->cobox_DataBits->setCurrentIndex(3);

    ui->cobox_StopBits->insertItem(0, "1");
    ui->cobox_StopBits->insertItem(1, "1.5");
    ui->cobox_StopBits->insertItem(2, "2");

    ui->tbtn_Send->setText(tr("Send"));

    QStringList cardno;
    for (int i = 0; i < 9; ++i)
        cardno << QString::number(i);
    ui->cobox_CardNO->insertItems(0, cardno);

    ui->txte_Receive->setFontPointSize(8);
    ui->txte_Receive->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->txte_Send->setFontPointSize(8);
    ui->txte_Send->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connect(ui->tbtn_OpenCom, SIGNAL(clicked()), this, SLOT(slotOpenPort()));
    connect(ui->tbtn_Send, SIGNAL(clicked()), this, SLOT(slotSend()));
    connect(serial_port_, SIGNAL(sigDisplayReceive(QVector<quint8>, bool)), this,
            SLOT(slotDisplayReceiveData(QVector<quint8>, bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    int line_height = 25;
    ui->txte_Receive->resize(320, 500);
    ui->txte_Receive->move(160, 2);

    ui->txte_Send->resize(320, 500);
    ui->txte_Send->move(480, 2);

    // 发送框
    ui->txte_SendWindow->resize(520, 50);
    ui->txte_SendWindow->move(160, 505);

    // 卡号
    ui->label_CardNO->resize(30, 20);
    ui->label_CardNO->move(690, 505);
    ui->cobox_CardNO->resize(70, 20);
    ui->cobox_CardNO->move(720, 505);

    // 发送按钮
    ui->tbtn_Send->resize(100, 30);
    ui->tbtn_Send->move(690, 527);

    // 串口号
    ui->label_comname->resize(50, line_height);
    ui->label_comname->move(5, 20);
    ui->cobox_ComName->resize(100, line_height);
    ui->cobox_ComName->move(50, 20);

    // 波特率
    ui->label_baudrate->resize(50, line_height);
    ui->label_baudrate->move(5, 45);
    ui->cobox_BaudRate->resize(100, line_height);
    ui->cobox_BaudRate->move(50, 45);

    // 校验位
    ui->label_parity->resize(50, line_height);
    ui->label_parity->move(5, 70);
    ui->cobox_Parity->resize(100, line_height);
    ui->cobox_Parity->move(50, 70);

    // 数据位
    ui->label_databits->resize(50, line_height);
    ui->label_databits->move(5, 95);
    ui->cobox_DataBits->resize(100, line_height);
    ui->cobox_DataBits->move(50, 95);

    // 停止位
    ui->label_stopbits->resize(50, line_height);
    ui->label_stopbits->move(5, 120);
    ui->cobox_StopBits->resize(100, line_height);
    ui->cobox_StopBits->move(50, 120);

    // 打开串口按钮
    ui->tbtn_OpenCom->resize(100, 32);
    ui->tbtn_OpenCom->move(20, 150);

    QMainWindow::resizeEvent(event);
}

/**
  * @brief 槽 打开串口
  **/
void MainWindow::slotOpenPort()
{
    if (is_port_opened)
    {
       serial_port_->closePort();
       is_port_opened = 0;
       ui->tbtn_OpenCom->setText(tr("Open Port"));
       return;
    }

    QString name = ui->cobox_ComName->currentText().trimmed();
    BaudRateType baudrate = (BaudRateType)ui->cobox_BaudRate->currentText().toInt();
    DataBitsType databits = (DataBitsType)ui->cobox_DataBits->currentText().toInt();
    ParityType parity = (ParityType)ui->cobox_Parity->currentIndex();
    StopBitsType stopbits = (StopBitsType)ui->cobox_StopBits->currentText().toInt();
    FlowType flow = (FlowType)0;

    serial_port_->openPort(name, baudrate, databits, parity, stopbits, flow);
    is_port_opened = 1;
    ui->tbtn_OpenCom->setText(tr("Close Port"));
}

/**
  * @brief 显示接收数据
  **/
void MainWindow::slotDisplayReceiveData(QVector<quint8>receive_data, bool is_check_ok)
{
    QString qstr_begin;
    QString qstr;
    QString qstr_end;

    for (int i = 0; i < 2; ++i)
        qstr_begin += QString::number(receive_data[i], 16).toUpper().rightJustified(2, '0') + " ";


    for (int i = 2; i < 14; ++i)
        qstr += QString::number(receive_data[i], 16).toUpper().rightJustified(2, '0') + " ";

    for (int i = 14; i < receive_data.size(); ++i)
        qstr_end += QString::number(receive_data[i], 16).toUpper().rightJustified(2, '0') + " ";

    ui->txte_Receive->setTextColor(Qt::gray);
    ui->txte_Receive->insertPlainText(qstr_begin);

    ui->txte_Receive->setTextColor(Qt::blue);
    ui->txte_Receive->insertPlainText(qstr);


    if (is_check_ok)
        ui->txte_Receive->setTextColor(Qt::gray);
    else
        ui->txte_Receive->setTextColor(Qt::red);
    ui->txte_Receive->insertPlainText(qstr_end);
}

/**
  * @brief 发送数据
  **/
void MainWindow::slotSend()
{
    QByteArray data;

    QString qstr = "FF C" + ui->cobox_CardNO->currentText()
            + " " + ui->txte_SendWindow->toPlainText();
    int len = StringToHex(qstr, data);

    serial_port_->sendData(data);

//    for (int i = 0; i < data.size(); ++i)
//        qDebug() << (unsigned char)data[i];
}

/**
  * @brief QString 转换为 Char
  **/
int MainWindow::StringToHex(QString qstr, QByteArray &data)
{
    unsigned char temp, temp_l;
    int return_len = 0;
    int len = qstr.length();
    data.resize(len / 2);

    unsigned char ch_l, ch_h;
    for (int i = 0; i < len; i++)
    {
        ch_h = qstr[i].toLatin1();
        if ((ch_h > 0) && (ch_h < 128) && (ch_h != 32))
        {
            if (i++ >= len)
                break;

            ch_l = qstr[i].toLatin1();
            temp = CharToHex(ch_h);
            temp_l = CharToHex(ch_l);
            temp = temp*16 + temp_l;
            data[return_len] = (char)temp;
            return_len++;
        }
        else if (ch_h == 32)
        {
        }
        else
        {
            data[return_len] = ch_h;
            return_len++;
        }
    }
    data.resize(return_len);

    return return_len;
}

/**
  * @brief 字符转为十六进制数
  **/
char MainWindow::CharToHex(char ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ch - 0x30;
    else if ((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    else if ((ch >= 'A') && (ch <= 'F'))
        return ch - 'A' + 10;
    else
        return ch;
}
