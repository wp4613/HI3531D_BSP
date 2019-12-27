#include "websocketserver.h"
#include <QWebSocket>

WebSocketServer::WebSocketServer(quint16 port,QObject *parent) : QObject(parent),
    m_pWebSocketServer(Q_NULLPTR),
    m_clients()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Video Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "video Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &WebSocketServer::onNewConnection,Qt::QueuedConnection);
    }
    connect(this,SIGNAL(sigSendData(QByteArray)),this,SLOT(slotSendData(QByteArray)));
}
#include <QTimer>
#include <QDebug>
WebSocketServer::~WebSocketServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);
    Client* cli = new Client(pSocket);
    this->m_clients << cli ;
    cli->run = true;

}

#include <QFile>
void WebSocketServer::slotSendData(const QByteArray data)
{

    for (int i=0;i<m_clients.count();i++) {
        if(m_clients[i]->run)
        {
            if(m_clients[i]->ws->isValid())
                m_clients[i]->ws->sendBinaryMessage(data);
            else
            {
                qDebug("ws invalid.");

            }
//            QFile f("/home/zxr/frame111.jpg");
//            f.open(QIODevice::ReadOnly);

//            QByteArray d = f.readAll();
//            int len = d.length();
//            QByteArray dd((char*)&len,4);
//            dd.append(d);
//            m_clients[i]->ws->sendBinaryMessage(dd);
//            f.close();
        }

    }
}

//! [processMessage]
void WebSocketServer::processMessage(QString message)
{
    QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
    Q_FOREACH (Client *pClient, m_clients)
    {
        if (pClient->ws != pSender) //don't echo message back to sender
        {
            pClient->ws->sendTextMessage(message);
        }
    }
}
//! [processMessage]

//! [socketDisconnected]
void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    Client *cli = NULL;
    for(int i = 0;i < m_clients.count();i++)
    {
        if(m_clients[i]->ws == pClient)
        {
            cli = m_clients[i];
        }
    }

    if (cli)
    {
        m_clients.removeAll(cli);
        delete cli;
    }
}
//! [socketDisconnected]
