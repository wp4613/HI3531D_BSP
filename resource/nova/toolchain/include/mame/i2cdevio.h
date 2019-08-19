/* eeprom ioctl decl */

#ifndef _HI_I2CIO_H_
#define _HI_I2CIO_H_


typedef enum {
	T_I2CDEV_DEVICES,			/* 设备直接 (Read/Write) */
	T_I2CDEV_CACHE,             /* 缓存信息 (Read/Write) */
	T_I2CDEV_UPDATE,			/* 高速缓存批量更新(Download/Upload) */
	T_I2CDEV_DEVICES_BIT,       /* 设备直接控制位(Read/Write) */
    T_I2CDEV_LOGLVL,
    T_I2CDEV_SUBTYPE_MAX
} i2cdev_subtype_e;

/* 匹配单元号和内核定义文件DEVTYPE的值  */
typedef enum {
    /******i2c switch 0*******/
    T_I2CDEV_DEVTYPE_I2CSW0             = 0,         /*  I2C SWITCH 0 */
    T_I2CDEV_DEVTYPE_CDCM6208_U0        = 1,         /* CDCM6208 : U20 */
    T_I2CDEV_DEVTYPE_CDCM6208_U1        = 2,         /* CDCM6208 :U21 */
    T_I2CDEV_DEVTYPE_SIL9136          	= 3,         /* SIL9136 */
    T_I2CDEV_DEVTYPE_AC_POWER_J1        = 4,         /* AC_POWER_J1 */
    T_I2CDEV_DEVTYPE_AC_EEPROM_J1       = 5,         /* AC_EEPROM_J1 */
    T_I2CDEV_DEVTYPE_AC_POWER_J2        = 6,         /* AC_POWER_J2 */
    T_I2CDEV_DEVTYPE_AC_EEPROM_J2       = 7,         /* AC_EEPROM_J2 */
    /*                                  = 8 */       /* 保留 */
    /*                                  = 9 */       /* 保留 */

    T_I2CDEV_DEVTYPE_NOUSE				= 10,
    T_I2CDEV_DEVTYPE_MAX
} i2cdev_devtype_e;


/* HI_I2C common subheader */
typedef struct {
	i2cdev_subtype_e	subtype;	/* command sub type */
	i2cdev_devtype_e	devtype;	/* device type */
} i2cdev_ioctl_t;

/* HI_I2C Data Access (Read/Write) */

typedef struct {
	i2cdev_ioctl_t			header;		/* header */
	union {
		uint32_t			offset;		/* offset address */

		struct {
			uint8_t	qsfp_upper_flg;	/* upper memory operation flag */
			uint8_t	selector;		/*  */
			uint8_t		dev_addr;	/* device address */
			uint8_t		data_addr;	/* data address */
        } dev_data;
    } addr;

	uint32_t			length;		/* access length */
} i2cdev_ioctl_ex_access_t;


#define i2cdev_offset addr.offset
#define i2cdev_dev_addr addr.dev_data.dev_addr
#define i2cdev_data_addr addr.dev_data.data_addr

#define i2cdev_selector addr.dev_data.selector

#define I2CDEV_IOCTL_ACCESS_DATA_LEN (24)
COMIF_IOCTL_STRUCT_DECL(i2cdev_ioctl_access_t, I2CDEV_IOCTL_ACCESS_DATA_LEN, i2cdev_ioctl_ex_access_t);

/*****************************************************************************/
/* i2cdev日志级别控制                                                        */
/*****************************************************************************/
typedef struct {
	i2cdev_ioctl_t header;
	devlog_lvl_e loglvl;
} i2cdev_ioctl_ex_loglvl_t;
/* I2CDEV的ioctl日志级别控制数据的长度 */
#define I2CDEV_IOCTL_LOGLVL_DATA_LEN (1)

/* COMIF ioctl 结构体  */
COMIF_IOCTL_STRUCT_DECL(i2cdev_ioctl_loglvl_t, I2CDEV_IOCTL_LOGLVL_DATA_LEN, i2cdev_ioctl_ex_loglvl_t);

#define I2CDEV_IOCTL _IOWR('e', 160, i2cdev_ioctl_access_t)
/****************************************oled spec*********************/
typedef struct {
	uint8_t cmd;
	uint8_t x; /*colum start */
	uint8_t y; /*row start*/
	uint8_t xsize; /*wdith*/
	uint8_t ysize;/*height*/
	/*scroll setup(page) */
	uint8_t start;
	uint8_t end;

	int length;
	uint8_t data[64];
} oled_t;

enum {
	LSD_SHOW_INFO,
	LSD_OFF_SCREEN,
	LSD_ON_SCREEN,
	LSD_CLEAR_SCREEN,
	LSD_HOMEPAGE,
	LSD_SETUP_SCROLL,
	LSD_START_SCROLL,
	LSD_STOP_SCROLL,
	LSD_MAX
};
/************************************************************************/
#endif
