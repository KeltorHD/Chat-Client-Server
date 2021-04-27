#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    this->ui->stackedWidget->setCurrentIndex(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotReadyRead()
{
    QString msg = this->socket->readAll();
    this->ui->textBrowser->setPlainText(this->ui->textBrowser->toPlainText() + msg.remove(0, 1) + "\n");
}

void MainWindow::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                         "The host was not found." :
                         err == QAbstractSocket::RemoteHostClosedError ?
                         "The remote host is closed." :
                         err == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused." :
                         QString(socket->errorString())
                        );
    this->ui->error->setText(strError);
    this->ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::slotConnected()
{
    this->all_done = true;
    this->ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    if (!this->ui->lineEdit_2->text().length())
        return;

    int32_t length = int32_t(this->ui->lineEdit_2->text().toUtf8().length());
    length++;
    char buf[4];
    std::memcpy(buf, &length, 4);
    this->socket->write(buf, 4);

    this->socket->write(this->ui->lineEdit_2->text().toUtf8());

    this->ui->textBrowser->setPlainText(this->ui->textBrowser->toPlainText() + this->name + ": " + this->ui->lineEdit_2->text() + "\n");
    this->ui->lineEdit_2->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    if (!this->ui->host->text().size() || this->ui->port->value() == 0 || !this->ui->name->text().size())
        return;

    socket->connectToHost(this->ui->host->text(), this->ui->port->value());
    connect(socket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this,  SLOT(slotError(QAbstractSocket::SocketError)));


    int32_t length = int32_t(this->ui->name->text().toUtf8().length());
    length++;
    char buf[4];
    std::memcpy(buf, &length, 4);
    this->socket->write(buf, 4);

    this->socket->write(this->ui->name->text().toUtf8());

    this->ui->stackedWidget->setCurrentIndex(1);
    this->name = this->ui->name->text();
    this->setWindowTitle("Чат. Ваш логин: " + this->name);
    this->ui->stackedWidget->setCurrentIndex(0);
}
