/*
 * evm.c
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <common.h>
#include <errno.h>
#include <spl.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/clock.h>
#include <asm/arch/sys_proto.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <i2c.h>
#include <miiphy.h>
#include <mmc.h>
#include <power/tps65910.h>
#include "am335xpia.h"

DECLARE_GLOBAL_DATA_PTR;

#define UART_RESET		(0x1 << 1)
#define UART_CLK_RUNNING_MASK	0x1
#define UART_SMART_IDLE_EN	(0x1 << 0x3)

#ifdef CONFIG_SPL_BUILD
#ifdef PIA_TESTING
#undef PIA_TESTING
#endif
#endif

#define PIA_RX8801_BUS 		1
#define PIA_RX8801_ADDRESS	0x32
#define PIA_TPS65910_CTRL_BUS 0
#define PIA_TPS65910_CTRL_ADDRESS 0x2D
#define PIA_TPS65910_SMART_ADDRESS 0x2D

#define TC(t) \
	t = ((t & 0x0f) + (10 *(t >> 4 && 0xf)))


static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;
#define DEVICE_ID_REVSHIFT 28
static inline int board_is_800mhz(void)
{
	/* Silicon Revisions 2.x (rev 1+) support 800 MHz */
	int rev = (readl(&cdev->deviceid) >> DEVICE_ID_REVSHIFT) & 0x3;
	printf("Device Revision: %d\n", rev);

	return (rev > 0);
}


#if defined(CONFIG_PIA_FIRSTSTART)
static int init_rtc_rx8801(void)
{
	u8 regval;

	/* RTC EX-8801 */
	puts("Initializing RTC (clearing error flags)\n");
	i2c_set_bus_num(PIA_RX8801_BUS);
	if (i2c_probe(PIA_RX8801_ADDRESS)) {
		puts(" FAIL: Could not probe RTC device\n");
		return -ENODEV;
	}

	/* clear RESET */
	regval = 0x40; /* 2s temp compensation, reset off */
	if (i2c_write(PIA_RX8801_ADDRESS, 0x0f, 1, &regval, 1)) {
		puts(" Couldn't write RTC control register\n");
		return -EIO;
	}

	/* clear error flags, they are undefined on first start */
	regval = 0x00; /* clear all interrupt + error flags */
	if (i2c_write(PIA_RX8801_ADDRESS, 0x0e, 1, &regval, 1)) {
		puts(" Couldn't clear RTC flag register\n");
		return -EIO;
	}

	return 0;
}
#else
static inline int init_rtc_rx8801(void) { return 0; }
#endif

void enable_i2c1_pin_mux(void);

#define EEPROM_POS_CONFIG_VARIANT	(0)
#define EEPROM_POS_CONFIG_NAND		(1)
#define EEPROM_POS_CONFIG_TOUCH		(2)
#define EEPROM_POS_CONFIG_MEMORY	(3)
#define EEPROM_POS_CONFIG_EMMC		(4)

#if defined(CONFIG_PIA_FIRSTSTART) && defined(CONFIG_SPL_BUILD)
/* TODO ugly */
static int init_eeprom(int expansion, int rewrite)
{
	int size, pos;
	int to; /* 10 ms timeout */
	int bus = 0;
	int addr = CONFIG_SYS_I2C_EEPROM_ADDR;

	struct am335x_baseboard_id config;

	if (!rewrite && config.magic == 0xEE3355AA) {
		printf("EEPROM already initialized\n");
		return 0;
	}


	config.magic = 0xEE3355AA;
	strncpy((char *)&config.serial, "000000000000", 12);
	memset(&config.config, 0, 32);
	if (expansion) {
#if defined (CONFIG_EXP_NAME)
		printf("(Re)Writing Expansion EEPROM content\n");
		/* init with default magic number, generic name and version info */
		strncpy((char *)&config.name, CONFIG_EXP_NAME, 8);
		strncpy((char *)&config.version, CONFIG_EXP_REV, 4);
		bus = 1;
#ifdef CONFIG_PIA_MMI
		addr = 0x51; /* LCD-EEPROM on 0x51 */
		config.config[2] = 'C';
#endif
#endif /* CONFIG_EXP_NAME */
	} else {
#if defined (CONFIG_BOARD_NAME)
		printf("(Re)Writing EEPROM content\n");
		/* init with default magic number, generic name and version info */
		strncpy((char *)&config.name, CONFIG_BOARD_NAME, 8);
		strncpy((char *)&config.version, CONFIG_BOARD_REVISION, 4);
		/* set board dependent config options */
#if (defined CONFIG_MMI_EXTENDED)
#if (CONFIG_MMI_EXTENDED == 0)
		config.config[0] = 'B';
#else
		config.config[0] = 'X';
#endif
#endif /* CONFIG_MMI_EXTENDED */
#if (defined CONFIG_PIA_E2)
		config.config[1] = 'N'; // NAND present
#endif
#if (defined CONFIG_PIA_PM)
		config.config[EEPROM_POS_CONFIG_MEMORY] = CONFIG_BOARD_MEMTYPE;
#endif
#endif /* CONFIG_BOARD_NAME */
	}

	size = sizeof(struct am335x_baseboard_id);
	pos = 0;

	i2c_set_bus_num(bus);
	if (i2c_probe(addr)) {
		printf(" WARN: No EEPROM on I2C %d:%02x\n", bus, addr);
		return -ENODEV;
	}
	printf("Writing EEPROM %d:0x%02x using MN:0x%x N:%.8s V:%.4s SN:%.12s\n",
			bus, addr,
			config.magic, config.name, config.version, config.serial);
	do {
		to = 10;
		/* page size is 8 bytes */
		do {
			if (!i2c_write(addr, pos, 1,
					&((uchar *)&config)[pos], 8)) {
				to = 0;
			} else {
				udelay(1000);
			}
		} while (--to > 0);
		/* wait to avoid NACK error spam */
		udelay(10000);
	} while ((pos = pos + 8) < size);

	memcpy(header, &config, sizeof(struct am335x_baseboard_id));

	return 0;
}

int am33xx_first_start(void)
{
#ifdef CONFIG_EXP_NAME
	enable_i2c1_pin_mux();
	init_eeprom(1, 1);
#endif
	init_eeprom(0, 1);

	if (board_is_e2(header))
		init_rtc_rx8801();

	puts("Board initialized, turn off the device and reboot "
		"with a real system.\n");
	return 0;
}
#endif

/*
 * Read header information from EEPROM into global structure.
 * in special FIRSTSTART build we write a new EEPROM configuration
 */
static int read_eeprom_on_bus(int i2cbus)
{
	debug(">>pia:read_eeprom(%d)\n", i2cbus);
	i2c_set_bus_num(i2cbus);

	/* Check if baseboard eeprom is available */
	if (i2c_probe(CONFIG_SYS_I2C_EEPROM_ADDR)) {
		printf("Could not probe the EEPROM on I2C%d\n", i2cbus);
		return -ENODEV;
	}

	if (i2c_read(CONFIG_SYS_I2C_EEPROM_ADDR, 0, 1,
			(uchar *)header, sizeof(struct am335x_baseboard_id))) {
		puts("Could not read the EEPROM; something "
				"fundamentally wrong on the I2C bus.\n");
		return -EIO;
	}


	if (header->magic != 0xEE3355AA || header->config[31] != 0) {
		printf("Incorrect magic number (0x%x) in EEPROM\n",
				header->magic);
		return -EIO;
	}

	return 0;
}
static int read_eeprom(void)
{
	int i, err = 0;

	debug(">>pia:read_eeprom()\n");
	err = read_eeprom_on_bus(0);

	/* TODO we don't care about PM for now, this could change
	 * use temp variable in reading function instead of 2 copies on error */
	if (err == -ENODEV || board_is_pm(header)) {
#ifdef CONFIG_SPL_BUILD
		enable_i2c1_pin_mux();
#endif
		struct am335x_baseboard_id oldheader;
		memcpy(&oldheader, header, sizeof(struct am335x_baseboard_id));
		if (read_eeprom_on_bus(1) < 0) {
			puts("Could not read baseboard EEPROM; only PM module"
				"will be configured! Check baseboards EEPROM"
				"content!\n");
			memcpy(header, &oldheader, sizeof(struct am335x_baseboard_id));
		}
		// keep memory configuration byte, essential for DDR setup
		if (board_is_pm(&oldheader)) {
			header->config[EEPROM_POS_CONFIG_MEMORY]
			               = oldheader.config[EEPROM_POS_CONFIG_MEMORY];
		}
	}

#if defined(CONFIG_PIA_FIRSTSTART) && defined(CONFIG_SPL_BUILD)
	puts("Special FIRSTSTART version\n");
	/* force reinitialization, normally the ID EEPROM is written here */
	am33xx_first_start();
#endif
	if (err < 0)
		return err;

	puts("Detecting board... ");
	i = 1;
	if (board_is_e2(header)) {
		puts("  PIA335E2 found\n");
	} else if (board_is_mmi(header)) {
		puts("  PIA335MI found\n");
	} else if (board_is_pm(header)) {
		puts("  PIA335PM found\n");
	} else if (board_is_em(header)) {
		puts("  Lokisa EM found\n");
	} else if (board_is_sk(header)) {
		puts("  SK with undetectable PM found\n");
	} else  if (board_is_ebtft(header)) {
		puts("  EB_TFT wih undetectable PM found\n");
	} else  if (board_is_apc(header)) {
		puts("  APC wih undetectable PM found\n");
	} else {
		i = 0;
	}

	if (i == 0) {
		puts("Board not specified or unknown! "
			"Check ID EEPROM content!\n");
		hang();
	}

	puts("  Options: ");
	for (i = 0; i < 32; ++i) {
		if (header->config[i]) {
			putc(header->config[i]);
		}
	}
	putc('\n');
	printf("  EEPROM: 0x%x - name:%.8s, - version: %.4s, - serial: %.12s\n",
			header->magic, header->name, header->version, header->serial);

	return 0;
}

#ifndef CONFIG_SPL_BUILD
#ifdef PIA_TESTING

static int test_rtc_tps(void)
{
	unsigned char sec, min, hr, day, mon, yr, fl;
	int ec;
	debug("CHECK TPS RTC TPS65910...\n");
	i2c_set_bus_num(0);
	if (i2c_probe(PIA_TPS65910_CTRL_ADDRESS)) {
		puts(" FAIL: Could not probe TPS device\n");
		return -ENODEV;
	}
	/* dummy reads neccessary */
	ec = i2c_read(PIA_TPS65910_CTRL_ADDRESS, 0x10, 1, &fl, 1);
	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 0x10, 1, &fl, 1);
	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 0x11, 1, &fl, 1);
	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 0x11, 1, &fl, 1);
	if ((fl & 0x02) == 0)
		init_tps65910();

	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 0, 1, &sec, 1);
	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 1, 1, &min, 1);
	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 2, 1, &hr, 1);
	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 3, 1, &day, 1);
	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 4, 1, &mon, 1);
	ec |= i2c_read(PIA_TPS65910_CTRL_ADDRESS, 5, 1, &yr, 1);

	if (ec) {
		puts("  FAIL: Unable to read TPS RTC register\n");
		return -EIO;
	}
	debug("  Date: 20%02d-%02d-%02d\n", TC(yr), TC(mon), TC(day));
	debug("  Time:   %02d:%02d:%02d\n", TC(hr), TC(min), TC(sec));

	puts("TPS RTC: OK\n");

	return 0;
}

static int test_rtc_rx8801(void)
{
	unsigned char sec, min, hr, day, mon, yr, fl;
	int ec;
	debug("CHECK RTC RX-8801...\n");
	i2c_set_bus_num(PIA_RX8801_BUS);
	if (i2c_probe(PIA_RX8801_ADDRESS)) {
		puts(" FAIL: Could not probe RTC device\n");
		return -ENODEV;
	}
	ec = i2c_read(PIA_RX8801_ADDRESS, 0x0e, 1, &fl, 1);
	if (fl & 0x01) {
		puts("  WARN: Voltage Drop Flag set. "
				"Temp. compensation was stopped, BAT might be damaged!\n");
	}
	if (fl & 0x02) {
		puts("  WARN: Voltage Low Flag set. "
				"Data loss - RTC must be be re-initialized, check BAT!\n");
	}

	if (!(fl & 0x03)) {
		puts("  Error Flags: OK\n");
	} else {
		puts("  Resetting RTC Flags\n");
	}
	ec = i2c_read(PIA_RX8801_ADDRESS, 0, 1, &sec, 1);
	ec = i2c_read(PIA_RX8801_ADDRESS, 1, 1, &min, 1);
	ec = i2c_read(PIA_RX8801_ADDRESS, 2, 1, &hr, 1);
	ec = i2c_read(PIA_RX8801_ADDRESS, 4, 1, &day, 1);
	ec = i2c_read(PIA_RX8801_ADDRESS, 5, 1, &mon, 1);
	ec = i2c_read(PIA_RX8801_ADDRESS, 6, 1, &yr, 1);

	if (ec) {
		puts("  FAIL: Unable to read RTC register\n");
		return -EIO;
	}
	debug("  Date: 20%02d-%02d-%02d\n", TC(yr), TC(mon), TC(day));
	debug("  Time:   %02d:%02d:%02d\n", TC(hr), TC(min), TC(sec));

	puts("RTC: OK\n");
	return 0;
}

static int test_temp_sensor(void)
{
	/* TODO */
	return 0;
}

static int test_supervisor_e2(void)
{
	int pb, wd;
	puts("CHECK RESET...\n");
	pb = gpio_get_value(CONFIG_E2_PB_RESET_GPIO);
	wd = gpio_get_value(CONFIG_E2_WD_RESET_GPIO);
	printf(" reset lines: (w%d p%d)\n", pb, wd);
	if (wd == 1 && pb == 1) {
		puts(" Cold Boot\n");
		puts(" Enabling Watchdog, wait for RESET\n");
		gpio_set_value(CONFIG_E2_WD_SET1_GPIO, 0);
		/* the WD has an initial timeout of >60s, so it would take at least
		 * 1 min for the board to restart after poweron */
		mdelay(1000);
		//hang();
	} else if (wd == 0) {
		puts(" WatchDog Reset\n");
	} else if (pb == 0) {
		puts(" PushButton Reset\n");
	} else {
		puts(" None or Soft Reset\n");
	}
#if 0
#ifdef CONFIG_E2_FF_CLOCK_GPIO
	puts(" Clearing RESET Flags\n");
	gpio_set_value(CONFIG_E2_FF_CLOCK_GPIO, 1); /* reset flipflops */
	mdelay(1);
	gpio_set_value(CONFIG_E2_FF_CLOCK_GPIO, 0);
#endif
#endif
	pb = gpio_get_value(CONFIG_E2_24V_FAIL_GPIO);
	printf("24V_Fail: %s\n", (pb ? "HIGH" : "LOW"));

	return 0;
}

static int test_supervisor_mmi(void)
{
	int pb;

	pb = gpio_get_value(CONFIG_MMI_3_3V_FAIL_GPIO);
	printf("3.3V_Fail: %s\n", (pb ? "HIGH" : "LOW"));

	return 0;
}

static int test_pia(void)
{
	int rc = 0;

	printf("\nRunning board tests on %.8s Rev%.4s\n\n",
			header.name, header.version);
	if (board_is_e2(header)) {
		rc |= test_supervisor_e2();
		rc |= test_rtc_rx8801();
		puts("Enabling POE output\n");
		gpio_direction_output(105, 1);
		gpio_direction_output(116, 1);
	} else if (board_is_mmi(header)) {
		rc |= test_supervisor_mmi();
		rc |= test_rtc_tps();
	}
	rc |= test_temp_sensor();


	return rc;
}

#else
static inline int test_pia(void) {
	puts("Board tests disabled\n");
	return 0;
}
#endif /* PIA_TESTING */

int board_phy_config(struct phy_device *phydev)
{
#if 0
	int reg;
	if (board_is_e2(header)) {
		int i, eth_cnt = 5;

		puts("Initializing ethernet switch\n");
		phy_write(phydev, 30, 0, 0x175c);
		mdelay(5); /* min 2 ms */
		reg = phy_read(phydev, 30, 0);
		debug(" master reset done: 0x%04x\n", reg);

		for (i = 0; i < eth_cnt; ++i) {
			reg = phy_read(phydev, i, MII_BMSR);
			debug(" P%d status: 0x%04x\n", i, reg);
		}
		mdelay(2);
	}
#endif

	return 0;
}
#if ((defined(CONFIG_SPL_ETH_SUPPORT) && defined(CONFIG_SPL_BUILD)) || \
	(defined(CONFIG_DRIVER_TI_CPSW) && !defined(CONFIG_SPL_BUILD)))
#include <cpsw.h>
static void cpsw_control(int enabled)
{
	/* VTP can be added here */

	return;
}

static struct cpsw_slave_data cpsw_slaves[] = {
	{
		.slave_reg_ofs	= 0x208,
		.sliver_reg_ofs	= 0xd80,
		.phy_addr	= 0,
	},
	{
		.slave_reg_ofs	= 0x308,
		.sliver_reg_ofs	= 0xdc0,
		.phy_addr	= 1,
	},
};

static struct cpsw_platform_data cpsw_data = {
	.mdio_base		= CPSW_MDIO_BASE,
	.cpsw_base		= CPSW_BASE,
	.mdio_div		= 0xff,
	.channels		= 8,
	.cpdma_reg_ofs		= 0x800,
	.slaves			= 1,
	.slave_data		= cpsw_slaves,
	.ale_reg_ofs		= 0xd00,
	.ale_entries		= 1024,
	.host_port_reg_ofs	= 0x108,
	.hw_stats_reg_ofs	= 0x900,
	.bd_ram_ofs		= 0x2000,
	.mac_control		= (1 << 5),
	.control		= cpsw_control,
	.host_port_num		= 0,
	.version		= CPSW_CTRL_VERSION_2,
};

int board_eth_init(bd_t *bis)
{
	int rv, n = 0;
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo;

	/* try reading mac address from efuse */
	mac_lo = readl(&cdev->macid0l);
	mac_hi = readl(&cdev->macid0h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;

#if (defined(CONFIG_DRIVER_TI_CPSW) && !defined(CONFIG_SPL_BUILD)) || \
	(defined(CONFIG_SPL_ETH_SUPPORT) && defined(CONFIG_SPL_BUILD))
	if (!getenv("ethaddr")) {
		printf("<ethaddr> not set. Validating first E-fuse MAC\n");

		if (is_valid_ethaddr(mac_addr))
			eth_setenv_enetaddr("ethaddr", mac_addr);
	}

#ifdef CONFIG_DRIVER_TI_CPSW

	mac_lo = readl(&cdev->macid1l);
	mac_hi = readl(&cdev->macid1h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	if (board_is_em(header)) {
		writel(MII_MODE_ENABLE, &cdev->miisel);
		cpsw_slaves[0].phy_if = cpsw_slaves[1].phy_if =
				PHY_INTERFACE_MODE_MII;
	}

	rv = cpsw_register(&cpsw_data);
	if (rv < 0)
		printf("Error %d registering CPSW switch\n", rv);
	else
		n += rv;
#endif
	if (board_is_apc(header)) {
		int res;
		unsigned short reg;
		const char *devname;

		devname = miiphy_get_current_dev();
		puts("Initializing ethernet switch\n");
		res = miiphy_write(devname, 20, 2, 0x175d);
		//phy_write(phydev, 20, 2, 0x175d);
		mdelay(5); /* min 2 ms */
		printf("APC: enabling Ethernet switch Mirror Mode on second port %d\n", res);
		miiphy_read(devname, 20, 21, &reg);
		printf("PHY 20:21 - %04x\n", reg);
		reg = 0x5002; // mirror second port to, cpu port
		res = miiphy_write(devname, 20, 21, reg);
		printf("res: %d\n", res);
		miiphy_read(devname, 20, 20, &reg);
		printf("PHY 20:20 - %04x\n", reg);
		reg = 0xe002;
		res = miiphy_write(devname, 20, 20, reg);
		printf("res: %d\n", res);
	}
#endif

	return n;
}
#endif

static int init_tps65910(void)
{
	u8 regval;

	/* RTC on TPS65910 */
	puts("Initializing TPS\n");
	i2c_set_bus_num(0);
	if (i2c_probe(PIA_TPS65910_CTRL_ADDRESS)) {
		puts(" FAIL: Could not probe RTC device\n");
		return -ENODEV;
	}

	i2c_read(PIA_TPS65910_CTRL_ADDRESS, 0x11, 1, &regval, 1); /* dummy */
	i2c_read(PIA_TPS65910_CTRL_ADDRESS, 0x11, 1, &regval, 1);
	printf(" TPS status: 0x%x\n", regval);
	/* clear powerup and alarm flags */
	regval = 0xC0;
	if (i2c_write(PIA_TPS65910_CTRL_ADDRESS, 0x11, 1, &regval, 1)) {
		puts(" FAIL: Couldn't write RTC STATUS register\n");
		return -EIO;
	}
	udelay(10000);
	if (board_is_pm(header) || board_is_mmi(header) ||
		board_is_em(header)) {
		/* start clock, safe to set again */
		regval = 0x01; /* 24 hour, direct reg access, rtc running */
		if (i2c_write(PIA_TPS65910_CTRL_ADDRESS, 0x10, 1, &regval, 1)) {
			puts(" Couldn't write RTC CONTROL register\n");
			return -EIO;
		}
		udelay(10000);
		if (i2c_read(PIA_TPS65910_CTRL_ADDRESS, 0x11, 1, &regval, 1) ||
				((regval & 0x02) == 0)) {
			puts(" WARN: RTC not running!");
		}
	}
	/* only enable battery charger for devices utilizing the TPS VBACKUP */
	if (board_is_mmi(header) || board_is_em(header) ||
		board_is_ebtft(header) || board_is_sk(header) ||
		board_is_apc(header)) {
		puts("Initializing TPS Battery Charger... 3.15V\n");
		// BBCHG 3.15V enable charge
		regval = ((0x02 << 1) | 0x01);
		if (i2c_write(PIA_TPS65910_CTRL_ADDRESS, 0x39, 1, &regval, 1)) {
			puts(" FAIL: Couldn't enable battery charger!\n");
			return -EIO;
		}
	}

	return 0;
}

int board_late_init()
{
	/* use this as testing function, ETH is not initialized here */
	debug("+pia:board_late_init()\n");
	init_tps65910();

	if (board_is_e2(header))
		init_rtc_rx8801();

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	char safe_string[HDR_NAME_LEN + 1];
	/* Now set variables based on the header. */
	strncpy(safe_string, (char *)header->name, sizeof(header->name));
	safe_string[sizeof(header->name)] = 0;
	setenv("board_name", safe_string);

	strncpy(safe_string, (char *)header->version, sizeof(header->version));
	safe_string[sizeof(header->version)] = 0;
	setenv("board_rev", safe_string);
#endif

#ifdef PIA_TESTING
	test_pia();
#endif
	return 0;
}
#endif /* !SPL_BUILD */

#define MPU     0
#define CORE    1

int voltage_update(unsigned int module, unsigned char vddx_op_vol_sel);

/* override from cpu/armv7/am33xx/board.c */
#ifdef CONFIG_SPL_BUILD

#define OSC	(V_OSCK/1000000)
const struct dpll_params dpll_ddr_params = {
		303, OSC-1, 1, -1, -1, -1, -1};

void am33xx_spl_board_init(void)
{
	uchar buf[4];
	int mpu_vdd, sil_rev;
	debug(">>pia:am33xx_spl_board_init()\n");

	i2c_init(CONFIG_SYS_OMAP24_I2C_SPEED, CONFIG_SYS_OMAP24_I2C_SLAVE);

	/* MPU voltage 1.2625V, CORE voltage 1.1375V.
	 * Correct for 720 and 800 MHz variants
	 * REVISIT update for future 1GHz variants
	 */
	i2c_set_bus_num(0);
	if (i2c_probe(TPS65910_CTRL_I2C_ADDR))
		return;

	/* Get the frequency */
	dpll_mpu_opp100.m = am335x_get_efuse_mpu_max_freq(cdev);
	printf("Setting up MPU max freq: %d\n", dpll_mpu_opp100.m);

	/* from ti evm
	 * Depending on MPU clock and PG we will need a different
	 * VDD to drive at that speed.
	 */
	sil_rev = readl(&cdev->deviceid) >> 28;
	mpu_vdd = am335x_get_tps65910_mpu_vdd(sil_rev, dpll_mpu_opp100.m);

	/* Tell the TPS65910 to use i2c */
	tps65910_set_i2c_control();
	tps65910_start_rtc(1);
	buf[0] = 0x34; /* make sure IT_POL is active low */
	i2c_write(TPS65910_CTRL_I2C_ADDR, TPS65910_DEVCTRL2_REG, 1, buf, 1);

	/* disable VDIG1, it's not used on PM module */
	if (board_is_pm(header)) {
#if 0
	/* FIXME use gd->arch.omap_boot_params.omap_bootdevice */
		/* use BCK1 register to store the boot device */
		i2c_read(TPS65910_CTRL_I2C_ADDR, TPS65910_BCK1_REG, 1, buf, 1);
		if (buf[0] != boot_params.omap_bootdevice) {
			buf[0] = boot_params.omap_bootdevice;
			i2c_write(TPS65910_CTRL_I2C_ADDR, TPS65910_BCK1_REG, 1, buf, 1);
		}
#endif
		i2c_read(TPS65910_CTRL_I2C_ADDR, TPS65910_VDIG1_REG, 1, buf, 1);
		debug("PMIC_VDIG1_REG %02x\n", buf[0]);
		buf[0] = 0;
		if (i2c_write(TPS65910_CTRL_I2C_ADDR, TPS65910_VDIG1_REG, 1, buf, 1))
			return;
	}

	/* First update MPU voltage. */
	if (tps65910_voltage_update(MPU, mpu_vdd))
		return;

	/* Second, update the CORE voltage. */
	if (tps65910_voltage_update(CORE, TPS65910_OP_REG_SEL_1_1_3))
		return;

	/* Set CORE Frequencies to OPP100 */
	do_setup_dpll(&dpll_core_regs, &dpll_core_opp100);

	/* Set MPU Frequency to what we detected now that voltages are set */
	do_setup_dpll(&dpll_mpu_regs, &dpll_mpu_opp100);
}

const struct dpll_params *get_dpll_ddr_params(void)
{
	debug(__FUNCTION__);
	/* this is called very early in the boot process
	 * init i2c here */
	enable_i2c0_pin_mux();
	i2c_init(CONFIG_SYS_OMAP24_I2C_SPEED, CONFIG_SYS_OMAP24_I2C_SLAVE);

	/* update here if we need different memory configs */
	return &dpll_ddr_params;
}

/* called at the beginning of s_init */
void set_uart_mux_conf(void)
{
	debug(__FUNCTION__);
	enable_uart0_pin_mux();
}

void set_mux_conf_regs(void)
{
	debug(__FUNCTION__);
	if (read_eeprom() < 0)
		puts("Could not get board ID.\n");

	enable_board_pin_mux(header);
}

const struct ctrl_ioregs ioregs_pia = {
	.cm0ioctl		= MT41K256M16HA125IT_IOCTRL_VALUE,
	.cm1ioctl		= MT41K256M16HA125IT_IOCTRL_VALUE,
	.cm2ioctl		= MT41K256M16HA125IT_IOCTRL_VALUE,
	.dt0ioctl		= MT41K256M16HA125IT_IOCTRL_VALUE,
	.dt1ioctl		= MT41K256M16HA125IT_IOCTRL_VALUE,
};
/* default CMD CTRL with inverted clkout, DDR_CK traces longer than DQS */
static const struct cmd_control ddr3_cmd_ctrl_data = {
	.cmd0csratio = MT41J128MJT125_RATIO,
	.cmd0iclkout = MT41J128MJT125_INVERT_CLKOUT,

	.cmd1csratio = MT41J128MJT125_RATIO,
	.cmd1iclkout = MT41J128MJT125_INVERT_CLKOUT,

	.cmd2csratio = MT41J128MJT125_RATIO,
	.cmd2iclkout = MT41J128MJT125_INVERT_CLKOUT,
};

/* PHY Leveling defaults */
static const struct ddr_data ddr3_data = {
	.datardsratio0 = MT41J128MJT125_RD_DQS,
	.datawdsratio0 = MT41J128MJT125_WR_DQS,
	.datafwsratio0 = MT41J128MJT125_PHY_FIFO_WE,
	.datawrsratio0 = MT41J128MJT125_PHY_WR_DATA,
};

#define MT41K256M16HA125IT_RATIO		0x80
#define MT41K256M16HA125IT_INVERT_CLKOUT	0x0
static const struct cmd_control ddr3_pia_cmd_ctrl_data = {
	.cmd0csratio = MT41K256M16HA125IT_RATIO,
	.cmd0iclkout = MT41K256M16HA125IT_INVERT_CLKOUT,

	.cmd1csratio = MT41K256M16HA125IT_RATIO,
	.cmd1iclkout = MT41K256M16HA125IT_INVERT_CLKOUT,

	.cmd2csratio = MT41K256M16HA125IT_RATIO,
	.cmd2iclkout = MT41K256M16HA125IT_INVERT_CLKOUT,
};

static struct emif_regs ddr3_emif_reg_data = {
	.sdram_config = MT41J128MJT125_EMIF_SDCFG,
	.ref_ctrl = MT41J128MJT125_EMIF_SDREF,
	.sdram_tim1 = MT41J128MJT125_EMIF_TIM1,
	.sdram_tim2 = MT41J128MJT125_EMIF_TIM2,
	.sdram_tim3 = MT41J128MJT125_EMIF_TIM3,
	.zq_config = MT41J128MJT125_ZQ_CFG,
	.emif_ddr_phy_ctlr_1 = MT41J128MJT125_EMIF_READ_LATENCY |
				PHY_EN_DYN_PWRDN,
};

/* PHY Leveling for piA-AM335x-PM */
#define MT41J128M16JT125_PM_RD_DQS		0x38
#define MT41J128M16JT125_PM_WR_DQS		0x3d
#define MT41J128M16JT125_PM_PHY_FIFO_WE		0xa2
#define MT41J128M16JT125_PM_PHY_WR_DATA		0x75
static const struct ddr_data ddr3_pm_data = {
	.datardsratio0 = MT41J128M16JT125_PM_RD_DQS,
	.datawdsratio0 = MT41J128M16JT125_PM_WR_DQS,
	.datafwsratio0 = MT41J128M16JT125_PM_PHY_FIFO_WE,
	.datawrsratio0 = MT41J128M16JT125_PM_PHY_WR_DATA,
};

/* PHY Leveling for KM MMI */
#define MT41J128M16JT125_MMI_RD_DQS		0x39
#define MT41J128M16JT125_MMI_WR_DQS		0x9d
#define MT41J128M16JT125_MMI_PHY_FIFO_WE	0x3b
#define MT41J128M16JT125_MMI_PHY_WR_DATA	0x74
static const struct ddr_data ddr3_mmi_data = {
	.datardsratio0 = MT41J128M16JT125_MMI_RD_DQS,
	.datawdsratio0 = MT41J128M16JT125_MMI_WR_DQS,
	.datafwsratio0 = MT41J128M16JT125_MMI_PHY_FIFO_WE,
	.datawrsratio0 = MT41J128M16JT125_MMI_PHY_WR_DATA,
};

static struct emif_regs ddr3_128m16_emif_reg_data = {
	.sdram_config = MT41J128MJT125_EMIF_SDCFG, // OK
	.ref_ctrl = MT41J128MJT125_EMIF_SDREF, // OK
	.sdram_tim1 = MT41J128MJT125_EMIF_TIM1,
	.sdram_tim2 = MT41J128MJT125_EMIF_TIM2,
	.sdram_tim3 = MT41J128MJT125_EMIF_TIM3,
	.zq_config = MT41J128MJT125_ZQ_CFG, // OK
	.emif_ddr_phy_ctlr_1 = MT41J128MJT125_EMIF_READ_LATENCY, // PWRDN included
};

/* PHY Leveling for KM E2 */
#define MT41K256M16HA125IT_E2_RD_DQS		0x03A
#define MT41K256M16HA125IT_E2_WR_DQS		0x03C
#define MT41K256M16HA125IT_E2_PHY_FIFO_WE	0x09B
#define MT41K256M16HA125IT_E2_PHY_WR_DATA	0x075
static const struct ddr_data ddr3_e2_data = {
	.datardsratio0 = MT41K256M16HA125IT_E2_RD_DQS,
	.datawdsratio0 = MT41K256M16HA125IT_E2_WR_DQS,
	.datafwsratio0 = MT41K256M16HA125IT_E2_PHY_FIFO_WE,
	.datawrsratio0 = MT41K256M16HA125IT_E2_PHY_WR_DATA,
};

#define KINGSTON256_PM_RD_DQS		0x038
#define KINGSTON256_PM_WR_DQS		0x03a
#define KINGSTON256_PM_PHY_FIFO_WE	0x09b
#define KINGSTON256_PM_PHY_WR_DATA	0x073
static const struct ddr_data ddr3_kingstron_data = {
	.datardsratio0 = KINGSTON256_PM_RD_DQS,
	.datawdsratio0 = KINGSTON256_PM_WR_DQS,
	.datafwsratio0 = KINGSTON256_PM_PHY_FIFO_WE,
	.datawrsratio0 = KINGSTON256_PM_PHY_WR_DATA,
};

/* for Micron MT41K256M16 and Kingston D2516EC4BXGGBI */
static struct emif_regs ddr3_pia_256m16_emif_reg_data = {
	.sdram_config	= MT41K256M16HA125IT_EMIF_SDCFG,
	.ref_ctrl	= MT41K256M16HA125IT_EMIF_SDREF,
	.sdram_tim1	= MT41K256M16HA125IT_EMIF_TIM1,
	.sdram_tim2	= MT41K256M16HA125IT_EMIF_TIM2,
	.sdram_tim3	= MT41K256M16HA125IT_EMIF_TIM3,
	.zq_config	= MT41K256M16HA125IT_ZQ_CFG,
	.emif_ddr_phy_ctlr_1 = MT41K256M16HA125IT_EMIF_READ_LATENCY |
				PHY_EN_DYN_PWRDN,
};

void sdram_init(void)
{
	/* safe config for all boards */
	if (board_is_e2(header)) {
		puts("Setup DDR3 MT41K256M16HA-125:IT\n");
		config_ddr(303, &ioregs_pia, &ddr3_e2_data,
			   &ddr3_pia_cmd_ctrl_data,
			   &ddr3_pia_256m16_emif_reg_data, 0);
	} else if (board_is_mmi(header)) {
		puts("Setup DDR3 MT41J128M16JT-125 memory\n");
		config_ddr(303, &ioregs_pia, &ddr3_mmi_data,
			   &ddr3_pia_cmd_ctrl_data,
			   &ddr3_128m16_emif_reg_data, 0);
	} else if (board_is_pm(header)) {
		if (header->config[EEPROM_POS_CONFIG_MEMORY] == 'K') {
			puts("Setup PM DDR3 Kingston 256M16\n");
			config_ddr(303, &ioregs_pia, &ddr3_kingstron_data,
				&ddr3_pia_cmd_ctrl_data,
				&ddr3_pia_256m16_emif_reg_data, 0);
		} else {
			// the same as MMI, slightly different leveling values
			puts("Setup DDR3 MT41J128M16JT-125 memory\n");
				config_ddr(303, &ioregs_pia, &ddr3_pm_data,
				&ddr3_pia_cmd_ctrl_data,
				&ddr3_128m16_emif_reg_data, 0);
		}
	} else {
		puts("Setup DDR3 default memory\n");
		config_ddr(303, &ioregs_pia, &ddr3_data,
			   &ddr3_cmd_ctrl_data, &ddr3_emif_reg_data, 0);
	}
}
#endif /* SPL_BUILD */

int board_mmc_getcd(struct mmc* mmc)
{
	return 1; // ignore CD only present on prototype board
}

/*
 * Basic board specific setup
 */
int board_init(void)
{
	debug(">>pia:board_init()\n");

#if defined(CONFIG_HW_WATCHDOG)
	// TODO OMAP watchdog support is currently disabled
	hw_watchdog_init();
#endif

	if (read_eeprom() < 0)
		puts("Could not get board ID\n");

	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

#if defined(CONFIG_NAND)
	gpmc_init();
#endif

	return 0;
}
