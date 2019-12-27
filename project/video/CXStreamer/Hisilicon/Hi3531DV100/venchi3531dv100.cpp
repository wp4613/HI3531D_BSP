#include "venchi3531dv100.h"
extern "C"
{
extern void SetH264Processor(void (*processor)(int vencChn,unsigned char* buffer,int len));
}

namespace CXS{
    #define ELEMENT_CLASS_NAME  "venc"

    VEncHi3531DV100::VEncHi3531DV100():HiElement(ELEMENT_CLASS_NAME),m_mode("HD1080"){
        for(int i =0;i < 32;i++){
            if(arr_VEncHi3531DV100[i] == nullptr){
                mHandle = i;
                break;
            }
        }
        arr_VEncHi3531DV100[mHandle] = this;
        mInPads.push_back(Pad(this));
        m_frame_rate = 15;
    }
    Element* VEncHi3531DV100::createInstance()
    {
        static bool inited = false;
        if(!inited)
        {
            inited = true;
            SetH264Processor(h264_processor);
        }
        VEncHi3531DV100* elem = new VEncHi3531DV100();
        return elem;
    }
    std::vector<VEncHi3531DV100*> VEncHi3531DV100::arr_VEncHi3531DV100 = std::vector<VEncHi3531DV100*>(32);

    static int init()
    {
        return HI_SUCCESS;
    }
    static void deInit()
    {

    }
    static ElementDescriptor elementDescriptors[] = {
        {
             "hi3531dv100",
             ELEMENT_CLASS_NAME,
             init,
             deInit,
             VEncHi3531DV100::createInstance
        },
    };
    REGISTER_ELEMENT(elementDescriptors,sizeof(elementDescriptors)/sizeof(elementDescriptors[0]));
}
