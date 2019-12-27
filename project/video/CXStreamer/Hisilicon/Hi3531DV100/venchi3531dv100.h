#ifndef VENCHI3531DV100_H
#define VENCHI3531DV100_H
#include <string.h>
#include "../../factory.h"
#include "../hielement.h"
#include "../cxs_hi_comm.h"
#include "hi3531dv100_lib.h"
#endif // VENCHI3531DV100_H
namespace CXS{
class VEncHi3531DV100 : public HiElement
{
//        HI_UNF_VI_ATTR_S mAttr;
//        HI_UNF_VI_E mViPort;
    std::string m_mode;
    int m_frame_rate;
    static std::vector<VEncHi3531DV100*> arr_VEncHi3531DV100;
    VEncHi3531DV100();

    void push(uint8_t* buffer,int len)
    {
        for(uint32_t i = 0;i < mOutPads.size();i++)
        {
            Pad* pad = mOutPads[i].linkedElemPad;
            pad->append(buffer,len);
        }
    }
    static void h264_processor(int vencChn,unsigned char* buffer,int len)
    {
        VEncHi3531DV100* venc = arr_VEncHi3531DV100[vencChn];
        venc->push(buffer,len);

    }
public:
    static Element* createInstance();
    static VEncHi3531DV100* findVencByHandle(int handle)
    {
        if(handle < (int)arr_VEncHi3531DV100.size() && arr_VEncHi3531DV100[handle] != nullptr)
            return arr_VEncHi3531DV100[handle];
        else
            return nullptr;
    }
    std::string getMode(){
        return m_mode;
    }
    int getFrameRate(){
        return m_frame_rate;
    }
    int startSelf(){

        int ret = hi3531dv100_venc_start ( mHandle);
        if(ret == 0)
            CXS_PRT("VENC(%d) start ok\n",mHandle);
        return ret;
    }
    int linkTo(Element* elem){
        elem = elem;
        return  0  ;
    }
    int getAttr(const char* attrName,int* attrVal)
    {
        if(strcmp(attrName,"VENC-FRAMERATE") == 0 )
        {
            *attrVal = m_frame_rate;
        }
        else if(strcmp(attrName,"VENC-CHN") == 0 )
        {
            *attrVal = mHandle;
        }
        return 0;
    }
    int setAttr(const char* attrName,int attrVal)
    {
        if(strcmp(attrName,"VENC-CHN") == 0 )
        {
            arr_VEncHi3531DV100[mHandle] = nullptr;
            mHandle = attrVal;
            arr_VEncHi3531DV100[mHandle] = this;
            return HI_SUCCESS;
        }
        else if(strcmp(attrName,"VENC-FRAMERATE") == 0 )
        {
            m_frame_rate = attrVal;
            return HI_SUCCESS;
        }
        else
            return -1;
    }
    int setAttr(const char* attrName,const char* attrVal)
    {
        if(strcmp(attrName,"VENC-MODE") == 0 )
        {
            m_mode = attrVal;
            return HI_SUCCESS;
        }
        else
            return -1;
    }

};
}
