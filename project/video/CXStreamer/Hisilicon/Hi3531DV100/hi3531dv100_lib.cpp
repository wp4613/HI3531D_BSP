#include <unistd.h>
#include <string.h>
#include "../../cxs_comm.h"
#include"hi_common.h"
#include"hi_comm_vi.h"
#include"hi_comm_vo.h"
#include"mpi_vi.h"
#include"mpi_vo.h"
#include "../cxs_hi_comm.h"

#include "common/sample_comm.h"

extern VI_DEV_ATTR_S DEV_ATTR_7441_BT1120_STANDARD_BASE;

static int bind_ch(HI_S32 ViChn)
{
    VI_CHN_BIND_ATTR_S stChnBindAttr;
    HI_S32 s32Ret;
    /* When in the 16x1080p mode, bind chn 2,6,10,14 to way1 is needed */
    if (ViChn%4 == 0)
    {
        s32Ret = HI_MPI_VI_GetChnBind(ViChn, &stChnBindAttr);
        if (HI_ERR_VI_FAILED_NOTBIND == s32Ret)
        {
            stChnBindAttr.ViDev = ViChn/4;
            stChnBindAttr.ViWay = 1;
            s32Ret = HI_MPI_VI_BindChn(ViChn, &stChnBindAttr);
            if (HI_SUCCESS != s32Ret)
            {
                CXS_PRT("call HI_MPI_VI_BindChn failed with %#x\n", s32Ret);
                *(int*)0 = 10;
                return HI_FAILURE;
            }
        }
    }
    return HI_SUCCESS;
}
static HI_S32 startChn(HI_S32 ViChn,HI_S32 width,HI_S32 height)
{
//    HI_S32 SAMPLE_COMM_VI_StartChn(VI_CHN ViChn, RECT_S *pstCapRect, SIZE_S *pstTarSize,
//        SAMPLE_VI_MODE_E enViMode, SAMPLE_VI_CHN_SET_E enViChnSet)

    HI_S32 s32Ret;
    VI_CHN_ATTR_S stChnAttr;
    RECT_S capRect ;
    memset(&capRect,0,sizeof(capRect));
    capRect.u32Width = width;
    capRect.u32Height = height;
    stChnAttr.stCapRect.s32X = 0;
    stChnAttr.stCapRect.s32Y = 0;
    stChnAttr.stCapRect.u32Width = width;
    stChnAttr.stCapRect.u32Height = height;
    /* step  5: config & start vicap dev */
    /* to show scale. this is a sample only, we want to show dist_size = D1 only */
    stChnAttr.stDestSize.u32Width = width;
    stChnAttr.stDestSize.u32Height = height;
    stChnAttr.enCapSel = VI_CAPSEL_BOTH;
    stChnAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;   /* sp420 or sp422 */

    stChnAttr.bMirror = HI_FALSE;
    stChnAttr.bFlip = HI_FALSE;
    stChnAttr.s32SrcFrameRate = -1;
    stChnAttr.s32DstFrameRate = -1;
    stChnAttr.enScanMode = VI_SCAN_PROGRESSIVE; //逐行


    s32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        CXS_PRT("failed with %#x!\n" ,s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_EnableChn(ViChn);
    if (s32Ret != HI_SUCCESS)
    {
        CXS_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
extern "C"
{
    extern HI_VOID SAMPLE_COMM_VI_SetMask(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr);
}
HI_S32 hi3531dv100_vi_start(HI_S32 hiViChn,HI_S32 width,HI_S32 height)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VI_DEV_ATTR_S stViDevAttr;
    HI_S32 hiViDev = 0;
    if(width == 1920 && height == 1080)
    {
        hiViDev = hiViChn/4;
        memcpy(&stViDevAttr,&DEV_ATTR_7441_BT1120_STANDARD_BASE,sizeof(stViDevAttr));
    }


    SAMPLE_COMM_VI_SetMask(hiViDev,&stViDevAttr);

    s32Ret = HI_MPI_VI_SetDevAttr(hiViDev, &stViDevAttr);
    if (s32Ret != HI_SUCCESS)
    {
        CXS_PRT("HI_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VI_EnableDev(hiViDev);
    if (s32Ret != HI_SUCCESS)
    {
        CXS_PRT("HI_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = bind_ch(hiViChn);
    if (s32Ret != HI_SUCCESS)
    {
        CXS_PRT("bind_ch failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    s32Ret = startChn(hiViChn, width, height);
    if (s32Ret != HI_SUCCESS)
    {
        CXS_PRT("startChn failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;

}


HI_S32 hi3531dv100_vo_start(HI_S32 handle,HI_S32 ,HI_S32 )
{

    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 VoDev = handle;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    VO_PUB_ATTR_S stVoPubAttr_hd0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VO_LAYER VoLayer = VoDev;

    stVoPubAttr_hd0.enIntfSync = VO_OUTPUT_1080P60;
    stVoPubAttr_hd0.enIntfType = VO_INTF_HDMI;
    stVoPubAttr_hd0.u32BgColor = 0x0;

    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr_hd0);
    if (HI_SUCCESS != s32Ret)
    {
            CXS_PRT("Start SAMPLE_COMM_VO_StartDev failed!\n");
            goto END_8_1080P_3;
    }

   memset(&(stLayerAttr), 0 , sizeof(VO_VIDEO_LAYER_ATTR_S));
    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr_hd0.enIntfSync, \
            &stLayerAttr.stImageSize.u32Width, \
            &stLayerAttr.stImageSize.u32Height, \
            &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
            CXS_PRT("Start SAMPLE_COMM_VO_GetWH failed!\n");
            goto END_8_1080P_4;
    }


    stLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    stLayerAttr.stDispRect.s32X       = 0;
    stLayerAttr.stDispRect.s32Y       = 0;
    stLayerAttr.stDispRect.u32Width   = stLayerAttr.stImageSize.u32Width;
    stLayerAttr.stDispRect.u32Height  = stLayerAttr.stImageSize.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
            SAMPLE_PRT("Start SAMPLE_COMM_VO_StartLayer failed!\n");
            goto END_8_1080P_4;
    }


    s32Ret = SAMPLE_COMM_VO_StartChn(VoLayer, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
            SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
            goto END_8_1080P_5;
    }
#define HDMI_SUPPORT
#ifdef HDMI_SUPPORT
        /* if it's displayed on HDMI, we should start HDMI */
        if (stVoPubAttr_hd0.enIntfType & VO_INTF_HDMI)
        {
                if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr_hd0.enIntfSync))
                {
                        SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
                        goto END_8_1080P_6;
                }
        }
#endif

        return s32Ret;

END_8_1080P_6:
    SAMPLE_COMM_VO_StopChn(VoLayer, enVoMode);
 END_8_1080P_5:

         SAMPLE_COMM_VO_StopLayer(VoLayer);
 END_8_1080P_4:
         SAMPLE_COMM_VO_StopDev(VoDev);
 END_8_1080P_3:  //vi unbind vpss
         return s32Ret;
}

#include "../Hi3531DV100/venchi3531dv100.h"
extern "C" {
    int get_framerate_from_venc(int vend_ch){
        return  CXS::VEncHi3531DV100::findVencByHandle(vend_ch)->getFrameRate();
    }
}


HI_S32 hi3531dv100_venc_start(HI_S32 handle)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 VencChn = handle;
    PAYLOAD_TYPE_E enPayLoad = PT_H264;
    VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_NTSC;
    SAMPLE_RC_E enRcMode = SAMPLE_RC_VBR;
    HI_U32 u32Profile = 0;
    PIC_SIZE_E enSize = PIC_HD1080;
    //HI_S32 s32ChnNum = 1;
    VENC_FRAME_RATE_S stFrameRate;

    CXS::VEncHi3531DV100* venc = CXS::VEncHi3531DV100::findVencByHandle(handle);
    if(venc->getMode() == "HD720"){
        enSize = PIC_HD720;
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx   %s\n",venc->getMode().c_str());
    }
    printf("================================   %s\n",venc->getMode().c_str());
    s32Ret = SAMPLE_COMM_VENC_Start(VencChn, enPayLoad, \
                                         gs_enNorm, enSize, enRcMode, u32Profile);
    if (HI_SUCCESS != s32Ret)
     {
         SAMPLE_PRT("Start Venc failed!\n");
         goto END_VENC_1080P_CLASSIC_3;
     }

    s32Ret = HI_MPI_VENC_GetFrameRate( VencChn, &stFrameRate);
    if (HI_SUCCESS != s32Ret)
     {
         SAMPLE_PRT("Get Venc FrameRate failed!\n");
         goto END_VENC_1080P_CLASSIC_3;
     }
    stFrameRate.s32SrcFrmRate = 60;
    stFrameRate.s32DstFrmRate = 15; //zxr
    printf("================= in frm=%d,out frm=%d\n",stFrameRate.s32SrcFrmRate,stFrameRate.s32DstFrmRate);
    s32Ret = HI_MPI_VENC_SetFrameRate( VencChn, &stFrameRate);
    if (HI_SUCCESS != s32Ret)
     {
         SAMPLE_PRT("Set Venc FrameRate failed!\n");
         goto END_VENC_1080P_CLASSIC_3;
     }


    //s32Ret = SAMPLE_COMM_VENC_StartGetStream_Svc_t(s32ChnNum);
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(handle);

     if (HI_SUCCESS != s32Ret)
     {
         SAMPLE_PRT("Start Venc failed!\n");
         goto END_VENC_1080P_CLASSIC_4;
     }

     return s32Ret;
     //SAMPLE_COMM_VENC_StopGetStream();

 END_VENC_1080P_CLASSIC_4:
     //SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
     SAMPLE_COMM_VENC_Stop(VencChn);



 END_VENC_1080P_CLASSIC_3:        //unbind vpss and venc
// END_VENC_1080P_CLASSIC_2:        //vpss stop
// END_VENC_1080P_CLASSIC_1:       //vi stop
// END_VENC_1080P_CLASSIC_0:       //system exit

    return s32Ret;
}


