#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QtSerialPort/qserialportinfo.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList serialnameport;

    serialPort = new QSerialPort(this);

    connect(serialPort,SIGNAL(readyRead()),this,SLOT(SerialPortReadyRead_Slot()));

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        serialnameport<<info.portName();
    }
    ui->SerialCb->addItems(serialnameport);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_OpenBtn_clicked()
{
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits databits;
    QSerialPort::StopBits stopbits;
    QSerialPort::Parity comparebits;

    if(ui->BaundrateCb->currentText() == "4800"){
        baudRate=QSerialPort::Baud4800;
    }else if(ui->BaundrateCb->currentText() == "9600"){
        baudRate=QSerialPort::Baud9600;
    }else if(ui->BaundrateCb->currentText() == "115200"){
        baudRate=QSerialPort::Baud115200;
    }

    if(ui->DataCb->currentText() == "8"){
        databits=QSerialPort::Data8;
    }else if(ui->DataCb->currentText() == "7"){
        databits=QSerialPort::Data7;
    }else if(ui->DataCb->currentText() == "6"){
        databits=QSerialPort::Data6;
    }else if(ui->DataCb->currentText() == "5"){
        databits=QSerialPort::Data5;
    }

    if(ui->StopCb->currentText() == "2"){
        stopbits=QSerialPort::TwoStop;
    }else if(ui->StopCb->currentText() == "1.5"){
        stopbits=QSerialPort::OneAndHalfStop;
    }else if(ui->StopCb->currentText() == "1"){
        stopbits=QSerialPort::OneStop;
    }

    if(ui->CompareCb->currentText() == "None"){
        comparebits=QSerialPort::NoParity;
    }

    serialPort->setPortName(ui->SerialCb->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(databits);
    serialPort->setStopBits(stopbits);
    serialPort->setParity(comparebits);

    if(serialPort->open(QIODevice::ReadWrite) == true){
        QMessageBox::information(this,"提示","打开成功");
    }else{
        QMessageBox::critical(this,"提示","打开失败");
    }
}


void Widget::on_CloseBtn_clicked()
{
    serialPort->close();
    QMessageBox::information(this,"提示","已关闭串口");
}

void Widget::SerialPortReadyRead_Slot()
{
    auto buf = (serialPort->readAll());
    if(ui->RFormatCb->currentText() == "HEX")
    {
        QString hex = buf.toHex(' ');
        ui->RecieveEdit->appendPlainText(hex);
    }else {
        QString text = QString(buf) ;
        ui->RecieveEdit->appendPlainText(text);
    }
}

void Widget::on_TransferBtn_clicked()
{
    QString data = ui->TransferEdit->toPlainText();
    if(ui->TFormatCb->currentText()=="HEX")
    {
        QByteArray arr;
        for(int i=0;i<data.size();i++)
        {
            if(data[i]==' ')continue;
            int num = data.mid(i,2).toUInt(nullptr,16);
            ++i;
            arr.append(num);
        }
        serialPort->write(arr);
    }else{
        serialPort->write(data.toLocal8Bit().data());
    }
}

void Widget::on_ClearBtn_clicked()
{
    ui->RecieveEdit->clear();
}

