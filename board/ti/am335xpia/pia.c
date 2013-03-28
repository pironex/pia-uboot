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

#define PIA_TPS65910_CTRL_BUS 0
#define PIA_TPS65910_CTRL_ADDRESS 0x2D
#define PIA_TPS65910_SMART_ADDRESS 0x2D

#ifndef CONFIG_SPL_BUILD

/*
 * Read header information from EEPROM into global structure.
 */
static int read_eeprom(void)
{
	debug(">>pia:read_eeprom()\n");
	/* Check if baseboard eeprom is available */
	if (i2c_probe(CONFIG_SYS_I2C_EEPROM_ADDR)) {
		puts("Could not probe the EEPROM; something fundamentally "
			"wrong on the I2C bus.\n");
		return -ENODEV;
	}

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

	if (header.magic != 0xEE3355AA ||
#if (defined PIA_KM_E2_REV) && (PIA_KM_E2_REV == 1)
			(strncmp(&header.name[0], "PIA335E2", 8))) {
#else
		{
#endif
		printf("Incorrect magic number (0x%x) in EEPROM\n",
				header.magic);
		am33xx_first_start();
	}
	debug("EEPROM: 0x%x - name:%.8s, - version: %.4s, - serial: %.12s\n",
			header.magic, header.name, header.version, header.serial);

	return 0;
}

#endif
int board_late_init()
{
	/* use this as testing function, ETH is not initialized here */
	//i2c_se
	debug("+pia:board_late_init()\n");

	read_eeprom();

	return 0;
}

int board_phy_config(struct phy_device *phydev)
{
	int reg, i;
	debug("+pia:board_phy_config()\n");
	phy_write(phydev, 30, 0, 0x175c);
	reg = phy_read(phydev, 30, 0);
	mdelay(5); /* min 2 ms */
	debug(" master reset done:\n");
	for (i = 0; i < 5; ++i) {
		reg = phy_read(phydev, i, 0);
		debug(" control %d:  0x%04x\n", i, reg);
		reg = phy_read(phydev, 30, 1);
		debug(" status  %d:  0x%04x\n", i, reg);
	}

	/* read IPC mode register */
	phy_write(phydev, 29, 31, 0x175c);
	reg = phy_read(phydev, 29, 31);
	debug("  29:31 == 0x%04x\n", reg);

	/* set port phy 100/FD */
	phy_write(phydev, 4, 0, 0x2100);
	/* enable force mode */
	phy_write(phydev, 29, 22, 0x8420);
	reg = phy_read(phydev, 29, 22);
	debug("  force == 0x%04x\n", reg);

	/* resetting ports (ports share regs 0 and 1 */
	for (i = 0; i < 5; ++i) {
		debug(" resettings ports...\n");
		phy_write(phydev, i, MII_BMCR, BMCR_RESET);
		reg = phy_read(phydev, i, MII_BMCR);
		debug(" P%d control: 0x%04x\n", i, reg);
	}
	mdelay(2);
	for (i = 0; i < 5; ++i) {
		reg = phy_read(phydev, i, MII_BMSR);
		debug(" P%d status: 0x%04x\n", i, reg);
	}
	reg = phy_read(phydev, 29, 22);
	debug("  port reset done: 0x%04x 0x%04x\n",
			phy_read(phydev, 0, 0), phy_read(phydev, 0, 1));

	reg = phy_read(phydev, 31, 3);
	debug("  mii0 mac mode 31:03: 0x%04x\n", reg);

	reg = phy_read(phydev, 31, 5);
	debug("  mii control   31:05: 0x%04x\n", reg);

	reg = phy_read(phydev, 31, 6);
	debug("  mii control2  31:06: 0x%04x\n", reg);

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
		if (board_is_evm_15_or_later())
			mpu_pll_config(MPUPLL_M_800);
		else
			mpu_pll_config(MPUPLL_M_720);
	}
}

int board_mmc_getcd(struct mmc* mmc)
{
#ifdef CONFIG_MMC_CD_GPIO
	return (1 ^ gpio_get_value(CONFIG_MMC_CD_GPIO));
#else
	return 1;
#endif
}


/* TODO override weak definition from am33xx/board.c
 * currently identical implementation */
#if 0
/*
 * Basic board specific setup
 */
int board_init(void)
{
	enable_uart0_pin_mux();
	debug(">>pia:board_init()\n");

#ifdef CONFIG_I2C
	enable_i2c0_pin_mux();
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	if (read_eeprom() < 0)
		puts("Could not get board ID\n");
#endif

	gd->bd->bi_boot_params = PHYS_DRAM_1 + 0x100;

	gpmc_init();

	return 0;
}
#endif
