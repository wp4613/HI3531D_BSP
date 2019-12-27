#include "playercxs.h"
#include <stdlib.h>
#define STREAM_CHN1_ON 1
#define STREAM_CHN2_ON 1
PlayerCxs::PlayerCxs()
{
    mElemHead1 = nullptr;
    mElemHead2 = nullptr;
}
extern "C"{
#define MAX_FRAME_RATE  15
#define MIN_FRAME_RATE  5
static void ws_cli_max_pending_changed_handler(void* user_data,void* event_para){
    int venc_handle = 0;
    CXS::Element* elem = static_cast<CXS::Element*>(user_data);
    elem->getAttr("VENC-CHN",&venc_handle);
    int max_pendings =  reinterpret_cast<int>(event_para);
    printf("max pending = %d,venc_handle = %d\n",max_pendings,venc_handle);

    int frame_rate = 0;
    elem->getAttr("VENC-FRAMERATE",&frame_rate);
    printf("frame_rate = %d\n",frame_rate);
    if(max_pendings > 8)
    {
        int t_rate = frame_rate*9/10;
        if(frame_rate == t_rate)
            frame_rate = t_rate-1;
        else
            frame_rate = t_rate;
    }
    else if(max_pendings < 2)
    {
        int t_rate = frame_rate *11 /10;
        if(frame_rate == t_rate)
            frame_rate = t_rate+1;
        else
            frame_rate = t_rate;
    }

    if(frame_rate > MAX_FRAME_RATE )
        frame_rate = MAX_FRAME_RATE;
    else if(frame_rate < MIN_FRAME_RATE )
        frame_rate = MIN_FRAME_RATE;
    printf("frame_rate2 = %d\n",frame_rate);
    elem->setAttr("VENC-FRAMERATE",frame_rate);
}
}
//arg == 1 for linkpi
PlayerCxs* PlayerCxs::getInstance(int chn1,int chn2,uint16_t port1,uint16_t port2)
{

    static PlayerCxs* instance = nullptr;

    if(!instance)
    {

        CXS::Factory* factory = CXS::Factory::getFactoryInstanceByName("hi3531dv100");
        CXS::Factory* factory_common = CXS::Factory::getFactoryInstanceByName("common");
//        factory->init();
//        factory_common->init();
        if(factory == NULL)
        {
            DEBUG("factory create failure\n");
            return nullptr;
        }
        instance = new PlayerCxs;

        if(chn1 >= 0){
            instance->mElemHead1 =  factory->createElementByName("vi");
            instance->mElemHead1->setAttr("VI-CHN",chn1*8);

            CXS::Element* elemVO =  factory->createElementByName("vo");
            elemVO->setAttr("VO-DEV",1);

            CXS::Element* elemVenc =  factory->createElementByName("venc");
            elemVenc->setAttr("VENC-CHN",0);
            elemVenc->setAttr("VENC-MODE","HD720");


            instance->mElemHead1->link(elemVO);
            instance->mElemHead1->link(elemVenc);
            CXS::Element* wsServer = factory_common->createElementByName("ws-server");

            wsServer->setAttr("port",port1);
            elemVenc->link(wsServer);

            wsServer->listenEvent("max_cli_pending_changed",ws_cli_max_pending_changed_handler,elemVenc);
        }


        if(chn2 >= 0){
            if(chn1 == chn2)
                instance->mElemHead2 = instance->mElemHead1;
            else{
               instance->mElemHead2 =  factory->createElementByName("vi");
               instance->mElemHead2->setAttr("VI-CHN",chn2*8);
            }


            CXS::Element* elemVenc2 =  factory->createElementByName("venc");
            elemVenc2->setAttr("VENC-CHN",1);
            elemVenc2->setAttr("VENC-MODE","HD720");
            instance->mElemHead2->link(elemVenc2);
            CXS::Element* wsServer2 = factory_common->createElementByName("ws-server");
            wsServer2->setAttr("port",port2);
            elemVenc2->link(wsServer2);

            wsServer2->listenEvent("max_cli_pending_changed",ws_cli_max_pending_changed_handler,elemVenc2);
        }
        char cmd[1024];
        sprintf(cmd,"sed -i 's/var vm1.*/var vm1 = new VideoModule(document.domain,%d,8,8,1280,720)/' ./mount-origin/static/video/test_zx.html",port1);
        system(cmd);
        sprintf(cmd,"sed -i 's/var vm2.*/var vm2 = new VideoModule(document.domain,%d,1,1,1280,720)/' ./mount-origin/static/video/test_zx.html",port2);
        system(cmd);
    }
    return instance;
}
int PlayerCxs::start()
{
    int res = 0;
    if(mElemHead1)
        res = mElemHead1->start();
    if(res == 0 && mElemHead2 && mElemHead1 != mElemHead2)
        res = mElemHead2->start();
    return res;
}
