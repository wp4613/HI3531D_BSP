/*
 * comifimx.h - implements comif "Inter Module eXchange" feature.
 */
#ifndef __MAME_COMIFIMX_H__
#define __MAME_COMIFIMX_H__

/*
 * comif to sub-module callbacks.
 *	callbacks followed are referenced by "comif" module, and passed
 *	control to each sub-module. Each callback function MUST be defined
 *	in sub-modules such as sram, egt, sgt, u2s, etc.
 *	Structures used for callbacking are hidden from sub-modules.
 */
/* ioctl wrapper */
typedef long (*comif_ioctl_func_t)(void *dev,
	unsigned int cmd, unsigned long data);

extern int comif_register_ioctl(int type, int index, comif_ioctl_func_t func, void *dev);
extern int comif_unregister_ioctl(int type, int index);
extern long comif_call_ioctl(int type, int index, unsigned int cmd, unsigned long data);

/* packet tx wrapper */
struct sk_buff;
typedef int (*comif_tx_func_t)(struct sk_buff *skb, void *dev);

extern int comif_register_tx(int type, int index, comif_tx_func_t func, void *dev);
extern int comif_unregister_tx(int type, int index);
extern int comif_call_tx(int type, int index, struct sk_buff *skb);

/* dump wrapper */
typedef int (*comif_dump_func_t)(void *dev, void *buff);

extern int comif_register_dump(int type, int index, comif_dump_func_t func, void *dev);
extern int comif_unregister_dump(int type, int index);
extern int comif_call_dump(void *buff);


/*
 * comif module service methods:
 *	comif module specific functions referenced from sub-modules.
 *	all the member of comif_interface structure is available only while
 *	comif module is loaded on memory.
 */
struct comif_interface {
    void *netdev;	/* net_device of 'comif' interface */
    int (*packet_rx)(void *arg1, void *arg2);
    int (*notify_intr)(int type, int index);
    int (*notify_stat)(int type, int index, void *arg1, void *arg2);
    int (*notify_stat_128bit)(int type, int index, void *arg1, void *arg2);
    int (*notify_fault)(void);
    int (*token_tryget)(void *, void *);
    int (*token_wait)(void *, void *);
    int (*get_boottype)(void);
    uint64_t (*vreg_read)(int offset);
    void (*vreg_write)(int offset, uint64_t val);
    uint32_t (*flash_read)(int offset);
    void (*flash_write)(int offset, uint32_t val);
    void (*flash_sync)(void);
    void *keypad;
    void (*keypad_led)(void *arg1, u_int16_t bmp, int val);
};

extern struct comif_interface comif_function;

#define comif_receive_skb(skb)	\
	( (comif_function.packet_rx) ? \
	 (*comif_function.packet_rx)((skb), comif_function.netdev) : -ENXIO )

#define comif_notice_intr(t,i)	\
	( (comif_function.notify_intr) ? \
	 (*comif_function.notify_intr)((t),(i)) : 0 )
#define comif_notice_intr_desc(t,i,s,m)	\
	( (comif_function.notify_stat) ? \
	 (*comif_function.notify_stat)((t), (i), (void *)(s), (void *)(m)) : 0 )
#define comif_notice_intr_desc_128bit(t,i,s,m)	\
    ( (comif_function.notify_stat_128bit) ? \
     (*comif_function.notify_stat_128bit)((t), (i), (void *)(s), (void *)(m)) : 0 )

#define comif_notice_fault() \
	( (comif_function.notify_fault) ? \
	 (*comif_function.notify_fault)() : 0 )

#define comif_token_tryget() \
	( (comif_function.token_tryget) ? \
	 (*comif_function.token_tryget)(NULL, NULL) : 0)
#define comif_token_wait_charge(to) \
	( (comif_function.token_wait) ? \
	 (*comif_function.token_wait)(((void *)to), NULL) : 0)

#define comif_vreg_read(addr) \
	( (comif_function.vreg_read) ? \
	 (*comif_function.vreg_read)((addr)) : -1)
#define comif_vreg_write(addr,val) \
	( (comif_function.vreg_write) ? \
	 (*comif_function.vreg_write)((addr),(val)) : -1)
#if defined(A16K_XL) && defined(OVER_AMIOS303_NETBSD)
#define comif_vreg_mask_write(addr,val,mask) \
	( (comif_function.vreg_mask_write) ? \
	 (*comif_function.vreg_mask_write)((addr),(val),(mask)) : -1)
#endif /* A16K_XL && OVER_AMIOS303_NETBSD */

#define comif_flash_read(off) \
	( (comif_function.flash_read) ? (*comif_function.flash_read)(off) : -1 )
#define comif_flash_write(off,v) \
	( (comif_function.flash_write) ? \
	 (*comif_function.flash_write)((off),(v)) : (void)0)
#define comif_flash_sync() \
	( (comif_function.flash_sync) ? (*comif_function.flash_sync)() : (void)0)
 
#define egt_sfptxdis_set(m,v) \
	( (comif_function.sfptxdis_set) ? \
	 (*comif_function.sfptxdis_set)(comif_function.egt, (m),(v)) : (void)0 )

#ifdef FEATURE_G26048C_XG4
#define sfpc_txdis_set(m,v) \
	( (comif_function.sfpc_txdis_set) ? \
	 (*comif_function.sfpc_txdis_set)(comif_function.sfpc, (m),(v)) : (void)0 )

#endif /* FEATURE_G26048C_XG4 */
#define bcm_u2s_misc_init() \
	( (comif_function.misc_init) ? \
	 (*comif_function.misc_init)(comif_function.u2s, NULL) : -ENODEV )

#define comif_do_forced_port_block(cbit, frport_bmp) \
    ( (comif_function.notify_forced_port_block) ? \
     (*comif_function.notify_forced_port_block)((cbit), (frport_bmp)) : 0)

#define comif_is_cpu_reset() \
    ( (comif_function.is_cpu_reset) ? \
	 (*comif_function.is_cpu_reset)() : 0)

typedef enum {
	comif_boot_warm,
	comif_boot_cold,
	/* if add new type , insert above */
	comif_boot_type_max,
} comif_boot_type_e;

#define comif_boot_type() \
	( (comif_function.get_boottype) ? \
	 (*comif_function.get_boottype)() : -ENODEV )

/*
 * V-reg map                  offset(32bit)
 *63                        0
 * +------------------------+ 0x00000000
 * |Interrupt               |     |
 * +------------------------+ 0x00000020
 * |POST Result             |     |
 * +------------------------+ 0x00000040
 * |POST Reason             |     |
 * +------------------------+ 0x00000060
 * |BIST Result             |     |
 * +------------------------+ 0x0000007f
 * |User Space              |     | (17 word)
 * +------------------------+ 0x00000080
 * |Access Fail             |
 * +------------------------+ 0x00000081
 * |Access Fail             |
 * +------------------------+ 0x00000082
 * |Access Fail             |
 * +------------------------+ 0x00000083
 * |SEM                     |
 * +------------------------+ 0x00000084
 * |SEM                     |
 * +------------------------+ 0x00000085
 * |BIST PROCESS(MC)        |
 * +------------------------+ 0x00000086
 * |BIST PROCESS(MC)        |
 * +------------------------+ 0x00000087
 * |BIST CONNECTION(MC)     |
 * +------------------------+ 0x00000088
 * |BIST FAULT(MC)          |
 * +------------------------+ 0x00000089
 * |BIST FAULT(MC)          |
 * +------------------------+ 0x0000008a
 * |BIST PORT BLOCK(MC)     |
 * +------------------------+ 0x0000008b
 * |BIST PORT BLOCK(MC)     |
 * +------------------------+ 0x0000008c
 * |BIST FORCE PWR DOWN(MC) |
 * +------------------------+ 0x0000008d
 * |BIST FORCE PWR DOWN(MC) |
 * +------------------------+ 0x0000008e
 * |BIST SESSION HUP(MC)    |
 * +------------------------+ 0x0000008f
 * |BIST SESSION HUP(MC)    |
 * +------------------------+ 0x00000090
 * |BIST SESSION TO(MC)     |
 * +------------------------+ 0x00000091
 * |BIST SESSION TO(MC)     |
 * +------------------------+ 0x00000092
 * |CHASSIS TYPE(LC)        |
 * +------------------------+ 0x00000093
 * |not used                |
 * +------------------------+ 0x000000a0
 * |POST REASON for SFC(MC) |
 * +------------------------+ 0x000000b0
 */
typedef enum {
    COMIF_VREG_INTERRUPT = 0x00000000,
    COMIF_VREG_POST_RESULT = 0x00000020,
    COMIF_VREG_U2S_SEM_START_ERR = 0x0000003f,
    COMIF_VREG_POST_REASON = 0x00000040,
    COMIF_VREG_BIST_RESULT = 0x00000060,
    COMIF_VREG_ACCESS_FAILURE_INTSTAT = 0x00000080,
    COMIF_VREG_ACCESS_FAILURE_INTLATCH = 0x00000081,
    COMIF_VREG_ACCESS_FAILURE_INTMASK = 0x00000082,
    COMIF_VREG_SEM_INTSTAT = 0x00000083,
    COMIF_VREG_SEM_INTMASK = 0x00000084,
    COMIF_VREG_BIST_PROCESS_TERMINATED       = 0x00000085,
    COMIF_VREG_BIST_PROCESS_NO_REACTION      = 0x00000086,
    COMIF_VREG_BIST_CONNECTION_TERMINATED    = 0x00000087,
    COMIF_VREG_BIST_FAULT_INTSTAT            = 0x00000088,
    COMIF_VREG_BIST_FAULT_INTMASK            = 0x00000089,
    COMIF_VREG_BIST_PORT_BLOCK_INTSTAT       = 0x0000008a,
    COMIF_VREG_BIST_PORT_BLOCK_INTMASK       = 0x0000008b,
    COMIF_VREG_BIST_FORCE_POWER_DOWN_INTSTAT = 0x0000008c,
    COMIF_VREG_BIST_FORCE_POWER_DOWN_INTMASK = 0x0000008d,
    COMIF_VREG_BIST_SESSION_HUP_INTSTAT      = 0x0000008e,
    COMIF_VREG_BIST_SESSION_HUP_INTMASK      = 0x0000008f,
    COMIF_VREG_BIST_SESSION_TO_INTSTAT       = 0x00000090,
    COMIF_VREG_BIST_SESSION_TO_INTMASK       = 0x00000091,
    COMIF_VREG_CHASSIS_TYPE = 0x00000092,
    COMIF_VREG_SFC_POST_REASON = 0x000000a0,
    /* if add new register , insert above */
    COMIF_VREG_MAX_OFFSET = 0x000000b0,
} comif_vreg_offset_e;

/* write 0 to target register */
#define COMIF_POST_SUCCESS(_type, _index) do{                          \
/* printf("comif post:write success(%d,%d)\n",(_type),(_index)); */    \
comif_vreg_write((COMIF_VREG_POST_RESULT + (_index)),                   \
		((~((uint64_t)0x1 << (_type)))                        \
		& comif_vreg_read(COMIF_VREG_POST_RESULT + (_index)))); \
}while(0)

/* write 1 to target register */
#define COMIF_POST_FAIL(_type, _index) do{                             \
/* printf("comif post:write fail(%d,%d)\n",(_type),(_index)); */       \
comif_vreg_write((COMIF_VREG_POST_RESULT + (_index)),                   \
		((uint64_t)0x1 << (_type))                            \
		| comif_vreg_read(COMIF_VREG_POST_RESULT + (_index)));  \
}while(0)

/* write post fail reason and desc */
#define COMIF_POST_FAIL_REASON(_type,_reason_bitmap,_desc) do{         \
  comif_vreg_write((COMIF_VREG_POST_REASON + (_type)),                   \
		   (((uint64_t)(_reason_bitmap)) << 32)               \
		   | ((uint64_t)(_desc)));                            \
}while(0)

/* write post fail reason for SFC */
#define COMIF_SFC_POST_FAIL_REASON(_index, _reason_bitmap) do{         \
  comif_vreg_write((COMIF_VREG_SFC_POST_REASON + (_index)),                   \
		   ((uint64_t)(_reason_bitmap)));               \
}while(0)

#endif /*__MAME_COMIFIMX_H__*/
