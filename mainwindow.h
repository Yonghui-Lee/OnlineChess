#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSignalMapper>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QtNetwork>
#include <QTimer>
#include <QLCDNumber>
#include "info.h"
#include "piece.h"

#define Maxtime 20;

extern Info *info;
namespace Ui {
class ClientWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initializeNet();
    ~MainWindow();

protected slots:
    void initServer();
    void connectHost();
    void serverwaitingconnect();
    void clientwaitingconnect();
    void acceptConnection();
    void connected();
    void connectFail();
    void serverwaitingcancel();
    void addnum(int);
    void recvMessage();
    void writeMessage(QString m);


private slots:
    void action_dogfall();

    void action_lose();

    void on_action_read_triggered();

    void on_action_save_triggered();

    void action_short();

    void action_long();

    void subcount();

private:
    int count1,count2;
    QTimer *timer;
    QLCDNumber *lcd_1;
    QLCDNumber *lcd_2;
    QLineEdit *edit;
    QLineEdit *line_1;
    QDialog *newWaitingDialog;
    QDialog *connectingDialog;
    QDialog *waitingDialog;
    QDialog *initializeDialog;
    QDialog *sueDialog;
    QString address;
    Ui::ClientWindow *ui;
    QTcpServer *listenSocket;
    QTcpSocket *readWriteSocket;
};

#endif // MAINWINDOW_H
