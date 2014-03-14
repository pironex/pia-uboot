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

#define CONFIG_AM33XX

/*#define PIA_ON_BONE*/
#define nPIA_DEBUG
#undef CONFIG_USB_SPL
#define PIA_TESTING
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_CMD_GPIO
/* TODO only for dev */
#ifdef PIA_DEBUG
#define CONFIG_DISPLAY_CPUINFO
#define DEBUG
#endif /* PIA_DEBUG */

#ifdef CONFIG_PIA_FIRSTSTART
#define CONFIG_PIA_REVISION CONFIG_PIA_FIRSTSTART
#else
#define CONFIG_PIA_REVISION "0.00"
#endif

#define CONFIG_BOARD_LATE_INIT

#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>

#define CONFIG_DMA_COHERENT
#define CONFIG_DMA_COHERENT_SIZE	(1 << 20)

#define CONFIG_SYS_MALLOC_LEN		(1024 << 10)
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT		"U-Boot# "
#define CONFIG_SYS_NO_FLASH
#define MACH_TYPE_PIA_AM335X		4706	/* sync with kernel! */
#define CONFIG_MACH_TYPE		MACH_TYPE_PIA_AM335X

#define CONFIG_OF_LIBFDT
#define CONFIG_CMDLINE_TAG		/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#define CONFIG_SYS_CACHELINE_SIZE       64

/* commands to include */
#include <config_cmd_default.h>

#if defined(CONFIG_PIA_NAND)
/* no NAND on MMI but doesn't hurt to enable anyway */
#define CONFIG_MTD_DEVICE	/* missing this causes error 'undefined reference to `get_mtd_device_nm' (was defined with SPI) */
#define CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT		"nand0=omap2-nand.0"
#ifdef NAND_4K_PAGES
#define MTDPARTS_DEFAULT	"mtdparts=omap2-nand.0:256k(SPL)," \
				"256k(SPL.backup1),256k(SPL.backup2)," \
				"256k(SPL.backup3),1280k(u-boot)," \
				"256k(u-boot-env),5m(kernel),-(rootfs)"
#else
#define MTDPARTS_DEFAULT	"mtdparts=omap2-nand.0:128k(SPL)," \
				"128k(SPL.backup1),128k(SPL.backup2)," \
				"128k(SPL.backup3),1920k(u-boot)," \
				"128k(u-boot-env),5m(kernel),-(rootfs)"
#endif
#endif

#define CONFIG_CMD_ASKENV
#define CONFIG_VERSION_VARIABLE

#if !(defined(CONFIG_USB_SPL) && defined(CONFIG_SPL_BUILD))
#if defined(CONFIG_PIA_NAND)
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x80200000\0" \
	"kloadaddr=0x80007fc0\0" \
	"fdtaddr=0x80F80000\0" \
	"rdaddr=0x81000000\0" \
	"bootfile=uImage\0" \
	"console=ttyO0,115200n8\0" \
	"optargs=\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 ro\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"nandroot=ubi0:rootfs rw ubi.mtd=7,2048\0" \
	"nandrootfstype=ubifs rootwait=1\0" \
	"nandsrcaddr=0x280000\0" \
	"nandimgsize=0x500000\0" \
	"mtdids=" MTDIDS_DEFAULT "\0" \
	"mtdparts=" MTDPARTS_DEFAULT "\0" \
	"rootpath=/export/rootfs\0" \
	"nfsopts=nolock\0" \
	"ramroot=/dev/ram0 rw ramdisk_size=65536 initrd=${rdaddr},64M\0" \
	"ramrootfstype=ext2\0" \
	"ip_method=none\0" \
	"static_ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}" \
		"::off\0" \
	"bootargs_defaults=setenv bootargs " \
		"console=${console} early_printk " \
		"${optargs}\0" \
	"mmcargs=run bootargs_defaults;" \
		"setenv bootargs ${bootargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype} ip=${ip_method}\0" \
	"nandargs=run bootargs_defaults;" \
		"setenv bootargs ${bootargs} " \
		"root=${nandroot} noinitrd " \
		"rootfstype=${nandrootfstype} ip=${ip_method}\0" \
	"ramroot=/dev/ram0 rw ramdisk_size=65536 initrd=${rdaddr},64M\0" \
	"ramrootfstype=ext2\0" \
	"netargs=run bootargs_defaults;" \
		"setenv bootargs ${bootargs} " \
		"root=/dev/nfs " \
		"nfsroot=${serverip}:${rootpath},${nfsopts} rw " \
		"ip=dhcp\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=fatload mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
	"ramargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${ramroot} " \
		"rootfstype=${ramrootfstype}\0" \
	"loadramdisk=fatload mmc ${mmcdev} ${rdaddr} ramdisk.gz\0" \
	"loaduimagefat=fatload mmc ${mmcdev} ${kloadaddr} ${bootfile}\0" \
	"loaduimage=ext2load mmc ${mmcdev}:2 ${kloadaddr} /boot/${bootfile}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"bootm ${kloadaddr}\0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"nand read.i ${kloadaddr} ${nandsrcaddr} ${nandimgsize}; " \
		"bootm ${kloadaddr}\0" \
	"netboot=echo Booting from network ...; " \
		"setenv autoload no; " \
		"dhcp; " \
		"tftp ${kloadaddr} ${bootfile}; " \
		"run netargs; " \
		"bootm ${kloadaddr}\0" \
	"ramboot=echo Booting from ramdisk ...; " \
		"run ramargs; " \
		"bootm ${loadaddr}\0" \
    CONFIG_DFU_ALT

#else /*if defined(CONFIG_PIA_MMI)*/
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x80200000\0" \
	"kloadaddr=0x80007fc0\0" \
	"fdtaddr=0x80F80000\0" \
	"rdaddr=0x81000000\0" \
	"bootfile=uImage\0" \
	"console=ttyO0,115200n8\0" \
	"optargs=\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 ro\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"rootpath=/export/rootfs\0" \
	"nfsopts=nolock\0" \
	"ramroot=/dev/ram0 rw ramdisk_size=65536 initrd=${rdaddr},64M\0" \
	"ramrootfstype=ext2\0" \
	"ip_method=none\0" \
	"static_ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}" \
		"::off\0" \
	"bootargs_defaults=setenv bootargs " \
		"console=${console} early_printk debug " \
		"${optargs}\0" \
	"mmcargs=run bootargs_defaults;" \
		"setenv bootargs ${bootargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype} ip=${ip_method}\0" \
	"ramroot=/dev/ram0 rw ramdisk_size=65536 initrd=${rdaddr},64M\0" \
	"ramrootfstype=ext2\0" \
	"netargs=run bootargs_defaults;" \
		"setenv bootargs ${bootargs} " \
		"root=/dev/nfs " \
		"nfsroot=${serverip}:${rootpath},${nfsopts} rw " \
		"ip=dhcp\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=fatload mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
	"ramargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${ramroot} " \
		"rootfstype=${ramrootfstype}\0" \
	"loadramdisk=fatload mmc ${mmcdev} ${rdaddr} ramdisk.gz\0" \
	"loaduimagefat=fatload mmc ${mmcdev} ${kloadaddr} ${bootfile}\0" \
	"loaduimage=ext2load mmc ${mmcdev}:2 ${kloadaddr} /boot/${bootfile}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"bootm ${kloadaddr}\0" \
	"netboot=echo Booting from network ...; " \
		"setenv autoload no; " \
		"dhcp; " \
		"tftp ${kloadaddr} ${bootfile}; " \
		"run netargs; " \
		"bootm ${kloadaddr}\0" \
	"ramboot=echo Booting from ramdisk ...; " \
		"run ramargs; " \
		"bootm ${loadaddr}\0" \
    CONFIG_DFU_ALT
#endif

/* set to negative value for no autoboot */
#define CONFIG_BOOTDELAY		3

#define CONFIG_BOOTCOMMAND \
	"if mmc rescan ${mmcdev}; then " \
		"echo SD/MMC found on device ${mmcdev};" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv};" \
			"run importbootenv;" \
		"fi;" \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...;" \
			"run uenvcmd;" \
		"fi;" \
		"if run loaduimagefat; then " \
			"run mmcboot;" \
		"elif run loaduimage; then " \
			"run mmcboot;" \
		"else " \
			"echo Cound not find ${bootfile} ;" \
		"fi;" \
	"else " \
		"run nandboot;" \
	"fi;" \

#else
#define CONFIG_BOOTDELAY		0

#define CONFIG_BOOTCOMMAND			\
	"setenv autoload no; "			\
	"dhcp; "				\
	"if tftp 80000000 debrick.scr; then "	\
		"source 80000000; "		\
	"fi"
#endif

/* Clock Defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

#define CONFIG_CMD_ECHO

/* max number of command args */
#define CONFIG_SYS_MAXARGS		16

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE		512

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE \
					+ sizeof(CONFIG_SYS_PROMPT) + 16)

/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

/*
 * memtest works on 8 MB in DRAM after skipping 32MB from
 * start addr of ram disk
 */
#define CONFIG_SYS_MEMTEST_START	(PHYS_DRAM_1 + (64 * 1024 * 1024))
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START \
					+ (8 * 1024 * 1024))

#define CONFIG_SYS_LOAD_ADDR		0x81000000 /* Default load address */
#define CONFIG_SYS_HZ			1000 /* 1ms clock */

#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_OMAP_HSMMC
#define CONFIG_MMI_MMC_CD_GPIO		(3 + (0 << 5))	//gpio0_3	//not used on board MMI
#define CONFIG_CMD_MMC
#define CONFIG_DOS_PARTITION
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_FAT_WRITE

/* TODO SPI can be removed completely? */
#if 0
#define CONFIG_SPI
#define CONFIG_OMAP3_SPI
#define CONFIG_MTD_DEVICE
#if 0
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_CMD_SF
#define CONFIG_SF_DEFAULT_SPEED		(24000000)
#endif
#endif

/* TODO no usb gadget on piA */
/* USB Composite download gadget - g_dnl */
#if 0
#define CONFIG_USB_GADGET
#define CONFIG_USBDOWNLOAD_GADGET

/* USB TI's IDs */
#define CONFIG_USBD_HS
#define CONFIG_G_DNL_VENDOR_NUM 0x0403
#define CONFIG_G_DNL_PRODUCT_NUM 0xBD00
#define CONFIG_G_DNL_MANUFACTURER "Texas Instruments"
#endif

/* USB Device Firmware Update (DFU) support */
#define DFU_ALT_INFO_NAND \
	"dfu_alt_info=" \
	"SPL part 0 1;" \
	"SPL.backup1 part 0 2;" \
	"SPL.backup2 part 0 3;" \
	"SPL.backup3 part 0 4;" \
	"u-boot part 0 5;" \
	"kernel part 0 7;" \
	"rootfs part 0 8\0" \

#define DFU_ALT_INFO_MMC \
	"dfu_alt_info=" \
	"boot part 0 1;" \
	"rootfs part 0 2;" \
	"MLO fat 0 1;" \
	"u-boot.img fat 0 1;" \
	"uEnv.txt fat 0 1\0"

/* TODO firmware update not possible on piA */
#define CONFIG_DFU_FUNCTION
#ifdef CONFIG_DFU_MMC
#define CONFIG_DFU_ALT			DFU_ALT_INFO_MMC
#elif CONFIG_DFU_NAND
#endif
#define CONFIG_DFU_ALT			DFU_ALT_INFO_NAND
#if 0
#define CONFIG_CMD_DFU
#endif

/* CPSW ethernet */
#if 0 /* no ethernet boot required */
#define CONFIG_CMD_NET
#endif
/* below only for testing, not really used */
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_DRIVER_TI_CPSW
#define CONFIG_MII
#define CONFIG_BOOTP_DEFAULT
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_NET_RETRY_COUNT         10
#define CONFIG_NET_MULTI
#if 0
#define CONFIG_PHY_GIGE
#endif
#define CONFIG_PHYLIB
#define CONFIG_PHY_SMSC

#define CONFIG_CMD_MII

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1		/*  1 bank of DRAM */
#define PHYS_DRAM_1			0x80000000	/* DRAM Bank #1 */
#if 0 /* FIXME not in EVM config */
#define PHYS_DRAM_1_SIZE		0x10000000 /*(0x80000000 / 8) 256 MB */
#endif
#define CONFIG_MAX_RAM_BANK_SIZE	(1024 << 20)	/* 1GB */

#define CONFIG_SYS_SDRAM_BASE		PHYS_DRAM_1
#define CONFIG_SYS_INIT_SP_ADDR         (NON_SECURE_SRAM_END - \
						GENERATED_GBL_DATA_SIZE)
/* Platform/Board specific defs */
#define CONFIG_SYS_TIMERBASE		0x48040000	/* Use Timer2 */
#define CONFIG_SYS_PTV			2	/* Divisor: 2^(PTV+1) => 8 */
#define CONFIG_SYS_HZ			1000

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		(48000000)
#define CONFIG_SYS_NS16550_COM1		0x44e09000	/* Base EVM has UART0 */

/* I2C Configuration */
#define CONFIG_I2C
#define CONFIG_CMD_I2C
#define CONFIG_HARD_I2C
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		1
#define CONFIG_I2C_MULTI_BUS
#define CONFIG_DRIVER_OMAP24XX_I2C
#define CONFIG_CMD_EEPROM
#define no_CONFIG_ENV_EEPROM_IS_ON_I2C
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* Main EEPROM */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1
#define CONFIG_SYS_I2C_MULTI_EEPROMS

#define CONFIG_OMAP_GPIO

#define CONFIG_BAUDRATE		115200
#define CONFIG_SYS_BAUDRATE_TABLE	{ 110, 300, 600, 1200, 2400, \
4800, 9600, 14400, 19200, 28800, 38400, 56000, 57600, 115200 }

/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1			1
#define CONFIG_CONS_INDEX		1
#define CONFIG_SYS_CONSOLE_INFO_QUIET

#define CONFIG_ENV_IS_NOWHERE

#ifndef NAND_4K_PAGES
/* NAND Configuration.
 * Page  2K + 64 Bytes
 * Block 64 Pages = 128K + 4K */
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }

#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	14
#define CONFIG_SYS_NAND_ECCSTEPS	4
#define	CONFIG_SYS_NAND_ECCTOTAL	(CONFIG_SYS_NAND_ECCBYTES * \
						CONFIG_SYS_NAND_ECCSTEPS)
#else
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_PAGE_SIZE	4096
#define CONFIG_SYS_NAND_OOBSIZE		224
#define CONFIG_SYS_NAND_BLOCK_SIZE	(256*1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
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

#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	14
#define CONFIG_SYS_NAND_ECCSTEPS	8
#define	CONFIG_SYS_NAND_ECCTOTAL	(CONFIG_SYS_NAND_ECCBYTES * \
						CONFIG_SYS_NAND_ECCSTEPS)
#endif
/* Defines for SPL */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_TEXT_BASE		0x402F0400
#define CONFIG_SPL_MAX_SIZE		(101 * 1024)
#define CONFIG_SPL_STACK		CONFIG_SYS_INIT_SP_ADDR

#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/omap-common/u-boot-spl.lds"
#define CONFIG_SPL_BSS_START_ADDR	0x80000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000		/* 512 KB */

/* Core features. */
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_GPIO_SUPPORT
#ifdef CONFIG_SPL_BUILD
#define CONFIG_PANIC_HANG		/* Do not call do_reset() */
#endif


/* USB (RNDIS) SPL */
#ifdef CONFIG_USB_SPL
#define CONFIG_SPL_MUSB_GADGET_SUPPORT
#define CONFIG_SPL_USB_ETH_SUPPORT
#define CONFIG_SPL_NET_SUPPORT
#define CONFIG_SPL_NET_VCI_STRING	"AM335x U-Boot SPL"
#ifdef CONFIG_SPL_BUILD
#undef CONFIG_PHYLIB			/* Only needed on CPSW */
#endif
#endif
/*
 * Due to size constraints we need to limit what goes into the USB SPL.
 * For now we allow either USB or CPSW but not both.  This is OK as the
 * ROM will only support booting over one in a given configuration as well.
 */
#ifndef CONFIG_USB_SPL
/* CPSW SPL */
/*#define CONFIG_SPL_ETH_SUPPORT*/
#endif

/* SD/MMC/eMMC */
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300 /* address 0x60000 */
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS	0x200 /* 256 KB */
#define CONFIG_SYS_MMC_SD_FAT_BOOT_PARTITION	1
#define CONFIG_SPL_FAT_LOAD_PAYLOAD_NAME	"u-boot.img"
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_FAT_SUPPORT
#define CONFIG_SPL_LIBDISK_SUPPORT

#if 0
/* Y-Modem. */
#define CONFIG_SPL_YMODEM_SUPPORT
#endif

/* NAND */
#if defined(CONFIG_PIA_NAND)
#define CONFIG_SPL_NAND_AM33XX_BCH
#define CONFIG_SPL_NAND_SUPPORT
#define	CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE

#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x80000
#endif
/*
 * 1MB into the SDRAM to allow for SPL's bss at the beginning of SDRAM
 * 64 bytes before this address should be set aside for u-boot.img's
 * header. That is 0x800FFFC0--0x80100000 should not be used for any
 * other needs.
 */
#define CONFIG_SYS_TEXT_BASE		0x80800000
#define CONFIG_SYS_SPL_MALLOC_START	0x80208000
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000

/* Since SPL did pll and ddr initialization for us,
 * we don't need to do it twice.
 */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT
#endif

/*
 * USB configuration
 */
#if 0 /* don't use USB in u-boot */
#define CONFIG_USB_MUSB_DSPS
#define CONFIG_ARCH_MISC_INIT
#define CONFIG_MUSB_GADGET
#define CONFIG_MUSB_PIO_ONLY
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET_VBUS_DRAW	2
#define CONFIG_MUSB_HOST
#define CONFIG_AM335X_USB0
#define CONFIG_AM335X_USB0_MODE	MUSB_PERIPHERAL
#define CONFIG_AM335X_USB1
#define CONFIG_AM335X_USB1_MODE MUSB_HOST
#endif

#ifdef CONFIG_MUSB_HOST
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#endif

#ifdef CONFIG_MUSB_GADGET
#if 0
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_RNDIS
#define CONFIG_USBNET_DEV_ADDR	"aa:bb:cc:00:11:33"
#define CONFIG_USBNET_HOST_ADDR	"aa:bb:cc:00:11:44"
#endif
#define CONFIG_CMD_NET
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_TFTP
#endif /* CONFIG_MUSB_GADGET */

/* ENV in SPI */
#if defined(CONFIG_SPI_BOOT)
# undef CONFIG_ENV_IS_NOWHERE
# define CONFIG_ENV_IS_IN_SPI_FLASH
# define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
# define CONFIG_ENV_OFFSET		(508 << 10) /* 508 KB in */
# define CONFIG_ENV_SECT_SIZE		(4 << 10) /* 4 KB sectors */
# define CONFIG_ENV_SIZE		CONFIG_ENV_SECT_SIZE
#endif /* SPI support */

/* Unsupported features */
#undef CONFIG_USE_IRQ

#if defined(CONFIG_PIA_NAND)
#define CONFIG_NAND
/* NAND support */
#ifdef CONFIG_NAND
#define CONFIG_CMD_NAND
#define CONFIG_NAND_OMAP_GPMC
#define GPMC_NAND_ECC_LP_x16_LAYOUT	1
#define CONFIG_SYS_NAND_BASE		(0x08000000)	/* physical address */
							/* to access nand at */
							/* CS0 */
#define CONFIG_SYS_MAX_NAND_DEVICE	1		/* Max number of NAND
							   devices */

#if !defined(CONFIG_SPI_BOOT) && !defined(CONFIG_NOR_BOOT) && defined(CONFIG_NAND)
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OFFSET		0x260000 /* environment starts here */
#define CONFIG_ENV_SIZE			(128 << 10)	/* 128 KiB */
#endif
#endif
#else
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE			4096	/* 128 KiB */
#endif

/* E2 settings */

#define CONFIG_E2_24V_FAIL_GPIO ((2 * 32) + 25)
#define CONFIG_E2_WD_RESET_GPIO ((1 * 32) + 14)
#define CONFIG_E2_PB_RESET_GPIO ((3 * 32) + 0)
#define CONFIG_E2_WD_SET1_GPIO  ((2 * 32) + 22)
#define CONFIG_E2_WD_SET2_GPIO  ((2 * 32) + 23)
#define CONFIG_E2_POE_POE_PS_SD_GPIO ((3 * 32) + 9)
#define CONFIG_E2_POE_PSE_SD_GPIO ((3 * 32) + 20)

/* MMI settings */
#define CONFIG_MMI_3_3V_FAIL_GPIO	((3 * 32) + 20)	//gpio3_20
#define CONFIG_MMI_WDI_GPIO  	 	((1 * 32) + 0)	//gpio1_0
#define CONFIG_MMI_WD_SET1_GPIO  	((1 * 32) + 1)	//gpio1_1
#define CONFIG_MMI_WD_SET2_GPIO  	((1 * 32) + 2)  //gpio1_2
#define CONFIG_MMI_LED1_GPIO  	 	((0 * 32) + 30) //gpio0_30
#define CONFIG_MMI_LED2_GPIO  	 	((0 * 32) + 31) //gpio0_31
#define CONFIG_MMI_ACC_INT1_GPIO  	((3 * 32) + 19) //gpio3_19
#define CONFIG_MMI_ACC_INT2_GPIO  	((0 * 32) + 7)	//gpio0_7
#define CONFIG_MMI_XDMA_EVENT_INTR0_GPIO  	((0 * 32) + 19)	//gpio0_19

/* TODO remove */
#if 0
/*
 * NOR Size = 16 MB
 * No.Of Sectors/Blocks = 128
 * Sector Size = 128 KB
 * Word lenght = 16 bits
 */
#if defined(CONFIG_NOR)
#undef CONFIG_SYS_NO_FLASH
#undef CONFIG_SYS_MALLOC_LEN
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE 1
#define CONFIG_SYS_FLASH_PROTECTION
#define CONFIG_SYS_MALLOC_LEN		(0x100000)
#define CONFIG_SYS_FLASH_CFI
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_FLASH_CFI_MTD
#define CONFIG_SYS_MAX_FLASH_SECT	128
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_FLASH_BASE		(0x08000000)
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_FLASH_BASE
#define NOR_SECT_SIZE			(128 * 1024)
#ifdef CONFIG_NOR_BOOT
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_FLASH	1
#define CONFIG_SYS_ENV_SECT_SIZE	(2 * NOR_SECT_SIZE)
#define CONFIG_ENV_SECT_SIZE		(2 * NOR_SECT_SIZE)
#define CONFIG_ENV_SIZE			CONFIG_ENV_SECT_SIZE
#define CONFIG_ENV_OFFSET		(2 * NOR_SECT_SIZE) /* After 1 MB */
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + \
	 CONFIG_ENV_OFFSET)
#endif
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_FLASH
#endif	/* NOR support */
#endif

#ifdef PIA_ON_BONE
#undef CONFIG_SPL_NAND_SUPPORT
#endif /* PIA_ON_BONE */

#endif	/* ! __CONFIG_AM335X_PIA_H */
