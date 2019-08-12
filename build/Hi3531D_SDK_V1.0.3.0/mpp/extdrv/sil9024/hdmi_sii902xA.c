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
#include <linux/ipu.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <mach/hardware.h>


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mxcfb.h>
#include <linux/regulator/consumer.h>
#include <mach/hardware.h>

#include "siHdmiTx_902x_TPI.h"

#define DRIVER_VER "1.4"

#define CI2CA false
#ifdef CI2CA
#define SII902xA_plus 0x02  //Define sii902xA's I2c Address of all pages by the status of CI2CA pin.
#else
#define SII902xA_plus 0x00  //Define sii902xA's I2c Address of all pages by the status of CI2CA pin.
#endif

static void HDMI_reset(void)
{
    gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSI1_D8), 0);
    msleep(200);
    gpio_set_value(IOMUX_TO_GPIO(MX51_PIN_CSI1_D8), 3);
}

struct platform_data {
    void (*reset) (void);
};

static struct platform_data HDMI_data = {
    .reset = HDMI_reset,
};

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
static spinlock_t sii902xA_lock = SPIN_LOCK_UNLOCKED;


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
    i2c_unregister_device(sil9204_client);
}
#endif

static int hdmi_sii_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int ret=0;
	#ifdef I2C_INTERNAL
    i2c_client_init();
	#endif
	
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
				
				siHdmiTx_VideoSel( HDMI_1024_768_60 );
				siHdmiTx_AudioSel( AFS_48K);
				siHdmiTx_TPI_Init();
				siHdmiTx_PowerStateD3();
				
				ret = request_irq(client->irq, sii902xA_interrupt, IRQ_TYPE_LEVEL_LOW,
					  client->name, client);
			
				if (ret)
					printk(KERN_INFO "%s:%d:902xA interrupt failed\n", __func__,__LINE__);	
					//free_irq(irq, iface);
				else
					enable_irq_wake(client->irq);	
			}			
		}
	else
		{
			dev_err(&client->adapter->dev, "invalid i2c adapter: can not found dev_id matched\n");
			return -EIO;
		}
	return ret;

}


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
static struct miscdevice sil9024_dev = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "sil9024",
	.fops  		= &sil9024_fops,
};
#endif

static int __init hdmi_sii_init(void)
{
		
	int ret;
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

    hdmi_sii_probe(sil9024_client,hmdi_sii_id[0]);
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
MODULE_VERSION(DRIVER_VER);
MODULE_AUTHOR("Silicon image SZ office, Inc.");
MODULE_DESCRIPTION("sii902xA HDMI driver");
MODULE_ALIAS("platform:hdmi-sii902xA");
