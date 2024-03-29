/* Copyright (c) 2011-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/i2c/smb349.h>
#include <linux/i2c/sx150x.h>
#include <linux/slimbus/slimbus.h>
#include <linux/mfd/wcd9xxx/core.h>
#include <linux/mfd/wcd9xxx/pdata.h>
#include <linux/mfd/pm8xxx/misc.h>
#include <linux/msm_ssbi.h>
#include <linux/spi/spi.h>
#include <linux/dma-contiguous.h>
#include <linux/dma-mapping.h>
#include <linux/platform_data/qcom_crypto_device.h>
#include <linux/msm_ion.h>
#include <linux/memory.h>
#include <linux/memblock.h>
#include <linux/msm_thermal.h>
#include <linux/i2c/atmel_mxt_ts.h>
#include <linux/cyttsp-qc.h>
#include <linux/i2c/isa1200.h>
#include <linux/gpio_keys.h>
#include <linux/epm_adc.h>
#include <linux/i2c/sx150x.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/hardware/gic.h>
#include <asm/mach/mmc.h>
#include <linux/platform_data/qcom_wcnss_device.h>

#include <mach/board.h>
#include <mach/msm_iomap.h>
#include <mach/ion.h>
#include <linux/usb/msm_hsusb.h>
#include <linux/usb/android.h>
#include <mach/socinfo.h>
#include <mach/msm_spi.h>
#include "timer.h"
#include "devices.h"
#include <mach/gpiomux.h>
#include <mach/rpm.h>
#ifdef CONFIG_ANDROID_PMEM
#include <linux/android_pmem.h>
#endif
#include <mach/msm_memtypes.h>
#include <linux/bootmem.h>
#include <asm/setup.h>
#include <mach/dma.h>
#include <mach/msm_dsps.h>
#include <mach/msm_bus_board.h>
#include <mach/cpuidle.h>
#include <mach/mdm2.h>
#include <linux/msm_tsens.h>
#include <mach/msm_xo.h>
#include <mach/msm_rtb.h>
#include <sound/cs8427.h>
#include <media/gpio-ir-recv.h>
#include <linux/fmem.h>
#include <mach/msm_pcie.h>
#include <mach/restart.h>
#include <mach/msm_iomap.h>
#include <mach/msm_serial_hs.h>
#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2013-02-20 yuyi Add begin nfc*/
#include <linux/nfc/pn544.h>
#include <linux/regulator/consumer.h>
/* OPPO 2013-02-20 yuyi Add end */
#endif
#include <linux/persistent_ram.h>
/* OPPO 2013-07-31 Neal Add begin bkl*/
#include <linux/pcb_version.h>

extern int get_pcb_version(void);
/* OPPO 2013-07-31 Neal Add begin bkl*/

#include "msm_watchdog.h"
#include "board-8064.h"
#include "clock.h"
#include "spm.h"
#include <mach/mpm.h>
#include "rpm_resources.h"
#include "pm.h"
#include "pm-boot.h"
#include "devices-msm8x60.h"
#include "smd_private.h"
#include "sysmon.h"
/* OPPO 2012-07-31 liujun Add begin for touchscreen */
#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_S3202_I2C_RMI
#include <linux/synaptics_i2c_rmi.h>
#endif
/* OPPO 2012-07-31 liujun Add end */
// oppo yanghai add
#ifdef CONFIG_VENDOR_EDIT
#include <linux/y8c20x66a_i2c_rmi.h>
#endif
//oppo yanghai add end
#define MSM_PMEM_ADSP_SIZE         0x7800000
#define MSM_PMEM_AUDIO_SIZE        0x4CF000
#ifdef CONFIG_FB_MSM_HDMI_AS_PRIMARY
#define MSM_PMEM_SIZE 0x4000000 /* 64 Mbytes */
#else
#define MSM_PMEM_SIZE 0x4000000 /* 64 Mbytes */
#endif

#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
#define HOLE_SIZE		0x20000
#define MSM_ION_MFC_META_SIZE  0x40000 /* 256 Kbytes */
#define MSM_CONTIG_MEM_SIZE  0x65000
#ifdef CONFIG_MSM_IOMMU
#define MSM_ION_MM_SIZE		0x3800000
#define MSM_ION_SF_SIZE		0
#define MSM_ION_QSECOM_SIZE	0x780000 /* (7.5MB) */
#define MSM_ION_HEAP_NUM	8
#else
#define MSM_ION_MM_SIZE		MSM_PMEM_ADSP_SIZE
#define MSM_ION_SF_SIZE		MSM_PMEM_SIZE
#define MSM_ION_QSECOM_SIZE	0x600000 /* (6MB) */
#define MSM_ION_HEAP_NUM	8
#endif
#define MSM_ION_MM_FW_SIZE	(0x200000 - HOLE_SIZE) /* (2MB - 128KB) */
#define MSM_ION_MFC_SIZE	(SZ_8K + MSM_ION_MFC_META_SIZE)
#define MSM_ION_AUDIO_SIZE	MSM_PMEM_AUDIO_SIZE
#else
#define MSM_CONTIG_MEM_SIZE  0x110C000
#define MSM_ION_HEAP_NUM	1
#endif

#define APQ8064_FIXED_AREA_START (0xa0000000 - (MSM_ION_MM_FW_SIZE + \
							HOLE_SIZE))
#define MAX_FIXED_AREA_SIZE	0x10000000
#define MSM_MM_FW_SIZE		(0x200000 - HOLE_SIZE)
#define APQ8064_FW_START	APQ8064_FIXED_AREA_START
#define MSM_ION_ADSP_SIZE	SZ_8M

#define QFPROM_RAW_FEAT_CONFIG_ROW0_MSB     (MSM_QFPROM_BASE + 0x23c)
#define QFPROM_RAW_OEM_CONFIG_ROW0_LSB      (MSM_QFPROM_BASE + 0x220)

/* PCIE AXI address space */
#define PCIE_AXI_BAR_PHYS   0x08000000
#define PCIE_AXI_BAR_SIZE   SZ_128M

/* PCIe pmic gpios */
#define PCIE_WAKE_N_PMIC_GPIO 12
#define PCIE_PWR_EN_PMIC_GPIO 13
#define PCIE_RST_N_PMIC_MPP 1

//#ifdef VENDOR_EDIT
//WuJinping@OnlineRD.AirService.Phone 2013.1.7, Add for modem subsystem restart not need pin
static struct kobject *modeminfo_kobj;
//#endif /* VENDOR_EDIT */

/* OPPO 2012-09-12 Van Modify begin for factory mode*/
static struct kobject *systeminfo_kobj;

enum{
	MSM_BOOT_MODE__NORMAL,
	MSM_BOOT_MODE__FASTBOOT,
	MSM_BOOT_MODE__RECOVERY,
	MSM_BOOT_MODE__FACTORY,
	MSM_BOOT_MODE__RF,
	MSM_BOOT_MODE__WLAN,
	MSM_BOOT_MODE__CHARGE,
};

static int ftm_mode = 0;

int __init board_mfg_mode_init(char *s)
{
	if (!strcmp(s, "normal"))
		ftm_mode = MSM_BOOT_MODE__NORMAL;
	else if (!strcmp(s, "factory2"))
		ftm_mode = MSM_BOOT_MODE__FACTORY;
/* OPPO 2013-09-18 gousj Modify begin for recovery mode  */
#ifndef VENDOR_EDIT
	else if (!strcmp(s, "recovery"))
#else
	else if (!strcmp(s, "ftmrecovery"))
#endif
/* OPPO 2013-09-18 gousj Modify end */
		ftm_mode = MSM_BOOT_MODE__RECOVERY;
	else if (!strcmp(s, "charge"))
		ftm_mode = MSM_BOOT_MODE__CHARGE;
	else if (!strcmp(s, "ftmwifi"))
		ftm_mode = MSM_BOOT_MODE__WLAN;
	else if (!strcmp(s, "ftmrf"))
		ftm_mode = MSM_BOOT_MODE__RF;
	else 
		ftm_mode = MSM_BOOT_MODE__NORMAL;

	//pr_err("board_mfg_mode_init" "ftm_mode=%d\n", ftm_mode);
	
	return 0;

}
__setup("oppo_ftm_mode=", board_mfg_mode_init);

int get_boot_mode(void)
{
	return ftm_mode;
}

static ssize_t ftmmode_show(struct kobject *kobj, struct kobj_attribute *attr,
			     char *buf)
{
	return sprintf(buf, "%d\n", ftm_mode);
}

struct kobj_attribute ftmmode_attr = {
  .attr = {"ftmmode", 0644},

    .show = &ftmmode_show,
};

/* OPPO 2013-01-04 Van add start for ftm close modem*/
#define mdm_drv_ap2mdm_pmic_pwr_en_gpio  27

static ssize_t closemodem_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	//writing '1' to close and '0' to open
	//pr_err("closemodem buf[0] = 0x%x",buf[0]);
	switch (buf[0]) {
	case 0x30:
		break;
	case 0x31:
	//	pr_err("closemodem now");
		gpio_direction_output(mdm_drv_ap2mdm_pmic_pwr_en_gpio, 0);
		mdelay(4000);
		break;
	default:
		break;
	}

	return count;
}


struct kobj_attribute closemodem_attr = {
  .attr = {"closemodem", 0644},
  //.show = &closemodem_show,
  .store = &closemodem_store
};
/* OPPO 2013-01-04 Van add end for ftm close modem*/
/* OPPO 2012-09-04 liujun Add begin for create pcb version attribute */
#include <linux/pcb_version.h>
static char *saved_command_line_pcb_version;
static int  current_pcb_version_num = PCB_VERSION_UNKNOWN;

int get_pcb_version(void)
{
	return current_pcb_version_num;
}
EXPORT_SYMBOL(get_pcb_version);

int __init board_pcb_verson_init(char *s)
{
	saved_command_line_pcb_version = s;

	if (!strcmp(s, "evb"))
		current_pcb_version_num = PCB_VERSION_EVB;
	else if (!strcmp(s, "evt"))
		current_pcb_version_num = PCB_VERSION_EVT;
	else if (!strcmp(s, "dvt"))
		current_pcb_version_num = PCB_VERSION_DVT;
	else if (!strcmp(s, "pvt"))
		current_pcb_version_num = PCB_VERSION_PVT;
	else if (!strcmp(s, "td_evb"))
		current_pcb_version_num = PCB_VERSION_EVB_TD;
	else if (!strcmp(s, "td_pvt2"))
		current_pcb_version_num = PCB_VERSION_PVT2_TD;
	else if (!strcmp(s, "td_pvt3"))
		current_pcb_version_num = PCB_VERSION_PVT3_TD;
	/*OPPO 2013-08-23 zhangpan add pcb version for N1*/
	else if (!strcmp(s, "n1t_evt_td"))
		current_pcb_version_num = PCB_VERSION_EVT_N1;
	else if (!strcmp(s, "n1f_evt"))
		current_pcb_version_num = PCB_VERSION_EVT_N1F;
	else if (!strcmp(s, "n1f_evt3"))
		current_pcb_version_num = PCB_VERSION_EVT3_N1F;
	else if (!strcmp(s, "n1f_dvt"))
		current_pcb_version_num = PCB_VERSION_DVT_N1F;
	else if (!strcmp(s, "n1f_pvt"))
		current_pcb_version_num = PCB_VERSION_PVT_N1F;
	else if (!strcmp(s, "n1t_evt3_td"))
		current_pcb_version_num = PCB_VERSION_EVT3_N1T;
	else if (!strcmp(s, "n1t_dvt_td"))
		current_pcb_version_num = PCB_VERSION_DVT_N1T;
	else if (!strcmp(s, "n1t_pvt_td"))
		current_pcb_version_num = PCB_VERSION_PVT_N1T;
	else if (!strcmp(s, "n1w_evt"))
			current_pcb_version_num = PCB_VERSION_EVT_N1W;
	else if (!strcmp(s, "n1w_dvt"))
			current_pcb_version_num = PCB_VERSION_DVT_N1W;
	else if (!strcmp(s, "n1w_pvt"))
			current_pcb_version_num = PCB_VERSION_PVT_N1W;
	/*OPPO 2013-08-23 zhangpan add end*/

	return 0;
}
__setup("oppo.pcb_version=", board_pcb_verson_init);
/* OPPO 2012-09-04 liujun Add end */
static struct attribute * g[] = {
	&ftmmode_attr.attr,
/* OPPO 2013-01-04 Van add start for ftm close modem*/
	&closemodem_attr.attr,
/* OPPO 2013-01-04 Van add end for ftm close modem*/


	NULL,
};

static struct attribute_group attr_group = {
	.attrs = g,
};
/* OPPO 2013-01-04 Van Modify end for boot modes*/

//#ifdef VENDOR_EDIT
//WuJinping@OnlineRD.AirService.Phone 2013.1.7, Add for modem subsystem restart not need pin
extern int get_modem_reset_num(void);
//#ifdef VENDOR_EDIT

static char pin_info[64] = {0}; 
static int modem_reset_count = 0;
static int need_pin_process_flag = -1;
static int sim_status = -1;

int get_sim_status(void)
{
	return gpio_get_value(72);
}

void set_need_pin_process_flag(int flag)
{
	need_pin_process_flag = flag;
}


static ssize_t pininfo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf){
    return snprintf(buf, 4096, "%s\n", pin_info);
}

static ssize_t pininfo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count){

	printk("pininfo_store----count:%d   wjp debug     \n", count);
	if(count >= sizeof(pin_info))
		count = sizeof(pin_info) - 1;
	
	strncpy(pin_info, buf, count);
	pin_info[count] = '\0';
	return count;
}

static ssize_t modem_reset_count_show(struct kobject *kobj, struct kobj_attribute *attr,
			     char *buf)
{
	modem_reset_count = get_modem_reset_num();
	return sprintf(buf, "%d\n", modem_reset_count);
}

static ssize_t modem_reset_count_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count){
	char *after;
	unsigned long reset_count = simple_strtoul(buf, &after, 10);			 
	modem_reset_count = (int)reset_count;
	return count;
}

static ssize_t need_pin_process_flag_show(struct kobject *kobj, struct kobj_attribute *attr,
			     char *buf)
{
	return sprintf(buf, "%d\n", need_pin_process_flag);
}

static ssize_t need_pin_process_flag_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count){
	char *after;
	unsigned long flag = simple_strtoul(buf, &after, 10);
	need_pin_process_flag = (int)flag;
	return count;
}

static ssize_t sim_status_show(struct kobject *kobj, struct kobj_attribute *attr,
			     char *buf)
{
      sim_status = gpio_get_value(72);
	return sprintf(buf, "%d\n", sim_status);
}

static ssize_t sim_status_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count){
	char *after;
	unsigned long status = simple_strtoul(buf, &after, 10);
	sim_status = (int)status;
	
	return count;
}


struct kobj_attribute pininfo_attr = {
/* OPPO 2013-08-16 zhaokai Add begin for CTS android.permission test*/
#ifdef OPPO_CTS
.attr = {"pin_info", 0660},
#else
.attr = {"pin_info", 0777},
#endif
/* OPPO 2013-08-16 zhaokai Add end for CTS android.permission test*/
    
    .show = &pininfo_show,
    .store = &pininfo_store
};


struct kobj_attribute modem_reset_count_attr = {
/* OPPO 2013-08-16 zhaokai Add begin for CTS android.permission test*/
#ifdef OPPO_CTS
.attr = {"modem_reset_count", 0660},
#else
.attr = {"modem_reset_count", 0777},
#endif
/* OPPO 2013-08-16 zhaokai Add end for CTS android.permission test*/
    
	
    .show = &modem_reset_count_show,
    .store = &modem_reset_count_store,
};

struct kobj_attribute need_pin_process_flag_attr = {
/* OPPO 2013-08-16 zhaokai Add begin for CTS android.permission test*/
#ifdef OPPO_CTS
 .attr = {"need_pin_process_flag", 0660},
#else
 .attr = {"need_pin_process_flag", 0777},
#endif
/* OPPO 2013-08-16 zhaokai Add end for CTS android.permission test*/
   
	
    .show = &need_pin_process_flag_show,
    .store = &need_pin_process_flag_store,
};

struct kobj_attribute sim_status_attr = {
/* OPPO 2013-08-16 zhaokai Add begin for CTS android.permission test*/
#ifdef OPPO_CTS
.attr = {"sim_status", 0660},
#else
.attr = {"sim_status", 0777},
#endif
/* OPPO 2013-08-16 zhaokai Add end for CTS android.permission test*/
    
    .show = &sim_status_show,
    .store = &sim_status_store,
};


static struct attribute * modeminfo_attr[] = {
	&sim_status_attr.attr,
	&need_pin_process_flag_attr.attr,
	&modem_reset_count_attr.attr,
	&pininfo_attr.attr,
	
	NULL,
};

static struct attribute_group modeminfo_attr_group = {
	.attrs = modeminfo_attr,
};
//#endif /* VENDOR_EDIT */
#ifdef CONFIG_KERNEL_MSM_CONTIG_MEM_REGION
static unsigned msm_contig_mem_size = MSM_CONTIG_MEM_SIZE;
static int __init msm_contig_mem_size_setup(char *p)
{
	msm_contig_mem_size = memparse(p, NULL);
	return 0;
}
early_param("msm_contig_mem_size", msm_contig_mem_size_setup);
#endif

#ifdef CONFIG_ANDROID_PMEM
static unsigned pmem_size = MSM_PMEM_SIZE;
static int __init pmem_size_setup(char *p)
{
	pmem_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_size", pmem_size_setup);

static unsigned pmem_adsp_size = MSM_PMEM_ADSP_SIZE;

static int __init pmem_adsp_size_setup(char *p)
{
	pmem_adsp_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_adsp_size", pmem_adsp_size_setup);

static unsigned pmem_audio_size = MSM_PMEM_AUDIO_SIZE;

static int __init pmem_audio_size_setup(char *p)
{
	pmem_audio_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_audio_size", pmem_audio_size_setup);
#endif

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct android_pmem_platform_data android_pmem_pdata = {
	.name = "pmem",
	.allocator_type = PMEM_ALLOCATORTYPE_ALLORNOTHING,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device apq8064_android_pmem_device = {
	.name = "android_pmem",
	.id = 0,
	.dev = {.platform_data = &android_pmem_pdata},
};

static struct android_pmem_platform_data android_pmem_adsp_pdata = {
	.name = "pmem_adsp",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};
static struct platform_device apq8064_android_pmem_adsp_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &android_pmem_adsp_pdata },
};

static struct android_pmem_platform_data android_pmem_audio_pdata = {
	.name = "pmem_audio",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device apq8064_android_pmem_audio_device = {
	.name = "android_pmem",
	.id = 4,
	.dev = { .platform_data = &android_pmem_audio_pdata },
};
#endif /* CONFIG_MSM_MULTIMEDIA_USE_ION */
#endif /* CONFIG_ANDROID_PMEM */

struct fmem_platform_data apq8064_fmem_pdata = {
};

static struct memtype_reserve apq8064_reserve_table[] __initdata = {
	[MEMTYPE_SMI] = {
	},
	[MEMTYPE_EBI0] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
	[MEMTYPE_EBI1] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
};

static void __init reserve_rtb_memory(void)
{
#if defined(CONFIG_MSM_RTB)
/* OPPO 2003-01-17 Van modified begin for FTM test no need rtb_memory */
	if (!strstr(boot_command_line,"oppo_ftm_mode=factory2"))
		apq8064_reserve_table[MEMTYPE_EBI1].size += apq8064_rtb_pdata.size;
/* OPPO 2003-01-17 Van modified end for FTM test no need rtb_memory */
#endif
}


static void __init size_pmem_devices(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	android_pmem_adsp_pdata.size = pmem_adsp_size;
	android_pmem_pdata.size = pmem_size;
	android_pmem_audio_pdata.size = MSM_PMEM_AUDIO_SIZE;
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/
}

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static void __init reserve_memory_for(struct android_pmem_platform_data *p)
{
	apq8064_reserve_table[p->memory_type].size += p->size;
}
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/

static void __init reserve_pmem_memory(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	reserve_memory_for(&android_pmem_adsp_pdata);
	reserve_memory_for(&android_pmem_pdata);
	reserve_memory_for(&android_pmem_audio_pdata);
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
	apq8064_reserve_table[MEMTYPE_EBI1].size += msm_contig_mem_size;
#endif /*CONFIG_ANDROID_PMEM*/
}

static int apq8064_paddr_to_memtype(unsigned int paddr)
{
	return MEMTYPE_EBI1;
}

#define FMEM_ENABLED 0

#ifdef CONFIG_ION_MSM
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct ion_cp_heap_pdata cp_mm_apq8064_ion_pdata = {
	.permission_type = IPT_TYPE_MM_CARVEOUT,
	.align = PAGE_SIZE,
	.reusable = FMEM_ENABLED,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_MIDDLE,
	.is_cma = 1,
	.no_nonsecure_alloc = 1,
};

static struct ion_cp_heap_pdata cp_mfc_apq8064_ion_pdata = {
	.permission_type = IPT_TYPE_MFC_SHAREDMEM,
	.align = PAGE_SIZE,
	.reusable = 0,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_HIGH,
	.no_nonsecure_alloc = 1,
};

static struct ion_co_heap_pdata co_apq8064_ion_pdata = {
	.adjacent_mem_id = INVALID_HEAP_ID,
	.align = PAGE_SIZE,
	.mem_is_fmem = 0,
};

static struct ion_co_heap_pdata fw_co_apq8064_ion_pdata = {
	.adjacent_mem_id = ION_CP_MM_HEAP_ID,
	.align = SZ_128K,
	.mem_is_fmem = FMEM_ENABLED,
	.fixed_position = FIXED_LOW,
};
#endif

static u64 msm_dmamask = DMA_BIT_MASK(32);

static struct platform_device ion_mm_heap_device = {
	.name = "ion-mm-heap-device",
	.id = -1,
	.dev = {
		.dma_mask = &msm_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	}
};

static struct platform_device ion_adsp_heap_device = {
	.name = "ion-adsp-heap-device",
	.id = -1,
	.dev = {
		.dma_mask = &msm_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	}
};
/**
 * These heaps are listed in the order they will be allocated. Due to
 * video hardware restrictions and content protection the FW heap has to
 * be allocated adjacent (below) the MM heap and the MFC heap has to be
 * allocated after the MM heap to ensure MFC heap is not more than 256MB
 * away from the base address of the FW heap.
 * However, the order of FW heap and MM heap doesn't matter since these
 * two heaps are taken care of by separate code to ensure they are adjacent
 * to each other.
 * Don't swap the order unless you know what you are doing!
 */
struct ion_platform_heap apq8064_heaps[] = {
		{
			.id	= ION_SYSTEM_HEAP_ID,
			.type	= ION_HEAP_TYPE_SYSTEM,
			.name	= ION_VMALLOC_HEAP_NAME,
		},
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
		{
			.id	= ION_CP_MM_HEAP_ID,
			.type	= ION_HEAP_TYPE_CP,
			.name	= ION_MM_HEAP_NAME,
			.size	= MSM_ION_MM_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &cp_mm_apq8064_ion_pdata,
			.priv	= &ion_mm_heap_device.dev
		},
		{
			.id	= ION_MM_FIRMWARE_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_MM_FIRMWARE_HEAP_NAME,
			.size	= MSM_ION_MM_FW_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &fw_co_apq8064_ion_pdata,
		},
		{
			.id	= ION_CP_MFC_HEAP_ID,
			.type	= ION_HEAP_TYPE_CP,
			.name	= ION_MFC_HEAP_NAME,
			.size	= MSM_ION_MFC_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &cp_mfc_apq8064_ion_pdata,
		},
#ifndef CONFIG_MSM_IOMMU
		{
			.id	= ION_SF_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_SF_HEAP_NAME,
			.size	= MSM_ION_SF_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_apq8064_ion_pdata,
		},
#endif
		{
			.id	= ION_IOMMU_HEAP_ID,
			.type	= ION_HEAP_TYPE_IOMMU,
			.name	= ION_IOMMU_HEAP_NAME,
		},
		{
			.id	= ION_QSECOM_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_QSECOM_HEAP_NAME,
			.size	= MSM_ION_QSECOM_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_apq8064_ion_pdata,
		},
		{
			.id	= ION_AUDIO_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_AUDIO_HEAP_NAME,
			.size	= MSM_ION_AUDIO_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_apq8064_ion_pdata,
		},
		{
			.id     = ION_ADSP_HEAP_ID,
			.type   = ION_HEAP_TYPE_DMA,
			.name   = ION_ADSP_HEAP_NAME,
			.size   = MSM_ION_ADSP_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_apq8064_ion_pdata,
			.priv = &ion_adsp_heap_device.dev,
		},
#endif
};

static struct ion_platform_data apq8064_ion_pdata = {
	.nr = MSM_ION_HEAP_NUM,
	.heaps = apq8064_heaps,
};

static struct platform_device apq8064_ion_dev = {
	.name = "ion-msm",
	.id = 1,
	.dev = { .platform_data = &apq8064_ion_pdata },
};
#endif

static struct platform_device apq8064_fmem_device = {
	.name = "fmem",
	.id = 1,
	.dev = { .platform_data = &apq8064_fmem_pdata },
};

static void __init reserve_mem_for_ion(enum ion_memory_types mem_type,
				      unsigned long size)
{
	apq8064_reserve_table[mem_type].size += size;
}

static void __init apq8064_reserve_fixed_area(unsigned long fixed_area_size)
{
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	int ret;

	if (fixed_area_size > MAX_FIXED_AREA_SIZE)
		panic("fixed area size is larger than %dM\n",
			MAX_FIXED_AREA_SIZE >> 20);

	reserve_info->fixed_area_size = fixed_area_size;
	reserve_info->fixed_area_start = APQ8064_FW_START;

	ret = memblock_remove(reserve_info->fixed_area_start,
		reserve_info->fixed_area_size);
	BUG_ON(ret);
#endif
}

/**
 * Reserve memory for ION and calculate amount of reusable memory for fmem.
 * We only reserve memory for heaps that are not reusable. However, we only
 * support one reusable heap at the moment so we ignore the reusable flag for
 * other than the first heap with reusable flag set. Also handle special case
 * for video heaps (MM,FW, and MFC). Video requires heaps MM and MFC to be
 * at a higher address than FW in addition to not more than 256MB away from the
 * base address of the firmware. This means that if MM is reusable the other
 * two heaps must be allocated in the same region as FW. This is handled by the
 * mem_is_fmem flag in the platform data. In addition the MM heap must be
 * adjacent to the FW heap for content protection purposes.
 */
static void __init reserve_ion_memory(void)
{
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	unsigned int i;
	unsigned int ret;
	unsigned int fixed_size = 0;
	unsigned int fixed_low_size, fixed_middle_size, fixed_high_size;
	unsigned long fixed_low_start, fixed_middle_start, fixed_high_start;
	unsigned long cma_alignment;
	unsigned int low_use_cma = 0;
	unsigned int middle_use_cma = 0;
	unsigned int high_use_cma = 0;


	fixed_low_size = 0;
	fixed_middle_size = 0;
	fixed_high_size = 0;

	cma_alignment = PAGE_SIZE << max(MAX_ORDER, pageblock_order);

	for (i = 0; i < apq8064_ion_pdata.nr; ++i) {
		struct ion_platform_heap *heap =
			&(apq8064_ion_pdata.heaps[i]);
		int use_cma = 0;


		if (heap->extra_data) {
			int fixed_position = NOT_FIXED;

			switch ((int)heap->type) {
			case ION_HEAP_TYPE_CP:
				if (((struct ion_cp_heap_pdata *)
					heap->extra_data)->is_cma) {
					heap->size = ALIGN(heap->size,
						cma_alignment);
					use_cma = 1;
				}
				fixed_position = ((struct ion_cp_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			case ION_HEAP_TYPE_DMA:
				use_cma = 1;
				/* Purposely fall through here */
			case ION_HEAP_TYPE_CARVEOUT:
				fixed_position = ((struct ion_co_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			default:
				break;
			}

			if (fixed_position != NOT_FIXED)
				fixed_size += heap->size;
			else
				reserve_mem_for_ion(MEMTYPE_EBI1, heap->size);

			if (fixed_position == FIXED_LOW) {
				fixed_low_size += heap->size;
				low_use_cma = use_cma;
			} else if (fixed_position == FIXED_MIDDLE) {
				fixed_middle_size += heap->size;
				middle_use_cma = use_cma;
			} else if (fixed_position == FIXED_HIGH) {
				fixed_high_size += heap->size;
				high_use_cma = use_cma;
			} else if (use_cma) {
				/*
				 * Heaps that use CMA but are not part of the
				 * fixed set. Create wherever.
				 */
				dma_declare_contiguous(
					heap->priv,
					heap->size,
					0,
					0xb0000000);

			}
		}
	}

	if (!fixed_size)
		return;

	/*
	 * Given the setup for the fixed area, we can't round up all sizes.
	 * Some sizes must be set up exactly and aligned correctly. Incorrect
	 * alignments are considered a configuration issue
	 */

	fixed_low_start = APQ8064_FIXED_AREA_START;
	if (low_use_cma) {
		BUG_ON(!IS_ALIGNED(fixed_low_size + HOLE_SIZE, cma_alignment));
		BUG_ON(!IS_ALIGNED(fixed_low_start, cma_alignment));
	} else {
		BUG_ON(!IS_ALIGNED(fixed_low_size + HOLE_SIZE, SECTION_SIZE));
		ret = memblock_remove(fixed_low_start,
				      fixed_low_size + HOLE_SIZE);
		BUG_ON(ret);
	}

	fixed_middle_start = fixed_low_start + fixed_low_size + HOLE_SIZE;
	if (middle_use_cma) {
		BUG_ON(!IS_ALIGNED(fixed_middle_start, cma_alignment));
		BUG_ON(!IS_ALIGNED(fixed_middle_size, cma_alignment));
	} else {
		BUG_ON(!IS_ALIGNED(fixed_middle_size, SECTION_SIZE));
		ret = memblock_remove(fixed_middle_start, fixed_middle_size);
		BUG_ON(ret);
	}

	fixed_high_start = fixed_middle_start + fixed_middle_size;
	if (high_use_cma) {
		fixed_high_size = ALIGN(fixed_high_size, cma_alignment);
		BUG_ON(!IS_ALIGNED(fixed_high_start, cma_alignment));
	} else {
		/* This is the end of the fixed area so it's okay to round up */
		fixed_high_size = ALIGN(fixed_high_size, SECTION_SIZE);
		ret = memblock_remove(fixed_high_start, fixed_high_size);
		BUG_ON(ret);
	}

	for (i = 0; i < apq8064_ion_pdata.nr; ++i) {
		struct ion_platform_heap *heap = &(apq8064_ion_pdata.heaps[i]);

		if (heap->extra_data) {
			int fixed_position = NOT_FIXED;
			struct ion_cp_heap_pdata *pdata = NULL;

			switch ((int) heap->type) {
			case ION_HEAP_TYPE_CP:
				pdata =
				(struct ion_cp_heap_pdata *)heap->extra_data;
				fixed_position = pdata->fixed_position;
				break;
			case ION_HEAP_TYPE_CARVEOUT:
			case ION_HEAP_TYPE_DMA:
				fixed_position = ((struct ion_co_heap_pdata *)
					heap->extra_data)->fixed_position;
				break;
			default:
				break;
			}

			switch (fixed_position) {
			case FIXED_LOW:
				heap->base = fixed_low_start;
				break;
			case FIXED_MIDDLE:
				heap->base = fixed_middle_start;
				if (middle_use_cma) {
					ret = dma_declare_contiguous(
						heap->priv,
						heap->size,
						fixed_middle_start,
						0xa0000000);
					WARN_ON(ret);
				}
				pdata->secure_base = fixed_middle_start
								- HOLE_SIZE;
				pdata->secure_size = HOLE_SIZE + heap->size;
				break;
			case FIXED_HIGH:
				heap->base = fixed_high_start;
				break;
			default:
				break;
			}
		}
	}
#endif
}

static void __init reserve_mdp_memory(void)
{
	apq8064_mdp_writeback(apq8064_reserve_table);
}

static void __init reserve_cache_dump_memory(void)
{
#ifdef CONFIG_MSM_CACHE_DUMP
	unsigned int total;

	total = apq8064_cache_dump_pdata.l1_size +
		apq8064_cache_dump_pdata.l2_size;
	apq8064_reserve_table[MEMTYPE_EBI1].size += total;
#endif
}

static void __init reserve_mpdcvs_memory(void)
{
	apq8064_reserve_table[MEMTYPE_EBI1].size += SZ_32K;
}

static void __init apq8064_calculate_reserve_sizes(void)
{
	size_pmem_devices();
	reserve_pmem_memory();
	reserve_ion_memory();
	reserve_mdp_memory();
	reserve_rtb_memory();
	reserve_cache_dump_memory();
	reserve_mpdcvs_memory();
}

static struct reserve_info apq8064_reserve_info __initdata = {
	.memtype_reserve_table = apq8064_reserve_table,
	.calculate_reserve_sizes = apq8064_calculate_reserve_sizes,
	.reserve_fixed_area = apq8064_reserve_fixed_area,
	.paddr_to_memtype = apq8064_paddr_to_memtype,
};

static char prim_panel_name[PANEL_NAME_MAX_LEN];
static char ext_panel_name[PANEL_NAME_MAX_LEN];

static int ext_resolution;

static int __init prim_display_setup(char *param)
{
	if (strnlen(param, PANEL_NAME_MAX_LEN))
		strlcpy(prim_panel_name, param, PANEL_NAME_MAX_LEN);
	return 0;
}
early_param("prim_display", prim_display_setup);

static int __init ext_display_setup(char *param)
{
	if (strnlen(param, PANEL_NAME_MAX_LEN))
		strlcpy(ext_panel_name, param, PANEL_NAME_MAX_LEN);
	return 0;
}
early_param("ext_display", ext_display_setup);

static int __init hdmi_resulution_setup(char *param)
{
	int ret;
	ret = kstrtoint(param, 10, &ext_resolution);
	return ret;
}
early_param("ext_resolution", hdmi_resulution_setup);

static void __init apq8064_reserve(void)
{
	apq8064_set_display_params(prim_panel_name, ext_panel_name,
		ext_resolution);
	msm_reserve();
}

static void __init apq8064_early_reserve(void)
{
	reserve_info = &apq8064_reserve_info;
}
#ifdef CONFIG_USB_EHCI_MSM_HSIC
/* Bandwidth requests (zero) if no vote placed */
static struct msm_bus_vectors hsic_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ab = 0,
		.ib = 0,
	},
};

/* Bus bandwidth requests in Bytes/sec */
static struct msm_bus_vectors hsic_max_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_SPS,
		.ab = 0,
		.ib = 256000000, /*vote for 32Mhz dfab clk rate*/
	},
};

static struct msm_bus_paths hsic_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(hsic_init_vectors),
		hsic_init_vectors,
	},
	{
		ARRAY_SIZE(hsic_max_vectors),
		hsic_max_vectors,
	},
};

static struct msm_bus_scale_pdata hsic_bus_scale_pdata = {
	hsic_bus_scale_usecases,
	ARRAY_SIZE(hsic_bus_scale_usecases),
	.name = "hsic",
};

static struct msm_hsic_host_platform_data msm_hsic_pdata = {
	.strobe			= 88,
	.data			= 89,
	.bus_scale_table	= &hsic_bus_scale_pdata,
};
#else
static struct msm_hsic_host_platform_data msm_hsic_pdata;
#endif

#define PID_MAGIC_ID		0x71432909
#define SERIAL_NUM_MAGIC_ID	0x61945374
#define SERIAL_NUMBER_LENGTH	127
#define DLOAD_USB_BASE_ADD	0x2A03F0C8

struct magic_num_struct {
	uint32_t pid;
	uint32_t serial_num;
};

struct dload_struct {
	uint32_t	reserved1;
	uint32_t	reserved2;
	uint32_t	reserved3;
	uint16_t	reserved4;
	uint16_t	pid;
	char		serial_number[SERIAL_NUMBER_LENGTH];
	uint16_t	reserved5;
	struct magic_num_struct magic_struct;
};

static int usb_diag_update_pid_and_serial_num(uint32_t pid, const char *snum)
{
	struct dload_struct __iomem *dload = 0;

	dload = ioremap(DLOAD_USB_BASE_ADD, sizeof(*dload));
	if (!dload) {
		pr_err("%s: cannot remap I/O memory region: %08x\n",
					__func__, DLOAD_USB_BASE_ADD);
		return -ENXIO;
	}

	pr_debug("%s: dload:%p pid:%x serial_num:%s\n",
				__func__, dload, pid, snum);
	/* update pid */
	dload->magic_struct.pid = PID_MAGIC_ID;
	dload->pid = pid;

	/* update serial number */
	dload->magic_struct.serial_num = 0;
	if (!snum) {
		memset(dload->serial_number, 0, SERIAL_NUMBER_LENGTH);
		goto out;
	}

	dload->magic_struct.serial_num = SERIAL_NUM_MAGIC_ID;
	strlcpy(dload->serial_number, snum, SERIAL_NUMBER_LENGTH);
out:
	iounmap(dload);
	return 0;
}

static struct android_usb_platform_data android_usb_pdata = {
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
/*OPPO,Jiangsm add begin for adding cdrom partion,2013-1-10*/
#ifdef CONFIG_VENDOR_EDIT
	.cdrom = true,
#endif
/*OPPO,Jiangsm add end*/
};

static struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id	= -1,
	.dev	= {
		.platform_data = &android_usb_pdata,
	},
};

/* Bandwidth requests (zero) if no vote placed */
static struct msm_bus_vectors usb_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
};

/* Bus bandwidth requests in Bytes/sec */
static struct msm_bus_vectors usb_max_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 60000000,		/* At least 480Mbps on bus. */
		.ib = 960000000,	/* MAX bursts rate */
	},
};

static struct msm_bus_paths usb_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(usb_init_vectors),
		usb_init_vectors,
	},
	{
		ARRAY_SIZE(usb_max_vectors),
		usb_max_vectors,
	},
};

static struct msm_bus_scale_pdata usb_bus_scale_pdata = {
	usb_bus_scale_usecases,
	ARRAY_SIZE(usb_bus_scale_usecases),
	.name = "usb",
};

static int phy_init_seq[] = {
/*OPPO,songxh add begain for USB Eye_Diagram ,2013-11-14*/
#ifndef CONFIG_VENDOR_EDIT	
	0x38, 0x81, /* update DC voltage level */	
	0x24, 0x82, /* set pre-emphasis and rise/fall time */
#else
       0x33, 0x81, /* update DC voltage level */	
       0x34, 0x82, /* set pre-emphasis and rise/fall time */
#endif
/*OPPO,songxh add end for USB Eye_Diagram ,2013-11-14*/
	-1
};

#define PMIC_GPIO_DP		27    /* PMIC GPIO for D+ change */
#define PMIC_GPIO_DP_IRQ	PM8921_GPIO_IRQ(PM8921_IRQ_BASE, PMIC_GPIO_DP)
#define MSM_MPM_PIN_USB1_OTGSESSVLD	40

static struct msm_otg_platform_data msm_otg_pdata = {
	.mode			= USB_OTG,
	.otg_control		= OTG_PMIC_CONTROL,
	.phy_type		= SNPS_28NM_INTEGRATED_PHY,
	.pmic_id_irq		= PM8921_USB_ID_IN_IRQ(PM8921_IRQ_BASE),
	.power_budget		= 750,
	.bus_scale_table	= &usb_bus_scale_pdata,
	.phy_init_seq		= phy_init_seq,
	.mpm_otgsessvld_int	= MSM_MPM_PIN_USB1_OTGSESSVLD,
};

static struct msm_usb_host_platform_data msm_ehci_host_pdata3 = {
	.power_budget = 500,
};

#ifdef CONFIG_USB_EHCI_MSM_HOST4
static struct msm_usb_host_platform_data msm_ehci_host_pdata4;
#endif

static void __init apq8064_ehci_host_init(void)
{
	if (machine_is_apq8064_liquid() || machine_is_mpq8064_cdp() ||
		machine_is_mpq8064_hrd() || machine_is_mpq8064_dtv()) {
		if (machine_is_apq8064_liquid())
			msm_ehci_host_pdata3.dock_connect_irq =
					PM8921_MPP_IRQ(PM8921_IRQ_BASE, 9);
		else
			msm_ehci_host_pdata3.pmic_gpio_dp_irq =
							PMIC_GPIO_DP_IRQ;

		apq8064_device_ehci_host3.dev.platform_data =
				&msm_ehci_host_pdata3;
		platform_device_register(&apq8064_device_ehci_host3);

#ifdef CONFIG_USB_EHCI_MSM_HOST4
		apq8064_device_ehci_host4.dev.platform_data =
				&msm_ehci_host_pdata4;
		platform_device_register(&apq8064_device_ehci_host4);
#endif
	}
}

static struct smb349_platform_data smb349_data __initdata = {
	.en_n_gpio		= PM8921_GPIO_PM_TO_SYS(37),
	.chg_susp_gpio		= PM8921_GPIO_PM_TO_SYS(30),
	.chg_current_ma		= 2200,
};

static struct i2c_board_info smb349_charger_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO(SMB349_NAME, 0x1B),
		.platform_data	= &smb349_data,
	},
};
/* OPPO 2013-07-08 sjc Add begin for smb358 */
#include <linux/power/smb358_charger.h>
static struct smb358_charger_platform_data  smb358_charger_pdata = {
	.irq_gpio = PM8921_GPIO_PM_TO_SYS(29),
	.max_charge_current = 2000,
	.max_charge_voltage = 4500,
};
static struct i2c_board_info smb358_charger_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO(SMB358_CHARGER_NAME, 0x57),
		.platform_data	= &smb358_charger_pdata,
	},
};
//sjc add 2013-07-23
static struct i2c_board_info bq27541_charger_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO("bq27541-gauge", 0x55),
	},
};
/* OPPO 2013-07-08 sjc Add end */

struct sx150x_platform_data apq8064_sx150x_data[] = {
	[SX150X_EPM] = {
		.gpio_base	= GPIO_EPM_EXPANDER_BASE,
		.oscio_is_gpo	= false,
		.io_pullup_ena	= 0x0,
		.io_pulldn_ena	= 0x0,
		.io_open_drain_ena = 0x0,
		.io_polarity	= 0,
		.irq_summary	= -1,
	},
};

static struct epm_chan_properties ads_adc_channel_data[] = {
	{10, 100}, {1000, 1}, {10, 100}, {1000, 1},
	{10, 100}, {1000, 1}, {10, 100}, {1000, 1},
	{10, 100}, {20, 100}, {500, 100}, {5, 100},
	{1000, 1}, {200, 100}, {50, 100}, {10, 100},
	{510, 100}, {50, 100}, {20, 100}, {100, 100},
	{510, 100}, {20, 100}, {50, 100}, {200, 100},
	{10, 100}, {20, 100}, {1000, 1}, {10, 100},
	{200, 100}, {510, 100}, {1000, 100}, {200, 100},
};

static struct epm_adc_platform_data epm_adc_pdata = {
	.channel		= ads_adc_channel_data,
	.bus_id	= 0x0,
	.epm_i2c_board_info = {
		.type	= "sx1509q",
		.addr = 0x3e,
		.platform_data = &apq8064_sx150x_data[SX150X_EPM],
	},
	.gpio_expander_base_addr = GPIO_EPM_EXPANDER_BASE,
};

static struct platform_device epm_adc_device = {
	.name   = "epm_adc",
	.id = -1,
	.dev = {
		.platform_data = &epm_adc_pdata,
	},
};

static void __init apq8064_epm_adc_init(void)
{
	epm_adc_pdata.num_channels = 32;
	epm_adc_pdata.num_adc = 2;
	epm_adc_pdata.chan_per_adc = 16;
	epm_adc_pdata.chan_per_mux = 8;
};

/* Micbias setting is based on 8660 CDP/MTP/FLUID requirement
 * 4 micbiases are used to power various analog and digital
 * microphones operating at 1800 mV. Technically, all micbiases
 * can source from single cfilter since all microphones operate
 * at the same voltage level. The arrangement below is to make
 * sure all cfilters are exercised. LDO_H regulator ouput level
 * does not need to be as high as 2.85V. It is choosen for
 * microphone sensitivity purpose.
 */
static struct wcd9xxx_pdata apq8064_tabla_platform_data = {
	.slimbus_slave_device = {
		.name = "tabla-slave",
		.e_addr = {0, 0, 0x10, 0, 0x17, 2},
	},
	.irq = MSM_GPIO_TO_INT(42),
	.irq_base = TABLA_INTERRUPT_BASE,
	.num_irqs = NR_WCD9XXX_IRQS,
	.reset_gpio = PM8921_GPIO_PM_TO_SYS(34),
	.micbias = {
		.ldoh_v = TABLA_LDOH_2P85_V,
		.cfilt1_mv = 1800,
		/*OPPO 2012-07-27 zhzhyon Modify for micbias voltage*/
		#ifndef CONFIG_VENDOR_EDIT	
		.cfilt2_mv = 2700,
		#else
		.cfilt2_mv = 2000,
		#endif
		/*OPPO 2012-07-27 zhzhyon Modify end*/
		.cfilt3_mv = 1800,
		/*OPPO 2012-07-27 zhzhyon Modify for micbias voltage*/
		#ifndef CONFIG_VENDOR_EDIT
		.bias1_cfilt_sel = TABLA_CFILT1_SEL,
		.bias2_cfilt_sel = TABLA_CFILT2_SEL,
		.bias3_cfilt_sel = TABLA_CFILT3_SEL,
		.bias4_cfilt_sel = TABLA_CFILT3_SEL,
		#else
		.bias1_cfilt_sel = TABLA_CFILT2_SEL,
		.bias2_cfilt_sel = TABLA_CFILT2_SEL,
		.bias3_cfilt_sel = TABLA_CFILT2_SEL,
		.bias4_cfilt_sel = TABLA_CFILT2_SEL,
		#endif
		/*OPPO 2012-07-27 zhzhyon Modify end*/
	},
	.regulator = {
	{
		.name = "CDC_VDD_CP",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_CP_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_RX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_RX_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_TX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_TX_CUR_MAX,
	},
	{
		.name = "VDDIO_CDC",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_VDDIO_CDC_CUR_MAX,
	},
	{
		.name = "VDDD_CDC_D",
		.min_uV = 1225000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_D_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_A_1P2V",
		.min_uV = 1225000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_A_CUR_MAX,
	},
	},
};

static struct slim_device apq8064_slim_tabla = {
	.name = "tabla-slim",
	.e_addr = {0, 1, 0x10, 0, 0x17, 2},
	.dev = {
		.platform_data = &apq8064_tabla_platform_data,
	},
};

static struct wcd9xxx_pdata apq8064_tabla20_platform_data = {
	.slimbus_slave_device = {
		.name = "tabla-slave",
		.e_addr = {0, 0, 0x60, 0, 0x17, 2},
	},
	.irq = MSM_GPIO_TO_INT(42),
	.irq_base = TABLA_INTERRUPT_BASE,
	.num_irqs = NR_WCD9XXX_IRQS,
	.reset_gpio = PM8921_GPIO_PM_TO_SYS(34),
	.micbias = {
		.ldoh_v = TABLA_LDOH_2P85_V,
		.cfilt1_mv = 1800,
		/*OPPO 2012-07-27 zhzhyon Modify for micbias voltage*/
		#ifndef CONFIG_VENDOR_EDIT
		.cfilt2_mv = 2700,
		#else
		.cfilt2_mv = 2000,
		#endif
		/*OPPO 2012-07-27 zhzhyon Modify end*/
		.cfilt3_mv = 1800,
		/*OPPO 2012-07-27 zhzhyon Modify for micbias voltage*/
		#ifndef CONFIG_VENDOR_EDIT
		.bias1_cfilt_sel = TABLA_CFILT1_SEL,
		.bias2_cfilt_sel = TABLA_CFILT2_SEL,
		.bias3_cfilt_sel = TABLA_CFILT3_SEL,
		.bias4_cfilt_sel = TABLA_CFILT3_SEL,
		#else
		.bias1_cfilt_sel = TABLA_CFILT2_SEL,
		.bias2_cfilt_sel = TABLA_CFILT2_SEL,
		.bias3_cfilt_sel = TABLA_CFILT2_SEL,
		.bias4_cfilt_sel = TABLA_CFILT2_SEL,
		#endif
		/*OPPO 2012-07-27 zhzhyon Modify end*/
	},
	.regulator = {
	{
		.name = "CDC_VDD_CP",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_CP_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_RX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_RX_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_TX",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_CDC_VDDA_TX_CUR_MAX,
	},
	{
		.name = "VDDIO_CDC",
		.min_uV = 1800000,
		.max_uV = 1800000,
		.optimum_uA = WCD9XXX_VDDIO_CDC_CUR_MAX,
	},
	{
		.name = "VDDD_CDC_D",
		.min_uV = 1225000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_D_CUR_MAX,
	},
	{
		.name = "CDC_VDDA_A_1P2V",
		.min_uV = 1225000,
		.max_uV = 1250000,
		.optimum_uA = WCD9XXX_VDDD_CDC_A_CUR_MAX,
	},
	},
};

static struct slim_device apq8064_slim_tabla20 = {
	.name = "tabla2x-slim",
	.e_addr = {0, 1, 0x60, 0, 0x17, 2},
	.dev = {
		.platform_data = &apq8064_tabla20_platform_data,
	},
};

/* enable the level shifter for cs8427 to make sure the I2C
 * clock is running at 100KHz and voltage levels are at 3.3
 * and 5 volts
 */
static int enable_100KHz_ls(int enable)
{
	int ret = 0;
	if (enable) {
		ret = gpio_request(SX150X_GPIO(1, 10),
					"cs8427_100KHZ_ENABLE");
		if (ret) {
			pr_err("%s: Failed to request gpio %d\n", __func__,
				SX150X_GPIO(1, 10));
			return ret;
		}
		gpio_direction_output(SX150X_GPIO(1, 10), 1);
	} else {
		gpio_direction_output(SX150X_GPIO(1, 10), 0);
		gpio_free(SX150X_GPIO(1, 10));
	}
	return ret;
}

static struct cs8427_platform_data cs8427_i2c_platform_data = {
	.irq = SX150X_GPIO(1, 4),
	.reset_gpio = SX150X_GPIO(1, 6),
	.enable = enable_100KHz_ls,
};

static struct i2c_board_info cs8427_device_info[] __initdata = {
	{
		I2C_BOARD_INFO("cs8427", CS8427_ADDR4),
		.platform_data = &cs8427_i2c_platform_data,
	},
};

#define HAP_SHIFT_LVL_OE_GPIO		PM8921_MPP_PM_TO_SYS(8)
#define ISA1200_HAP_EN_GPIO		PM8921_GPIO_PM_TO_SYS(33)
#define ISA1200_HAP_LEN_GPIO		PM8921_GPIO_PM_TO_SYS(20)
#define ISA1200_HAP_CLK_PM8921		PM8921_GPIO_PM_TO_SYS(44)
#define ISA1200_HAP_CLK_PM8917		PM8921_GPIO_PM_TO_SYS(38)

static int isa1200_clk_enable(bool on)
{
	unsigned int gpio = ISA1200_HAP_CLK_PM8921;
	int rc = 0;

	if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917)
		gpio = ISA1200_HAP_CLK_PM8917;

	gpio_set_value_cansleep(gpio, on);

	if (on) {
		rc = pm8xxx_aux_clk_control(CLK_MP3_2, XO_DIV_1, true);
		if (rc) {
			pr_err("%s: unable to write aux clock register(%d)\n",
				__func__, rc);
			goto err_gpio_dis;
		}
	} else {
		rc = pm8xxx_aux_clk_control(CLK_MP3_2, XO_DIV_NONE, true);
		if (rc)
			pr_err("%s: unable to write aux clock register(%d)\n",
				__func__, rc);
	}

	return rc;

err_gpio_dis:
	gpio_set_value_cansleep(gpio, !on);
	return rc;
}

static int isa1200_dev_setup(bool enable)
{
	unsigned int gpio = ISA1200_HAP_CLK_PM8921;
	int rc = 0;

	if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917)
		gpio = ISA1200_HAP_CLK_PM8917;

	if (!enable)
		goto free_gpio;

	rc = gpio_request(gpio, "haptics_clk");
	if (rc) {
		pr_err("%s: unable to request gpio %d config(%d)\n",
			__func__, gpio, rc);
		return rc;
	}

	rc = gpio_direction_output(gpio, 0);
	if (rc) {
		pr_err("%s: unable to set direction\n", __func__);
		goto free_gpio;
	}

	return 0;

free_gpio:
	gpio_free(gpio);
	return rc;
}

static struct isa1200_regulator isa1200_reg_data[] = {
	{
		.name = "vddp",
		.min_uV = ISA_I2C_VTG_MIN_UV,
		.max_uV = ISA_I2C_VTG_MAX_UV,
		.load_uA = ISA_I2C_CURR_UA,
	},
};

static struct isa1200_platform_data isa1200_1_pdata = {
	.name = "vibrator",
	.dev_setup = isa1200_dev_setup,
	.clk_enable = isa1200_clk_enable,
	.need_pwm_clk = true,
	.hap_en_gpio = ISA1200_HAP_EN_GPIO,
	.hap_len_gpio = ISA1200_HAP_LEN_GPIO,
	.max_timeout = 15000,
	.mode_ctrl = PWM_GEN_MODE,
	.pwm_fd = {
		.pwm_div = 256,
	},
	.is_erm = false,
	.smart_en = true,
	.ext_clk_en = true,
	.chip_en = 1,
	.regulator_info = isa1200_reg_data,
	.num_regulators = ARRAY_SIZE(isa1200_reg_data),
};

static struct i2c_board_info isa1200_board_info[] __initdata = {
	{
		I2C_BOARD_INFO("isa1200_1", 0x90>>1),
		.platform_data = &isa1200_1_pdata,
	},
};
/* OPPO 2012-09-08 liujun Delete begin for disable original touchscreen define */
#ifndef CONFIG_VENDOR_EDIT
/* configuration data for mxt1386e using V2.1 firmware */
static const u8 mxt1386e_config_data_v2_1[] = {
	/* T6 Object */
	0, 0, 0, 0, 0, 0,
	/* T38 Object */
	14, 3, 0, 5, 7, 12, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0,
	/* T7 Object */
	32, 10, 50,
	/* T8 Object */
	25, 0, 20, 20, 0, 0, 0, 0, 0, 0,
	/* T9 Object */
	139, 0, 0, 26, 42, 0, 32, 80, 2, 5,
	0, 5, 5, 79, 10, 30, 10, 10, 255, 2,
	85, 5, 0, 5, 9, 5, 12, 35, 70, 40,
	20, 5, 0, 0, 0,
	/* T18 Object */
	0, 0,
	/* T24 Object */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* T25 Object */
	1, 0, 60, 115, 156, 99,
	/* T27 Object */
	0, 0, 0, 0, 0, 0, 0,
	/* T40 Object */
	0, 0, 0, 0, 0,
	/* T42 Object */
	0, 0, 255, 0, 255, 0, 0, 0, 0, 0,
	/* T43 Object */
	0, 0, 0, 0, 0, 0, 0, 64, 0, 8,
	16,
	/* T46 Object */
	68, 0, 16, 16, 0, 0, 0, 0, 0,
	/* T47 Object */
	0, 0, 0, 0, 0, 0, 3, 64, 66, 0,
	/* T48 Object */
	1, 64, 64, 0, 0, 0, 0, 0, 0, 0,
	32, 40, 0, 10, 10, 0, 0, 100, 10, 90,
	0, 0, 0, 0, 0, 0, 0, 10, 1, 10,
	52, 10, 12, 0, 33, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0,
	/* T56 Object */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,
};

#define MXT_TS_GPIO_IRQ			6
#define MXT_TS_PWR_EN_GPIO		PM8921_GPIO_PM_TO_SYS(23)
#define MXT_TS_RESET_GPIO		33

static struct mxt_config_info mxt_config_array[] = {
	{
		.config		= mxt1386e_config_data_v2_1,
		.config_length	= ARRAY_SIZE(mxt1386e_config_data_v2_1),
		.family_id	= 0xA0,
		.variant_id	= 0x7,
		.version	= 0x21,
		.build		= 0xAA,
		.bootldr_id	= MXT_BOOTLOADER_ID_1386E,
		.fw_name	= "atmel_8064_liquid_v2_2_AA.hex",
	},
	{
		/* The config data for V2.2.AA is the same as for V2.1.AA */
		.config		= mxt1386e_config_data_v2_1,
		.config_length	= ARRAY_SIZE(mxt1386e_config_data_v2_1),
		.family_id	= 0xA0,
		.variant_id	= 0x7,
		.version	= 0x22,
		.build		= 0xAA,
		.bootldr_id	= MXT_BOOTLOADER_ID_1386E,
	},
};

static struct mxt_platform_data mxt_platform_data = {
	.config_array		= mxt_config_array,
	.config_array_size	= ARRAY_SIZE(mxt_config_array),
	.panel_minx		= 0,
	.panel_maxx		= 1365,
	.panel_miny		= 0,
	.panel_maxy		= 767,
	.disp_minx		= 0,
	.disp_maxx		= 1365,
	.disp_miny		= 0,
	.disp_maxy		= 767,
	.irqflags		= IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
	.i2c_pull_up		= true,
	.reset_gpio		= MXT_TS_RESET_GPIO,
	.irq_gpio		= MXT_TS_GPIO_IRQ,
};

static struct i2c_board_info mxt_device_info[] __initdata = {
	{
		I2C_BOARD_INFO("atmel_mxt_ts", 0x5b),
		.platform_data = &mxt_platform_data,
		.irq = MSM_GPIO_TO_INT(MXT_TS_GPIO_IRQ),
	},
};
#define CYTTSP_TS_GPIO_IRQ		6
#define CYTTSP_TS_GPIO_SLEEP		33
#define CYTTSP_TS_GPIO_SLEEP_ALT	12

static ssize_t tma340_vkeys_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, 200,
	__stringify(EV_KEY) ":" __stringify(KEY_BACK) ":73:1120:97:97"
	":" __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":230:1120:97:97"
	":" __stringify(EV_KEY) ":" __stringify(KEY_HOME) ":389:1120:97:97"
	":" __stringify(EV_KEY) ":" __stringify(KEY_SEARCH) ":544:1120:97:97"
	"\n");
}

static struct kobj_attribute tma340_vkeys_attr = {
	.attr = {
		.mode = S_IRUGO,
	},
	.show = &tma340_vkeys_show,
};

static struct attribute *tma340_properties_attrs[] = {
	&tma340_vkeys_attr.attr,
	NULL
};

static struct attribute_group tma340_properties_attr_group = {
	.attrs = tma340_properties_attrs,
};

static int cyttsp_platform_init(struct i2c_client *client)
{
	int rc = 0;
	static struct kobject *tma340_properties_kobj;

	tma340_vkeys_attr.attr.name = "virtualkeys.cyttsp-i2c";
	tma340_properties_kobj = kobject_create_and_add("board_properties",
								NULL);
	if (tma340_properties_kobj)
		rc = sysfs_create_group(tma340_properties_kobj,
					&tma340_properties_attr_group);
	if (!tma340_properties_kobj || rc)
		pr_err("%s: failed to create board_properties\n",
				__func__);

	return 0;
}

static struct cyttsp_regulator cyttsp_regulator_data[] = {
	{
		.name = "vdd",
		.min_uV = CY_TMA300_VTG_MIN_UV,
		.max_uV = CY_TMA300_VTG_MAX_UV,
		.hpm_load_uA = CY_TMA300_CURR_24HZ_UA,
		.lpm_load_uA = CY_TMA300_CURR_24HZ_UA,
	},
	{
		.name = "vcc_i2c",
		.min_uV = CY_I2C_VTG_MIN_UV,
		.max_uV = CY_I2C_VTG_MAX_UV,
		.hpm_load_uA = CY_I2C_CURR_UA,
		.lpm_load_uA = CY_I2C_CURR_UA,
	},
};

static struct cyttsp_platform_data cyttsp_pdata = {
	.panel_maxx = 634,
	.panel_maxy = 1166,
	.disp_minx = 18,
	.disp_maxx = 617,
	.disp_miny = 18,
	.disp_maxy = 1041,
	.flags = 0x01,
	.gen = CY_GEN3,
	.use_st = CY_USE_ST,
	.use_mt = CY_USE_MT,
	.use_hndshk = CY_SEND_HNDSHK,
	.use_trk_id = CY_USE_TRACKING_ID,
	.use_sleep = CY_USE_DEEP_SLEEP_SEL,
	.use_gestures = CY_USE_GESTURES,
	.fw_fname = "cyttsp_8064_mtp.hex",
	/* change act_intrvl to customize the Active power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.act_intrvl = CY_ACT_INTRVL_DFLT,
	/* change tch_tmout to customize the touch timeout for the
	 * Active power state for Operating mode
	 */
	.tch_tmout = CY_TCH_TMOUT_DFLT,
	/* change lp_intrvl to customize the Low Power power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.lp_intrvl = CY_LP_INTRVL_DFLT,
	.sleep_gpio = CYTTSP_TS_GPIO_SLEEP,
	.resout_gpio = -1,
	.irq_gpio = CYTTSP_TS_GPIO_IRQ,
	.regulator_info = cyttsp_regulator_data,
	.num_regulators = ARRAY_SIZE(cyttsp_regulator_data),
	.init = cyttsp_platform_init,
	.correct_fw_ver = 17,
};

static struct i2c_board_info cyttsp_info[] __initdata = {
	{
		I2C_BOARD_INFO(CY_I2C_NAME, 0x24),
		.platform_data = &cyttsp_pdata,
		.irq = MSM_GPIO_TO_INT(CYTTSP_TS_GPIO_IRQ),
	},
};
#else

/* OPPO 2012-07-31 liujun Add begin for define touch screen data */

#if defined(CONFIG_TOUCHSCREEN_SYNAPTICS_S3202_I2C_RMI)
static struct regulator *vreg_tp_2P8V = NULL;
static struct regulator *vreg_tp_1P8V = NULL;
static DEFINE_MUTEX(TP_POWER_LOCK);
static DEFINE_MUTEX(TP_BK_POWER_LOCK);

static int init_tp_regulator(void)
{
	int rc = 0;
	if(vreg_tp_2P8V == NULL)
	{
		vreg_tp_2P8V = regulator_get(NULL, "8921_l16");						
		if (IS_ERR(vreg_tp_2P8V))
			return PTR_ERR(vreg_tp_2P8V);

		rc = regulator_set_voltage(vreg_tp_2P8V, 2800000, 2800000);
		if (rc)
		{
			pr_err("%s: unable to set the voltage for regulator vreg_2P8V\n", __func__);
			regulator_put(vreg_tp_2P8V);
            vreg_tp_2P8V = NULL;
			return rc;
		}
	}
	return rc;
}

static int inline enable_tp_regulator(struct regulator **vreg_tp)
{
	int rc = regulator_enable(*vreg_tp);
	if (rc)
	{
		pr_err("%s: unable to enable tp regulator \n",	__func__);
		regulator_put(*vreg_tp);
		*vreg_tp = NULL;
	}
	return rc;
}

static int inline disable_tp_regulator(struct regulator *vreg_tp)
{
	int rc = regulator_disable(vreg_tp);
	if (rc)
		pr_err("%s: Unable to disable tp regulator\n",	__func__);
	return rc;
}

static int oppo_total_touchscreen_power(struct regulator* regula,int on);
static int oppo_touchscreen_power(int on)
{
	int rc = 0;
	mutex_lock(&TP_POWER_LOCK);
	if((rc = init_tp_regulator()) == 0)
		rc = oppo_total_touchscreen_power(vreg_tp_2P8V,on);
	mutex_unlock(&TP_POWER_LOCK);
	return 	rc = 0;
}
static int oppo_touchpad_power(int on) {
	int rc = 0;

/* OPPO 2013-10-15 ranfei Modify begin for ���������汾 */
#if 0
    if((get_pcb_version() >= PCB_VERSION_EVT3_N1T)&&(get_pcb_version() <= PCB_VERSION_PVT_N1T)) {
#else
	if((get_pcb_version() >= PCB_VERSION_EVT3_N1F)&&(get_pcb_version() <= PCB_VERSION_PVT_N1W)) {
#endif
/* OPPO 2013-10-15 ranfei Modify end */
        
        mutex_lock(&TP_BK_POWER_LOCK);

	    if(vreg_tp_1P8V == NULL) {
		    vreg_tp_1P8V = regulator_get(NULL, "8921_l21");						
		    if (IS_ERR(vreg_tp_1P8V)) {
			    mutex_unlock(&TP_BK_POWER_LOCK);
			    return PTR_ERR(vreg_tp_1P8V);
		    }
	    }
	    rc = oppo_total_touchscreen_power(vreg_tp_1P8V,on);

	    mutex_unlock(&TP_BK_POWER_LOCK);

	    return rc ;
    
    } else {
        return rc;
    }	
}

static int oppo_total_touchscreen_power(struct regulator* regula,int on)
{
	int rc = 0;

	if(on == 0xEF)
	{
		// Power reset
		rc = disable_tp_regulator(regula);
		if (rc)
			goto oppo_tp_power_return;
		msleep(20);

		rc = enable_tp_regulator(&regula);
		if (rc)
			goto oppo_tp_power_return;
		mdelay(50);

		pr_debug("[TSP] %s: power reset\n", __func__);
	} 
	else if (on)
	{
		// Power on
		rc = enable_tp_regulator(&regula);
		if (rc)
			goto oppo_tp_power_return;
		mdelay(50);

		pr_debug("[TSP] %s: power on\n", __func__);
	}
	else
	{
		// Power off
		rc = disable_tp_regulator(regula);
		if (rc)
			goto oppo_tp_power_return;
		mdelay(5);

		pr_debug("[TSP] %s: power off\n", __func__);
	}
	printk(KERN_INFO "tp power %s, now:%d.\n", on?"on":"off", regulator_is_enabled(regula));

oppo_tp_power_return:
	return rc;
 }
#endif

#define GPIO_TOUCH_RST  7
#define GPIO_TOUCH_INT	6
#define GPIO_TP_WAKEUP  (14)
#define GPIO_TP_ID      (15)
#define GPIO_TP_ID_N1  (60)
#define GPIO_TP_ID2_N1 (59)

int remote_rmi4_get_irq_gpio(void)   //add by yben
{
	return (int)GPIO_TOUCH_INT ;
}
static void touch_init_hw(void)
{
	gpio_request(GPIO_TOUCH_INT, "TOUCH_INT");
	gpio_direction_input(GPIO_TOUCH_INT);
#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_S3202_I2C_RMI
	if (get_pcb_version() >= PCB_VERSION_EVT_N1)
	{
		gpio_tlmm_config(GPIO_CFG(GPIO_TP_ID_N1, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TP_ID2_N1, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	}
    else
    {
        gpio_tlmm_config(GPIO_CFG(GPIO_TP_WAKEUP, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(GPIO_TP_ID, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
/* OPPO 2013-08-23 ranfei Add begin for reason */
        gpio_tlmm_config(GPIO_CFG(GPIO_TOUCH_RST, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
        gpio_request(GPIO_TOUCH_RST, "TOUCH_RST");
	    gpio_direction_output(GPIO_TOUCH_RST, 1);
/* OPPO 2013-08-23 ranfei Add end */
    }
    
	oppo_touchscreen_power(1);
#endif
}

#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_S3202_I2C_RMI
static struct synaptics_i2c_rmi_platform_data synap_s3202_touch_platform_data[] = {
	{
		.version = 0x0101,
		.power = oppo_touchscreen_power,
		.flags = SYNAPTICS_SNAP_TO_INACTIVE_EDGE,
		.irqflags = IRQF_TRIGGER_LOW,//IRQF_TRIGGER_FALLING,
	}
};

static struct i2c_board_info synaptics_s3202_touch_info[] = {
	{
		I2C_BOARD_INFO(SYNAPTICS_I2C_RMI_NAME, 0x20),
		.irq		= MSM_GPIO_TO_INT(GPIO_TOUCH_INT),
		.platform_data = synap_s3202_touch_platform_data,
	}
};
#endif


#endif
/* OPPO 2012-07-31 liujun Add end */

//////////////////////////////////////
//yanghai add 
#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2012-07-31 liujun Add begin for define touch screen data */


#define GPIO_TOUCHPAD_INT	56
#define GPIO_TOUCHPAD_RST   57

static void touchpad_init_hw(void)
{
    gpio_tlmm_config(GPIO_CFG(GPIO_TOUCHPAD_INT, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(GPIO_TOUCHPAD_RST, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);

    oppo_touchpad_power(1);
}

#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_S3202_I2C_RMI
static struct y8c20x66a_i2c_rmi_platform_data synap_y8c20x66a_touch_platform_data[] = {
	{
		.power = oppo_touchpad_power,
		.irqflags = IRQF_TRIGGER_FALLING,
		.reset_gpio = GPIO_TOUCHPAD_RST,
	}
};


static struct i2c_board_info synaptics_y8c20x66a_touch_info[] = {
	{
#ifdef CYPRESS_CHIP
		I2C_BOARD_INFO(y8c20x66a_I2C_RMI_NAME, 0x64),   //cypress
#endif
#ifdef MICROCHIP_CHIP
		I2C_BOARD_INFO(y8c20x66a_I2C_RMI_NAME, 0x25),   //microchip
#endif
		.irq		= MSM_GPIO_TO_INT(GPIO_TOUCHPAD_INT),
		.platform_data = synap_y8c20x66a_touch_platform_data,
	}
};
#endif


#endif

//yanghai add  end
///////////////////////////////////
#define MSM_WCNSS_PHYS	0x03000000
#define MSM_WCNSS_SIZE	0x280000

static struct resource resources_wcnss_wlan[] = {
	{
		.start	= RIVA_APPS_WLAN_RX_DATA_AVAIL_IRQ,
		.end	= RIVA_APPS_WLAN_RX_DATA_AVAIL_IRQ,
		.name	= "wcnss_wlanrx_irq",
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= RIVA_APPS_WLAN_DATA_XFER_DONE_IRQ,
		.end	= RIVA_APPS_WLAN_DATA_XFER_DONE_IRQ,
		.name	= "wcnss_wlantx_irq",
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_WCNSS_PHYS,
		.end	= MSM_WCNSS_PHYS + MSM_WCNSS_SIZE - 1,
		.name	= "wcnss_mmio",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= 64,
		.end	= 68,
		.name	= "wcnss_gpios_5wire",
		.flags	= IORESOURCE_IO,
	},
};

static struct qcom_wcnss_opts qcom_wcnss_pdata = {
	.has_48mhz_xo	= 1,
};

static struct platform_device msm_device_wcnss_wlan = {
	.name		= "wcnss_wlan",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(resources_wcnss_wlan),
	.resource	= resources_wcnss_wlan,
	.dev		= {.platform_data = &qcom_wcnss_pdata},
};

static struct platform_device msm_device_iris_fm __devinitdata = {
	.name = "iris_fm",
	.id   = -1,
};

#ifdef CONFIG_QSEECOM
/* qseecom bus scaling */
static struct msm_bus_vectors qseecom_clks_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = 0,
		.ab = 0,
	},
};

static struct msm_bus_vectors qseecom_enable_dfab_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 70000000UL,
		.ib = 70000000UL,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 2480000000UL,
		.ib = 2480000000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = 0,
		.ab = 0,
	},
};

static struct msm_bus_vectors qseecom_enable_sfpb_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 0,
		.ib = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = (64 * 8) * 1000000UL,
		.ab = (64 * 8) *  100000UL,
	},
};

static struct msm_bus_vectors qseecom_enable_dfab_sfpb_vectors[] = {
	{
		.src = MSM_BUS_MASTER_ADM_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 70000000UL,
		.ib = 70000000UL,
	},
	{
		.src = MSM_BUS_MASTER_ADM_PORT1,
		.dst = MSM_BUS_SLAVE_GSBI1_UART,
		.ab = 2480000000UL,
		.ib = 2480000000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = (64 * 8) * 1000000UL,
		.ab = (64 * 8) *  100000UL,
	},
};

static struct msm_bus_paths qseecom_hw_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(qseecom_clks_init_vectors),
		qseecom_clks_init_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_dfab_vectors),
		qseecom_enable_dfab_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_sfpb_vectors),
		qseecom_enable_sfpb_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_dfab_sfpb_vectors),
		qseecom_enable_dfab_sfpb_vectors,
	},
};

static struct msm_bus_scale_pdata qseecom_bus_pdata = {
	qseecom_hw_bus_scale_usecases,
	ARRAY_SIZE(qseecom_hw_bus_scale_usecases),
	.name = "qsee",
};

static struct platform_device qseecom_device = {
	.name		= "qseecom",
	.id		= 0,
	.dev		= {
		.platform_data = &qseecom_bus_pdata,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

#define QCE_SIZE		0x10000
#define QCE_0_BASE		0x11000000

#define QCE_HW_KEY_SUPPORT	0
#define QCE_SHA_HMAC_SUPPORT	1
#define QCE_SHARE_CE_RESOURCE	3
#define QCE_CE_SHARED		0

static struct resource qcrypto_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV8064_CE_IN_CHAN,
		.end = DMOV8064_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV8064_CE_IN_CRCI,
		.end = DMOV8064_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV8064_CE_OUT_CRCI,
		.end = DMOV8064_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

static struct resource qcedev_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV8064_CE_IN_CHAN,
		.end = DMOV8064_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV8064_CE_IN_CRCI,
		.end = DMOV8064_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV8064_CE_OUT_CRCI,
		.end = DMOV8064_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)

static struct msm_ce_hw_support qcrypto_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = NULL,
};

static struct platform_device qcrypto_device = {
	.name		= "qcrypto",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcrypto_resources),
	.resource	= qcrypto_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcrypto_ce_hw_suppport,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

static struct msm_ce_hw_support qcedev_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = NULL,
};

static struct platform_device qcedev_device = {
	.name		= "qce",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcedev_resources),
	.resource	= qcedev_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcedev_ce_hw_suppport,
	},
};
#endif

static struct mdm_vddmin_resource mdm_vddmin_rscs = {
	.rpm_id = MSM_RPM_ID_VDDMIN_GPIO,
	.ap2mdm_vddmin_gpio = 30,
	.modes  = 0x03,
	.drive_strength = 8,
	.mdm2ap_vddmin_gpio = 80,
};

static struct gpiomux_setting mdm2ap_status_gpio_run_cfg = {
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

/* OPPO 2013-05-20 zhenwx Modify begin for decrease modem shut down time */
#if 0
static struct mdm_platform_data mdm_platform_data = {
	.mdm_version = "3.0",
	.ramdump_delay_ms = 2000,
	.early_power_on = 1,
	.sfr_query = 1,
	.vddmin_resource = &mdm_vddmin_rscs,
	.peripheral_platform_device = &apq8064_device_hsic_host,
	.ramdump_timeout_ms = 120000,
	.mdm2ap_status_gpio_run_cfg = &mdm2ap_status_gpio_run_cfg,
	.sysmon_subsys_id_valid = 1,
	.sysmon_subsys_id = SYSMON_SS_EXT_MODEM,
};

#else

static struct mdm_platform_data mdm_platform_data = {
	.mdm_version = "3.0",
	.ramdump_delay_ms = 2000,
	.early_power_on = 1,
	.sfr_query = 1,  
	.send_shdn = 1,
	.vddmin_resource = &mdm_vddmin_rscs,
	.peripheral_platform_device = &apq8064_device_hsic_host,
	.ramdump_timeout_ms = 120000,
	.mdm2ap_status_gpio_run_cfg = &mdm2ap_status_gpio_run_cfg,
	.sysmon_subsys_id_valid = 1,
	.sysmon_subsys_id = SYSMON_SS_EXT_MODEM,
};
#endif
/* OPPO 2013-05-20 zhenwx Modify end */

static struct mdm_platform_data amdm_platform_data = {
	.mdm_version = "3.0",
	.ramdump_delay_ms = 2000,
	.early_power_on = 1,
	.sfr_query = 1,
	.send_shdn = 1,
	.vddmin_resource = &mdm_vddmin_rscs,
	.peripheral_platform_device = &apq8064_device_hsic_host,
	.ramdump_timeout_ms = 120000,
	.mdm2ap_status_gpio_run_cfg = &mdm2ap_status_gpio_run_cfg,
	.sysmon_subsys_id_valid = 1,
	.sysmon_subsys_id = SYSMON_SS_EXT_MODEM,
	.no_a2m_errfatal_on_ssr = 1,
};

static struct mdm_vddmin_resource bmdm_vddmin_rscs = {
	.rpm_id = MSM_RPM_ID_VDDMIN_GPIO,
	.ap2mdm_vddmin_gpio = 30,
	.modes  = 0x03,
	.drive_strength = 8,
	.mdm2ap_vddmin_gpio = 64,
};

static struct mdm_platform_data bmdm_platform_data = {
	.mdm_version = "3.0",
	.ramdump_delay_ms = 2000,
	.sfr_query = 1,
	.send_shdn = 1,
	.vddmin_resource = &bmdm_vddmin_rscs,
	.peripheral_platform_device = &apq8064_device_ehci_host3,
	.ramdump_timeout_ms = 120000,
	.mdm2ap_status_gpio_run_cfg = &mdm2ap_status_gpio_run_cfg,
	.sysmon_subsys_id_valid = 1,
	.sysmon_subsys_id = SYSMON_SS_EXT_MODEM2,
	.no_a2m_errfatal_on_ssr = 1,
};

static struct mdm_platform_data sglte2_mdm_platform_data = {
	.mdm_version = "3.0",
	.ramdump_delay_ms = 2000,
	.early_power_on = 1,
	.sfr_query = 1,
	.vddmin_resource = &mdm_vddmin_rscs,
	.peripheral_platform_device = &apq8064_device_hsic_host,
	.ramdump_timeout_ms = 120000,
	.mdm2ap_status_gpio_run_cfg = &mdm2ap_status_gpio_run_cfg,
	.sysmon_subsys_id_valid = 1,
	.sysmon_subsys_id = SYSMON_SS_EXT_MODEM,
	.no_a2m_errfatal_on_ssr = 1,
};

static struct mdm_platform_data sglte2_qsc_platform_data = {
	.mdm_version = "3.0",
	.ramdump_delay_ms = 2000,
	.ramdump_timeout_ms = 600000,
	.no_powerdown_after_ramdumps = 1,
	.image_upgrade_supported = 1,
	.no_a2m_errfatal_on_ssr = 1,
	.no_reset_on_first_powerup = 1,
};

static struct tsens_platform_data apq_tsens_pdata  = {
		.tsens_factor		= 1000,
		.hw_type		= APQ_8064,
		.tsens_num_sensor	= 11,
		.slope = {1176, 1176, 1154, 1176, 1111,
			1132, 1132, 1199, 1132, 1199, 1132},
};

static struct platform_device msm_tsens_device = {
	.name   = "tsens8960-tm",
	.id = -1,
};

static struct msm_thermal_data msm_thermal_pdata = {
	.sensor_id = 7,
	.poll_ms = 250,
	.limit_temp_degC = 60,
	.temp_hysteresis_degC = 10,
	.freq_step = 2,
	.core_limit_temp_degC = 80,
	.core_temp_hysteresis_degC = 10,
	.core_control_mask = 0xe,
};

#define MSM_SHARED_RAM_PHYS 0x80000000
static void __init apq8064_map_io(void)
{
	msm_shared_ram_phys = MSM_SHARED_RAM_PHYS;
	msm_map_apq8064_io();
	if (socinfo_init() < 0)
		pr_err("socinfo_init() failed!\n");
}

static void __init apq8064_init_irq(void)
{
	struct msm_mpm_device_data *data = NULL;

#ifdef CONFIG_MSM_MPM
	data = &apq8064_mpm_dev_data;
#endif

	msm_mpm_irq_extn_init(data);
	gic_init(0, GIC_PPI_START, MSM_QGIC_DIST_BASE,
						(void *)MSM_QGIC_CPU_BASE);
}

static struct platform_device msm8064_device_saw_regulator_core0 = {
	.name	= "saw-regulator",
	.id	= 0,
	.dev	= {
		.platform_data = &msm8064_saw_regulator_pdata_8921_s5,
	},
};

static struct platform_device msm8064_device_saw_regulator_core1 = {
	.name	= "saw-regulator",
	.id	= 1,
	.dev	= {
		.platform_data = &msm8064_saw_regulator_pdata_8921_s6,
	},
};

static struct platform_device msm8064_device_saw_regulator_core2 = {
	.name	= "saw-regulator",
	.id	= 2,
	.dev	= {
		.platform_data = &msm8064_saw_regulator_pdata_8821_s0,
	},
};

static struct platform_device msm8064_device_saw_regulator_core3 = {
	.name	= "saw-regulator",
	.id	= 3,
	.dev	= {
		.platform_data = &msm8064_saw_regulator_pdata_8821_s1,

	},
};

static struct msm_rpmrs_level msm_rpmrs_levels[] = {
	{
		MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		1, 784, 180000, 100,
	},

	{
		MSM_PM_SLEEP_MODE_RETENTION,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		415, 715, 340827, 475,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		1300, 228, 1200000, 2000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, GDHS, MAX, ACTIVE),
		false,
		2000, 138, 1208400, 3200,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		6000, 119, 1850300, 9000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, GDHS, MAX, ACTIVE),
		false,
		9200, 68, 2839200, 16400,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, MAX, ACTIVE),
		false,
		10300, 63, 3128000, 18200,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		18000, 10, 4602600, 27000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, RET_HIGH, RET_LOW),
		false,
		20000, 2, 5752000, 32000,
	},
};

static struct msm_pm_boot_platform_data msm_pm_boot_pdata __initdata = {
	.mode = MSM_PM_BOOT_CONFIG_TZ,
};

static struct msm_rpmrs_platform_data msm_rpmrs_data __initdata = {
	.levels = &msm_rpmrs_levels[0],
	.num_levels = ARRAY_SIZE(msm_rpmrs_levels),
	.vdd_mem_levels  = {
		[MSM_RPMRS_VDD_MEM_RET_LOW]	= 750000,
		[MSM_RPMRS_VDD_MEM_RET_HIGH]	= 750000,
		[MSM_RPMRS_VDD_MEM_ACTIVE]	= 1050000,
		[MSM_RPMRS_VDD_MEM_MAX]		= 1150000,
	},
	.vdd_dig_levels = {
		[MSM_RPMRS_VDD_DIG_RET_LOW]	= 500000,
		[MSM_RPMRS_VDD_DIG_RET_HIGH]	= 750000,
		[MSM_RPMRS_VDD_DIG_ACTIVE]	= 950000,
		[MSM_RPMRS_VDD_DIG_MAX]		= 1150000,
	},
	.vdd_mask = 0x7FFFFF,
	.rpmrs_target_id = {
		[MSM_RPMRS_ID_PXO_CLK]		= MSM_RPM_ID_PXO_CLK,
		[MSM_RPMRS_ID_L2_CACHE_CTL]	= MSM_RPM_ID_LAST,
		[MSM_RPMRS_ID_VDD_DIG_0]	= MSM_RPM_ID_PM8921_S3_0,
		[MSM_RPMRS_ID_VDD_DIG_1]	= MSM_RPM_ID_PM8921_S3_1,
		[MSM_RPMRS_ID_VDD_MEM_0]	= MSM_RPM_ID_PM8921_L24_0,
		[MSM_RPMRS_ID_VDD_MEM_1]	= MSM_RPM_ID_PM8921_L24_1,
		[MSM_RPMRS_ID_RPM_CTL]		= MSM_RPM_ID_RPM_CTL,
	},
};

static uint8_t spm_wfi_cmd_sequence[] __initdata = {
	0x03, 0x0f,
};

static uint8_t spm_power_collapse_without_rpm[] __initdata = {
	0x00, 0x24, 0x54, 0x10,
	0x09, 0x03, 0x01,
	0x10, 0x54, 0x30, 0x0C,
	0x24, 0x30, 0x0f,
};

static uint8_t spm_retention_cmd_sequence[] __initdata = {
	0x00, 0x05, 0x03, 0x0D,
	0x0B, 0x00, 0x0f,
};

static uint8_t spm_retention_with_krait_v3_cmd_sequence[] __initdata = {
	0x42, 0x1B, 0x00,
	0x05, 0x03, 0x0D, 0x0B,
	0x00, 0x42, 0x1B,
	0x0f,
};

static uint8_t spm_power_collapse_with_rpm[] __initdata = {
	0x00, 0x24, 0x54, 0x10,
	0x09, 0x07, 0x01, 0x0B,
	0x10, 0x54, 0x30, 0x0C,
	0x24, 0x30, 0x0f,
};

/* 8064AB has a different command to assert apc_pdn */
static uint8_t spm_power_collapse_without_rpm_krait_v3[] __initdata = {
	0x00, 0x24, 0x84, 0x10,
	0x09, 0x03, 0x01,
	0x10, 0x84, 0x30, 0x0C,
	0x24, 0x30, 0x0f,
};

static uint8_t spm_power_collapse_with_rpm_krait_v3[] __initdata = {
	0x00, 0x24, 0x84, 0x10,
	0x09, 0x07, 0x01, 0x0B,
	0x10, 0x84, 0x30, 0x0C,
	0x24, 0x30, 0x0f,
};

static struct msm_spm_seq_entry msm_spm_boot_cpu_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_MODE_CLOCK_GATING,
		.notify_rpm = false,
		.cmd = spm_wfi_cmd_sequence,
	},
	[1] = {
		.mode = MSM_SPM_MODE_POWER_RETENTION,
		.notify_rpm = false,
		.cmd = spm_retention_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = false,
		.cmd = spm_power_collapse_without_rpm,
	},
	[3] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = spm_power_collapse_with_rpm,
	},
};
static struct msm_spm_seq_entry msm_spm_nonboot_cpu_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_MODE_CLOCK_GATING,
		.notify_rpm = false,
		.cmd = spm_wfi_cmd_sequence,
	},
	[1] = {
		.mode = MSM_SPM_MODE_POWER_RETENTION,
		.notify_rpm = false,
		.cmd = spm_retention_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = false,
		.cmd = spm_power_collapse_without_rpm,
	},
	[3] = {
		.mode = MSM_SPM_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = spm_power_collapse_with_rpm,
	},
};

static uint8_t l2_spm_wfi_cmd_sequence[] __initdata = {
	0x00, 0x20, 0x03, 0x20,
	0x00, 0x0f,
};

static uint8_t l2_spm_gdhs_cmd_sequence[] __initdata = {
	0x00, 0x20, 0x34, 0x64,
	0x48, 0x07, 0x48, 0x20,
	0x50, 0x64, 0x04, 0x34,
	0x50, 0x0f,
};
static uint8_t l2_spm_power_off_cmd_sequence[] __initdata = {
	0x00, 0x10, 0x34, 0x64,
	0x48, 0x07, 0x48, 0x10,
	0x50, 0x64, 0x04, 0x34,
	0x50, 0x0F,
};

static struct msm_spm_seq_entry msm_spm_l2_seq_list[] __initdata = {
	[0] = {
		.mode = MSM_SPM_L2_MODE_RETENTION,
		.notify_rpm = false,
		.cmd = l2_spm_wfi_cmd_sequence,
	},
	[1] = {
		.mode = MSM_SPM_L2_MODE_GDHS,
		.notify_rpm = true,
		.cmd = l2_spm_gdhs_cmd_sequence,
	},
	[2] = {
		.mode = MSM_SPM_L2_MODE_POWER_COLLAPSE,
		.notify_rpm = true,
		.cmd = l2_spm_power_off_cmd_sequence,
	},
};


static struct msm_spm_platform_data msm_spm_l2_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW_L2_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x02020204,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x00A000AE,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A00020,
		.modes = msm_spm_l2_seq_list,
		.num_modes = ARRAY_SIZE(msm_spm_l2_seq_list),
	},
};

static struct msm_spm_platform_data msm_spm_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW0_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x03020004,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0084009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A4001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_boot_cpu_seq_list),
		.modes = msm_spm_boot_cpu_seq_list,
	},
	[1] = {
		.reg_base_addr = MSM_SAW1_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x03020004,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0084009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A4001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_nonboot_cpu_seq_list),
		.modes = msm_spm_nonboot_cpu_seq_list,
	},
	[2] = {
		.reg_base_addr = MSM_SAW2_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x03020004,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0084009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A4001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_nonboot_cpu_seq_list),
		.modes = msm_spm_nonboot_cpu_seq_list,
	},
	[3] = {
		.reg_base_addr = MSM_SAW3_BASE,
		.reg_init_values[MSM_SPM_REG_SAW2_CFG] = 0x1F,
#if defined(CONFIG_MSM_AVS_HW)
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_CTL] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW2_AVS_HYSTERESIS] = 0x00,
#endif
		.reg_init_values[MSM_SPM_REG_SAW2_SPM_CTL] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DLY] = 0x03020004,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_0] = 0x0084009C,
		.reg_init_values[MSM_SPM_REG_SAW2_PMIC_DATA_1] = 0x00A4001C,
		.vctl_timeout_us = 50,
		.num_modes = ARRAY_SIZE(msm_spm_nonboot_cpu_seq_list),
		.modes = msm_spm_nonboot_cpu_seq_list,
	},
};

static void __init apq8064ab_update_krait_spm(void)
{
	int i;

	/* Update the SPM sequences for SPC and PC */
	for (i = 0; i < ARRAY_SIZE(msm_spm_data); i++) {
		int j;
		struct msm_spm_platform_data *pdata = &msm_spm_data[i];
		for (j = 0; j < pdata->num_modes; j++) {
			if (pdata->modes[j].cmd ==
					spm_power_collapse_without_rpm)
				pdata->modes[j].cmd =
				spm_power_collapse_without_rpm_krait_v3;
			else if (pdata->modes[j].cmd ==
					spm_power_collapse_with_rpm)
				pdata->modes[j].cmd =
				spm_power_collapse_with_rpm_krait_v3;
		}
	}
}

static void __init apq8064_init_buses(void)
{
	msm_bus_rpm_set_mt_mask();
	msm_bus_8064_apps_fabric_pdata.rpm_enabled = 1;
	msm_bus_8064_sys_fabric_pdata.rpm_enabled = 1;
	msm_bus_8064_mm_fabric_pdata.rpm_enabled = 1;
	msm_bus_8064_apps_fabric.dev.platform_data =
		&msm_bus_8064_apps_fabric_pdata;
	msm_bus_8064_sys_fabric.dev.platform_data =
		&msm_bus_8064_sys_fabric_pdata;
	msm_bus_8064_mm_fabric.dev.platform_data =
		&msm_bus_8064_mm_fabric_pdata;
	msm_bus_8064_sys_fpb.dev.platform_data = &msm_bus_8064_sys_fpb_pdata;
	msm_bus_8064_cpss_fpb.dev.platform_data = &msm_bus_8064_cpss_fpb_pdata;
}

/* PCIe gpios */
static struct msm_pcie_gpio_info_t msm_pcie_gpio_info[MSM_PCIE_MAX_GPIO] = {
	{"rst_n", PM8921_MPP_PM_TO_SYS(PCIE_RST_N_PMIC_MPP), 0},
	{"pwr_en", PM8921_GPIO_PM_TO_SYS(PCIE_PWR_EN_PMIC_GPIO), 1},
};

static struct msm_pcie_platform msm_pcie_platform_data = {
	.gpio = msm_pcie_gpio_info,
	.axi_addr = PCIE_AXI_BAR_PHYS,
	.axi_size = PCIE_AXI_BAR_SIZE,
	.wake_n = PM8921_GPIO_IRQ(PM8921_IRQ_BASE, PCIE_WAKE_N_PMIC_GPIO),
};

static int __init mpq8064_pcie_enabled(void)
{
	return !((readl_relaxed(QFPROM_RAW_FEAT_CONFIG_ROW0_MSB) & BIT(21)) ||
		(readl_relaxed(QFPROM_RAW_OEM_CONFIG_ROW0_LSB) & BIT(4)));
}

static void __init mpq8064_pcie_init(void)
{
	if (mpq8064_pcie_enabled()) {
		msm_device_pcie.dev.platform_data = &msm_pcie_platform_data;
		platform_device_register(&msm_device_pcie);
	}
}

#ifdef CONFIG_VENDOR_EDIT
static struct platform_device ram_console_device = {
	.name = "ram_console",
	.id = -1,
};

static struct persistent_ram_descriptor msm_prd[] __initdata = {
	{
		.name = "ram_console",
		.size = SZ_1M,
	},
};

static struct persistent_ram msm_pr __initdata = {
	.descs = msm_prd,
	.num_descs = ARRAY_SIZE(msm_prd),
	.start = 0xE0200000,//PLAT_PHYS_OFFSET + SZ_1G + SZ_512M,
	.size = SZ_1M,
};
#endif


static struct platform_device apq8064_device_ext_5v_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= PM8921_MPP_PM_TO_SYS(7),
	.dev	= {
		.platform_data
			= &apq8064_gpio_regulator_pdata[GPIO_VREG_ID_EXT_5V],
	},
};

static struct platform_device apq8064_device_ext_mpp8_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= PM8921_MPP_PM_TO_SYS(8),
	.dev	= {
		.platform_data
			= &apq8064_gpio_regulator_pdata[GPIO_VREG_ID_EXT_MPP8],
	},
};

static struct platform_device apq8064_device_ext_3p3v_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= APQ8064_EXT_3P3V_REG_EN_GPIO,
	.dev	= {
		.platform_data =
			&apq8064_gpio_regulator_pdata[GPIO_VREG_ID_EXT_3P3V],
	},
};

static struct platform_device apq8064_device_ext_ts_sw_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= PM8921_GPIO_PM_TO_SYS(23),
	.dev	= {
		.platform_data
			= &apq8064_gpio_regulator_pdata[GPIO_VREG_ID_EXT_TS_SW],
	},
};

static struct platform_device apq8064_device_rpm_regulator __devinitdata = {
	.name	= "rpm-regulator",
	.id	= 0,
	.dev	= {
		.platform_data = &apq8064_rpm_regulator_pdata,
	},
};

static struct platform_device
apq8064_pm8921_device_rpm_regulator __devinitdata = {
	.name	= "rpm-regulator",
	.id	= 1,
	.dev	= {
		.platform_data = &apq8064_rpm_regulator_pm8921_pdata,
	},
};

static struct gpio_ir_recv_platform_data gpio_ir_recv_pdata = {
	.gpio_nr = 88,
	.active_low = 1,
};

static struct platform_device gpio_ir_recv_pdev = {
	.name = "gpio-rc-recv",
	.dev = {
		.platform_data = &gpio_ir_recv_pdata,
	},
};

static struct platform_device *common_not_mpq_devices[] __initdata = {
	&apq8064_device_qup_i2c_gsbi1,
	&apq8064_device_qup_i2c_gsbi3,
};

/* OPPO 2013-02-04 kangjian Add begin for reason */
static struct platform_device *gsbi7_i2c_devices[] __initdata = {
	&apq8064_device_qup_i2c_gsbi7,
};
/* OPPO 2013-02-04 kangjian Add end */

/* OPPO 2013-07-24 sjc Add begin for reason */
static struct platform_device *gsbi5_i2c_devices[] __initdata = {
	&apq8064_device_qup_i2c_gsbi5,
};
/* OPPO 2013-07-24 sjc Add end */

static struct platform_device *early_common_devices[] __initdata = {
#ifdef CONFIG_VENDOR_EDIT
	&ram_console_device,
#endif
	&apq8064_device_acpuclk,
	&apq8064_device_dmov,
/* OPPO 2013-03-18 zhenwx  Delete begin for del unused code */
#if 0
	&apq8064_device_qup_spi_gsbi5,
#endif
/* OPPO 2013-03-18 zhenwx  Delete end */	
};

static struct platform_device *pm8921_common_devices[] __initdata = {
	&apq8064_device_ext_5v_vreg,
	&apq8064_device_ext_mpp8_vreg,
	&apq8064_device_ext_3p3v_vreg,
	&apq8064_device_ssbi_pmic1,
	&apq8064_device_ssbi_pmic2,
	&apq8064_device_ext_ts_sw_vreg,
};

static struct platform_device *pm8917_common_devices[] __initdata = {
	&apq8064_device_ext_mpp8_vreg,
	&apq8064_device_ext_3p3v_vreg,
	&apq8064_device_ssbi_pmic1,
	&apq8064_device_ssbi_pmic2,
	&apq8064_device_ext_ts_sw_vreg,
};

static struct platform_device *common_devices[] __initdata = {
	&msm_device_smd_apq8064,
	&apq8064_device_otg,
	&apq8064_device_gadget_peripheral,
	&apq8064_device_hsusb_host,
	&android_usb_device,
	&msm_device_wcnss_wlan,
	&msm_device_iris_fm,
	&apq8064_fmem_device,
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	&apq8064_android_pmem_device,
	&apq8064_android_pmem_adsp_device,
	&apq8064_android_pmem_audio_device,
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/
#ifdef CONFIG_ION_MSM
	&apq8064_ion_dev,
#endif
	&msm8064_device_watchdog,
	&msm8064_device_saw_regulator_core0,
	&msm8064_device_saw_regulator_core1,
	&msm8064_device_saw_regulator_core2,
	&msm8064_device_saw_regulator_core3,
#if defined(CONFIG_QSEECOM)
	&qseecom_device,
#endif

	&msm_8064_device_tsif[0],
	&msm_8064_device_tsif[1],

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)
	&qcrypto_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)
	&qcedev_device,
#endif

#ifdef CONFIG_HW_RANDOM_MSM
	&apq8064_device_rng,
#endif
	&apq_pcm,
	&apq_pcm_routing,
	&apq_cpudai0,
	&apq_cpudai1,
	&mpq_cpudai_sec_i2s_rx,
	&mpq_cpudai_mi2s_tx,
	&apq_cpudai_hdmi_rx,
	&apq_cpudai_bt_rx,
	&apq_cpudai_bt_tx,
	&apq_cpudai_fm_rx,
	&apq_cpudai_fm_tx,
	&apq_cpu_fe,
	&apq_stub_codec,
	&apq_voice,
	&apq_voip,
	&apq_lpa_pcm,
	&apq_compr_dsp,
	&apq_multi_ch_pcm,
	&apq_lowlatency_pcm,
	&apq_pcm_hostless,
	&apq_cpudai_afe_01_rx,
	&apq_cpudai_afe_01_tx,
	&apq_cpudai_afe_02_rx,
	&apq_cpudai_afe_02_tx,
	&apq_pcm_afe,
	&apq_cpudai_auxpcm_rx,
	&apq_cpudai_auxpcm_tx,
	&apq_cpudai_stub,
	&apq_cpudai_slimbus_1_rx,
	&apq_cpudai_slimbus_1_tx,
	&apq_cpudai_slimbus_2_rx,
	&apq_cpudai_slimbus_2_tx,
	&apq_cpudai_slimbus_3_rx,
	&apq_cpudai_slimbus_3_tx,
	&apq8064_rpm_device,
	&apq8064_rpm_log_device,
	&apq8064_rpm_stat_device,
	&apq8064_rpm_master_stat_device,
	&apq_device_tz_log,
	&msm_bus_8064_apps_fabric,
	&msm_bus_8064_sys_fabric,
	&msm_bus_8064_mm_fabric,
	&msm_bus_8064_sys_fpb,
	&msm_bus_8064_cpss_fpb,
	&apq8064_msm_device_vidc,
	&msm_pil_dsps,
	&msm_8960_q6_lpass,
	&msm_pil_vidc,
	&msm_gss,
	&apq8064_rtb_device,
	&apq8064_dcvs_device,
	&apq8064_msm_gov_device,
	&apq8064_device_cache_erp,
/* delete by zhengzk on 2013-03-18 start */
#if 0
	&msm8960_device_ebi1_ch0_erp,
	&msm8960_device_ebi1_ch1_erp,
#endif
/* delete by zhengzk on 2013-03-18 end */
	&epm_adc_device,
	&coresight_tpiu_device,
	&coresight_etb_device,
	&apq8064_coresight_funnel_device,
	&coresight_etm0_device,
	&coresight_etm1_device,
	&coresight_etm2_device,
	&coresight_etm3_device,
	&apq_cpudai_slim_4_rx,
	&apq_cpudai_slim_4_tx,
#ifdef CONFIG_MSM_GEMINI
	&msm8960_gemini_device,
#endif
	&apq8064_iommu_domain_device,
	&msm_tsens_device,
	&apq8064_cache_dump_device,
	&msm_8064_device_tspp,
#ifdef CONFIG_BATTERY_BCL
	&battery_bcl_device,
#endif
	&apq8064_msm_mpd_device,
};

static struct platform_device *cdp_devices[] __initdata = {
/* OPPO 2013-03-18 zhenwx  Modify begin for uart debug */
#if 0
	&apq8064_device_uart_gsbi1,
	&apq8064_device_uart_gsbi7,
#else
	&apq8064_device_uart_gsbi5,	
#endif
/* OPPO 2013-03-18 zhenwx   Modify end */
	&msm_device_sps_apq8064,
#ifdef CONFIG_MSM_ROTATOR
	&msm_rotator_device,
#endif
	&msm8064_pc_cntr,
};

static struct platform_device
mpq8064_device_ext_1p2_buck_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= SX150X_GPIO(4, 2),
	.dev	= {
		.platform_data =
		 &mpq8064_gpio_regulator_pdata[GPIO_VREG_ID_AVC_1P2V],
	},
};

static struct platform_device
mpq8064_device_ext_1p8_buck_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= SX150X_GPIO(4, 4),
	.dev	= {
		.platform_data =
		&mpq8064_gpio_regulator_pdata[GPIO_VREG_ID_AVC_1P8V],
	},
};

static struct platform_device
mpq8064_device_ext_2p2_buck_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= SX150X_GPIO(4, 14),
	.dev	= {
		.platform_data =
		&mpq8064_gpio_regulator_pdata[GPIO_VREG_ID_AVC_2P2V],
	},
};

static struct platform_device
mpq8064_device_ext_5v_buck_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= SX150X_GPIO(4, 3),
	.dev	= {
		.platform_data =
		 &mpq8064_gpio_regulator_pdata[GPIO_VREG_ID_AVC_5V],
	},
};

static struct platform_device
mpq8064_device_ext_3p3v_ldo_vreg __devinitdata = {
	.name	= GPIO_REGULATOR_DEV_NAME,
	.id	= SX150X_GPIO(4, 15),
	.dev	= {
		.platform_data =
		&mpq8064_gpio_regulator_pdata[GPIO_VREG_ID_AVC_3P3V],
	},
};

static struct platform_device rc_input_loopback_pdev = {
	.name	= "rc-user-input",
	.id	= -1,
};

static int rf4ce_gpio_init(void)
{
	if (!machine_is_mpq8064_cdp() &&
		!machine_is_mpq8064_hrd() &&
			!machine_is_mpq8064_dtv())
		return -EINVAL;

	/* CC2533 SRDY Input */
	if (!gpio_request(SX150X_GPIO(4, 6), "rf4ce_srdy")) {
		gpio_direction_input(SX150X_GPIO(4, 6));
		gpio_export(SX150X_GPIO(4, 6), true);
	}

	/* CC2533 MRDY Output */
	if (!gpio_request(SX150X_GPIO(4, 5), "rf4ce_mrdy")) {
		gpio_direction_output(SX150X_GPIO(4, 5), 1);
		gpio_export(SX150X_GPIO(4, 5), true);
	}

	/* CC2533 Reset Output */
	if (!gpio_request(SX150X_GPIO(4, 7), "rf4ce_reset")) {
		gpio_direction_output(SX150X_GPIO(4, 7), 0);
		gpio_export(SX150X_GPIO(4, 7), true);
	}

	return 0;
}
late_initcall(rf4ce_gpio_init);

static struct platform_device *mpq_devices[] __initdata = {
	&msm_device_sps_apq8064,
	&mpq8064_device_qup_i2c_gsbi5,
#ifdef CONFIG_MSM_ROTATOR
	&msm_rotator_device,
#endif
	&gpio_ir_recv_pdev,
	&mpq8064_device_ext_1p2_buck_vreg,
	&mpq8064_device_ext_1p8_buck_vreg,
	&mpq8064_device_ext_2p2_buck_vreg,
	&mpq8064_device_ext_5v_buck_vreg,
	&mpq8064_device_ext_3p3v_ldo_vreg,
#ifdef CONFIG_MSM_VCAP
	&msm8064_device_vcap,
#endif
	&rc_input_loopback_pdev,
};


static struct msm_spi_platform_data apq8064_qup_spi_gsbi5_pdata = {
	.max_clock_speed = 1100000,
};
/* OPPO 2013-07-24 liubin Add for spi gsbi4 start */
static struct msm_spi_platform_data apq8064_qup_spi_gsbi4_pdata = {
	.max_clock_speed = 24000000,
};
/* OPPO 2013-07-24 liubin Add end */


#define KS8851_IRQ_GPIO		43

static struct spi_board_info spi_board_info[] __initdata = {
	{
		.modalias               = "ks8851",
		.irq                    = MSM_GPIO_TO_INT(KS8851_IRQ_GPIO),
		.max_speed_hz           = 19200000,
		.bus_num                = 0,
		.chip_select            = 2,
		.mode                   = SPI_MODE_0,
	},
	{
		.modalias		= "epm_adc",
		.max_speed_hz		= 1100000,
		.bus_num		= 0,
		.chip_select		= 3,
		.mode			= SPI_MODE_0,
	},
	
	/* OPPO 2013-07-24 liubin Add for m9mo_spi board info start */
	{
		.modalias			= "m9mo_spi",
		.max_speed_hz		= 10000000,
		.bus_num			= 0,
		.chip_select		= 0,
		.mode				= SPI_MODE_0,
	},
	/* OPPO 2013-07-24 liubin Add end */
};

static struct slim_boardinfo apq8064_slim_devices[] = {
	{
		.bus_num = 1,
		.slim_slave = &apq8064_slim_tabla,
	},
	{
		.bus_num = 1,
		.slim_slave = &apq8064_slim_tabla20,
	},
	/* add more slimbus slaves as needed */
};

static struct msm_i2c_platform_data apq8064_i2c_qup_gsbi1_pdata = {
	.clk_freq = 245000,/*100000,   //changed by yubin, oppo*/
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data apq8064_i2c_qup_gsbi3_pdata = {
	.clk_freq = 384000,
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data apq8064_i2c_qup_gsbi4_pdata = {
	.clk_freq = 384000,//100000,/*OPPO*/
	.src_clk_rate = 24000000,
};

static struct msm_i2c_platform_data mpq8064_i2c_qup_gsbi5_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};

/* OPPO 2013-02-04 kangjian Add begin for reason */
static struct msm_i2c_platform_data apq8064_i2c_qup_gsbi7_pdata = {
	.clk_freq = 384000,//100000,/*OPPO*/
	.src_clk_rate = 24000000,
};
/* OPPO 2013-02-04 kangjian Add end */

/* OPPO 2013-07-24 sjc Add begin for reason */
static struct msm_i2c_platform_data apq8064_i2c_qup_gsbi5_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
};
/* OPPO 2013-07-24 sjc Add end */

#define GSBI_DUAL_MODE_CODE 0x60
#define MSM_GSBI1_PHYS		0x12440000
static void __init apq8064_i2c_init(void)
{
	void __iomem *gsbi_mem;

	apq8064_device_qup_i2c_gsbi1.dev.platform_data =
					&apq8064_i2c_qup_gsbi1_pdata;
	gsbi_mem = ioremap_nocache(MSM_GSBI1_PHYS, 4);
	writel_relaxed(GSBI_DUAL_MODE_CODE, gsbi_mem);
	/* Ensure protocol code is written before proceeding */
	wmb();
	iounmap(gsbi_mem);
	apq8064_i2c_qup_gsbi1_pdata.use_gsbi_shared_mode = 1;
	apq8064_device_qup_i2c_gsbi3.dev.platform_data =
					&apq8064_i2c_qup_gsbi3_pdata;
	apq8064_device_qup_i2c_gsbi1.dev.platform_data =
					&apq8064_i2c_qup_gsbi1_pdata;

	/* Add GSBI4 I2C pdata for non-fusion3 SGLTE2 */
	if (socinfo_get_platform_subtype() !=
				PLATFORM_SUBTYPE_SGLTE2) {
		apq8064_device_qup_i2c_gsbi4.dev.platform_data =
					&apq8064_i2c_qup_gsbi4_pdata;
		}
/* OPPO 2013-02-04 kangjian Add begin for s5k6a3yx's I2C */
	if (get_pcb_version() >= PCB_VERSION_DVT) {
    		apq8064_device_qup_i2c_gsbi7.dev.platform_data =
					&apq8064_i2c_qup_gsbi7_pdata;
	}
/* OPPO 2013-02-04 kangjian Add end */

/* OPPO 2013-07-24 sjc Add begin for bq27541 I2C */
	if (get_pcb_version() >= PCB_VERSION_EVT_N1) {
    		apq8064_device_qup_i2c_gsbi5.dev.platform_data =
					&apq8064_i2c_qup_gsbi5_pdata;
	}
/* OPPO 2013-07-24 sjc Add end */

	mpq8064_device_qup_i2c_gsbi5.dev.platform_data =
					&mpq8064_i2c_qup_gsbi5_pdata;
}

#if defined(CONFIG_KS8851) || defined(CONFIG_KS8851_MODULE)
static int ethernet_init(void)
{
/* OPPO 2013-07-23 ranfei Delete begin for reason */
#if 0
	int ret;
	ret = gpio_request(KS8851_IRQ_GPIO, "ks8851_irq");
	if (ret) {
		pr_err("ks8851 gpio_request failed: %d\n", ret);
		goto fail;
	}
#endif
/* OPPO 2013-07-23 ranfei Delete end */

	return 0;
/* OPPO 2013-07-23 ranfei Delete begin for reason */
#if 0
fail:
	return ret;
#endif
/* OPPO 2013-07-23 ranfei Delete end */
}
#else
static int ethernet_init(void)
{
	return 0;
}
#endif

#ifndef CONFIG_VENDOR_EDIT
// LiuJun@OnlineRD.Driver.Key, 2012/07/24, Modify for re-define keys
#define GPIO_KEY_HOME			PM8921_GPIO_PM_TO_SYS(27)
#define GPIO_KEY_VOLUME_UP		PM8921_GPIO_PM_TO_SYS(35)
#define GPIO_KEY_VOLUME_DOWN_PM8921	PM8921_GPIO_PM_TO_SYS(38)
#define GPIO_KEY_VOLUME_DOWN_PM8917	PM8921_GPIO_PM_TO_SYS(30)
#define GPIO_KEY_CAM_FOCUS		PM8921_GPIO_PM_TO_SYS(3)
#define GPIO_KEY_CAM_SNAP		PM8921_GPIO_PM_TO_SYS(4)
#define GPIO_KEY_ROTATION_PM8921	PM8921_GPIO_PM_TO_SYS(42)
#define GPIO_KEY_ROTATION_PM8917	PM8921_GPIO_PM_TO_SYS(8)

static struct gpio_keys_button cdp_keys_pm8921[] = {
	{
		.code           = KEY_HOME,
		.gpio           = GPIO_KEY_HOME,
		.desc           = "home_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = KEY_VOLUMEUP,
		.gpio           = GPIO_KEY_VOLUME_UP,
		.desc           = "volume_up_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = KEY_VOLUMEDOWN,
		.gpio           = GPIO_KEY_VOLUME_DOWN_PM8921,
		.desc           = "volume_down_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = SW_ROTATE_LOCK,
		.gpio           = GPIO_KEY_ROTATION_PM8921,
		.desc           = "rotate_key",
		.active_low     = 1,
		.type		= EV_SW,
		.debounce_interval = 15,
	},
};

static struct gpio_keys_button cdp_keys_pm8917[] = {
	{
		.code           = KEY_HOME,
		.gpio           = GPIO_KEY_HOME,
		.desc           = "home_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = KEY_VOLUMEUP,
		.gpio           = GPIO_KEY_VOLUME_UP,
		.desc           = "volume_up_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = KEY_VOLUMEDOWN,
		.gpio           = GPIO_KEY_VOLUME_DOWN_PM8917,
		.desc           = "volume_down_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = SW_ROTATE_LOCK,
		.gpio           = GPIO_KEY_ROTATION_PM8917,
		.desc           = "rotate_key",
		.active_low     = 1,
		.type		= EV_SW,
		.debounce_interval = 15,
	},
};

static struct gpio_keys_platform_data cdp_keys_data = {
	.buttons        = cdp_keys_pm8921,
	.nbuttons       = ARRAY_SIZE(cdp_keys_pm8921),
};

static struct platform_device cdp_kp_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &cdp_keys_data,
	},
};

static struct gpio_keys_button mtp_keys[] = {
	{
		.code           = KEY_CAMERA_FOCUS,
		.gpio           = GPIO_KEY_CAM_FOCUS,
		.desc           = "cam_focus_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = KEY_VOLUMEUP,
		.gpio           = GPIO_KEY_VOLUME_UP,
		.desc           = "volume_up_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = KEY_VOLUMEDOWN,
		.gpio           = GPIO_KEY_VOLUME_DOWN_PM8921,
		.desc           = "volume_down_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = KEY_CAMERA_SNAPSHOT,
		.gpio           = GPIO_KEY_CAM_SNAP,
		.desc           = "cam_snap_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.debounce_interval = 15,
	},
};

static struct gpio_keys_platform_data mtp_keys_data = {
	.buttons        = mtp_keys,
	.nbuttons       = ARRAY_SIZE(mtp_keys),
};

static struct platform_device mtp_kp_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &mtp_keys_data,
	},
};

static struct gpio_keys_button mpq_keys[] = {
	{
		.code           = KEY_VOLUMEDOWN,
		.gpio           = GPIO_KEY_VOLUME_DOWN_PM8921,
		.desc           = "volume_down_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
	{
		.code           = KEY_VOLUMEUP,
		.gpio           = GPIO_KEY_VOLUME_UP,
		.desc           = "volume_up_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1,
		.debounce_interval = 15,
	},
};

static struct gpio_keys_platform_data mpq_keys_data = {
	.buttons        = mpq_keys,
	.nbuttons       = ARRAY_SIZE(mpq_keys),
};

static struct platform_device mpq_gpio_keys_pdev = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &mpq_keys_data,
	},
};

#define MPQ_KP_ROW_BASE		SX150X_EXP2_GPIO_BASE
#define MPQ_KP_COL_BASE		(SX150X_EXP2_GPIO_BASE + 4)

static unsigned int mpq_row_gpios[] = {MPQ_KP_ROW_BASE, MPQ_KP_ROW_BASE + 1,
				MPQ_KP_ROW_BASE + 2, MPQ_KP_ROW_BASE + 3};
static unsigned int mpq_col_gpios[] = {MPQ_KP_COL_BASE, MPQ_KP_COL_BASE + 1,
				MPQ_KP_COL_BASE + 2};

static const unsigned int mpq_keymap[] = {
	KEY(0, 0, KEY_UP),
	KEY(0, 1, KEY_ENTER),
	KEY(0, 2, KEY_3),

	KEY(1, 0, KEY_DOWN),
	KEY(1, 1, KEY_EXIT),
	KEY(1, 2, KEY_4),

	KEY(2, 0, KEY_LEFT),
	KEY(2, 1, KEY_1),
	KEY(2, 2, KEY_5),

	KEY(3, 0, KEY_RIGHT),
	KEY(3, 1, KEY_2),
	KEY(3, 2, KEY_6),
};

static struct matrix_keymap_data mpq_keymap_data = {
	.keymap_size	= ARRAY_SIZE(mpq_keymap),
	.keymap		= mpq_keymap,
};

static struct matrix_keypad_platform_data mpq_keypad_data = {
	.keymap_data		= &mpq_keymap_data,
	.row_gpios		= mpq_row_gpios,
	.col_gpios		= mpq_col_gpios,
	.num_row_gpios		= ARRAY_SIZE(mpq_row_gpios),
	.num_col_gpios		= ARRAY_SIZE(mpq_col_gpios),
	.col_scan_delay_us	= 32000,
	.debounce_ms		= 20,
	.wakeup			= 1,
	.active_low		= 1,
	.no_autorepeat		= 1,
};

static struct platform_device mpq_keypad_device = {
	.name           = "matrix-keypad",
	.id             = -1,
	.dev            = {
		.platform_data  = &mpq_keypad_data,
	},
};

#else  /* VENDOR_EDIT */

#define GPIO_KEY_VOLUME_UP_EVB		(57)
#define GPIO_KEY_VOLUME_DOWN_EVB	(59)
#define GPIO_KEY_VOLUME_UP		(29)
#define GPIO_KEY_VOLUME_DOWN	(32)

/* OPPO 2012-10-29 wangjw Add begin for hall sensor gpio 23*/
#define GPIO_KEY_HALLSENSOR (23)
#define GPIO_KEY_HALLSENSOR_PVT (44)
/* OPPO 2012-10-29 wangjw Add end */
/* OPPO 2013-07-23 ranfei Add begin for reason */
#define GPIO_HALLSENSOR_N1  (15)
#define GPIO_KEY_MR1        (43)
#define GPIO_KEY_MR2        (44)
/* OPPO 2013-07-23 ranfei Add end */
/*yuyi add begin*/
#define GPIO_BUTTON_BACK  PM8921_GPIO_PM_TO_SYS(26)
#define GPIO_BUTTON_HOME  PM8921_GPIO_PM_TO_SYS(27)
#define GPIO_BUTTON_MENU  PM8921_GPIO_PM_TO_SYS(28)

/*yuyi add end*/
struct gpio_keys_button vol_keys[] = {
	{
		.code              = KEY_VOLUMEDOWN,
		.gpio              = GPIO_KEY_VOLUME_DOWN,
		.active_low        = 0,
		.desc              = "volume_down_key",
		.type              = EV_KEY,
		.wakeup            = 1,
		.debounce_interval = 20,
		.can_disable       = true,
	},
	{
		.code              = KEY_VOLUMEUP,
		.gpio              = GPIO_KEY_VOLUME_UP,
		.active_low        = 0,
		.desc              = "volume_up_key",
		.type              = EV_KEY,
		.wakeup            = 1,
		.debounce_interval = 20,
		.can_disable       = true,
	},
	{
		.code              = SW_LID,
		.active_low        = 1,
		.desc              = "hallsensor_key",
		.type              = EV_SW,
		.wakeup            = 1,
		.debounce_interval = 20,
		.can_disable       = true,
	},
};

struct gpio_keys_platform_data gpio_keys_pdata = {
	.buttons = vol_keys,
	.nbuttons = ARRAY_SIZE(vol_keys),
};
	
static struct platform_device gpio_keys_device ={
	.name = "gpio-keys",
	.id = -1,
	.dev = {
		.platform_data = &gpio_keys_pdata,
	},
};

/* OPPO 2013-07-26 ranfei Add begin for reason */
struct gpio_keys_button vol_keys_n1[] = {
	{
		.code              = KEY_VOLUMEDOWN,
		.gpio              = GPIO_KEY_VOLUME_DOWN,
		.active_low        = 1,
		.desc              = "volume_down_key",
		.type              = EV_KEY,
		.wakeup            = 0,
		.debounce_interval = 20,
		.can_disable       = true,
	},
	{
		.code              = KEY_VOLUMEUP,
		.gpio              = GPIO_KEY_VOLUME_UP,
		.active_low        = 0,
		.desc              = "volume_up_key",
		.type              = EV_KEY,
		.wakeup            = 0,
		.debounce_interval = 20,
		.can_disable       = true,
	},
	{
		.code              = SW_LID,
        .gpio              = GPIO_HALLSENSOR_N1,
		.active_low        = 1,
		.desc              = "hallsensor_key",
		.type              = EV_SW,
		.wakeup            = 1,
		.debounce_interval = 20,
		.can_disable       = true,
	},
	{
		.code              = KEY_BACK,
		.gpio              = GPIO_BUTTON_BACK,
		.active_low        = 1,
		.desc              = "button_back",
		.type              = EV_KEY,
		.wakeup            = 0,
		.debounce_interval = 20,
		.can_disable       = true,
	},
	{
		.code              = KEY_HOME,
		.gpio              = GPIO_BUTTON_HOME,
		.active_low        = 1,
		.desc              = "button_home",
		.type              = EV_KEY,
		.wakeup            = 0,
		.debounce_interval = 20,
		.can_disable       = true,
	},
	{
		.code              = KEY_MENU ,
		.gpio              = GPIO_BUTTON_MENU,
		.active_low        = 1,
		.desc              = "button_menu",
		.type              = EV_KEY,
		.wakeup            = 0,
		.debounce_interval = 20,
		.can_disable       = true,
	},
};

struct gpio_keys_platform_data gpio_keys_pdata_n1 = {
	.buttons = vol_keys_n1,
	.nbuttons = ARRAY_SIZE(vol_keys_n1),
};

static struct platform_device gpio_keys_device_n1 ={
	.name = "gpio-keys",
	.id = -1,
	.dev = {
		.platform_data = &gpio_keys_pdata_n1,
	},
};
/* OPPO 2013-07-26 ranfei Add end */

/* OPPO 2013-08-06 liubin Add for camera rotate key start */
struct gpio_keys_button camera_rotate_keys[] = {
	{
		.code              = KEY_F1,
        .gpio              = GPIO_KEY_MR1,
		.active_low        = 1,
		.desc              = "MRsensor_key1",
		.type              = EV_KEY,
		.wakeup            = 0,
		.debounce_interval = 20,
		.can_disable       = true,
	},
    {
		.code              = KEY_F1,
        .gpio              = GPIO_KEY_MR2,
		.active_low        = 1,
		.desc              = "MRsensor_key2",
		.type              = EV_KEY,
		.wakeup            = 1,
		.debounce_interval = 20,
		.can_disable       = true,
	}
};
struct gpio_keys_platform_data camera_rotate_keys_pdata = {
	.buttons = camera_rotate_keys,
	.nbuttons = ARRAY_SIZE(camera_rotate_keys),
};
static struct platform_device camera_rotate_keys_device ={
	.name = "camera-rotate-keys",
	.id = -1,
	.dev = {
		.platform_data = &camera_rotate_keys_pdata,
	},
};
/* OPPO 2013-07-24 liubin Add end */

static void apq8064_init_gpio_key(void)
{
	if (get_pcb_version() < PCB_VERSION_EVT)
	{
		vol_keys[0].gpio = GPIO_KEY_VOLUME_DOWN_EVB;
		vol_keys[1].gpio = GPIO_KEY_VOLUME_UP_EVB;
	}
	if (get_pcb_version() < PCB_VERSION_DVT) {
		// volume down key in evt, default set pull down
		gpio_tlmm_config(GPIO_CFG(vol_keys[0].gpio, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA), GPIO_CFG_ENABLE);
	} else {
		// volume down key in dvt, default set pull up
		gpio_tlmm_config(GPIO_CFG(vol_keys[0].gpio, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_8MA), GPIO_CFG_ENABLE);
		vol_keys[0].active_low = 1;
	}
	if(get_pcb_version() < PCB_VERSION_PVT){
		vol_keys[2].gpio = GPIO_KEY_HALLSENSOR;
	} else{
		vol_keys[2].gpio = GPIO_KEY_HALLSENSOR_PVT;
	}
	gpio_tlmm_config(GPIO_CFG(vol_keys[1].gpio, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA), GPIO_CFG_ENABLE);
    gpio_tlmm_config(GPIO_CFG(vol_keys[2].gpio, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_8MA), GPIO_CFG_ENABLE);

    if(get_pcb_version() >= PCB_VERSION_EVT_N1) {
        gpio_tlmm_config(GPIO_CFG(GPIO_KEY_VOLUME_DOWN, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_8MA), GPIO_CFG_ENABLE); 
        gpio_tlmm_config(GPIO_CFG(GPIO_KEY_VOLUME_UP, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA), GPIO_CFG_ENABLE);
        gpio_tlmm_config(GPIO_CFG(GPIO_HALLSENSOR_N1, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_8MA), GPIO_CFG_ENABLE);
        platform_device_register(&gpio_keys_device_n1);

		/* OPPO 2013-08-06 liubin Add for camera rotate key start */
		gpio_tlmm_config(GPIO_CFG(GPIO_KEY_MR1, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_8MA), GPIO_CFG_ENABLE);
        gpio_tlmm_config(GPIO_CFG(GPIO_KEY_MR2, 0, GPIO_CFG_INPUT,
				GPIO_CFG_PULL_UP, GPIO_CFG_8MA), GPIO_CFG_ENABLE);
		platform_device_register(&camera_rotate_keys_device);
		/* OPPO 2013-08-06 liubin Add end */
    } else {
        platform_device_register(&gpio_keys_device);
    }
}

#endif /* VENDOR_EDIT */

/* Sensors DSPS platform data */
#define DSPS_PIL_GENERIC_NAME		"dsps"
static void __init apq8064_init_dsps(void)
{
	struct msm_dsps_platform_data *pdata =
		msm_dsps_device_8064.dev.platform_data;
	pdata->pil_name = DSPS_PIL_GENERIC_NAME;
	pdata->gpios = NULL;
	pdata->gpios_num = 0;

	platform_device_register(&msm_dsps_device_8064);
}

#define I2C_SURF 1
#define I2C_FFA  (1 << 1)
#define I2C_RUMI (1 << 2)
#define I2C_SIM  (1 << 3)
#define I2C_LIQUID (1 << 4)
#define I2C_MPQ_CDP	BIT(5)
#define I2C_MPQ_HRD	BIT(6)
#define I2C_MPQ_DTV	BIT(7)

struct i2c_registry {
	u8                     machs;
	int                    bus;
	struct i2c_board_info *info;
	int                    len;
};

#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2013-07-26 ranfei Add begin for reason */
static struct regulator *ldo121 = NULL;

static void sensor_i2c_power_init(void) {
    ldo121 = regulator_get(NULL, "8921_l21");
	if (IS_ERR(ldo121)){
		pr_err("%s: VREG ldo121 get failed\n", __func__);
		ldo121 = NULL;
	}
	if (regulator_set_voltage(ldo121, 1800000, 1800000)) {
		pr_err("%s: VREG ldo121 set voltage failed\n",	__func__);
	}
	if (regulator_enable(ldo121)) {
		pr_err("%s: VREG ldo121 enable failed\n", __func__);
	}	
}
/* OPPO 2013-07-26 ranfei Add end */
#endif

#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2013-02-20 yuyi Add begin for nfc */


#define APQ_NFC_VEN_GPIO 53  //NFC_ENABLE
#define APQ_NFC_FIRM_GPIO 54  //NFC_UPDATE
#define APQ_NFC_IRQ_GPIO 55   //NFC_IRQ
/*OPPO yuyi add for N1*/
#define APQ_NFC_VEN_GPIO_N1  37 //NFC_ENABLE
#define APQ_NFC_FIRM_GPIO_N1 31  //NFC_UPDATE
/*yuyi add end*/
 static struct regulator *ldol23;

 struct pn544_i2c_platform_data nfc_pdata  = {
		.irq_gpio = APQ_NFC_IRQ_GPIO,   //irq gpio
		.ven_gpio = APQ_NFC_VEN_GPIO,  
		.firm_gpio = APQ_NFC_FIRM_GPIO,  
};

 static struct i2c_board_info nfc_board_info[] __initdata = {
	 {
		I2C_BOARD_INFO("pn544", 0x28),
		.platform_data = &nfc_pdata,
		.irq = MSM_GPIO_TO_INT(APQ_NFC_IRQ_GPIO),
	 },
};
 
#define PN544_VEN	GPIO_CFG(APQ_NFC_VEN_GPIO, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA)
#define PN544_FIRM	GPIO_CFG(APQ_NFC_FIRM_GPIO, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA)
#define PN544_IRQ	GPIO_CFG(APQ_NFC_IRQ_GPIO, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA)

#define PN544_VEN_N1	GPIO_CFG(APQ_NFC_VEN_GPIO_N1, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA)
#define PN544_FIRM_N1	GPIO_CFG(APQ_NFC_FIRM_GPIO_N1, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA)

 static void pn544_power_init(void)
 {
	 int ret = 0  ;

	 if( get_pcb_version() >= PCB_VERSION_EVT_N1 ) 
	 {
	 	nfc_pdata.ven_gpio = APQ_NFC_VEN_GPIO_N1;
		nfc_pdata.firm_gpio = APQ_NFC_FIRM_GPIO_N1;
	 }
	 
    //l23
	ldol23 = regulator_get(NULL, "8921_l23");
	if (IS_ERR(ldol23)){
		pr_err("%s: VREG ldol23 get failed\n", __func__);
		ldol23 = NULL;
		goto ldo123_get_failed;
	}
	if (regulator_set_voltage(ldol23, 1800000, 1800000)) {
		pr_err("%s: VREG ldol23 set voltage failed\n",  __func__);
		goto ldo123_get_failed;
	}
	if (regulator_enable(ldol23)) {
		pr_err("%s: VREG ldol23 enable failed\n", __func__);
		goto ldo123_get_failed;
	}

	 //irq
	 ret = gpio_tlmm_config(PN544_IRQ, GPIO_CFG_ENABLE);
	 if (ret) {
		 printk(KERN_ERR "%s:gpio_tlmm_config(%#x)=%d\n",
				 __func__, PN544_IRQ, ret);
	 }

	 if(get_pcb_version() >= PCB_VERSION_EVT_N1) 
	{
	 	 ret = gpio_tlmm_config(PN544_VEN_N1, GPIO_CFG_ENABLE);
	 	if (ret) {
			 printk(KERN_ERR "%s:gpio_tlmm_config(%#x)=%d\n",
				 __func__, PN544_VEN_N1, ret);
	 	}	
	 
	 	gpio_set_value(APQ_NFC_VEN_GPIO_N1, 1);
		 msleep(100);
  //firmware gpio
	  	ret = gpio_tlmm_config(PN544_FIRM_N1, GPIO_CFG_ENABLE);
	 	 if (ret) {
			  printk(KERN_ERR "%s:gpio_tlmm_config(%#x)=%d\n",
				  __func__, PN544_FIRM_N1, ret);
	 	 }
	 	 gpio_set_value(APQ_NFC_FIRM_GPIO_N1, 0);
	} else
	{
	 //ven 
	 	ret = gpio_tlmm_config(PN544_VEN, GPIO_CFG_ENABLE);
		 if (ret) {
			 printk(KERN_ERR "%s:gpio_tlmm_config(%#x)=%d\n",
				 __func__, PN544_VEN, ret);
		 }
	 
		 gpio_set_value(APQ_NFC_VEN_GPIO, 1);
		 msleep(100);
  //firmware gpio
	 	 ret = gpio_tlmm_config(PN544_FIRM, GPIO_CFG_ENABLE);
		  if (ret) {
			  printk(KERN_ERR "%s:gpio_tlmm_config(%#x)=%d\n",
				  __func__, PN544_FIRM, ret);
	 	 }
	 	 gpio_set_value(APQ_NFC_FIRM_GPIO, 0);
	}
	  
	  printk(KERN_ERR "%s:liuhd for nfc gpio---\n",__func__);
	  //yuyi add 
	  printk("nfc gpio , gpios=%d %d %d\n", 
				nfc_pdata.irq_gpio, nfc_pdata.ven_gpio, nfc_pdata.firm_gpio);

 ldo123_get_failed:
	 regulator_disable(ldol23);

 }
#endif


/* OPPO 2013-02-20 yuyi Add end */

//OPPO 2013-02-21 yuyi add begin for breathe led
#ifdef CONFIG_VENDOR_EDIT
#define APQ_SLED_SDB_GPIO 82  //NFC_UPDATE
 static struct i2c_board_info sled_board_info[] __initdata = {
	 {
		I2C_BOARD_INFO("SN3193", 0x68),
	 },
};

#define SN3193_SDB	GPIO_CFG(APQ_SLED_SDB_GPIO, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA)

static void SN3193_power_init(void)
{
	int ret = 0  ;	
 //firmware gpio
	 ret = gpio_tlmm_config(SN3193_SDB, GPIO_CFG_ENABLE);
	 if (ret) {
		 printk(KERN_ERR "%s:gpio_tlmm_config(%#x)=%d\n",
			 __func__, SN3193_SDB, ret);
	 }
	 gpio_set_value(APQ_SLED_SDB_GPIO, 1);
	 
	 printk(KERN_ERR "%s:yuyi for SN3193_SDB gpio---\n",__func__);
}
#endif
//OPPO 2013-02-21 yuyi add end

//OPPO 2012-10-23 huyu add for lcd compatible
static struct i2c_board_info lcd_1080p_info[] = {

	{
		I2C_BOARD_INFO("lm3630", 0x38),
	},

	{
		I2C_BOARD_INFO("lm3528", 0x36),
	},
	
};

/* OPPO 2013-07-31 Neal Add begin bkl*/
static struct i2c_board_info lcd_1080p_info_N1[] = {

	{
		I2C_BOARD_INFO("lm3630", 0x38),
	},
};
/* OPPO 2013-07-31 Neal Add begin bkl*/

static struct i2c_registry lcd_1080p_i2c_devices[] __initdata = {
 {
	 I2C_SURF | I2C_LIQUID | I2C_FFA |I2C_MPQ_CDP |I2C_RUMI |I2C_MPQ_HRD | I2C_MPQ_DTV,
	 APQ_8064_GSBI3_QUP_I2C_BUS_ID,
	 lcd_1080p_info,
	 ARRAY_SIZE(lcd_1080p_info),
 },
};
/* OPPO 2013-07-31 Neal Add begin bkl*/
static struct i2c_registry lcd_1080p_i2c_devices_N1[] __initdata = {
 {
	 I2C_SURF | I2C_LIQUID | I2C_FFA |I2C_MPQ_CDP |I2C_RUMI |I2C_MPQ_HRD | I2C_MPQ_DTV,
	 APQ_8064_GSBI3_QUP_I2C_BUS_ID,
	 lcd_1080p_info_N1,
	 ARRAY_SIZE(lcd_1080p_info_N1),
 },
};
/* OPPO 2013-07-31 Neal Add end*/

static void register_lcd_1080p_i2c_devices(void) {
	 printk("----%s: register lcd device is 1080p --\n", __func__);
/* OPPO 2013-07-31 Neal Add begin bkl*/
	 if(get_pcb_version() >= PCB_VERSION_EVT_N1){
			i2c_register_board_info(lcd_1080p_i2c_devices_N1[0].bus,
		 	lcd_1080p_i2c_devices_N1[0].info, lcd_1080p_i2c_devices_N1[0].len);
	 }
	 else{
	 	
	 		i2c_register_board_info(lcd_1080p_i2c_devices[0].bus,
		 	lcd_1080p_i2c_devices[0].info, lcd_1080p_i2c_devices[0].len);
	 }
/* OPPO 2013-07-31 Neal Add begin bkl*/
}
//OPPO 2012-10-23 huyu add for lcd compatible
static struct i2c_registry apq8064_i2c_devices[] __initdata = {
	{
		I2C_LIQUID,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		smb349_charger_i2c_info,
		ARRAY_SIZE(smb349_charger_i2c_info)
	},
/* OPPO 2012-09-08 liujun Delete begin for disable original touchscreen define */
#ifndef CONFIG_VENDOR_EDIT
	{
		I2C_SURF | I2C_LIQUID,
		APQ_8064_GSBI3_QUP_I2C_BUS_ID,
		mxt_device_info,
		ARRAY_SIZE(mxt_device_info),
	},
	{
		I2C_FFA,
		APQ_8064_GSBI3_QUP_I2C_BUS_ID,
		cyttsp_info,
		ARRAY_SIZE(cyttsp_info),
	},
#else
/* OPPO 2012-09-08 liujun Delete end */
/* OPPO 2012-07-31 liujun Add begin for define touchscreen data */
#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_S3202_I2C_RMI
	{
		I2C_SURF | I2C_LIQUID | I2C_FFA,
		APQ_8064_GSBI3_QUP_I2C_BUS_ID,
		synaptics_s3202_touch_info,
		ARRAY_SIZE(synaptics_s3202_touch_info),
	},
#endif
#endif
/* OPPO 2012-07-31 liujun Add end */
	{
		I2C_FFA | I2C_LIQUID,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		isa1200_board_info,
		ARRAY_SIZE(isa1200_board_info),
	},
	{
		I2C_MPQ_CDP,
		APQ_8064_GSBI5_QUP_I2C_BUS_ID,
		cs8427_device_info,
		ARRAY_SIZE(cs8427_device_info),
	},
#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2013-02-20 yuyi Add begin for reason */


	{
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		nfc_board_info,
		ARRAY_SIZE(nfc_board_info),
	},


/* OPPO 2013-02-20 yuyi Add end */
#endif
/* OPPO 2013-02-21 yuyi Add begin for breathe led */
#ifdef CONFIG_VENDOR_EDIT
	{
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		sled_board_info,
		ARRAY_SIZE(sled_board_info),
	},
#endif
/* OPPO 2013-02-21 yuyi Add end */
};

static struct i2c_registry apq8064_i2c_devices_n1[] __initdata = {
/* OPPO 2013-07-08 sjc Add begin for reason */
	{
		I2C_LIQUID,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		smb349_charger_i2c_info,
		ARRAY_SIZE(smb349_charger_i2c_info),
	},
	{
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		smb358_charger_i2c_info,
		ARRAY_SIZE(smb358_charger_i2c_info),
	},
	//sjc add 2013-07-23
	{
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI5_QUP_I2C_BUS_ID,
		bq27541_charger_i2c_info,
		ARRAY_SIZE(bq27541_charger_i2c_info),
	},
/* OPPO 2013-07-08 sjc Add end */

/* OPPO 2012-09-08 liujun Delete begin for disable original touchscreen define */
#ifndef CONFIG_VENDOR_EDIT
	{
		I2C_SURF | I2C_LIQUID,
		APQ_8064_GSBI3_QUP_I2C_BUS_ID,
		mxt_device_info,
		ARRAY_SIZE(mxt_device_info),
	},
	{
		I2C_FFA,
		APQ_8064_GSBI3_QUP_I2C_BUS_ID,
		cyttsp_info,
		ARRAY_SIZE(cyttsp_info),
	},
#else
/* OPPO 2012-09-08 liujun Delete end */
/* OPPO 2012-07-31 liujun Add begin for define touchscreen data */
#ifdef CONFIG_TOUCHSCREEN_SYNAPTICS_S3202_I2C_RMI
	{
		I2C_SURF | I2C_LIQUID | I2C_FFA,
		APQ_8064_GSBI3_QUP_I2C_BUS_ID,
		synaptics_s3202_touch_info,
		ARRAY_SIZE(synaptics_s3202_touch_info),
	},
#endif
#endif
#ifdef CONFIG_VENDOR_EDIT
//yanghai add for backgroud touch 
	 {
		 I2C_SURF | I2C_LIQUID | I2C_FFA,
		 APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		 synaptics_y8c20x66a_touch_info,
		 ARRAY_SIZE(synaptics_y8c20x66a_touch_info),
	 },
//yanghai add end
#endif

/* OPPO 2012-07-31 liujun Add end */
	{
		I2C_FFA | I2C_LIQUID,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		isa1200_board_info,
		ARRAY_SIZE(isa1200_board_info),
	},
	{
		I2C_MPQ_CDP,
		APQ_8064_GSBI5_QUP_I2C_BUS_ID,
		cs8427_device_info,
		ARRAY_SIZE(cs8427_device_info),
	},
#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2013-02-20 yuyi Add begin for reason */


	{
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		nfc_board_info,
		ARRAY_SIZE(nfc_board_info),
	},


/* OPPO 2013-02-20 yuyi Add end */
#endif
};
#define SX150X_EXP1_INT_N	PM8921_MPP_IRQ(PM8921_IRQ_BASE, 9)
#define SX150X_EXP2_INT_N	MSM_GPIO_TO_INT(81)

struct sx150x_platform_data mpq8064_sx150x_pdata[] = {
	[SX150X_EXP1] = {
		.gpio_base	= SX150X_EXP1_GPIO_BASE,
		.oscio_is_gpo	= false,
		.io_pullup_ena	= 0x0,
		.io_pulldn_ena	= 0x0,
		.io_open_drain_ena = 0x0,
		.io_polarity	= 0,
		.irq_summary	= SX150X_EXP1_INT_N,
		.irq_base	= SX150X_EXP1_IRQ_BASE,
	},
	[SX150X_EXP2] = {
		.gpio_base	= SX150X_EXP2_GPIO_BASE,
		.oscio_is_gpo	= false,
		.io_pullup_ena	= 0x0f,
		.io_pulldn_ena	= 0x70,
		.io_open_drain_ena = 0x0,
		.io_polarity	= 0,
		.irq_summary	= SX150X_EXP2_INT_N,
		.irq_base	= SX150X_EXP2_IRQ_BASE,
	},
	[SX150X_EXP3] = {
		.gpio_base	= SX150X_EXP3_GPIO_BASE,
		.oscio_is_gpo	= false,
		.io_pullup_ena	= 0x0,
		.io_pulldn_ena	= 0x0,
		.io_open_drain_ena = 0x0,
		.io_polarity	= 0,
		.irq_summary	= -1,
	},
	[SX150X_EXP4] = {
		.gpio_base	= SX150X_EXP4_GPIO_BASE,
		.oscio_is_gpo	= false,
		.io_pullup_ena	= 0x0,
		.io_pulldn_ena	= 0x0,
		.io_open_drain_ena = 0x0,
		.io_polarity	= 0,
		.irq_summary	= -1,
	},
};

static struct i2c_board_info sx150x_gpio_exp_info[] = {
	{
		I2C_BOARD_INFO("sx1509q", 0x70),
		.platform_data = &mpq8064_sx150x_pdata[SX150X_EXP1],
	},
	{
		I2C_BOARD_INFO("sx1508q", 0x23),
		.platform_data = &mpq8064_sx150x_pdata[SX150X_EXP2],
	},
	{
		I2C_BOARD_INFO("sx1508q", 0x22),
		.platform_data = &mpq8064_sx150x_pdata[SX150X_EXP3],
	},
	{
		I2C_BOARD_INFO("sx1509q", 0x3E),
		.platform_data = &mpq8064_sx150x_pdata[SX150X_EXP4],
	},
};

#define MPQ8064_I2C_GSBI5_BUS_ID	5

static struct i2c_registry mpq8064_i2c_devices[] __initdata = {
	{
		I2C_MPQ_CDP,
		MPQ8064_I2C_GSBI5_BUS_ID,
		sx150x_gpio_exp_info,
		ARRAY_SIZE(sx150x_gpio_exp_info),
	},
};

static void __init register_i2c_devices(void)
{
	u8 mach_mask = 0;
	int i;

#ifdef CONFIG_MSM_CAMERA
	struct i2c_registry apq8064_camera_i2c_devices = {
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI4_QUP_I2C_BUS_ID,
		apq8064_camera_board_info.board_info,
		apq8064_camera_board_info.num_i2c_board_info,
	};
	/* OPPO 2013-07-24 liubin Add for m9mo on gsbi7 start */
	struct i2c_registry apq8064_camera_i2c_devices_N1 = {
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI7_QUP_I2C_BUS_ID,
		apq8064_camera_board_info.board_info,
		apq8064_camera_board_info.num_i2c_board_info,
	};
	/* OPPO 2013-07-24 liubin Add end */
/* OPPO 2013-02-04 kangjian added begin for s5k6a3yx on GSBI7(DVT) */
	struct i2c_registry apq8064_subcamera_i2c_devices = {
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI7_QUP_I2C_BUS_ID,
		apq8064_subcamera_board_info.board_info,
		apq8064_subcamera_board_info.num_i2c_board_info,
	};
/* OPPO 2013-02-04 kangjian added end */

/* OPPO 2013-02-04 kangjian added begin for s5k6a3yx on GSBI1(EVT) */
	struct i2c_registry apq8064_subcamera_evt_i2c_devices = {
		I2C_SURF | I2C_FFA | I2C_LIQUID | I2C_RUMI,
		APQ_8064_GSBI1_QUP_I2C_BUS_ID,
		apq8064_subcamera_board_info.board_info,
		apq8064_subcamera_board_info.num_i2c_board_info,
	};
/* OPPO 2013-02-04 kangjian added end */
#endif
	/* Build the matching 'supported_machs' bitmask */
	if (machine_is_apq8064_cdp())
		mach_mask = I2C_SURF;
	else if (machine_is_apq8064_mtp())
		mach_mask = I2C_FFA;
	else if (machine_is_apq8064_liquid())
		mach_mask = I2C_LIQUID;
	else if (PLATFORM_IS_MPQ8064())
		mach_mask = I2C_MPQ_CDP;
	else
		pr_err("unmatched machine ID in register_i2c_devices\n");


#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2012-07-31 liujun Add begin for init tp hw */
	touch_init_hw();
/* OPPO 2012-07-31 liujun Add end */
#endif
//yanghai add
#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2012-07-31 liujun Add begin for init tp hw */
	touchpad_init_hw();
/* OPPO 2012-07-31 liujun Add end */
#endif
//yanghai add end
#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2013-02-20 yuyi Add begin for reason */
	pn544_power_init();
/* OPPO 2013-02-20 yuyi Add end */
#endif

#ifdef CONFIG_VENDOR_EDIT
/* OPPO 2013-07-26 ranfei Add begin for reason */
    if((get_pcb_version() >= PCB_VERSION_EVT_N1) && (get_pcb_version() <= PCB_VERSION_EVT_N1F)) {
        sensor_i2c_power_init();
    }
/* OPPO 2013-07-26 ranfei Add end */
#endif

//OPPO 213-02-21 yuyi add begin for breathe led
if( get_pcb_version() < PCB_VERSION_EVT_N1)
{
#ifdef CONFIG_VENDOR_EDIT
	SN3193_power_init();
#endif
//OPPO 213-02-21 yuyi add end

	/* Run the array and install devices as appropriate */
	for (i = 0; i < ARRAY_SIZE(apq8064_i2c_devices); ++i) {
		if (apq8064_i2c_devices[i].machs & mach_mask)
			i2c_register_board_info(apq8064_i2c_devices[i].bus,
						apq8064_i2c_devices[i].info,
						apq8064_i2c_devices[i].len);
	}
} else {
	for (i = 0; i < ARRAY_SIZE(apq8064_i2c_devices_n1); ++i) {
		if (apq8064_i2c_devices_n1[i].machs & mach_mask)
			i2c_register_board_info(apq8064_i2c_devices_n1[i].bus,
						apq8064_i2c_devices_n1[i].info,
						apq8064_i2c_devices_n1[i].len);
	}
}
#ifdef CONFIG_MSM_CAMERA
	/* OPPO 2013-07-24 liubin Modify for N1 camera i2c on gsbi7 start */
	#if 0
	if (apq8064_camera_i2c_devices.machs & mach_mask)
			i2c_register_board_info(apq8064_camera_i2c_devices.bus,
				apq8064_camera_i2c_devices.info,
				apq8064_camera_i2c_devices.len);
	#else
	if (get_pcb_version() >= PCB_VERSION_EVT_N1)
	{
		if (apq8064_camera_i2c_devices_N1.machs & mach_mask)
			i2c_register_board_info(apq8064_camera_i2c_devices_N1.bus,
				apq8064_camera_i2c_devices_N1.info,
				apq8064_camera_i2c_devices_N1.len);
	}
	else
	{
		if (apq8064_camera_i2c_devices.machs & mach_mask)
			i2c_register_board_info(apq8064_camera_i2c_devices.bus,
				apq8064_camera_i2c_devices.info,
				apq8064_camera_i2c_devices.len);
	}
	#endif
	/* OPPO 2013-07-24 liubin Modify end */
	
/* OPPO 2013-02-04 kangjian added begin for s5k6a3yx */
    if (get_pcb_version() >= PCB_VERSION_DVT){ //DVT
    	if (apq8064_subcamera_i2c_devices.machs & mach_mask)
    		i2c_register_board_info(apq8064_subcamera_i2c_devices.bus,
    			apq8064_subcamera_i2c_devices.info,
    			apq8064_subcamera_i2c_devices.len);
    } else {  //EVT
    	if (apq8064_subcamera_evt_i2c_devices.machs & mach_mask)
    		i2c_register_board_info(apq8064_subcamera_evt_i2c_devices.bus,
    			apq8064_subcamera_evt_i2c_devices.info,
    			apq8064_subcamera_evt_i2c_devices.len);
    }
/* OPPO 2013-02-04 kangjian added end */
#endif

	for (i = 0; i < ARRAY_SIZE(mpq8064_i2c_devices); ++i) {
		if (mpq8064_i2c_devices[i].machs & mach_mask)
			i2c_register_board_info(
					mpq8064_i2c_devices[i].bus,
					mpq8064_i2c_devices[i].info,
					mpq8064_i2c_devices[i].len);
	}
}

static void enable_avc_i2c_bus(void)
{
	int avc_i2c_en_mpp = PM8921_MPP_PM_TO_SYS(8);
	int rc;

	rc = gpio_request(avc_i2c_en_mpp, "avc_i2c_en");
	if (rc)
		pr_err("request for avc_i2c_en mpp failed,"
						 "rc=%d\n", rc);
	else
		gpio_set_value_cansleep(avc_i2c_en_mpp, 1);
}

#ifndef CONFIG_VENDOR_EDIT
// LiuJun@OnlineRD.Driver.Key, 2012/07/24, Add for delete keys
/* Modify platform data values to match requirements for PM8917. */
static void __init apq8064_pm8917_pdata_fixup(void)
{
	cdp_keys_data.buttons = cdp_keys_pm8917;
	cdp_keys_data.nbuttons = ARRAY_SIZE(cdp_keys_pm8917);
}
#endif /* VENDOR_EDIT */

#ifdef CONFIG_SERIAL_MSM_HS
static int configure_uartdm_gsbi4_gpios(int on)
{
	int ret = 0, i;
	int uart_gpios[] = {10, 11, 12, 13};

	for (i = 0; i < ARRAY_SIZE(uart_gpios); i++) {
		if (on) {
			ret = gpio_request(uart_gpios[i], NULL);
			if (ret) {
				pr_err("%s: unable to request uart gpio[%d]\n",
						__func__, uart_gpios[i]);
				break;
			}
		} else {
			gpio_free(uart_gpios[i]);
		}
	}

	if (ret && on && i)
		for (; i >= 0; i--)
			gpio_free(uart_gpios[i]);
	return ret;
}

static struct msm_serial_hs_platform_data apq8064_uartdm_gsbi4_pdata = {
	.gpio_config	= configure_uartdm_gsbi4_gpios,
};
#else
static struct msm_serial_hs_platform_data apq8064_uartdm_gsbi4_pdata;
#endif

static void __init apq8064ab_update_retention_spm(void)
{
	int i;

	/* Update the SPM sequences for krait retention on all cores */
	for (i = 0; i < ARRAY_SIZE(msm_spm_data); i++) {
		int j;
		struct msm_spm_platform_data *pdata = &msm_spm_data[i];
		for (j = 0; j < pdata->num_modes; j++) {
			if (pdata->modes[j].cmd ==
					spm_retention_cmd_sequence)
				pdata->modes[j].cmd =
				spm_retention_with_krait_v3_cmd_sequence;
		}
	}
}

static void __init apq8064_common_init(void)
{
/* OPPO 2012-08-01 zwx Add begin for FTM */
	int rc = 0;
/* OPPO 2012-08-01 zwx Add end */
	u32 platform_version = socinfo_get_platform_version();

#ifndef CONFIG_VENDOR_EDIT
// LiuJun@OnlineRD.Driver.Key, 2012/07/24, Add for delete keys
	if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917)
		apq8064_pm8917_pdata_fixup();
#endif /* VENDOR_EDIT */
	platform_device_register(&msm_gpio_device);
	if (cpu_is_apq8064ab())
		apq8064ab_update_krait_spm();
	if (cpu_is_krait_v3()) {
		msm_pm_set_tz_retention_flag(0);
		apq8064ab_update_retention_spm();
	} else {
		msm_pm_set_tz_retention_flag(1);
	}
	msm_spm_init(msm_spm_data, ARRAY_SIZE(msm_spm_data));
	msm_spm_l2_init(msm_spm_l2_data);
	msm_tsens_early_init(&apq_tsens_pdata);
	msm_thermal_init(&msm_thermal_pdata);
	if (socinfo_init() < 0)
		pr_err("socinfo_init() failed!\n");
	BUG_ON(msm_rpm_init(&apq8064_rpm_data));
	BUG_ON(msm_rpmrs_levels_init(&msm_rpmrs_data));
	regulator_suppress_info_printing();
	if (socinfo_get_pmic_model() == PMIC_MODEL_PM8917)
		configure_apq8064_pm8917_power_grid();
	platform_device_register(&apq8064_device_rpm_regulator);
	if (socinfo_get_pmic_model() != PMIC_MODEL_PM8917)
		platform_device_register(&apq8064_pm8921_device_rpm_regulator);
#ifdef CONFIG_VENDOR_EDIT
// LiuJun@OnlineRD.Driver.Key, 2012/07/24, Add for register gpio keys
	apq8064_init_gpio_key();
#endif /* VENDOR_EDIT */
	if (msm_xo_init())
		pr_err("Failed to initialize XO votes\n");
	msm_clock_init(&apq8064_clock_init_data);
	apq8064_init_gpiomux();
	apq8064_i2c_init();
	register_i2c_devices();

//OPPO 2012-10-23 huyu add for lcd compatible
	register_lcd_1080p_i2c_devices();			//added by huyu
//OPPO 2012-10-23 huyu add for lcd compatible

	/* OPPO 2013-07-24 liubin Modify for spi device register and init default m9mo inter gpio start*/
	#if 0
	apq8064_device_qup_spi_gsbi5.dev.platform_data =
						&apq8064_qup_spi_gsbi5_pdata;
	#else
	if (get_pcb_version() >= PCB_VERSION_EVT_N1)
	{
	    msm8064_device_qup_spi_gsbi4.dev.platform_data = &apq8064_qup_spi_gsbi4_pdata;
		platform_device_register(&msm8064_device_qup_spi_gsbi4);
		gpio_request(26, "m9mo_irq_gpio");
		gpio_direction_input(26);
	}
	else
	{
		apq8064_device_qup_spi_gsbi5.dev.platform_data = &apq8064_qup_spi_gsbi5_pdata;
	}
	#endif
	/* OPPO 2013-07-24 liubin Modify end */
	
	apq8064_init_pmic();
	if (machine_is_apq8064_liquid())
		msm_otg_pdata.mhl_enable = true;

	android_usb_pdata.swfi_latency =
		msm_rpmrs_levels[0].latency_us;

	apq8064_device_otg.dev.platform_data = &msm_otg_pdata;
	apq8064_ehci_host_init();
	apq8064_init_buses();

	platform_add_devices(early_common_devices,
				ARRAY_SIZE(early_common_devices));
	if (socinfo_get_pmic_model() != PMIC_MODEL_PM8917)
		platform_add_devices(pm8921_common_devices,
					ARRAY_SIZE(pm8921_common_devices));
	else
		platform_add_devices(pm8917_common_devices,
					ARRAY_SIZE(pm8917_common_devices));
	platform_add_devices(common_devices, ARRAY_SIZE(common_devices));
/* OPPO 2013-02-04 kangjian modified begin for s5k6a3yx's I2c */
#if 0
	if (!(machine_is_mpq8064_cdp() || machine_is_mpq8064_hrd() ||
			machine_is_mpq8064_dtv()))
		platform_add_devices(common_not_mpq_devices,
			ARRAY_SIZE(common_not_mpq_devices));
#else
	if (!(machine_is_mpq8064_cdp() || machine_is_mpq8064_hrd() ||
			machine_is_mpq8064_dtv())) {
		platform_add_devices(common_not_mpq_devices,
			ARRAY_SIZE(common_not_mpq_devices));
		if (get_pcb_version() >= PCB_VERSION_DVT) { //DVT
			platform_add_devices(gsbi7_i2c_devices,
				ARRAY_SIZE(gsbi7_i2c_devices));
		}

		/* Add GSBI4 I2C Device for non-fusion3 platform */
		/* OPPO 2013-07-24 liubin Modify for gsbi4 use for spi start */
		#if 0
		if (socinfo_get_platform_subtype() !=
					PLATFORM_SUBTYPE_SGLTE2) {
			platform_device_register(&apq8064_device_qup_i2c_gsbi4);
		}
		#else
		if (get_pcb_version() < PCB_VERSION_EVT_N1)
		{
			if (socinfo_get_platform_subtype() !=
						PLATFORM_SUBTYPE_SGLTE2) {
				platform_device_register(&apq8064_device_qup_i2c_gsbi4);
			}
		}
		#endif
		/* OPPO 2013-07-24 liubin Modify end */
	}
#endif
/* OPPO 2013-02-04 kangjian modified end */

/* OPPO 2013-07-24 sjc add begin for bq27541 I2C */
	if (get_pcb_version() >= PCB_VERSION_EVT_N1) {
		platform_add_devices(gsbi5_i2c_devices,
			ARRAY_SIZE(gsbi5_i2c_devices));
	}
/* OPPO 2013-07-24 sjc add end */

	msm_hsic_pdata.swfi_latency =
		msm_rpmrs_levels[0].latency_us;
	if (machine_is_apq8064_mtp()) {
		msm_hsic_pdata.log2_irq_thresh = 5,
		apq8064_device_hsic_host.dev.platform_data = &msm_hsic_pdata;
		device_initialize(&apq8064_device_hsic_host.dev);
		if (socinfo_get_platform_subtype() == PLATFORM_SUBTYPE_DSDA2) {
			apq8064_device_ehci_host3.dev.platform_data =
				&msm_ehci_host_pdata3;
			device_initialize(&apq8064_device_ehci_host3.dev);
		}
	}
	apq8064_pm8xxx_gpio_mpp_init();
	apq8064_init_mmc();

	if (machine_is_apq8064_mtp()) {
		if (socinfo_get_platform_subtype() == PLATFORM_SUBTYPE_DSDA2) {
			amdm_8064_device.dev.platform_data =
				&amdm_platform_data;
			platform_device_register(&amdm_8064_device);
			bmdm_8064_device.dev.platform_data =
				&bmdm_platform_data;
			platform_device_register(&bmdm_8064_device);
		} else if (socinfo_get_platform_subtype() ==
				   PLATFORM_SUBTYPE_SGLTE2) {
			sglte_mdm_8064_device.dev.platform_data =
				&sglte2_mdm_platform_data;
			platform_device_register(&sglte_mdm_8064_device);
			sglte2_qsc_8064_device.dev.platform_data =
				&sglte2_qsc_platform_data;
			platform_device_register(&sglte2_qsc_8064_device);

			/* GSBI4 UART device for Primay IPC */
			apq8064_uartdm_gsbi4_pdata.wakeup_irq = gpio_to_irq(10);
			apq8064_device_uartdm_gsbi4.dev.platform_data =
						&apq8064_uartdm_gsbi4_pdata;
			platform_device_register(&apq8064_device_uartdm_gsbi4);
		} else if (SOCINFO_VERSION_MINOR(platform_version) == 1) {
			i2s_mdm_8064_device.dev.platform_data =
				&mdm_platform_data;
			platform_device_register(&i2s_mdm_8064_device);
		} else {
			mdm_8064_device.dev.platform_data = &mdm_platform_data;
			platform_device_register(&mdm_8064_device);
		}
	}
	platform_device_register(&apq8064_slim_ctrl);
	slim_register_board_info(apq8064_slim_devices,
		ARRAY_SIZE(apq8064_slim_devices));
	if (!PLATFORM_IS_MPQ8064()) {
		apq8064_init_dsps();
		platform_device_register(&msm_8960_riva);
	}
	BUG_ON(msm_pm_boot_init(&msm_pm_boot_pdata));
	apq8064_epm_adc_init();
	/* OPPO 2012-09-12 Van Modify begin for factory mode*/
		systeminfo_kobj = kobject_create_and_add("systeminfo", NULL);
		printk("songxh create systeminto node!\n");
		if (systeminfo_kobj)
			rc = sysfs_create_group(systeminfo_kobj, &attr_group);
	/* OPPO 2012-09-12 Van Modify end */
//#ifdef VENDOR_EDIT
//WuJinping@OnlineRD.AirService.Phone 2013.1.7, Add for modem subsystem restart not need pin
		modeminfo_kobj = kobject_create_and_add("modeminfo", NULL);
		printk("create modeminfo node by wjp!\n");
		if (modeminfo_kobj)
			rc = sysfs_create_group(modeminfo_kobj, &modeminfo_attr_group);
//#endif /* VENDOR_EDIT */
}

static void __init apq8064_allocate_memory_regions(void)
{
	apq8064_allocate_fb_region();
	persistent_ram_early_init(&msm_pr);
}

static void __init apq8064_cdp_init(void)
{
	if (meminfo_init(SYS_MEMORY, SZ_256M) < 0)
		pr_err("meminfo_init() failed!\n");
#ifndef CONFIG_VENDOR_EDIT
// LiuJun@OnlineRD.Driver.TouchScreen, 2013/03/19, Remove unused tp define
	if (machine_is_apq8064_mtp() &&
		SOCINFO_VERSION_MINOR(socinfo_get_platform_version()) == 1)
			cyttsp_pdata.sleep_gpio = CYTTSP_TS_GPIO_SLEEP_ALT;
#endif /* VENDOR_EDIT */
	apq8064_common_init();
	if (machine_is_mpq8064_cdp() || machine_is_mpq8064_hrd() ||
		machine_is_mpq8064_dtv()) {
		enable_avc_i2c_bus();
		msm_rotator_set_split_iommu_domain();
		platform_add_devices(mpq_devices, ARRAY_SIZE(mpq_devices));
		mpq8064_pcie_init();
	} else {
		ethernet_init();
		msm_rotator_set_split_iommu_domain();
		platform_add_devices(cdp_devices, ARRAY_SIZE(cdp_devices));
		spi_register_board_info(spi_board_info,
						ARRAY_SIZE(spi_board_info));
	}
	apq8064_init_fb();
	apq8064_init_gpu();
	platform_add_devices(apq8064_footswitch, apq8064_num_footswitch);
#ifdef CONFIG_MSM_CAMERA
	apq8064_init_cam();
#endif
#ifndef CONFIG_VENDOR_EDIT
// LiuJun@OnlineRD.Driver.Key, 2012/07/24, Remove for disable original key init
	if (machine_is_apq8064_cdp() || machine_is_apq8064_liquid())
		platform_device_register(&cdp_kp_pdev);

	if (machine_is_apq8064_mtp())
		platform_device_register(&mtp_kp_pdev);
#endif /* VENDOR_EDIT */


#ifndef CONFIG_VENDOR_EDIT
// LiuJun@OnlineRD.Driver.Key, 2012/07/24, Remove for disable original key init
	if (machine_is_mpq8064_cdp()) {
		platform_device_register(&mpq_gpio_keys_pdev);
		platform_device_register(&mpq_keypad_device);
	}
#endif /* VENDOR_EDIT */
}
/* OPPO 2012-11-05 Van Modify begin for add interface start reason and boot_mode begin */
char pwron_event[16];

static int __init start_reason_setup(char *str)
{
    strcpy(pwron_event, str);
    printk(KERN_INFO "%s: parse poweron reason %s\n", __func__, pwron_event);
	
	return 1;
}
__setup("androidboot.startupmode=", start_reason_setup);

char boot_mode[16];
static int __init boot_mode_setup(char *str)
{
    strcpy(boot_mode, str);

    printk(KERN_INFO "%s: parse boot_mode is %s\n", __func__, boot_mode);
    return 1;
}
__setup("androidboot.mode=", boot_mode_setup);
/* OPPO 2012-11-05 Van Modify begin for add interface start reason and boot_mode end */

MACHINE_START(APQ8064_CDP, "QCT APQ8064 CDP")
	.map_io = apq8064_map_io,
	.reserve = apq8064_reserve,
	.init_irq = apq8064_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = apq8064_cdp_init,
	.init_early = apq8064_allocate_memory_regions,
	.init_very_early = apq8064_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(APQ8064_MTP, "QCT APQ8064 MTP")
	.map_io = apq8064_map_io,
	.reserve = apq8064_reserve,
	.init_irq = apq8064_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = apq8064_cdp_init,
	.init_early = apq8064_allocate_memory_regions,
	.init_very_early = apq8064_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(APQ8064_LIQUID, "QCT APQ8064 LIQUID")
	.map_io = apq8064_map_io,
	.reserve = apq8064_reserve,
	.init_irq = apq8064_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = apq8064_cdp_init,
	.init_early = apq8064_allocate_memory_regions,
	.init_very_early = apq8064_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(MPQ8064_CDP, "QCT MPQ8064 CDP")
	.map_io = apq8064_map_io,
	.reserve = apq8064_reserve,
	.init_irq = apq8064_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = apq8064_cdp_init,
	.init_early = apq8064_allocate_memory_regions,
	.init_very_early = apq8064_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(MPQ8064_HRD, "QCT MPQ8064 HRD")
	.map_io = apq8064_map_io,
	.reserve = apq8064_reserve,
	.init_irq = apq8064_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = apq8064_cdp_init,
	.init_early = apq8064_allocate_memory_regions,
	.init_very_early = apq8064_early_reserve,
	.restart = msm_restart,
MACHINE_END

MACHINE_START(MPQ8064_DTV, "QCT MPQ8064 DTV")
	.map_io = apq8064_map_io,
	.reserve = apq8064_reserve,
	.init_irq = apq8064_init_irq,
	.handle_irq = gic_handle_irq,
	.timer = &msm_timer,
	.init_machine = apq8064_cdp_init,
	.init_early = apq8064_allocate_memory_regions,
	.init_very_early = apq8064_early_reserve,
	.restart = msm_restart,
MACHINE_END

