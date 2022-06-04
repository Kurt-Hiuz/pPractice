#include "server.h"

Server::Server(){
    if(this->listen(QHostAddress::Any, 2323)){
        ui->label->setText("Server start");
    } else {
        ui->label->setText("Something happened :(");
    }
}

void Server::incomingConnection(qintptr socketDescriptor){
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    ui->statusbar->showMessage("new client");
}

void Server::slotReadyRead(){
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok){
        ui->statusbar->showMessage("reading...");
        QString str;
        in >> str;
    } else {
        ui->statusbar->showMessage("Something happened :(");
    }
}

void Server::SendToClient(QString str){
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    socket->write(Data);
}
