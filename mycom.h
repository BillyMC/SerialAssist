#ifndef MYCOM_H
#define MYCOM_H

#include <QObject>
#include <QVector>
#include "qextserialport.h"

class MyCom : public QObject
{
    Q_OBJECT
public:
    explicit MyCom(QObject *parent = 0);

    QextSerialPort *port;

    bool openPort(QString name, BaudRateType baudrate, DataBitsType databits, ParityType parity,
                   StopBitsType stopbits, FlowType flow);
    bool closePort();
    void sendData(QByteArray send_data);

private:
    quint32 getCrc32Code(quint8* p, int len);
    bool checkCrc32(quint8* p, int len);

    quint8 receive_bits_;
    QVector<quint8> data_receive_;

private slots:
    void onReadyRead();

signals:
    void sigDisplayReceive(const QVector<quint8> receive_data, bool is_check_ok) const;
};

#endif // MYCOM_H
