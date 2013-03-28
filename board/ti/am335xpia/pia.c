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
int board_late_init()
{
	/* use this as testing function, ETH is not initialized here */


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
	debug(">>pia:board_mmc_getcd()");
	return (gpio_get_value(CONFIG_MMC_CD_GPIO));
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
