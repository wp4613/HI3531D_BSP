/***************************************************************************
*
* SIMG PART NUMBER - HDMI Transmitter Driver
*
* Copyright (C) (2011, Silicon Image)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation version 2.
*
* This program is distributed "as is" WITHOUT ANY WARRANTY of any
* kind, whether express or implied; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
*****************************************************************************/

#include <linux/i2c.h>
#include <linux/miscdevice.h>  //zxr
//#include <linux/ipu.h>    //zxr
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/irq.h>
//#include <mach/hardware.h>    //zxr


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/platform_device.h>
//#include <linux/mxcfb.h>  //zxr
#include <linux/regulator/consumer.h>
#include <mach/io.h>    //zxr
//#include <mach/hardware.h>    //zxr

#include "siHdmiTx_902x_TPI.h"
#include "hi_gpio_defs.h"

#define DRIVER_VER "1.4"

#define CI2CA false
#ifdef CI2CA
#define SII902xA_plus 0x02  //Define sii902xA's I2c Address of all pages by the status of CI2CA pin.
#else
#define SII902xA_plus 0x00  //Define sii902xA's I2c Address of all pages by the status of CI2CA pin.
#endif
#define GPIO15_1_MODE __io_address(0x120F02F8)  // <<0
#if 1//zxr
static void HDMI_reset(void)
{
 //   gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSI1_D8), 0);   //zxr
    msleep(200);
 //   gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSI1_D8), 3);   //zxr
}

struct platform_data {
    void (*reset) (void);
};
static struct platform_data HDMI_data = {
    .reset = HDMI_reset,
};
#endif

#ifdef I2C_INTERNAL
static struct i2c_board_info hi_info =
{
    I2C_BOARD_INFO("sil9204", 0x39),
};
static struct i2c_client* sil9024_client;
#endif

#define DEVICE_NAME "sii902xA"
#define sii902xA_DEVICE_ID   0xB0

extern struct i2c_client *sii902xA;
extern struct i2c_client *siiEDID;
extern struct i2c_client *siiSegEDID;
extern struct i2c_client *siiHDCP;




struct work_struct	*sii902xAwork;
//static spinlock_t sii902xA_lock = SPIN_LOCK_UNLOCKED;//zxr
//static DEFINE_SPINLOCK(sii902xA_lock);  //zxr


static int match_id(const struct i2c_device_id *id, const struct i2c_client *client)
{
	if (strcmp(client->name, id->name) == 0)
		return true;

	return false;
}

static void work_queue(struct work_struct *work)
{
	siHdmiTx_TPI_Poll();
	enable_irq(sii902xA->irq);
}

#if 0//zxr
static irqreturn_t sii902xA_interrupt(int irq, void *dev_id)
{
	unsigned long lock_flags = 0;	 
	disable_irq_nosync(irq);
	spin_lock_irqsave(&sii902xA_lock, lock_flags);	
	//printk("The sii902xA interrupt handeler is working..\n");  
	//printk("The most of sii902xA interrupt work will be done by following tasklet..\n");  

	schedule_work(sii902xAwork);

	printk("The sii902xA interrupt's top_half has been done and bottom_half will be processed..\n");  
	spin_unlock_irqrestore(&sii902xA_lock, lock_flags);
	return IRQ_HANDLED;
}
#endif

static const struct i2c_device_id hmdi_sii_id[] = {
	{ "sii902xA", 0 },
	{ "siiEDID", 0 },
	{ "siiSegEDID", 0 },
	{ "siiHDCP", 0 },
};
#ifdef I2C_INTERNAL
static int i2c_client_init(void)
{
    struct i2c_adapter* i2c_adap;

    #ifdef HI_CHIP_HI3521A
//	printk("HI_CHIP_HI3521A\n");
    i2c_adap = i2c_get_adapter(0);
    #else
//	printk("HI_CHIP_HI3531D\n");
    i2c_adap = i2c_get_adapter(1);
    #endif
    sil9024_client = i2c_new_device(i2c_adap, &hi_info);
    i2c_put_adapter(i2c_adap);

    return 0;
}

static void i2c_client_exit(void)
{
    i2c_unregister_device(sil9024_client);
}
#endif

static int hdmi_sii_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int ret=0;
	
	//static struct mxc_lcd_platform_data *plat_data;
	if (!i2c_check_functionality(client->adapter,
				     I2C_FUNC_SMBUS_BYTE | I2C_FUNC_I2C))
		return -ENODEV;
	
	
	if(match_id(&hmdi_sii_id[1], client))
		{
			siiEDID = client;
			dev_info(&client->adapter->dev, "attached hmdi_EDID: %s "
				"into i2c adapter successfully \n", id->name);
		}
	else if(match_id(&hmdi_sii_id[2], client))
		{
			siiSegEDID = client;
			dev_info(&client->adapter->dev, "attached hmdi_Seg_EDID: %s "
				"into i2c adapter successfully \n", id->name);
		}
	else if(match_id(&hmdi_sii_id[3], client))
		{
			siiHDCP = client;
			dev_info(&client->adapter->dev, "attached hmdi_HDCP: %s "
				"into i2c adapter successfully \n", id->name);
		}
	else if(match_id(&hmdi_sii_id[0], client))
		{	
			/*
			plat_data = client->dev.platform_data;
			if (plat_data->reset)
				plat_data->reset();
			*/
			sii902xA = client;
			dev_info(&client->adapter->dev, "attached hmdi_sii_id[0] %s "
				"into i2c adapter successfully\n", id->name);
			
			if(sii902xA != NULL )
			{				
				printk("\n============================================\n");
				printk("SiI-902xA Driver Version 1.4 \n");
				printk("============================================\n");
				
				sii902xAwork = kmalloc(sizeof(*sii902xAwork), GFP_ATOMIC);
				INIT_WORK(sii902xAwork, work_queue); 
				//
				// Initialize the registers as required. Setup firmware vars.
				//
				
				siHdmiTx_VideoSel( HDMI_1080P60 );
				//siHdmiTx_VideoSel( HDMI_1024_768_60 );
				siHdmiTx_AudioSel( AFS_48K);
				siHdmiTx_TPI_Init();
				siHdmiTx_PowerStateD3();
				/*   zxr
				ret = request_irq(client->irq, sii902xA_interrupt, IRQ_TYPE_LEVEL_LOW,
					  client->name, client);
			
				if (ret)
					printk(KERN_INFO "%s:%d:902xA interrupt failed\n", __func__,__LINE__);	
					//free_irq(irq, iface);
				else
					enable_irq_wake(client->irq);	
                    */
			}			
		}
	else
		{
			dev_err(&client->adapter->dev, "invalid i2c adapter: can not found dev_id matched\n");
			return -EIO;
		}
	return ret;

}

#if 0   //zxr
static int hdmi_sii_remove(struct i2c_client *client)
{
	//int irq = (int)(sii902xA->irq);
	free_irq(sii902xA->irq,&sii902xA); 
	hdmi_sii_remove(sii902xA);
	dev_info(&client->adapter->dev, "detached from i2c adapter successfully\n");

	return 0;
}

static struct i2c_driver hdmi_sii_i2c_driver = {
	.driver = {
		.name = DEVICE_NAME,
		.owner = THIS_MODULE,
	},
	.probe = hdmi_sii_probe,
	.remove =  __exit_p(hdmi_sii_remove),
	.id_table = hmdi_sii_id,
};
#endif

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static struct himedia_device s_stSil9024Device;
#endif

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
struct himedia_ops stSil9024DrvOps =
{
    .pm_freeze = sil9024_freeze,
    .pm_restore  = sil9024_restore
};
#else
static int sil9024_open(struct inode * inode, struct file * file)
{
	return 0;
} 

static int sil9024_close(struct inode * inode, struct file * file)
{
	return 0;
}
long sil9024_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    file = file;
    cmd = cmd;
    arg = arg;
#if 0//zxr
	unsigned int __user *argp = (unsigned int __user *)arg;	
	unsigned int on;
	unsigned char i;
	unsigned long ptz_ch;
	unsigned int sens[16];
    nvp6134_opt_mode optmode;
	nvp6134_video_mode vmode;
	nvp6134_chn_mode schnmode;
	nvp6134_video_adjust v_adj;
	nvp6134_input_videofmt vfmt;
	nvp6134_input_videofmt_ch vfmt_ch;
	nvp6134_acp_rw_data ispdata;
	nvp6134_acp_rw_data_extention acpdata;
	nvp6124_i2c_mode i2c;
	FIRMWARE_UP_FILE_INFO sfirmup_info;
	int ret;

#endif

	#if 0//1  //zxr
	/* you must skip other command to improve speed of f/w update when you are updating cam's f/w up. we need to review and test */
	if( acp_dvr_checkFWUpdateStatus( cmd ) == -1 )
		{
		//printk(">>>>> DRV[%s:%d] Now cam f/w update mode. so Skip other command.\n", __func__, __LINE__ );
			return 0;
		}
	#endif

#if 0 //zxr
	switch (cmd)
	{
		/***************************************************************
		* ACP firmware update - by Andy(2016-06-26)
		***************************************************************/
		case IOC_VDEC_ACP_POSSIBLE_FIRMUP:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_ispossible_update( &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				{
				   printk("IOC_VDEC_ACP_POSSIBLE_FIRMUP error\n");
				}
		break;
		case IOC_VDEC_ACP_FIRMUP:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_firmware_update( &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				{
				printk("IOC_VDEC_ACP_FIRMUP error\n");
             }
		break;
		case IOC_VDEC_ACP_CHECK_ISPSTATUS:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_check_ispstatus( &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				printk("IOC_VDEC_ACP_CHECK_ISPSTATUS error\n");
		break;
		case IOC_VDEC_ACP_START_FIRMUP:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_start_command( &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				printk("IOC_VDEC_ACP_START_FIRMUP error\n");
		break;
		case IOC_VDEC_ACP_FIRMUP_END:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_end_command( sfirmup_info.result, &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				printk("IOC_VDEC_ACP_FIRMUP_END error\n");
		break;
		/***************************************************************
		* End updating firmware using ACP(TX<->RX)
		***************************************************************/

		case IOC_VDEC_SET_I2C : // nextchip demoboard test
			if (copy_from_user(&i2c, argp, sizeof(nvp6124_i2c_mode)))
				return -1;
			down(&nvp6134_lock);

			if(i2c.flag == 0) // read
			{
				gpio_i2c_write(i2c.slaveaddr, 0xFF, i2c.bank);
				i2c.data = gpio_i2c_read(i2c.slaveaddr, i2c.address);
			}
			else //write
			{
				gpio_i2c_write(i2c.slaveaddr, 0xFF, i2c.bank);
				gpio_i2c_write(i2c.slaveaddr, i2c.address, i2c.data);
			}
			up(&nvp6134_lock);
			if(copy_to_user(argp, &i2c, sizeof(nvp6124_i2c_mode)))
				printk("IOC_VDEC_I2C error\n");
		break;
		case IOC_VDEC_GET_VIDEO_LOSS:
			down(&nvp6134_lock);	
			g_vloss = nvp6134_getvideoloss();
			up(&nvp6134_lock);
			if(copy_to_user(argp, &g_vloss, sizeof(unsigned int)))
				printk("IOC_VDEC_GET_VIDEO_LOSS error\n");
			break;
		case IOC_VDEC_SET_EQUALIZER:
			if (copy_from_user(&s_eq_type, argp, sizeof(nvp6134_equalizer)))
				return -1;
			down(&nvp6134_lock);
			for(i=0;i<nvp6134_cnt*4;i++)
			{
				nvp6134_set_equalizer(i);
			}
			if(copy_to_user(argp, &s_eq, sizeof(nvp6134_equalizer)))
				printk("IOC_VDEC_SET_EQUALIZER error\n");
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_GET_DRIVERVER:			
			if(copy_to_user(argp, &DRIVER_VER, sizeof(DRIVER_VER)))
				printk("IOC_VDEC_GET_DRIVERVER error\n");
			break;
		case IOC_VDEC_ACP_WRITE:
			if (copy_from_user(&ispdata, argp, sizeof(nvp6134_acp_rw_data)))
				return -1;
			down(&nvp6134_lock);			
			if(ispdata.opt == 0)
				acp_isp_write(ispdata.ch, ispdata.addr, ispdata.data);
			else
			{
				ispdata.data = acp_isp_read(ispdata.ch, ispdata.addr);
				if(copy_to_user(argp, &ispdata, sizeof(nvp6134_acp_rw_data)))
					printk("IOC_VDEC_ACP_WRITE error\n");	
			}
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_ACP_WRITE_EXTENTION:
			if (copy_from_user(&acpdata, argp, sizeof(nvp6134_acp_rw_data_extention)))
				return -1;
			down(&nvp6134_lock);
			acp_isp_write_extention( acpdata.ch, &acpdata );
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_ACP_READ:
			if (copy_from_user(&vfmt, argp, sizeof(nvp6134_input_videofmt)))
				return -1;
			down(&nvp6134_lock);
			for(i=0;i<(4*nvp6134_cnt);i++)
			{
				if(1)
				{
					/* read A-CP */
					if(((g_vloss>>i)&0x01) == 0x00)
						acp_read(&vfmt, i);
				}
			}							
			up(&nvp6134_lock);
			if(copy_to_user(argp, &vfmt, sizeof(nvp6134_input_videofmt)))
				printk("IOC_VDEC_PTZ_ACP_READ error\n");			
			break;	
		case IOC_VDEC_PTZ_ACP_READ_EACH_CH:
			if (copy_from_user(&vfmt_ch, argp, sizeof(nvp6134_input_videofmt_ch)))
				return -1;
			down(&nvp6134_lock);
			/* read A-CP */
			if(((g_vloss>>vfmt_ch.ch)&0x01) == 0x00)
			{
				acp_read(&vfmt_ch.vfmt, vfmt_ch.ch);
			}

			up(&nvp6134_lock);
			if(copy_to_user(argp, &vfmt_ch, sizeof(nvp6134_input_videofmt_ch)))
				printk("IOC_VDEC_PTZ_ACP_READ_EACH_CH error\n");
			break;
		case IOC_VDEC_GET_INPUT_VIDEO_FMT:
			if (copy_from_user(&vfmt, argp, sizeof(nvp6134_input_videofmt)))
				return -1;
			down(&nvp6134_lock);
			video_fmt_det(&vfmt);
			up(&nvp6134_lock);
			if(copy_to_user(argp, &vfmt, sizeof(nvp6134_input_videofmt)))
				printk("IOC_VDEC_GET_INPUT_VIDEO_FMT error\n");
			break;	
        case IOC_VDEC_SET_VIDEO_MODE:
            if (copy_from_user(&vmode, argp, sizeof(nvp6134_video_mode)))
				return -1;
			down(&nvp6134_lock);
			//not used.				
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_CHNMODE:
            if (copy_from_user(&schnmode, argp, sizeof(nvp6134_chn_mode))) return -1;
			down(&nvp6134_lock);
			if(0 == nvp6134_set_chnmode(schnmode.ch, schnmode.vformat, schnmode.chmode))
				// printk("IOC_VDEC_SET_CHNMODE OK\n");
			up(&nvp6134_lock);
			break;
             
		case IOC_VDEC_SET_OUTPORTMODE:
            if(copy_from_user(&optmode, argp, sizeof(nvp6134_opt_mode))) return -1;
			down(&nvp6134_lock);
            // printk("IOC_VDEC_SET_CHNMODE OK\n");
            //nvp6134_set_portmode(chip, 0, NVP6134_OUTMODE_1MUX_FHD, 0);
			nvp6134_set_portmode(optmode.chipsel, optmode.portsel, optmode.portmode, optmode.chid);
			up(&nvp6134_lock);
			break;	
		case IOC_VDEC_SET_BRIGHTNESS:
            if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			//nvp6134_video_set_brightness(v_adj.ch, v_adj.value, ch_vfmt_status[v_adj.ch]);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_CONTRAST:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			//nvp6134_video_set_contrast(v_adj.ch, v_adj.value, ch_vfmt_status[v_adj.ch]);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_HUE:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			//nvp6134_video_set_hue(v_adj.ch, v_adj.value, ch_vfmt_status[v_adj.ch]);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_SATURATION:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			//nvp6134_video_set_saturation(v_adj.ch, v_adj.value, ch_vfmt_status[v_adj.ch]);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_SHARPNESS:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			nvp6134_video_set_sharpness(v_adj.ch, v_adj.value);
			up(&nvp6134_lock);
			break; 
		case IOC_VDEC_PTZ_PELCO_INIT:
			down(&nvp6134_lock);
			//nvp6134_pelco_coax_mode();
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_RESET:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_RESET);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_SET:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_SET);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_CHANNEL_SEL:
            if (copy_from_user(&ptz_ch, argp, sizeof(ptz_ch)))
			{
				return -1;
			}
			down(&nvp6134_lock);
			g_coax_ch = ptz_ch;
			//printk("g_coax_ch ==> %d\n", g_coax_ch);
			//g_coax_ch = ptz_ch%4;  //nextchip demoboard shift test...
			
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_UP:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_UP);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_DOWN:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_DOWN);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_LEFT:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_LEFT);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_RIGHT:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_RIGHT);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_FOCUS_NEAR:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_FOCUS_NEAR);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_FOCUS_FAR:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_FOCUS_FAR);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_ZOOM_WIDE:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_ZOOM_WIDE);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_ZOOM_TELE:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_ZOOM_TELE);
			up(&nvp6134_lock);
			break;

		case IOC_VDEC_INIT_MOTION:
			down(&nvp6134_lock);
			nvp6134_motion_init(0, 0);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_ENABLE_MOTION:
			break;
		case IOC_VDEC_DISABLE_MOTION:
			break;
		case IOC_VDEC_SET_MOTION_AREA:
			break;
		case IOC_VDEC_GET_MOTION_INFO:
			nvp6134_get_motion_ch();
			break;
		case IOC_VDEC_SET_MOTION_DISPLAY:
            if(copy_from_user(&on, argp, sizeof(unsigned int))) return -1;
			down(&nvp6134_lock);
			nvp6134_motion_display(0,on);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_MOTION_SENS:
            if(copy_from_user(&sens, argp, sizeof(unsigned int)*16)) return -1;
			down(&nvp6134_lock);
			nvp6134_motion_sensitivity(sens);
			up(&nvp6134_lock);
			break;
        case NVP6134_SET_AUDIO_PLAYBACK:
			{
				break;
			}
        case NVP6134_SET_AUDIO_DA_VOLUME:
			{
				break;
			}
		case NVP6134_SET_AUDIO_DA_MUTE:
			{
				break;
			}
		case NVP6134_SET_AUDIO_PB_FORMAT:
			{
				int ret = 0;
				nvp6134_audio_format audio_format;
				if(copy_from_user(&audio_format, argp, sizeof(nvp6134_audio_format))) return -1;
				down(&nvp6134_lock);			
				ret = nvp6134_audio_set_format(1, audio_format);
				up(&nvp6134_lock);
				return ret;
			}
		case NVP6134_SET_AUDIO_R_FORMAT:
			{
				int ret = 0;
				nvp6134_audio_format audio_format;
				if(copy_from_user(&audio_format, argp, sizeof(nvp6134_audio_format))) return -1;
				down(&nvp6134_lock);			
				ret = nvp6134_audio_set_format(0, audio_format);
				up(&nvp6134_lock);
				return ret;
			}
		default:
            //printk("drv:invalid nc decoder ioctl cmd[%x]\n", cmd);
			break;
	}
#endif
	return 0;
}
static struct file_operations sil9024_fops = {
	.owner      = THIS_MODULE,
    .unlocked_ioctl	= sil9024_ioctl,
	.open       = sil9024_open,
	.release    = sil9024_close
};
static struct miscdevice sil9024_dev = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "sil9024",
	.fops  		= &sil9024_fops,
};
#endif

static int __init hdmi_sii_init(void)
{
		
	int ret;
    printk(KERN_EMERG "xxxxxxxxxxxx\n");
    //ret = i2c_add_driver(&hdmi_sii_i2c_driver);
	//if (ret)
	//	printk(KERN_ERR "%s: failed to add sii902xA i2c driver\n", __func__);
	//return ret;	

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    snprintf(s_stSil9024Device.devfs_name, sizeof(s_stSil9024Device.devfs_name), DEV_NAME);
    s_stSil9024Device.minor  = HIMEDIA_DYNAMIC_MINOR;
    s_stSil9024Device.fops   = &sil9024_fops;
    s_stSil9024Device.drvops = &stSil9024DrvOps;
    s_stSil9024Device.owner  = THIS_MODULE;

    ret = himedia_register(&s_stSil9024Device);
    if (ret)
    {
        printk(0, "could not register sil9024_dev device");
        return -1;
    }
#else
	ret = misc_register(&sil9024_dev);
   	if (ret)
	{
		printk("ERROR: could not register sil9024_dev devices:%#x \n",ret);		
        return -1;
	}
#endif
	printk("SIL9024 EXT Driver %s COMPILE TIME[%s %s]\n", DRIVER_VER, __DATE__,__TIME__);
	#ifdef I2C_INTERNAL
    i2c_client_init();
	#endif

    hdmi_sii_probe(sil9024_client,hmdi_sii_id);
    return 0;//zxr
}

static void __exit hdmi_sii_exit(void)
{
	kfree(sii902xAwork);
    //i2c_del_driver(&hdmi_sii_i2c_driver);
	//i2c_del_driver(&EDID_i2c_driver);
	
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    himedia_unregister(&s_stSil9024Device);
#else
    misc_deregister(&sil9024_dev);
#endif	
#ifdef I2C_INTERNAL
    i2c_client_exit();
#endif	
	printk("Module is leaving..\n");  
	
}

module_init(hdmi_sii_init);
module_exit(hdmi_sii_exit);
//MODULE_VERSION(DRIVER_VER);
//MODULE_AUTHOR("Silicon image SZ office, Inc.");
//MODULE_DESCRIPTION("sii902xA HDMI driver");
//MODULE_ALIAS("platform:hdmi-sii902xA");
MODULE_LICENSE("GPL");
