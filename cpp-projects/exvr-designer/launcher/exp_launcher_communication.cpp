
/***********************************************************************************
** exvr-designer                                                                  **
** MIT License                                                                    **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
************************************************************************************/

#include "exp_launcher_communication.hpp"

// Qt
#include <QWidget>

// qt-utility
#include "qt_logger.hpp"

using namespace tool::ex;

void ExpLauncherCommunication::clean(){

    if(readSocket){
        readSocket->close();
        readSocket = nullptr;
    }
    if(writeSocket){
        writeSocket->close();
        writeSocket = nullptr;
    }
}

ExpLauncherCommunication::ExpLauncherCommunication(QObject *parent){

    Q_UNUSED(parent)

    // initialize reading socket
    readSocket  = std::make_unique<QUdpSocket>();
    connect(readSocket.get(), &QUdpSocket::readyRead, this, &ExpLauncherCommunication::read_pending_datagrams);
    connect(readSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),[=](QAbstractSocket::SocketError){
        emit error_signal(QSL("[IPC] Socket error\n"));
    });

    bool success = false;
    for(; readingPort < 8216; ++readingPort){
        success = readSocket->bind(QHostAddress::LocalHost, readingPort, QAbstractSocket::ReuseAddressHint);
        if(success){
            QtLogger::message(QSL("[IPC] UDP read port connected: ") % QString::number(readingPort));
            break;
        }
    }
    if(!success){
        QtLogger::error(QSL("[IPC] UDP port error: ") % QString::number(readingPort));
    }

    // initialize writing socket
    writeSocket  = std::make_unique<QUdpSocket>();
}

void ExpLauncherCommunication::read_pending_datagrams(){

    bool readDatagram = true;
    QHostAddress sender;
    quint16 senderPort;

    while (readDatagram){

        if(readSocket->state() == QAbstractSocket::BoundState){
            readDatagram = readSocket->hasPendingDatagrams();
        }else{
            readDatagram = false;
        }

        if(!readDatagram){
            return;
        }

        QByteArray datagram;
        datagram.resize(static_cast<int>(readSocket->pendingDatagramSize()));

        auto size = readSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if(size > 0){
            emit message_received_signal(QString(datagram));
        }else{
            emit error_signal(QSL("[IPC] Error while reading udp packet.\n"));
        }
    }
}

void ExpLauncherCommunication::send_udp_command(QStringView message){
    if(writingPort == 0){
        return;
    }
    writeSocket->writeDatagram(message.toUtf8(), QHostAddress::LocalHost, writingPort);
}

#include "moc_exp_launcher_communication.cpp"
