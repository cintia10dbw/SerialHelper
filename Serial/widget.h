#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QSerialPort* serialPort;

private slots:
    void on_OpenBtn_clicked();

    void on_CloseBtn_clicked();

    void SerialPortReadyRead_Slot();

    void on_TransferBtn_clicked();

    void on_ClearBtn_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
