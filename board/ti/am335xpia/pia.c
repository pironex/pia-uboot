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
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <serial.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_RESET		(0x1 << 1)
#define UART_CLK_RUNNING_MASK	0x1
#define UART_SMART_IDLE_EN	(0x1 << 0x3)

#ifdef CONFIG_SPL_BUILD
#ifdef PIA_TESTING
#undef PIA_TESTING
#endif
#endif

#include <errno.h>
#include <i2c.h>
#include <mmc.h>
#include <phy.h>

#define PIA_RX8801_BUS 		1
#define PIA_RX8801_ADDRESS	0x32
#define PIA_TPS65910_CTRL_BUS 0
#define PIA_TPS65910_CTRL_ADDRESS 0x2D
#define PIA_TPS65910_SMART_ADDRESS 0x2D

#define TC(t) \
	t = ((t & 0x0f) + (10 *(t >> 4 && 0xf)))

extern struct am335x_baseboard_id header;
int board_is_e2(void)
{
	return (strncmp(header.name, "PIA335E2", 8) == 0);
}

int board_is_mmi(void)
{
	return (strncmp(header.name, "PIA335MI", 8) == 0);
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
#endif

#if defined(CONFIG_PIA_FIRSTSTART) || defined(PIA_TESTING)
static int init_tps65910(void)
{
	u8 regval;

	/* RTC on TPS65910 */
	puts("Initializing TPS RTC (clearing flags and starting RTC)\n");
	i2c_set_bus_num(0);
	if (i2c_probe(PIA_TPS65910_CTRL_ADDRESS)) {
		puts(" FAIL: Could not probe RTC device\n");
		return -ENODEV;
	}

	/* start clock */
	regval = 0x01; /* 24 hour, direct reg access, rtc running */
	if (i2c_write(PIA_TPS65910_CTRL_ADDRESS, 0x10, 1, &regval, 1)) {
		puts(" Couldn't write RTC CONTROL register\n");
		return -EIO;
	}
	udelay(10000);

	/* clear powerup and alarm flags */
	regval = 0xC0;
	if (i2c_write(PIA_TPS65910_CTRL_ADDRESS, 0x11, 1, &regval, 1)) {
		puts(" Couldn't write RTC STATUS register\n");
		return -EIO;
	}
	udelay(10000);

	return 0;
}
#endif

#ifdef CONFIG_PIA_FIRSTSTART
int am33xx_first_start(void)
{
	int size, pos;
	int to; /* 10 ms timeout */

	printf("(Re)Writing EEPROM content\n");
	/* EUI EEPROM */
	/* init with default magic number, generic name and version info */
	header.magic = 0xEE3355AA;
#if (defined CONFIG_PIA_E2)
	strncpy((char *)&header.name, "PIA335E2", 8);
	strncpy((char *)&header.version, CONFIG_PIA_REVISION, 4);
	strncpy((char *)&header.serial, "000000000000", 12);
#elif (defined CONFIG_PIA_MMI)
	strncpy((char *)&header.name, "PIA335MI", 8);
	strncpy((char *)&header.version, CONFIG_PIA_REVISION, 4);
	strncpy((char *)&header.serial, "000000000000", 12);
#else
	strncpy((char *)&header.name, "PIA335__", 8);
	strncpy((char *)&header.version, "0.00", 4);
	strncpy((char *)&header.serial, "000000000000", 12);
#endif
	memset(&header.config, 0, 32);
	debug("Using MN:0x%x N:%.8s V:%.4s SN:%.12s\n",
			header.magic, header.name, header.version, header.serial);
	size = sizeof(header);
	pos = 0;
	do {
		to = 10;
		/* page size is 8 bytes */
		do {
			if (!i2c_write(CONFIG_SYS_I2C_EEPROM_ADDR, pos, 1,
					&((uchar *)&header)[pos], 8)) {
				to = 0;
			} else {
				udelay(1000);
			}
		} while (--to > 0);
		/* wait to avoid NACK error spam */
		udelay(10000);
	} while ((pos = pos + 8) < size);

	init_rtc_rx8801();
	init_tps65910();

	return 0;
}
#endif

/*
 * Read header information from EEPROM into global structure.
 */
static int read_eeprom(void)
{
	int i;

	debug(">>pia:read_eeprom()\n");
	i2c_set_bus_num(0);

	/* Check if baseboard eeprom is available */
	if (i2c_probe(CONFIG_SYS_I2C_EEPROM_ADDR)) {
		puts("Could not probe the EEPROM; something fundamentally "
			"wrong on the I2C bus.\n");
		return -ENODEV;
	}

#ifdef CONFIG_PIA_FIRSTSTART
	puts("Special FIRSTSTART version\n");
	/* force reinitialization, normally the ID EEPROM is written here */
	am33xx_first_start();
#endif
	/*
	 * read the eeprom using i2c again,
	 * but use only a 1 byte address
	 */
	if (i2c_read(CONFIG_SYS_I2C_EEPROM_ADDR, 0, 1,
			(uchar *)&header, sizeof(header))) {
		puts("Could not read the EEPROM; something "
				"fundamentally wrong on the I2C bus.\n");
		return -EIO;
	}


	if (header.magic != 0xEE3355AA || header.config[31] != 0) {
		printf("Incorrect magic number (0x%x) in EEPROM\n",
				header.magic);
		return -EIO;
	}

	printf("Detecting board...");
	i = 0;
	if (strncmp(&header.name[0], "PIA335E2", 8) == 0) {
		printf("PIA335E2 found\n");
		i++;
	}

	if (strncmp(&header.name[0], "PIA335MI", 8) == 0) {
		printf("PIA335MI found\n");
		i++;
	}

	if (!i) {
		printf("board not specified\n");
	}

	debug("EEPROM: 0x%x - name:%.8s, - version: %.4s, - serial: %.12s\n",
			header.magic, header.name, header.version, header.serial);

	return 0;
}

#ifdef CONFIG_SPL_BUILD
int board_identify(void)
{
	debug(">>pia:board_identify\n");
	return (read_eeprom());
}
#endif

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
	if(strncmp(header.name,"PIA335E2",8) == 0) {
		rc |= test_supervisor_e2();
		rc |= test_rtc_rx8801();
	} else if(strncmp(header.name,"PIA335MI",8) == 0) {
		rc |= test_supervisor_mmi();
	}
	rc |= test_rtc_tps();
	rc |= test_temp_sensor();

	return rc;
}

#else
static inline int test_pia(void) {
	puts("Board tests disabled\n");
	return 0;
}
#endif

int board_late_init()
{
	/* use this as testing function, ETH is not initialized here */
	//i2c_se
	debug("+pia:board_late_init()\n");

	read_eeprom();

	test_pia();
	return 0;
}

int board_phy_config(struct phy_device *phydev)
{
	if (board_is_e2()) {
		int reg, i, eth_cnt = 5;

#if 1
		puts("Initializing ethernet switch\n");
		phy_write(phydev, 30, 0, 0x175c);
		mdelay(5); /* min 2 ms */
		reg = phy_read(phydev, 30, 0);
		debug(" master reset done: 0x%04x\n", reg);

		/* read IPC mode register */
//		phy_write(phydev, 29, 31, 0x175c);
//		phy_write(phydev, 29, 22, 0x420);

//		for (i = 0; i < eth_cnt; ++i) {
//			debug(" resettings ports...\n");
//			phy_write(phydev, i, MII_BMCR, BMCR_RESET);
//			reg = phy_read(phydev, i, MII_BMCR);
//			debug(" P%d control: 0x%04x\n", i, reg);
//		}
		for (i = 0; i < eth_cnt; ++i) {
			reg = phy_read(phydev, i, MII_BMSR);
			debug(" P%d status: 0x%04x\n", i, reg);
		}
		mdelay(2);
#else
		/* set port phy 100/FD */
		phy_write(phydev, 4, 0, 0x2100);
		/* enable force mode */
		phy_write(phydev, 29, 22, 0x8420);
		reg = phy_read(phydev, 29, 22);

		if(board_is_e2()) {
			eth_cnt = 5;
		} else {
			eth_cnt = 1;
		}

		/* resetting ports */
		for (i = 0; i < eth_cnt; ++i) {
			debug(" resettings ports...\n");
			phy_write(phydev, i, MII_BMCR, BMCR_RESET);
			reg = phy_read(phydev, i, MII_BMCR);
			debug(" P%d control: 0x%04x\n", i, reg);
		}
		mdelay(2);
		for (i = 0; i < eth_cnt; ++i) {
			reg = phy_read(phydev, i, MII_BMSR);
			debug(" P%d status: 0x%04x\n", i, reg);
		}
#endif
	}

	return 0;
}

#include "pmic.h"
#include "asm/arch/clocks_am33xx.h"

#define MPU     0
#define CORE    1

int voltage_update(unsigned int module, unsigned char vddx_op_vol_sel);
/* override from cpu/armv7/am33xx/board.c */
void am33xx_spl_board_init(void)
{
	debug(">>pia:am33xx_spl_board_init()\n");
	uchar buf[4];
	/*
	 * TODO check
	 * EVM PMIC code.  All boards currently want an MPU voltage
	 * of 1.2625V and CORE voltage of 1.1375V to operate at
	 * 720MHz.
	 */
	i2c_set_bus_num(0);
	if (i2c_probe(PMIC_CTRL_I2C_ADDR))
		return;

	/* VDD1/2 voltage selection register access by control i/f */
	if (i2c_read(PMIC_CTRL_I2C_ADDR, PMIC_DEVCTRL_REG, 1, buf, 1))
		return;

	debug("PMIC DEVCTRL: %02x\n", buf[0]);
	buf[0] |= PMIC_DEVCTRL_REG_SR_CTL_I2C_SEL_CTL_I2C;
	buf[0] |= 0x20; /* CK32K_CTRL internal 32k */
	buf[0] &= (~0x40); /* enable RTC */

	if (i2c_write(PMIC_CTRL_I2C_ADDR, PMIC_DEVCTRL_REG, 1, buf, 1))
		return;

	if (!voltage_update(MPU, PMIC_OP_REG_SEL_1_2_6) &&
			!voltage_update(CORE, PMIC_OP_REG_SEL_1_1_3)) {
		mpu_pll_config(MPUPLL_M_720);
	}
}

int board_mmc_getcd(struct mmc* mmc)
{
	return 1; // ignore CD only present on prototype board
}


/* TODO override weak definition from am33xx/board.c
 * currently identical implementation */
/*
 * Basic board specific setup
 */
int board_init(void)
{
	enable_uart0_pin_mux();
	debug(">>pia:board_init()\n");

#ifdef CONFIG_I2C
	enable_i2c0_pin_mux();
	timer_init();
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	if (read_eeprom() < 0)
		puts("Could not get board ID\n");
#endif

	gd->bd->bi_boot_params = PHYS_DRAM_1 + 0x100;

	if (board_is_e2())
			gpmc_init();

	return 0;
}
