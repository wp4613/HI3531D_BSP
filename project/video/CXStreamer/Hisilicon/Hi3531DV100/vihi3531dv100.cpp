#include <string.h>
#include "../../factory.h"
#include "../hielement.h"
#include "../cxs_hi_comm.h"
#include "hi3531dv100_lib.h"
#include "common/sample_comm.h"
namespace CXS{
    #define ELEMENT_CLASS_NAME  "vi"
    class VIHi3531DV100 : public HiElement
    {
//        HI_UNF_VI_ATTR_S mAttr;
//        HI_UNF_VI_E mViPort;
        HI_S32 mWidth;
        HI_S32 mHeight;
        VIHi3531DV100():HiElement(ELEMENT_CLASS_NAME){
            mWidth = 1920;
            mHeight = 1080;
//            mViPort = HI_UNF_VI_PORT0;
//            mAttr.bVirtual = HI_TRUE;
//            mAttr.u32BufNum = 6;
            mOutPads.push_back(Pad(this));


        }
    public:
        static Element* createInstance()
        {
            VIHi3531DV100* vi = new VIHi3531DV100();
            return vi;
        }
        int startSelf(){
            int ret = hi3531dv100_vi_start ( mHandle,mWidth,mHeight);
            if(ret == 0)
                CXS_PRT("VI(%d) start ok\n",mHandle);
            return ret;
        }
        int linkTo(Element* elem){
            HiElement* hiElem = dynamic_cast<HiElement*>(elem);
            HI_S32 ret = HI_FAILURE;
            if(strcmp(hiElem->getClassName(),"vo") == 0)
            {
                ret = SAMPLE_COMM_VO_BindVi( hiElem->getHandle(),  0,  mHandle);
                if (HI_SUCCESS != ret)
                {
                         SAMPLE_PRT("bind  Vo and vi failed!\n");
                }
            }
            else if(strcmp(hiElem->getClassName(),"venc") == 0)
            {
                ret = SAMPLE_COMM_VENC_BindVi(hiElem->getHandle(),  mHandle);
                if (HI_SUCCESS != ret)
                {
                         SAMPLE_PRT("bind  Venc and vi failed!\n");
                }
            }
            return ret == HI_SUCCESS ? 0 : -1 ;
        }
        int setAttr(const char* attrName,int attrVal)
        {
            if(strcmp(attrName,"VI-CHN") == 0 )
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
             VIHi3531DV100::createInstance
        },
    };
    REGISTER_ELEMENT(elementDescriptors,sizeof(elementDescriptors)/sizeof(elementDescriptors[0]));
}
