/************************************************************************
 *
 *                Copyright (c) 2017 NovaStar Tech Co. Ltd.
 *
 * PROPRIETARY RIGHTS of the following material in either electronic or
 * paper format are pertained to DinTon Cable Ltd.  All manufacturing,
 * reproduction, use, and sales involved with this subject MUST be
 * conformed to the license agreement signed with NovaStar Tech Co. Ltd.
 *
 ************************************************************************/
#ifndef _SYSINIT_H_
#define _SYSINIT_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/*
 * machine dependent initialization and termination functions.  they well be
 * called directly from init() and exit() functions of the module, and should
 * be provided by machine dependent part of SysInit module.
 */
extern int __init machdep_init(void);
extern void __init machdep_exit(void);

/*
 * machine dependent identification name string.  it should also be provided
 * by the machine dependent part of SysInit module.
 */
extern const char *machdep_name;

/*
 * cardinfo/chassisinfo print functions.  these functions can be used to
 * show the contents of cardinfo/chassisinfo on the console.
 */
extern void show_cardinfo(const char *level);
extern void show_chassisinfo(const char *level);

/*
 * cardinfo/chassinfo notifier functions.  these functions should be called
 * from machine dependent part to notify other modules that 'ap_cardinfo'
 * and/or 'ap_chassisinfo' are ready to access.
 */
extern void cardinfo_ok(void);
extern void chassisinfo_ok(void);

extern struct i2c_driver pca954x_driver;

#ifdef _LINUX_SYSCTL_H
/*
 * sysctl handler functions for u8/u16/u32 scalar variable and MAC address.
 * they are defined in this module due to the lack of Linux kernel support.
 */
extern int proc_do_u8(struct ctl_table *, int, void *, size_t *, loff_t *);
extern int proc_do_u16(struct ctl_table *, int, void *, size_t *, loff_t *);
extern int proc_do_u32(struct ctl_table *, int, void *, size_t *, loff_t *);
extern int proc_do_macaddr(struct ctl_table *, int, void *, size_t *, loff_t *);
#endif /* _LINUX_SYSCTL_H */

#endif /* _SYSINIT_H_ */
