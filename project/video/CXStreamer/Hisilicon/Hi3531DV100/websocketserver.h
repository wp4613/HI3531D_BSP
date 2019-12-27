#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <vector>
#include <string>
#include "websocketserverinf.h"
class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketServer(quint16 port, QObject *parent = Q_NULLPTR);
    virtual ~WebSocketServer();


    void sendData(unsigned char* data,int len)
    {
        emit sigSendData(QByteArray::fromRawData((char*)data,len));
    }
    class Client{

    public:
      explicit Client(QWebSocket *ws){

            run = false;
            this->ws = ws;
        }
        ~Client(){
            run = false;
            delete ws;
        }

      bool run;

      QWebSocket *ws;
    };

signals:
    void sigSendData(const QByteArray data);
public slots:
    void slotSendData(const QByteArray data);
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();
private:
    QWebSocketServer *m_pWebSocketServer;
    QList<Client *> m_clients;

};

#endif // WEBSOCKETSERVER_H
