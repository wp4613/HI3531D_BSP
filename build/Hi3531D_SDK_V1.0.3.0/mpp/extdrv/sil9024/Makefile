#
# Makefile for HDMI sii9022 chip drivers.

#obj-$(CONFIG_HDMI_Sii902xA)	+= SiIIIC.o
obj-$(CONFIG_HDMI_Sii902xA)	+= siHdmiTx_902x_TPI.o	#zxr
obj-$(CONFIG_HDMI_Sii902xA)	+= hdmi_sii902xA.o		#zxr
#obj-$(CONFIG_HDMI_Sii902xA)	+= hdmi_EDID.o

#ccflags-$(CONFIG_I2C_DEBUG_BUS) := -DDEBUG



ifeq ($(PARAM_FILE), )
	PARAM_FILE:=../../Makefile.param
		include $(PARAM_FILE)
endif
obj-m += siHdmiTx_902x_TPI.o
obj-m += hdmi_sii902xA.o

EXTRA_CFLAGS += -Wno-date-time  

ifeq ($(HIARCH),hi3531d)
EXTRA_CFLAGS += -DHI_CHIP_HI3531D
EXTRA_CFLAGS += -DI2C_INTERNAL
EXTRA_CFLAGS += -I$(REL_INC)
EXTRA_CFLAGS += $(DRV_CFLAGS)
endif

EXTRA_CFLAGS += -D__DEC_HIDE_SHOW_FUNCTION

default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules 

ifeq ($(HIARCH),hi3531d)
	cp *.ko $(REL_KO)/extdrv/
endif
clean: 
	@make -C $(LINUX_ROOT) M=$(PWD) clean 


