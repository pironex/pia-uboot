/*
 * am335x_pia.h
 *
 * Copyright (C) 2012 pironex GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_AM335X_PIA_H
#define __CONFIG_AM335X_PIA_H

#include <configs/ti_am335x_common.h>

/*#define PIA_ON_BONE*/
#define PIA_DEBUG
/*#define PIA_TESTINGn*/

#ifdef PIA_DEBUG
#define DEBUG
#endif /* PIA_DEBUG */

/* defined in ti_armv7_common
 * we don't allow booting from SPL to Linux directly for now */
#undef CONFIG_SPL_OS_BOOT
#define CONFIG_ENV_IS_NOWHERE

#if defined(CONFIG_PIA_APC) || defined(CONFIG_PIA_EBTFT) || defined(CONFIG_PIA_SK)
#define CONFIG_PIA_PM 2 /* number is current PM revision */
#define CONFIG_EMMC_BOOT
#endif

#ifdef CONFIG_PIA_FIRSTSTART
 #if defined(CONFIG_PIA_PM)
  #define CONFIG_BOARD_NAME     "PIA335PM"
  #define CONFIG_BOARD_REVISION "0.02"
  #ifndef CONFIG_BOARD_MEMTYPE
   #define CONFIG_BOARD_MEMTYPE 0
  #endif
  #if defined(CONFIG_PIA_EBTFT)
   #define CONFIG_EXP_NAME "P335BEBT"
   #define CONFIG_EXP_REV "0.02"
  #elif defined(CONFIG_PIA_APC)
   #define CONFIG_EXP_NAME "P335BAPC"
   #define CONFIG_EXP_REV "0.02"
  #elif defined(CONFIG_PIA_SK)
   #define CONFIG_EXP_NAME "P335BSK"
   #define CONFIG_EXP_REV "0.02"
  #endif
 #elif defined(CONFIG_PIA_EM)
  #define CONFIG_BOARD_NAME "PIA335EM"
  #define CONFIG_BOARD_REVISION "0.02"
 #elif defined(CONFIG_PIA_E2)
  #define CONFIG_BOARD_NAME "PIA335E2"
  #define CONFIG_BOARD_REVISION "0.03"
 #elif defined(CONFIG_PIA_MMI)
  #define CONFIG_BOARD_NAME "PIA335MI"
  #define CONFIG_BOARD_REVISION "0.02"
  #define CONFIG_EXP_NAME "LCDKMMMI"
  #define CONFIG_EXP_REV "0.01"
 #else
  #define CONFIG_BOARD_NAME "PIA335__"
  #define CONFIG_BOARD_REVISION "0.00"
 #endif
#endif


#define MACH_TYPE_PIA_AM335X		4706	/* sync with kernel! */
#define CONFIG_MACH_TYPE		MACH_TYPE_PIA_AM335X
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_DISPLAY_CPUINFO

#if !defined(CONFIG_SPL_BUILD)
/* print timestamp for loaded images */
#define CONFIG_TIMESTAMP
#define CONFIG_LZO
#endif

#if !defined(DEBUG)
#undef CONFIG_SYS_CONSOLE_INFO_QUIET
#endif

#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/am33xx/u-boot-spl.lds"

/* allow boot image > 8 MB */
#define CONFIG_SYS_BOOTM_LEN		(16 << 20)

/* Clock Defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

/* TODO re-enable Watchdog */
#undef CONFIG_HW_WATCHDOG
#undef CONFIG_OMAP_WATCHDOG

/* Always 128 KiB env size
 * TODO does this hurt 256K page NAND? */
#define CONFIG_ENV_SIZE			(128 << 10)

/* Enhance our eMMC support / experience. */
#define CONFIG_CMD_GPT
#define CONFIG_EFI_PARTITION

/* Environment default definitions */
#if defined(CONFIG_NAND)
#define NANDARGS \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"nandargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${nandroot} " \
		"rootfstype=${nandrootfstype}\0" \
	"nandroot=ubi0:rootfs rw ubi.mtd=NAND.file-system,2048\0" \
	"nandrootfstype=ubifs rootwait=1\0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"nand read ${fdtaddr} NAND.u-boot-spl-os; " \
		"nand read ${loadaddr} NAND.kernel; " \
		"bootz ${loadaddr} - ${fdtaddr}\0"
#define NANDBOOT "run nandboot;"
#else
#define NANDARGS ""
#define NANDBOOT ""
#endif

#if defined(CONFIG_SPI_BOOT)
#define SPIARGS \
	"spiroot=/dev/mtdblock4 rw\0" \
	"spirootfstype=jffs2\0" \
	"spisrcaddr=0xe0000\0" \
	"spiimgsize=0x362000\0" \
	"spibusno=0\0" \
	"spiargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${spiroot} " \
		"rootfstype=${spirootfstype}\0" \
	"spiboot=echo Booting from spi ...; " \
		"run spiargs; " \
		"sf probe ${spibusno}:0; " \
		"sf read ${loadaddr} ${spisrcaddr} ${spiimgsize}; " \
		"bootz ${loadaddr}\0"
#else
#define SPIARGS ""
#endif

#if !defined(CONFIG_SPL_BUILD)
/* TODO check partitions */
#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	"boot_fdt=try\0" \
	"bootpart=1\0" \
	"bootdir=\0" \
	"bootfile=zImage\0" \
	"bootenv=uEnv.txt\0" \
	"fdtfile=undefined\0" \
	"console=ttyO0,115200n8\0" \
	"optargs=\0" \
	"debug=early_printk debug\0" \
	"partitions=" \
		"uuid_disk=${uuid_gpt_disk};" \
		"name=rootfs,start=2MiB,size=-,uuid=${uuid_gpt_rootfs}\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 ro\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"mmcargs=setenv bootargs console=${console} ${debug} ${optargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t -r $loadaddr $filesize\0" \
	"ramroot=/dev/ram0 rw\0" \
	"ramrootfstype=ext2\0" \
	"ramargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${ramroot} " \
		"rootfstype=${ramrootfstype}\0" \
	"loadramdisk=load mmc ${mmcdev} ${rdaddr} ramdisk.gz\0" \
	"loadimage=load mmc ${mmcdev}:${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadfdt=load mmc ${mmcdev}:${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"mmcloados=run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"mmcboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadbootenv; then " \
				"echo Loaded environment from ${bootenv};" \
				"run importbootenv;" \
			"fi;" \
			"if test -n $uenvcmd; then " \
				"echo Running uenvcmd ...;" \
				"run uenvcmd;" \
			"fi;" \
			"if run loadimage; then " \
				"run mmcloados;" \
			"fi;" \
		"fi;\0" \
	"ramboot=echo Booting from ramdisk ...; " \
		"run ramargs; " \
		"bootz ${loadaddr} ${rdaddr} ${fdtaddr}\0" \
	"findfdt="\
		"if test $board_name = A335BONE; then " \
			"setenv fdtfile am335x-bone.dtb; fi; " \
		"if test $board_name = A335BNLT; then " \
			"setenv fdtfile am335x-boneblack.dtb; fi; " \
		"if test $board_name = A33515BB; then " \
			"setenv fdtfile am335x-evm.dtb; fi; " \
		"if test $board_name = A335X_SK; then " \
			"setenv fdtfile am335x-evmsk.dtb; fi; " \
		"if test $fdtfile = undefined; then " \
			"echo WARNING: Could not determine device tree to use; fi; \0" \
	NANDARGS \
	SPIARGS
#endif /* !SPL_BUILD */

#define CONFIG_BOOTCOMMAND \
	"setenv fdtfile am335x-pia-sk.dtb;" \
	"run mmcboot;" \
	"setenv mmcdev 1; " \
	"setenv bootpart 2; " \
	"setenv bootdir /boot;" \
	"run mmcboot;" \
	NANDBOOT

/* allow extra environment variable to be set in board_late_init */
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

/* Debug UART: NS16550 Configuration */
#define CONFIG_SYS_NS16550_COM1		0x44e09000	/* Base EVM has UART0 */
#define CONFIG_BAUDRATE			115200
/*select serial console configuration */
#define CONFIG_CONS_INDEX		1


/* Don't use normal abort on any-key pressed, but only 's' key
 * bootdelay is defined in generic header (1s) */
#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_STOP_STR	"s"

/* Bootcount using the RTC block */
#define CONFIG_BOOTCOUNT_LIMIT
#define CONFIG_BOOTCOUNT_AM33XX
#define CONFIG_SYS_BOOTCOUNT_BE

/* Ethernet PHY */
/* allow detection of GigabitEthernet */
#define CONFIG_PHY_GIGE
#define CONFIG_PHYLIB
/* enable driver for SMSC PHYs used as single port PHY on piA
 * other boards contain IP173 switch ICs witch work without special init */
#define CONFIG_PHY_SMSC
#define CONFIG_CMD_MEMTEST
#define CONFIG_CMD_MEMINFO

/* I2C Configuration */
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* Main EEPROM */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1
/* multiple EEPROMs on board supported */
#define CONFIG_SYS_I2C_MULTI_EEPROMS
#define CONFIG_CMD_EEPROM
#define CONFIG_ENV_EEPROM_IS_ON_I2C

/* Memory
 * memtest works on 8 MB in DRAM after skipping 64MB from
 * start addr of ram disk
 */
#define CONFIG_SYS_MEMTEST_START	(CONFIG_SYS_SDRAM_BASE + (64 * 1024 * 1024))
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START \
					+ (8 * 1024 * 1024))

/* NAND: driver settings */
#ifdef CONFIG_NAND
#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_NAND_OMAP_ELM
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW
#define CONFIG_SYS_NAND_ONFI_DETECTION
#define CONFIG_NAND_OMAP_GPMC_PREFETCH
#define MTDIDS_DEFAULT			"nand0=nand.0"
/* parameters common for 2 and 4K pages */
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_NAND
#ifdef NAND_4K_PAGES
/* NAND: 4K page size */
#define CONFIG_SYS_NAND_PAGE_SIZE	4096
#define CONFIG_SYS_NAND_OOBSIZE		224
#define CONFIG_SYS_NAND_BLOCK_SIZE	(256*1024)
#define CONFIG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, \
					 58, 59, 60, 61, 62, 63, 64, 65, \
					 66, 67, 68, 69, 70, 71, 72, 73, \
					 74, 75, 76, 77, 78, 79, 80, 81, \
					 82, 83, 84, 85, 86, 87, 88, 89, \
					 90, 91, 82, 93, 94, 95, 96, 97, \
					 98, 99,100,101,102,103,104,105, \
					106,107,108,109,110,111,112,113, \
	}
/* NAND: partition layout - is 512 k enough for u-boot.img?*/
#define MTDPARTS_DEFAULT	"mtdparts=nand.0:" \
				"256k(NAND.SPL)," \
				"256k(NAND.SPL.backup1)," \
				"256k(NAND.SPL.backup2)," \
				"256k(NAND.SPL.backup3)," \
				"512k(NAND.u-boot)," \
				"256k(NAND.u-boot-env)," \
				"256k(NAND.u-boot-env,backup1)," \
				"8m(NAND.kernel)," \
				"-(NAND.file-system)"
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x00100000
#define CONFIG_ENV_OFFSET		0x00180000 /* environment starts here */
#define CONFIG_ENV_OFFSET_REDUND	0x001C0000
#define CONFIG_SYS_NAND_ECCSTEPS	8
#else
/* NAND: 2K page size config
 * Page  2K + 64 Bytes
 * Block 64 Pages = 128K + 4K */
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }
#define MTDPARTS_DEFAULT	"mtdparts=nand.0:" \
				"128k(NAND.SPL)," \
				"128k(NAND.SPL.backup1)," \
				"128k(NAND.SPL.backup2)," \
				"128k(NAND.SPL.backup3)," \
				"1280k(NAND.u-boot)," \
				"128k(NAND.u-boot-env)," \
				"128k(NAND.u-boot-env.backup1)," \
				"8m(NAND.kernel)," \
				"-(NAND.file-system)"
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x00080000
#define CONFIG_ENV_OFFSET		0x001C0000 /* environment starts here */
#define CONFIG_ENV_OFFSET_REDUND	0x001E0000
#define CONFIG_SYS_NAND_ECCSTEPS	4
#endif /* 4K_PAGES */
#define CONFIG_SYS_ENV_SECT_SIZE	CONFIG_SYS_NAND_BLOCK_SIZE

#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	14
#define	CONFIG_SYS_NAND_ECCTOTAL	(CONFIG_SYS_NAND_ECCBYTES * \
						CONFIG_SYS_NAND_ECCSTEPS)
/* NAND: SPL */
#ifdef CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_NAND_AM33XX_BCH
#ifdef CONFIG_SPL_OS_BOOT /* disable direct OS boot */
#undef CONFIG_SPL_OS_BOOT
#endif
#ifdef CONFIG_SPL_OS_BOOT
#define CONFIG_CMD_SPL_NAND_OFS	0x00080000 /* os parameters */
#define CONFIG_SYS_NAND_SPL_KERNEL_OFFS	0x00200000 /* kernel offset */
#define CONFIG_CMD_SPL_WRITE_SIZE	0x2000
#endif /* SPL_OS_BOOT */
#endif /* SPL_NAND_SUPPORT */
#endif /* NAND */

/* PMIC TPS65910 on all piA-AM335x */
#define CONFIG_POWER_TPS65910

/* SPI flash */
#ifdef CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_SF_DEFAULT_SPEED		(24000000)
#define CONFIG_CMD_SF
#endif

/* SPL additional config */
#define CONFIG_SPL_POWER_SUPPORT
/* Y-Modem. */
#define CONFIG_SPL_YMODEM_SUPPORT


#ifdef CONFIG_USB
/* USB support disabled for now
 * most of the piA boards don't use/support a peripheral USB implementation
 * for a working ethernet via USB implementation see configs/am335x_evm.h */
/* USB MUSB support for SPL */
#define CONFIG_SPL_MUSB_NEW_SUPPORT
/* USB MUSB on AM33xx
 * - DSPS is the AM33xx specific MUSB implementation
 * - am33xx/board.c:arch_misc_init() contains init code for MUSB
 * - USB0 and USB1 ports can be configured to PERIPHERAL or HOST mode
 *   depending on board schematics */
#define CONFIG_USB_MUSB_DSPS
#define CONFIG_ARCH_MISC_INIT
#define CONFIG_MUSB_GADGET
#define CONFIG_MUSB_PIO_ONLY
#define CONFIG_MUSB_DISABLE_BULK_COMBINE_SPLIT
#define CONFIG_USB_GADGET
#define CONFIG_USBDOWNLOAD_GADGET
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET_VBUS_DRAW	2
#define CONFIG_MUSB_HOST
#define CONFIG_AM335X_USB0
#define CONFIG_AM335X_USB0_MODE	MUSB_PERIPHERAL
#define CONFIG_AM335X_USB1
#define CONFIG_AM335X_USB1_MODE MUSB_HOST

#ifdef CONFIG_MUSB_HOST
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#endif

#ifdef CONFIG_MUSB_GADGET
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_RNDIS
#define CONFIG_USBNET_HOST_ADDR	"de:ad:be:af:00:00"
/* USB TI's IDs for reference, replace if used */
#define CONFIG_G_DNL_VENDOR_NUM 0x0451
#define CONFIG_G_DNL_PRODUCT_NUM 0xD022
#define CONFIG_G_DNL_MANUFACTURER "pironex"
#endif /* CONFIG_MUSB_GADGET */
#endif


/* Special Boot configuration handling */
/* SPI flash */
#if defined(CONFIG_SPI_BOOT)
#undef CONFIG_SPL_OS_BOOT		/* Not supported by existing map */
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
#define CONFIG_ENV_SECT_SIZE		(4 << 10) /* 4 KB sectors */
#define CONFIG_ENV_OFFSET		(768 << 10) /* 768 KiB in */
#define CONFIG_ENV_OFFSET_REDUND	(896 << 10) /* 896 KiB in */
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT

#define CONFIG_SPL_SPI_SUPPORT
#define CONFIG_SPL_SPI_FLASH_SUPPORT
#define CONFIG_SPL_SPI_LOAD
#define CONFIG_SYS_SPI_U_BOOT_OFFS	0x20000
/*
 * Default to using SPI for environment, etc.
 * 0x000000 - 0x020000 : SPL (128KiB)
 * 0x020000 - 0x0A0000 : U-Boot (512KiB)
 * 0x0A0000 - 0x0BFFFF : First copy of U-Boot Environment (128KiB)
 * 0x0C0000 - 0x0DFFFF : Second copy of U-Boot Environment (128KiB)
 * 0x0E0000 - 0x442000 : Linux Kernel
 * 0x442000 - 0x800000 : Userland
 */
#define MTDIDS_DEFAULT			"nor0=m25p80-flash.0"
#define MTDPARTS_DEFAULT		"mtdparts=m25p80-flash.0:" \
					"128k(SPL)," \
					"512k(u-boot)," \
					"128k(u-boot-env1)," \
					"128k(u-boot-env2)," \
					"3464k(kernel)," \
					"-(rootfs)"
#elif defined(CONFIG_EMMC_BOOT)
/* EMMC */
#if 0 /* EMMC untested */
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SPL_ENV_SUPPORT
#define CONFIG_SYS_MMC_ENV_DEV		1
#define CONFIG_SYS_MMC_ENV_PART		2
#define CONFIG_ENV_OFFSET		0x0
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#endif
#endif /* SPI support */


#if 0 /* TODO move GPIO definitions to board code */
#define CONFIG_MMI_MMC_CD_GPIO		(3 + (0 << 5))	//gpio0_3	//not used on board MMI

/* MMI settings */

#endif /* GPIO */

#endif	/* ! __CONFIG_AM335X_PIA_H */
