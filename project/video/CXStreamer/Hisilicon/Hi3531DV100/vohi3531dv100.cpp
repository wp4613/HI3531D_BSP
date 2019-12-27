#include <string.h>
#include "../../factory.h"
#include "../hielement.h"
#include "../cxs_hi_comm.h"
#include "hi3531dv100_lib.h"

namespace CXS{
    #define ELEMENT_CLASS_NAME  "vo"
    class VOHi3531DV100 : public HiElement
    {
//        HI_UNF_VI_ATTR_S mAttr;
//        HI_UNF_VI_E mViPort;
        HI_S32 mWidth;
        HI_S32 mHeight;
        VOHi3531DV100():HiElement(ELEMENT_CLASS_NAME){
            mWidth = 1920;
            mHeight = 1080;
//            mViPort = HI_UNF_VI_PORT0;
//            mAttr.bVirtual = HI_TRUE;
//            mAttr.u32BufNum = 6;
            mInPads.push_back(Pad(this));
        }
    public:
        static Element* createInstance()
        {
            VOHi3531DV100* elem = new VOHi3531DV100();
            return elem;
        }
        int startSelf(){

            int ret = hi3531dv100_vo_start ( mHandle,mWidth,mHeight);
            if(ret == 0)
                CXS_PRT("VO(%d) start ok\n",mHandle);
            return ret;
        }
        int linkTo(Element* elem){
            elem = elem;

            return  HI_FAILURE  ;
        }
        int setAttr(const char* attrName,int attrVal)
        {
            if(strcmp(attrName,"VO-DEV") == 0 )
            {
                mHandle = attrVal;
                //HI_S32 ret = HI_UNF_VI_SetAttr ( mHandle,&mAttr);
                return HI_SUCCESS;
            }
            else
                return -1;
        }
    };
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
             VOHi3531DV100::createInstance
        },
    };
    REGISTER_ELEMENT(elementDescriptors,sizeof(elementDescriptors)/sizeof(elementDescriptors[0]));
}
