#include "mainwindow.h"
#include "ui_clientwindow.h"
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    timer = new QTimer;
    count1=Maxtime;
    count2=Maxtime;
     connect(timer,SIGNAL(timeout()),this,SLOT(subcount()));

    /*
    lcd_1= new QLCDNumber;
    lcd_2= new QLCDNumber;
    lcd_1->display(count1);
    lcd_2->display(count2);

    QPushButton *Button_1 = new QPushButton("求和");
    QPushButton *Button_2 = new QPushButton("认输");
    QPushButton *Button_3 = new QPushButton("短易位");
    QPushButton *Button_4 = new QPushButton("长易位");
    Button_1->setFixedHeight(50);
    Button_2->setFixedHeight(50);
    Button_3->setFixedHeight(50);
    Button_4->setFixedHeight(50);
    connect(Button_1,SIGNAL(clicked()),this,SLOT(action_dogfall()));
    connect(Button_2,SIGNAL(clicked()),this,SLOT(action_lose()));
    connect(Button_3,SIGNAL(clicked()),this,SLOT(action_short()));
    connect(Button_4,SIGNAL(clicked()),this,SLOT(action_long()));

    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *hlayout = new QHBoxLayout;
    vlayout->addWidget(lcd_1);
    vlayout->addWidget(Button_1);
    vlayout->addWidget(Button_2);
    vlayout->addWidget(Button_3);
    vlayout->addWidget(Button_4);
    vlayout->addWidget(lcd_2);
    hlayout->addWidget(game);
    hlayout->addLayout(vlayout);


    ui->centralWidget->setLayout(hlayout);

*/
    //setCentralWidget(game);
    connect(info,SIGNAL(needtowrite(QString)),this,SLOT(writeMessage(QString)));
}

void MainWindow::initializeNet()
{
    initializeDialog = new QDialog;
    QLabel *label_0 = new QLabel;
    QPushButton *button_1 = new QPushButton;
    QPushButton *button_2 = new QPushButton;
    QVBoxLayout *layout = new QVBoxLayout;

    label_0->setText(tr("Start your game"));
    button_1->setText(tr("Server"));
    button_2->setText(tr("Client"));
    layout->addWidget(label_0);
    layout->addWidget(button_1);
    layout->addWidget(button_2);

    initializeDialog->setLayout(layout);
    connect(button_1,SIGNAL(clicked(bool)),this,SLOT(initServer()));
    connect(button_2,SIGNAL(clicked(bool)),this,SLOT(connectHost()));
    initializeDialog->show();
    initializeDialog->exec();
}

void MainWindow::initServer()
{
    info->isserver=true;
    initializeDialog->close();
    waitingDialog = new QDialog;
    QLabel *label_1 = new QLabel;

    QPushButton *button1 = new QPushButton;
    QPushButton *button2 = new QPushButton;
    edit = new QLineEdit;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QHBoxLayout *layout_2 = new QHBoxLayout;


    label_1->setText("Host IP:");
    edit->setText("127.0.0.1");
    button1->setText(tr("OK"));
    button2->setText(tr("Cancel"));
    layout_1->addWidget(label_1);
    layout_1->addWidget(edit);
    layout_2->addWidget(button1);
    layout_2->addWidget(button2);
    layout->addLayout(layout_1);
    layout->addLayout(layout_2);
    waitingDialog->setLayout(layout);
    connect(button1,SIGNAL(clicked()),this,SLOT(serverwaitingconnect()));
    connect(button2,SIGNAL(clicked()),waitingDialog,SLOT(close()));
     this->listenSocket =new QTcpServer;
    waitingDialog->show();
    waitingDialog->exec();


}


void MainWindow::serverwaitingconnect()
{
    waitingDialog->close();
    newWaitingDialog = new QDialog;
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    QPushButton *button = new QPushButton;
    button->setText(tr("Cancel"));
    QString sentence_1;
    sentence_1.append("Your IP:");
    sentence_1.append("127.0.0.1");
    label_2->setText(sentence_1);
    label_1->setText(tr("Waiting..."));
    QVBoxLayout *layout_1 = new QVBoxLayout;
    layout_1->addWidget(label_2);
    layout_1->addWidget(label_1);
    layout_1->addWidget(button);
    newWaitingDialog->setLayout(layout_1);
    this->listenSocket->listen(QHostAddress::Any,55555);
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    connect(button,SIGNAL(clicked()),this,SLOT(serverwaitingcancel()));


    newWaitingDialog->show();
    newWaitingDialog->exec();

}

void MainWindow::serverwaitingcancel()
{
    QObject::disconnect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    delete this->listenSocket;
     newWaitingDialog->close();
        initServer();

}

void MainWindow::connectHost()
{
    info->isserver=false;
    initializeDialog->close();
    connectingDialog = new QDialog;
    QLabel *label_1 = new QLabel;
    line_1 = new QLineEdit;
    QPushButton *button = new QPushButton;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    QGridLayout *layout_2 = new QGridLayout;
    label_1->setText(tr("Server's IP:"));
    button->setText(tr("OK"));
    layout_1->addWidget(label_1);
    layout_1->addWidget(line_1);
    layout->addLayout(layout_1);


    QSignalMapper*m=new QSignalMapper(this);
    QPushButton **number=new QPushButton*[10];
    for(int i=0;i<10;++i)
    {
        number[i]=new QPushButton(tr("%1").arg(i));
        connect(number[i],SIGNAL(clicked(bool)),m,SLOT(map()));
        m->setMapping(number[i],i);
    }
    QPushButton *dot = new QPushButton(".");
    QPushButton *del = new QPushButton("Del");
    connect(dot,SIGNAL(clicked(bool)),m,SLOT(map()));
    m->setMapping(dot,10);
    connect(del,SIGNAL(clicked(bool)),m,SLOT(map()));
    m->setMapping(del,11);

    connect(m,SIGNAL(mapped(int)),this,SLOT(addnum(int)));
    for(int i=0;i<9;++i)
        layout_2->addWidget(number[i+1],i/3,i%3,1,1);
    layout_2->addWidget(number[0],3,1,1,1);
    layout_2->addWidget(dot,3,0,1,1);
    layout_2->addWidget(del,3,2,1,1);

    layout->addLayout(layout_2);
    layout->addWidget(button);
    connectingDialog->setLayout(layout);
    this->readWriteSocket = new QTcpSocket;
    connect(button,SIGNAL(clicked(bool)),this,SLOT(clientwaitingconnect()));
    connectingDialog->show();
    connectingDialog->exec();
}

void MainWindow::addnum(int m)
{
  if(m>=0&&m<=9)
    this->line_1->setText(this->line_1->text()+QString::number(m));
  if(m==10)
    this->line_1->setText(this->line_1->text()+".");
  if(m==11)
  {
      QString tem = this->line_1->text();
      tem.chop(1);
      this->line_1->setText(tem);
  }
}


void MainWindow::writeMessage(QString m)
{   qDebug()<<"sent:"<<m;
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(m);
    readWriteSocket->write(array->data());
    timer->stop();
    if(info->isserver)
    {
        count2=Maxtime;
        lcd_2->display(count2);
    }
    else
    {
        count1=Maxtime;
        lcd_1->display(count1);
    }
}

void MainWindow::recvMessage()
{
    //info[0]: 0:white  1:black
    // m1234i:move 12to34 becomes i ,1:bishop,2:king,3:knight,4:pawn,5:queen,6：rook
//t12i: 12变成了i   f:enemy give up or fall;   w:enemy win
 //  h:ask for draw;  y:accept draw;   n:refuse draw;

    QString infomation;
    infomation += this->readWriteSocket->readAll();
    qDebug()<<"receive:"<<infomation;
    QString sidename;
    sidename.clear();
    if(infomation[0].digitValue())
        sidename+="BLACK";
    else sidename+="WHITE";

            Bishop *piece1=new Bishop(sidename);
            King *piece2 = new King(sidename);
           Knight *piece3 = new Knight(sidename);
          Pawn *piece4 = new Pawn(sidename);
           Queen *piece5 = new Queen(sidename);
           Rook *piece6 = new Rook(sidename);
    if(infomation=="wl")
    {timer->start(1000);
        King *king=dynamic_cast<King *>(info->grids[7][4]->nowPiece);
        Rook *rook=dynamic_cast<Rook *>(info->grids[7][0]->nowPiece);

        info->grids[7][4]->setPiece(false);
        info->grids[7][4]->setColor("NONE");
        info->grids[7][4]->nowPiece = NULL;
        info->grids[7][0]->setPiece(false);
        info->grids[7][0]->setColor("NONE");
        info->grids[7][0]->nowPiece = NULL;
        info->grids[7][2]->placePiece(king);
        info->grids[7][3]->placePiece(rook);
        info->changeTurn();
        info->Display->setPlainText("Turn : WHITE");





    }
    if(infomation=="ws")
    {timer->start(1000);
        King *king=dynamic_cast<King *>(info->grids[7][4]->nowPiece);
        Rook *rook=dynamic_cast<Rook *>(info->grids[7][7]->nowPiece);

        info->grids[7][4]->setPiece(false);
        info->grids[7][4]->setColor("NONE");
        info->grids[7][4]->nowPiece = NULL;
        info->grids[7][7]->setPiece(false);
        info->grids[7][7]->setColor("NONE");
        info->grids[7][7]->nowPiece = NULL;
        info->grids[7][6]->placePiece(king);
        info->grids[7][5]->placePiece(rook);

        info->changeTurn();
        info->Display->setPlainText("Turn : WHITE");
    }
    if(infomation=="bl")
    {timer->start(1000);
        King *king=dynamic_cast<King *>(info->grids[0][4]->nowPiece);
        Rook *rook=dynamic_cast<Rook *>(info->grids[0][0]->nowPiece);

        info->grids[0][4]->setPiece(false);
        info->grids[0][4]->setColor("NONE");
        info->grids[0][4]->nowPiece = NULL;
        info->grids[0][0]->setPiece(false);
        info->grids[0][0]->setColor("NONE");
        info->grids[0][0]->nowPiece = NULL;
        info->grids[0][2]->placePiece(king);
        info->grids[0][3]->placePiece(rook);

        info->changeTurn();
        info->Display->setPlainText("Turn : BLACK");
    }
    if(infomation=="bs")
    {timer->start(1000);
        King *king=dynamic_cast<King *>(info->grids[0][4]->nowPiece);
        Rook *rook=dynamic_cast<Rook *>(info->grids[0][7]->nowPiece);

        info->grids[0][4]->setPiece(false);
        info->grids[0][4]->setColor("NONE");
        info->grids[0][4]->nowPiece = NULL;
        info->grids[0][7]->setPiece(false);
        info->grids[0][7]->setColor("NONE");
        info->grids[0][7]->nowPiece = NULL;
        info->grids[0][6]->placePiece(king);
        info->grids[0][5]->placePiece(rook);

        info->changeTurn();
        info->Display->setPlainText("Turn : BLACK");
    }
    if(infomation[1]=='m')
    {timer->start(1000);
        Grid *box1 = info->grids[infomation[2].digitValue()][infomation[3].digitValue()];
        info->alivePiece.removeAll(box1->nowPiece);
        info->removeFromScene(box1->nowPiece);
        box1->nowPiece->hide();
        box1->setPiece(false);
        box1->setColor("NONE");
        box1->nowPiece = NULL;
        Grid *box2 = info->grids[infomation[4].digitValue()][infomation[5].digitValue()];
        if(box2->ifhasPiece())
        {info->alivePiece.removeAll(box2->nowPiece);
            info->removeFromScene(box2->nowPiece);
            box2->nowPiece->hide();
        }
        box2->setPiece(false);
        box2->setColor("NONE");
        box2->nowPiece = NULL;
        switch (infomation[6].digitValue()) {
        case 1:

            box2->placePiece(piece1);
            piece1->setIsPlaced(true);
            piece1->firstMove = true;
             info->addToScene(piece1);
            info->alivePiece.append(piece1);
            break;
        case 2:

            box2->placePiece(piece2);
            piece2->setIsPlaced(true);
            piece2->firstMove = true;
             info->addToScene(piece2);
            info->alivePiece.append(piece2);
            break;
        case 3:

            box2->placePiece(piece3);
            piece3->setIsPlaced(true);
            piece3->firstMove = true;
             info->addToScene(piece3);
            info->alivePiece.append(piece3);
            break;
        case 4:
            box2->placePiece(piece4);
            piece4->setIsPlaced(true);
            piece4->firstMove = true;
             info->addToScene(piece4);
            info->alivePiece.append(piece4);
            break;
        case 5:

            box2->placePiece(piece5);
            piece5->setIsPlaced(true);
            piece5->firstMove = true;
             info->addToScene(piece5);
            info->alivePiece.append(piece5);
            break;
        case 6:

            box2->placePiece(piece6);
            piece6->setIsPlaced(true);
            piece6->firstMove = true;
             info->addToScene(piece6);
            info->alivePiece.append(piece6);
            break;
        default:
            break;
        }
            info->changeTurn();

    }
    if(infomation[1]=='t')
    {timer->start(1000);


        Grid *box1 = info->grids[infomation[2].digitValue()][infomation[3].digitValue()];
        info->alivePiece.removeAll(box1->nowPiece);
        info->removeFromScene(box1->nowPiece);
        box1->nowPiece->hide();
        box1->setPiece(false);
        box1->setColor("NONE");
        box1->nowPiece = NULL;
        int ii=infomation[4].digitValue();
        if(ii==1)
        {
            Bishop *piece=new Bishop(sidename);
            box1->placePiece(piece);
            piece->setIsPlaced(true);
            piece->firstMove = true;
             info->addToScene(piece);
            info->alivePiece.append(piece);
            return ;
        }
        if(ii==3)
        {
            Knight *piece = new Knight(sidename);
            box1->placePiece(piece);
            piece->setIsPlaced(true);
            piece->firstMove = true;
             info->addToScene(piece);
            info->alivePiece.append(piece);
             return ;
        }
        if(ii==5)
        {
            Queen *piece = new Queen(sidename);
            box1->placePiece(piece);
            piece->setIsPlaced(true);
            piece->firstMove = true;
             info->addToScene(piece);
            info->alivePiece.append(piece);
             return ;
        }
        if(ii==6)
        {
            Rook *piece = new Rook(sidename);
            box1->placePiece(piece);
            piece->setIsPlaced(true);
            piece->firstMove = true;
             info->addToScene(piece);
            info->alivePiece.append(piece);
             return ;
        }
    }
    if(infomation[0]=='f')
    {
        QMessageBox::about(NULL,tr("Info"),tr("YOU WIN"));
        info->gameOver();
    }
    if(infomation=="w")
    {
        QMessageBox::about(NULL,tr("Info"),tr("YOU LOSE"));
        info->gameOver();
    }
    if(infomation=="h")
    {
        QMessageBox whetherAccept(QMessageBox::NoIcon,tr("求和"),tr("你接受对方的求和吗？"),QMessageBox::Yes | QMessageBox::No,NULL);
        if(whetherAccept.exec() == QMessageBox::Yes)
        {
            QString ss;
            ss.clear();
            ss.append("y");
            QByteArray *array =new QByteArray;
            array->clear();
            array->append(ss);
            readWriteSocket->write(array->data());
            info->gameOver();
            QMessageBox::about(NULL,tr("Info"),tr("DRAW"));




        }
        else{
            QString ss;
            ss.clear();
            ss.append("n");
            QByteArray *array =new QByteArray;
            array->clear();
            array->append(ss);
            readWriteSocket->write(array->data());

        }
    }
    if(infomation=="y")
    {
        QMessageBox::about(NULL,tr("Info"),tr("DRAW"));
        info->gameOver();
        if(info->isserver)
        {
            count2=Maxtime;
            lcd_2->display(count2);
            timer->stop();
            if(info->getTurn()=="WHITE")
                timer->start(1000);
        }
        else
        {
            count1=Maxtime;
            lcd_1->display(count1);
            timer->stop();
            if(info->getTurn()=="BLACK")
                timer->start(1000);
        }
    }
    if(infomation=="n")
    {
         QMessageBox::about(NULL,tr("Info"),tr("Refused"));
         if(info->isserver)
         {
             count2=Maxtime;
             lcd_2->display(count2);
             timer->stop();
             if(info->getTurn()=="WHITE")
                 timer->start(1000);
         }
         else
         {
             count1=Maxtime;
             lcd_1->display(count1);
             timer->stop();
             if(info->getTurn()=="BLACK")
                 timer->start(1000);
         }
    }



}

void MainWindow::clientwaitingconnect()
{

    QString address = line_1->text();
    this->readWriteSocket->connectToHost(QHostAddress(address),55555);
    connect(readWriteSocket,SIGNAL(connected()),this,SLOT(connected()));
    if(!readWriteSocket->waitForConnected())
         connectFail();
}

void MainWindow::acceptConnection()
{
    newWaitingDialog->close();
    this->readWriteSocket = this->listenSocket->nextPendingConnection();
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));



    lcd_2= new QLCDNumber;
    lcd_2->display(count2);
    QPushButton *Button_1 = new QPushButton("求和");
    QPushButton *Button_2 = new QPushButton("认输");
    QPushButton *Button_3 = new QPushButton("短易位");
    QPushButton *Button_4 = new QPushButton("长易位");
    Button_1->setFixedHeight(50);
    Button_2->setFixedHeight(50);
    Button_3->setFixedHeight(50);
    Button_4->setFixedHeight(50);
    connect(Button_1,SIGNAL(clicked()),this,SLOT(action_dogfall()));
    connect(Button_2,SIGNAL(clicked()),this,SLOT(action_lose()));
    connect(Button_3,SIGNAL(clicked()),this,SLOT(action_short()));
    connect(Button_4,SIGNAL(clicked()),this,SLOT(action_long()));
    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *hlayout = new QHBoxLayout;
    vlayout->addWidget(Button_1);
    vlayout->addWidget(Button_2);
    vlayout->addWidget(Button_3);
    vlayout->addWidget(Button_4);
    vlayout->addWidget(lcd_2);
    hlayout->addWidget(info);
    hlayout->addLayout(vlayout);
    ui->centralWidget->setLayout(hlayout);


    info->start();
    this->setWindowTitle("WHITE");
    this->show();
    timer->start(1000);


}

void MainWindow::connected()
{
     connectingDialog->close();
     QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));

     lcd_1= new QLCDNumber;
     lcd_1->display(count1);
     QPushButton *Button_1 = new QPushButton("求和");
     QPushButton *Button_2 = new QPushButton("认输");
     QPushButton *Button_3 = new QPushButton("短易位");
     QPushButton *Button_4 = new QPushButton("长易位");
     Button_1->setFixedHeight(50);
     Button_2->setFixedHeight(50);
     Button_3->setFixedHeight(50);
     Button_4->setFixedHeight(50);
     connect(Button_1,SIGNAL(clicked()),this,SLOT(action_dogfall()));
     connect(Button_2,SIGNAL(clicked()),this,SLOT(action_lose()));
     connect(Button_3,SIGNAL(clicked()),this,SLOT(action_short()));
     connect(Button_4,SIGNAL(clicked()),this,SLOT(action_long()));
     QVBoxLayout *vlayout = new QVBoxLayout;
     QHBoxLayout *hlayout = new QHBoxLayout;
     vlayout->addWidget(lcd_1);
     vlayout->addWidget(Button_1);
     vlayout->addWidget(Button_2);
     vlayout->addWidget(Button_3);
     vlayout->addWidget(Button_4);
     hlayout->addWidget(info);
     hlayout->addLayout(vlayout);
     ui->centralWidget->setLayout(hlayout);











     info->start();
     this->setWindowTitle("BLACK");
    this->show();
     timer->start(1000);
}

void MainWindow::connectFail()
{
    QMessageBox::about(NULL,tr("Wrong"),tr("请填入正确的IP地址！"));
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::action_dogfall()
{
    QString ss;
    ss.clear();
    ss.append('h');
    info->sentstring(ss);
}

void MainWindow::action_lose()
{
    QString ss;
    ss.clear();
    ss.append('f');
    info->sentstring(ss);
    info->gameOver();
    QMessageBox message(QMessageBox::Warning,"Info","YOU LOSE",QMessageBox::Ok,NULL);

                        if (message.exec()==QMessageBox::Ok)
                            {
                              qDebug()<<"clicked Ok\n";
                            }
                        else
                            {
                              qDebug()<<"clicked no\n";
                            }


}

void MainWindow::on_action_read_triggered()
{
    info->readfile();
    if(info->isserver)
    {
        count2=Maxtime;
        lcd_2->display(count2);
        timer->stop();
        if(info->getTurn()=="WHITE")
            timer->start(1000);
    }
    else
    {
        count1=Maxtime;
        lcd_1->display(count1);
        timer->stop();
        if(info->getTurn()=="BLACK")
            timer->start(1000);
    }





}

void MainWindow::on_action_save_triggered()
{
    info->savefile();
}

void MainWindow::action_long()
{
    info->checkCastling();
    if(info->getTurn()=="WHITE"&&info->whitelongcastling&&info->isserver)
        {
        King *king=dynamic_cast<King *>(info->grids[7][4]->nowPiece);
        Rook *rook=dynamic_cast<Rook *>(info->grids[7][0]->nowPiece);

        info->grids[7][4]->setPiece(false);
        info->grids[7][4]->setColor("NONE");
        info->grids[7][4]->nowPiece = NULL;
        info->grids[7][0]->setPiece(false);
        info->grids[7][0]->setColor("NONE");
        info->grids[7][0]->nowPiece = NULL;
        info->grids[7][2]->placePiece(king);
        info->grids[7][3]->placePiece(rook);
        info->changeTurn();
        info->Display->setPlainText("Turn : WHITE");


        info->sentstring("wl");



        return;
        }

    if(info->getTurn()=="BLACK"&&info->blacklongcastling&&!info->isserver)
        {
        King *king=dynamic_cast<King *>(info->grids[0][4]->nowPiece);
        Rook *rook=dynamic_cast<Rook *>(info->grids[0][0]->nowPiece);

        info->grids[0][4]->setPiece(false);
        info->grids[0][4]->setColor("NONE");
        info->grids[0][4]->nowPiece = NULL;
        info->grids[0][0]->setPiece(false);
        info->grids[0][0]->setColor("NONE");
        info->grids[0][0]->nowPiece = NULL;
        info->grids[0][2]->placePiece(king);
        info->grids[0][3]->placePiece(rook);

        info->changeTurn();
        info->Display->setPlainText("Turn : BLACK");

        info->sentstring("bl");

        return;
        }

    QMessageBox message(QMessageBox::Warning,"WARNING","YOU CAN'T DO IT",QMessageBox::Ok,NULL);

                        if (message.exec()==QMessageBox::Ok)
                            {
                              qDebug()<<"clicked Ok\n";
                            }
                        else
                            {
                              qDebug()<<"clicked no\n";
                            }

}




void MainWindow::action_short()
{
    info->checkCastling();
    if(info->getTurn()=="WHITE"&&info->whiteshortcastling&&info->isserver)
        {
           King *king=dynamic_cast<King *>(info->grids[7][4]->nowPiece);
           Rook *rook=dynamic_cast<Rook *>(info->grids[7][7]->nowPiece);

           info->grids[7][4]->setPiece(false);
           info->grids[7][4]->setColor("NONE");
           info->grids[7][4]->nowPiece = NULL;
           info->grids[7][7]->setPiece(false);
           info->grids[7][7]->setColor("NONE");
           info->grids[7][7]->nowPiece = NULL;
           info->grids[7][6]->placePiece(king);
           info->grids[7][5]->placePiece(rook);

           info->changeTurn();
           info->Display->setPlainText("Turn : WHITE");

           info->sentstring("ws");

        return;
        }

    if(info->getTurn()=="BLACK"&&info->blackshortcastling&&!info->isserver)
        {
        King *king=dynamic_cast<King *>(info->grids[0][4]->nowPiece);
        Rook *rook=dynamic_cast<Rook *>(info->grids[0][7]->nowPiece);

        info->grids[0][4]->setPiece(false);
        info->grids[0][4]->setColor("NONE");
        info->grids[0][4]->nowPiece = NULL;
        info->grids[0][7]->setPiece(false);
        info->grids[0][7]->setColor("NONE");
        info->grids[0][7]->nowPiece = NULL;
        info->grids[0][6]->placePiece(king);
        info->grids[0][5]->placePiece(rook);

        info->changeTurn();
        info->Display->setPlainText("Turn : BLACK");

        info->sentstring("bs");

        return;
        }

    QMessageBox message(QMessageBox::Warning,"WARNING","YOU CAN'T DO IT",QMessageBox::Ok,NULL);

                        if (message.exec()==QMessageBox::Ok)
                            {
                              qDebug()<<"clicked Ok\n";
                            }
                        else
                            {
                              qDebug()<<"clicked no\n";
                            }
}


void MainWindow::subcount()
{

    if(info->isserver&&info->getTurn()=="WHITE")
    {
        count2--;
        lcd_2->display(count2);
        if(count2==0)
        {
            QString ss;
            ss.clear();
            ss.append('f');
            info->sentstring(ss);
            info->gameOver();
            QMessageBox message(QMessageBox::Warning,"Info","OUT OF TIME\nYOU LOSE",QMessageBox::Ok,NULL);

                                if (message.exec()==QMessageBox::Ok)
                                    {
                                      qDebug()<<"clicked Ok\n";
                                    }
                                else
                                    {
                                      qDebug()<<"clicked no\n";
                                    }
        }

    }
    if(!info->isserver&&info->getTurn()=="BLACK")
    {
        count1--;
        lcd_1->display(count1);
        if(count1==0)
        {
            QString ss;
            ss.clear();
            ss.append('f');
            info->sentstring(ss);
            info->gameOver();
            QMessageBox message(QMessageBox::Warning,"Info","OUT OF TIME\nYOU LOSE",QMessageBox::Ok,NULL);

                                if (message.exec()==QMessageBox::Ok)
                                    {
                                      qDebug()<<"clicked Ok\n";
                                    }
                                else
                                    {
                                      qDebug()<<"clicked no\n";
                                    }
        }

    }

}
