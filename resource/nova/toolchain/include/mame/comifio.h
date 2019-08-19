/*************************************************************************
 *
 *             Copyright (c) 2017  NovaStar Tech Co. Ltd.
 *
 * PROPRIETARY RIGHTS of the following material in either
 * electronic or paper format are pertained to DinTon Cable Ltd.
 * All manufacturing, reproduction, use, and sales involved with
 * this subject MUST be conformed to the license agreement signed
 * with NovaStar Tech Co. Ltd.
 *************************************************************************/

/* comif ioctl io header */

#ifndef _COMIFIO_H_
#define _COMIFIO_H_

/* type define */
/* device type enum */
typedef enum _comif_type_e {
	T_SPIDEV,			/* Generic SPI DEV */
	T_I2CDEV,	     	/* Generic I2C DEV */
	T_COMIF,			/* Switch Interface pseudo driver */
	T_CCID,
	T_FAN,
	T_FPGA_CFG,			/* Config FPGA */
    T_KEYPAD,
	T_UART_COM,
	T_DRV_MAX,
	T_COMIF_TYPE_MAX,
} comif_type_e;

/* access type enum */
typedef enum _comif_cmd_e {
	COMIF_INFO,		/* special:information get:value and mask need to be 0 */
	COMIF_READ,	
	COMIF_WRITE,
	COMIF_CFG_FPGA,
	COMIF_READ_TX,
	COMIF_WRITE_TX,	
	COMIF_CMD_MAX,
} comif_cmd_e;

/* error code */
typedef
    enum {
	err_unknown = -1,
	err_normal = 0,		/* 正常 */

	/* CCID */
	err_ccid_ = (T_CCID << 24),
	err_ccid_system,		/* System                  */
	err_ccid_inv_cmd_subtype,	/*      */
	err_ccid_command_invalid,	/*     */
	err_ccid_command_tmo,	/*  */
	err_ccid_internal,	/*           */
	err_ccid_param,		/* */
	
	/* FAN */
	err_fandev_ = (T_FAN << 24),
	err_fandev_system,		/* System                  */
	err_fandev_nomem,	/* malloc()       */
	err_fandev_inv_cmd_subtype,
	err_fandev_internal,
	err_fandev_wt_tmo,
	err_fandev_wt_st,
	err_fandev_busbusy,
	err_fandev_inv_length,
	
	/* I2CDEV */
	err_i2cdev_ = (T_I2CDEV << 24),
	err_i2cdev_system,		/*              */
	err_i2cdev_inv_cmd_subtype,	/*      */
	err_i2cdev_inv_length,	/* invalid length                    */
	err_i2cdev_nomem,		/* malloc()       */
	err_i2cdev_ioclt_wait_timeout,		/* ioctl     */
	err_i2cdev_internal,	/*              */ 
	err_i2cdev_rd_st,		/* i2c_read_bytes() error status(1st check) */
    	err_i2cdev_rd_tmo2,	    /* i2c_read_bytes() timeout(2nd check) */
	err_i2cdev_rd_st2,		/* i2c_read_bytes() error status(2nd check) */
	err_i2cdev_wt_tmo,		/* i2c_write_bytes() timeout(1st check) */
	err_i2cdev_wt_st,		/* i2c_write_bytes() error status(1st check) */
	err_i2cdev_wt_tmo2,	    /* i2c_write_bytes() timeout(2nd check) */
	err_i2cdev_wt_st2,		/* i2c_write_bytes() error status(2nd check) */
	err_i2cdev_nack1,		/* Recv NACK */
	err_i2cdev_nack2,		/* Recv NACK */
	err_i2cdev_nack3,		/* Recv NACK */
	err_i2cdev_busbusy,	    /*       */
	err_i2cdev_dw_tmo,		/* i2c_dummy_write_bytes() timeout */
	err_i2cdev_dw_st,		/* i2c_dummy_write_bytes() error status */          
     
		/* SPIDEV */
	err_spidev_ = (T_SPIDEV << 24),
	err_spidev_system,		/*              */
	err_spidev_inv_cmd_subtype,	/*      */
	err_spidev_inv_length,	/* invalid length                    */
	err_spidev_nomem,		/* malloc()       */
	err_spidev_ioclt_wait_timeout,		/* ioctl     */
	err_spidev_internal,	/*              */ 
	err_spidev_rd_st,		/* () error status(1st check) */
        err_spidev_rd_tmo2,	    /* () timeout(2nd check) */
	err_spidev_rd_st2,		/* () error status(2nd check) */
	err_spidev_wt_tmo,		/* () timeout(1st check) */
	err_spidev_wt_st,		/* () error status(1st check) */
	err_spidev_wt_tmo2,	    /* () timeout(2nd check) */
	err_spidev_wt_st2,		/* () error status(2nd check) */
	err_spidev_nack1,		/* Recv NACK */
	err_spidev_nack2,		/* Recv NACK */
	err_spidev_nack3,		/* Recv NACK */
	err_spidev_busbusy,	    /*       */
	err_spidev_dw_tmo,		/* () timeout */
	err_spidev_dw_st,		/* () error status */   
     /* COMIF */
	err_comif_ = (T_COMIF << 24),
	err_comif_system,	/* System             */
	err_comif_inv_cmd_subtype,	/*     */
	err_comif_internal,	/*             */
	err_comif_nomem,		/* malloc()     */
	err_comif_copyin,	/* copyin()     */
	err_comif_copyout,	/* copyout()   */
	err_comif_table_null,	/*         */
	err_comif_unknown_cmd,	/* ioctl()    */
	err_comif_inv_type_index,	/* type, index       */

	/* FPGA_CFG */
	err_fpga_cfg_ = (T_FPGA_CFG << 24),
	err_fpga_cfg_system,				/*              */
	err_fpga_cfg_inv_cmd_subtype,		/*      */
	err_fpga_cfg_inv_length,			/* invalid length    */
	err_fpga_cfg_nomem,					/* malloc()       */
	err_fpga_cfg_ioclt_wait_timeout,	/* ioctl     */
	err_fpga_cfg_internal,				/*              */ 
	err_fpga_cfg_cfg_failed,			/* configure FPGA failed */
	err_fpga_cfg_unknown_cmd,			/* unkown command */


        /* KEYPAD */
        err_keypad_ = (T_KEYPAD << 24),
        err_keypad_system,                            /*              */
        err_keypad_inv_cmd_subtype,           /*      */
        err_keypad_inv_length,                        /* invalid length    */
        err_keypad_nomem,                                     /* malloc()       */
        err_keypad_ioclt_wait_timeout,        /* ioctl     */
        err_keypad_internal,                          /*              */
        err_keypad_cfg_failed,                        /* configure FPGA failed */
        err_keypad_unknown_cmd,                       /* unkown command */ 

	/* UART_COM */
	err_uart_com_ = (T_UART_COM << 24),
	err_uart_com_system,				/*              */
	err_uart_com_inv_cmd_subtype,		/*      */
	err_uart_com_inv_length,			/* invalid length    */
	err_uart_com_nomem,					/* malloc()       */
	err_uart_com_ioclt_wait_timeout,	/* ioctl     */
	err_uart_com_internal,				/*              */ 
	err_uart_com_unknown_cmd,			/* unkown command */
} comif_err_t;

/* util macro define */
/* define custom structure */
#define COMIF_IOCTL_STRUCT_DECL(name,data_len,extra_type) \
typedef struct { \
  comif_ioctl_t header; \
  uint32_t value[data_len]; \
  uint32_t mask[data_len]; \
  extra_type extra; \
}name

/* allocate any size structure method */
#define COMIF_IOCTL_ALLOC(val,name,datalen) \
do{ \
  val = (void *)malloc(sizeof(name)); \
  if(NULL != val){ \
    val->header.length   = sizeof(name); \
    val->header.data_len = datalen; \
  } \
}while(0)

/* value,mask,extra pointer get method */
#define COMIF_IOCTL_GET_VALUE(p) (uint32_t *)((char *)p + sizeof(comif_ioctl_t))
#define COMIF_IOCTL_GET_MASK(p) \
(uint32_t *)((char *)p + sizeof(comif_ioctl_t) + (sizeof(uint32_t) * ((comif_ioctl_t *)p)->data_len))
#define COMIF_IOCTL_GET_EXTRA(p) \
(void *)((char *)p + sizeof(comif_ioctl_t) + (2 * sizeof(uint32_t) * ((comif_ioctl_t *)p)->data_len))

/* value define */
#define COMIF_INDEX_MAX (28)	/* pipe is max 4,but EEPROM index is max 28 */


#define COMIF_IOCTL_STRING_MAX (64)

/* strcut define */
/* standard (lcd route) ioctl header */
typedef struct {
	uint32_t length;
	comif_type_e type;
	comif_cmd_e cmd;
	uint32_t index;		/* pipe: max value COMIF_INDEX_MAX */
	uint32_t error;
	/* if append member, insert here */
	int data_len;		/* 4byte count (ex.if value[1],data_len = 1) */

} comif_ioctl_t;

/* normal ioctl (burst io) */
typedef struct {
	unsigned long int length;	/* total data size (byte) */
	unsigned int count;	/* total data count (if use?) */
	void *data;		/* burst ioctl data body */
} comif_ioctl_burst_t;

/* in extra: device information */
typedef struct {
	uint32_t vendor;
	uint32_t product;
	uint32_t revision;
	uint32_t index;
	char string[COMIF_IOCTL_STRING_MAX + 1];
} comif_ioctl_info_ex_t;

/* ioctl intr bitmap get */
typedef struct {
	unsigned long long int bmpi[COMIF_INDEX_MAX];
	/* debug */
	uint32_t stat[T_DRV_MAX][COMIF_INDEX_MAX];
	uint32_t mask[T_DRV_MAX][COMIF_INDEX_MAX];
} comif_ioctl_intr_t;

/* direct register i/o ioctl !!!debug only!!! */
typedef struct {
	comif_type_e type;	/* if call /dev/comif, redirect target */
	comif_cmd_e cmd;		/* only read or write */
	uint32_t addr;		/* register address  */
	uint32_t index;		/* or register label */
	uint32_t value;		/* register value    */
	uint32_t mask;		/* need (support) ? */
} comif_ioctl_reg_t;

/* internal ether set */
typedef struct {
	uint32_t linecard_num;	/* ex. LC1=0,LC2=1,,, refs, apcard_bit.h */
	uint32_t enable;		/* 0:disable or 1:enable */
} comif_ioctl_internal_eth_t;

/* ioctl define */
/* common */
/* decl struct: command COMIF_INFO */
#define COMIF_IOCTL_INFO_DATA_LEN (1)
COMIF_IOCTL_STRUCT_DECL(comif_ioctl_info_t, COMIF_IOCTL_INFO_DATA_LEN, comif_ioctl_info_ex_t);


/* use all device */
#define SIOCGDRVSPEC  _IOWR('i', 124, comif_ioctl_reg_t)
#define SIOCGGENREGIO _IOWR('i', 125, comif_ioctl_reg_t)	/* Generic register read,write per device */

/* comif specific */
typedef enum {
	T_COMIF_DELAYMSEC,	/* msec delay */
	T_COMIF_VREG,		/* virtual register */
	T_COMIF_DELAYUSEC,	/* usec delay */
    T_COMIF_LOGLVL,      /* log level control */
	/* if add new item , insert above */
	T_COMIF_SUBTYPE_MAX
} comif_subtype_e;

typedef struct {
	comif_subtype_e subtype;
} comif_ioctl_subheader_t;

#define COMIF_IOCTL_SUB_DELAYMSEC_DATA_LEN (1)
COMIF_IOCTL_STRUCT_DECL(comif_ioctl_sub_delaymsec_t, COMIF_IOCTL_SUB_DELAYMSEC_DATA_LEN, comif_ioctl_subheader_t);

typedef struct {
	comif_ioctl_subheader_t header;
	uint32_t address;
} comif_ioctl_ex_vreg_t;

#define COMIF_IOCTL_VREG_DATA_LEN (2)
COMIF_IOCTL_STRUCT_DECL(comif_ioctl_vreg_t, COMIF_IOCTL_VREG_DATA_LEN, comif_ioctl_ex_vreg_t);

#define COMIF_IOCTL_SUB_DELAYUSEC_DATA_LEN (1)
COMIF_IOCTL_STRUCT_DECL(comif_ioctl_sub_delayusec_t, COMIF_IOCTL_SUB_DELAYUSEC_DATA_LEN,
    comif_ioctl_subheader_t);

#define COMIF_DELAYUSEC_MAX (10000)	/* max 10msec = 10,000usec */

/* use only comif */
#define SIOCGBURSTIO  _IOWR('i', 130, comif_ioctl_burst_t)
#define SIOCGINTERBMP _IOWR('i', 131, comif_ioctl_intr_t)
#define SIOCGDUMPLOG  _IOWR('i', 132, comif_ioctl_intr_t)
#define SIOCGDUMPMSG  _IOWR('i', 133, comif_ioctl_intr_t)
#define SIOPANIC      _IOWR('i', 134, int)
#define SIOWDT        _IOWR('i', 135, int)
#define SIOCSPCLOSE   _IOWR('i', 136, int)  /*  */
#define SIOCSINTETH   _IOWR('i', 137, comif_ioctl_internal_eth_t)
#define SIOOOPS       _IOWR('i', 138, int)

typedef enum _ccid_ex_data_type {
	CCID_EX_DATA_TYPE_PORT    = 1,
	CCID_EX_DATA_TYPE_CONSOLE
} CCID_EX_DATA_TYPE;


/* comif pkt tx ex header definition */
typedef struct _comif_pkt_ex_hdr_s {
	uint8_t type;		/* 0: type */
	uint8_t index;		/* 1: index */

	uint8_t _rsrvd1;	/* 2: alinment */
	uint8_t _rsrvd2;	/* 3: alinment */
} comif_pkt_ex_hdr_t;

/* 增加一个header，用于定位offset，len(包长) */
typedef struct _spi_pkt_ex_hdr_s {
    uint32_t offset; /* offset  */
    uint32_t length; /* pkt length */
} spi_pkt_ex_hdr_t;

/* 4bit serial definition */
typedef enum {
	T_SERIAL_BIT_0,		/* initial */
	T_SERIAL_BIT_1,		/* cardd */
	T_SERIAL_BIT_SFC_SERIAL_PERR,	/* fltd,tmd */
	T_SERIAL_BIT_3,		/* fltd,tmd */
	T_SERIAL_BIT_125M_CLK,	/* fltd */
	T_SERIAL_BIT_SFC_SERIAL_ABNORMAL, /* fltd,tmd */
	T_SERIAL_BIT_SFC_SERIAL_SYNCDAN,  /* fltd,tmd */
	T_SERIAL_BIT_SFC_SERIAL_CLKDAN,   /* fltd,tmd */
	T_SERIAL_BIT_SFC_SERIAL_RESET,    /* tmd */
	T_SERIAL_BIT_9,
	T_SERIAL_BIT_10,
	T_SERIAL_BIT_11,
	T_SERIAL_BIT_12,
	T_SERIAL_BIT_13,
	T_SERIAL_BIT_14,
	T_SERIAL_BIT_15,
	T_SERIAL_BIT_MAX
} comif_serial_bit_e;

#define COMIF_INTERRUPT_SUPPORT_MAX (16)

/* DEV_LOG */
// #define KVM_DEBUG
#ifdef KVM_DEBUG
#define DEV_LOG(log_level,format,args...) \
	do {if (log_level >= global_log_lvl) { dev_process_log (log_level, __FILE__, __FUNCTION__, __LINE__, format, ## args);}} while (0)
#else
#define DEV_LOG(log_level,...)
#endif/*KVM_DEBUG*/

extern void dev_process_log (int loglvl, char *file, const char *func, int line,
                const char *format, ...)  __attribute__((format(printf, 5, 6)));
#define sdc_err_string(err)  "dummy error"

typedef enum
{
	KVM_LOGLVL_DEBUG = 0,   /*  */
	KVM_LOGLVL_INFO,        /*  */
	KVM_LOGLVL_WARNING,     /*  */
	KVM_LOGLVL_ERROR,       /*  */
	KVM_LOGLVL_MAX          /*  */
}devlog_lvl_e;

typedef struct {
	comif_ioctl_subheader_t header;
	devlog_lvl_e loglvl;
} comif_ioctl_ex_loglvl_t;

#define COMIF_IOCTL_LOGLVL_DATA_LEN (1)
COMIF_IOCTL_STRUCT_DECL(comif_ioctl_loglvl_t, COMIF_IOCTL_LOGLVL_DATA_LEN, comif_ioctl_ex_loglvl_t);

#endif /* ndef _COMIFIO_H_ */
