/* eeprom ioctl decl */

 #ifndef _HI_SPIIO_H_
 #define _HI_SPIIO_H_


 typedef enum {
         T_SPIDEV_REG,                   /* 设备直接 (Read/Write) */
         T_SPIDEV_UPDATE,                        /* 高速缓存批量更新(Download/Upload) */
         T_SPIDEV_LOGLVL,
         T_SPIDEV_SUBTYPE_MAX
 } spidev_subtype_e;

 /* 匹配单元号和内核定义文件DEVTYPE的值  */
 typedef enum {
     /****** switch 0*******/
     T_SPIDEV_DEVTYPE_FPGA0             = 0,              /*   FPGA PVW */
     T_SPIDEV_DEVTYPE_FPGA1             = 1,              /*   FPGA PGM */
     T_SPIDEV_DEVTYPE_MAX
 } spidev_devtype_e;


 typedef struct {
         spidev_subtype_e        subtype;        /* command sub type */
 } spidev_ioctl_t;

typedef struct {
	spidev_ioctl_t			header;		/* header */
	uint32_t			offset;		/* offset address */
	uint32_t			length;		/* access length */
} spidev_ioctl_ex_access_t;


 #define SPIDEV_IOCTL_ACCESS_DATA_LEN (256) /* 1024B */
 COMIF_IOCTL_STRUCT_DECL(spidev_ioctl_access_t, SPIDEV_IOCTL_ACCESS_DATA_LEN, spidev_ioctl_ex_access_t);

 /*****************************************************************************/
 /* i2cdev日志级别控制                                                        */
 /*****************************************************************************/
 typedef struct {
         spidev_ioctl_t header;
         devlog_lvl_e loglvl;
 } spidev_ioctl_ex_loglvl_t;
 /* I2CDEV的ioctl日志级别控制数据的长度 */
 #define SPIDEV_IOCTL_LOGLVL_DATA_LEN (1)

 /* COMIF ioctl 结构体  */
 COMIF_IOCTL_STRUCT_DECL(spidev_ioctl_loglvl_t, SPIDEV_IOCTL_LOGLVL_DATA_LEN, spidev_ioctl_ex_loglvl_t);

 #define SPIDEV_IOCTL _IOWR('e', 160, spidev_ioctl_access_t)

/************************************************************************/
#endif 

