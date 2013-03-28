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
#include <i2c.h>
#include <mmc.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_RESET		(0x1 << 1)
#define UART_CLK_RUNNING_MASK	0x1
#define UART_SMART_IDLE_EN	(0x1 << 0x3)


int board_late_init()
{
	/* use this as testing function, ETH is not initialized here */


	return 0;
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
