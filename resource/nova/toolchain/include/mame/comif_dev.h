/************************************************************************* 
 *                                                                         
 *             Copyright (c) 2017 NovaStar Tech Co. Ltd.
 *                                                                         
 * PROPRIETARY RIGHTS of the following material in either
 * electronic or paper format are pertained to DinTon Cable Ltd.
 * All manufacturing, reproduction, use, and sales involved with
 * this subject MUST be conformed to the license agreement signed
 * with NovaStar Tech Co. Ltd.
 *
 *************************************************************************/

/*
 * Definitions for the comif pseudo device.
 */
#ifndef COMIF_DEV_H
#define COMIF_DEV_H
#ifdef __KERNEL__

/* check macro */
#ifdef AMIOS301_NETBSD
#define INVALID_TI(type, index)\
	((type < J_BCM) || ( J_DRV_MAX <= type) || (COMIF_INDEX_MAX <= index))
#else /* AMIOS301_NETBSD */
#define INVALID_TI(type, index)\
	((type < J_BCM) || ( J_DRV_MAX <= type) ||\
	 (index < 0) || (COMIF_INDEX_MAX <= index))
#endif /* AMIOS301_NETBSD */


/* nvram r/w macros for util all devs */

#if defined(A18K_MC) || defined(A16K_MC) || defined(A18K_LC) || defined(A16K_LC) || defined(A26K_MC) || defined(A26K_LC)
#define COMIF_NVRAM_SOFTC_REG(sc) ((NULL == comif_nvram_softc)?(comif_nvram_softc = (sc)):0)
#define COMIF_NVRAM_SOFTC_READ(addr) \
 ((NULL != comif_nvram_softc)? NVRAMREAD2((nvram_softc_t *)comif_nvram_softc,(addr)):0)
#else
#define COMIF_NVRAM_SOFTC_REG(sc) (0)
#define COMIF_NVRAM_SOFTC_READ(addr) (0)
#endif

/* flash r/w macros for util all devs*/
#define COMIF_FLASH_DATASIZE (64 * 1024)	/* 1sector 64Kbyte */
#if defined(A18K_MC) || defined(A16K_MC) || defined(A18K_LC) || defined(A16K_LC) || defined(A26K_MC) || defined(A26K_LC)
#define COMIF_FLASH_DEVICE (0)
#define COMIF_FLASH_START_ADDRESS (0x007c0000 /* HCBOOT start/Firmware last */ - COMIF_FLASH_DATASIZE)
#else /* ARB_SFC */
#define COMIF_FLASH_DEVICE (0)
#define COMIF_FLASH_START_ADDRESS (0x00800000 /* Firmware last */ - COMIF_FLASH_DATASIZE)
#endif /* MC/LC/ARB_SFC */

#define COMIF_FLASH_SOFTC_REG(sc,_i) ((NULL == comif_flash_softc[(_i)])?(comif_flash_softc[(_i)] = (sc)):0)
/* address are custom area offset (0-COMIF_FLASH_DATASIZE) */
#define COMIF_FLASH_SOFTC_READ(_off) comif_flash_read(_off)
#define COMIF_FLASH_SOFTC_WRITE(_off,_val) comif_flash_write(_off,_val)
#define COMIF_FLASH_SOFTC_FLASH() comif_flash_flash()

/* packet token backet */
#define COMIF_TOKEN_BACKET_CHARGE_MAX_SIZE (25000)	/* max 25,000 packet / sec */
#define COMIF_TOKEN_BACKET_CHARGE_COUNT_PER_SEC (50)	/* (re)charge count 50 per 1sec */
#define COMIF_TOKEN_BACKET_CHARGE_SIZE_PER_COUNT \
(COMIF_TOKEN_BACKET_CHARGE_MAX_SIZE/COMIF_TOKEN_BACKET_CHARGE_COUNT_PER_SEC)	/* size = max / count */
#define COMIF_TOKEN_BACKET_WAKEUP_INTERVAL_TIMO \
	( (HZ > COMIF_TOKEN_BACKET_CHARGE_COUNT_PER_SEC) ? \
	  (HZ / COMIF_TOKEN_BACKET_CHARGE_COUNT_PER_SEC) : 1 )
#define COMIF_TOKEN_WAIT_FOREVER_TIMO (HZ / 10)	/* 100ms */

typedef struct {
	uint32_t intstat;
	uint32_t intmask;
} comif_intr_t;

typedef struct {
	uint32_t intstat;
	uint32_t intlatch;
} comif_latch_t;

#endif /*__KERNEL__*/
#endif /*COMIF_DEV_H*/
