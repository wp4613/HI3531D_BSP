#include <string.h>
#include "../factory.h"
#include "../element.h"

#include <thread>
#include <mutex>
#include <map>
#include <list>
#include <vector>
#include <stdint.h>
#include "httpserver.h"
#include "websocketserverimplqt.h"
namespace CXS{
    //WebsocketServer类作为Element体系下实现websocket转发的子类Element， WebsocketServerInf作为websocket通信功能的抽象，具体通信功能由WebsocketServerInf的子类实现;
    //与业务相关的逻辑应收敛于WebsocketServer，然而这里的实现却把业务相关代码(分析处理客户端未决h264包数)柔和进了WebsocketServerInf子类，致使这里代码颇为凌乱，应该改进。
    #define ELEMENT_CLASS_NAME  "ws-server"
    class WebsocketServer : public Element
    {
        static void OnAppend(uint8_t* data,int32_t len,Element* elem)
        {
            WebsocketServer* ws = (WebsocketServer*)elem;
            ws->pushData( data, len);

        }

            quint16 mPort;
        WebsocketServerInf* mWebsocketServer;
        void (*m_eventHandler)(void* user_data,void* event_para);
        void* m_user_data;
        WebsocketServer():Element(){
            Pad pad(this);
            pad.OnAppend = WebsocketServer::OnAppend;
            mInPads.push_back(pad);

            m_eventHandler = nullptr;
            m_user_data = nullptr;
            static HttpServer* httpServer = nullptr;
            if(httpServer == nullptr)
                httpServer = new HttpServer(8085);
            mPort = 8081;


        }
        ~WebsocketServer(){
            if(mWebsocketServer)
            {
                delete mWebsocketServer;
                mWebsocketServer = nullptr;
            }
        }


    public:
        void pushData(uint8_t* buff,int len)
        {
            mWebsocketServer->pushData(buff,len);

            return;
        }

        static Element* createInstance()
        {
            WebsocketServer* elem = new WebsocketServer();


            return elem;
        }


        int startSelf(){
            mWebsocketServer = new WebsocketServerQt(mPort);
            if(m_eventHandler != nullptr)
            {
                mWebsocketServer->setMaxPendingChangedCallback(m_eventHandler,m_user_data);
            }
            return 0;
        }
        int linkTo(Element* elem){
            elem = elem;
            return  -1  ;
        }

        int setAttr(const char* attrName,int val)
        {
            if(strcmp(attrName,"port") == 0 )
            {
                mPort = val;
                return 0;
            }
            else
                return -1;
        }
        int setAttr(const char* attrName,uint8_t* buff,int len)
        {
            if(strcmp(attrName,"send-data") == 0 )
            {

                pushData(buff,len);
                return 0;
            }
            else
                return -1;
        }
        int listenEvent(const char* eventName,void (*eventHandler)(void* user_data,void* event_para),void* user_data){

            if(strcmp(eventName,"max_cli_pending_changed") == 0 )
            {
                m_eventHandler = eventHandler;
                m_user_data = user_data;

                return 0;
            }
            else
                return -1;
        }

    };



    static int init()
    {
        return 0;
    }
    static void deInit()
    {

    }
    static ElementDescriptor elementDescriptors[] = {
        {
             "common",
             ELEMENT_CLASS_NAME,
             init,
             deInit,
             WebsocketServer::createInstance
        },
    };
    REGISTER_ELEMENT(elementDescriptors,sizeof(elementDescriptors)/sizeof(elementDescriptors[0]));
}
