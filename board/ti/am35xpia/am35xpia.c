/*
 * am35xpia.c - board file for piA AM35x.
 *
 * Author: Bjoern Krombholz <b.krombholz@pironex.de>
 *
 * Based on ti/am3517evm/am3517evm.c
 *
 * Copyright (C) 2011 pironex GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <common.h>
#include <netdev.h>
#include <asm/io.h>
#include <asm/arch/mem.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/emac_defs.h>
#include <asm/arch/gpio.h>
#include <i2c.h>
#include <asm/mach-types.h>
#include "am35xpia.h"

#if defined(CONFIG_DRIVER_TI_EMAC)
#define AM3517_IP_SW_RESET  0x48002598
#define CPGMACSS_SW_RST     (1 << 1)
#define ETHERNET_NRST       65  /* GPIO for ETHERNET nRST */
#define EMACID_ADDR_LSB     0x48002380
#define EMACID_ADDR_MSB     0x48002384
#endif

DECLARE_GLOBAL_DATA_PTR;

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_PIA_AM35X;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}

/*
 * Routine: misc_init_r
 * Description: Init ethernet (done here so udelay works)
 */
int misc_init_r(void)
{
	u32 ctr;
	u32 reset;

#ifdef CONFIG_DRIVER_OMAP34XX_I2C
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
#endif
	dieid_num_r();
#if defined(CONFIG_DRIVER_TI_EMAC)
	omap_request_gpio(ETHERNET_NRST);
	omap_set_gpio_direction(ETHERNET_NRST, 0);
	omap_set_gpio_dataout(ETHERNET_NRST, 0);
	ctr  = 0;
	do {
		udelay(1000);
		ctr++;
		} while (ctr < 300);
	omap_set_gpio_dataout(ETHERNET_NRST, 1);
	ctr = 0;
	/* allow the PHY to stabilize and settle down */
	do {
		udelay(1000);
		ctr++;
		} while (ctr < 300);

	/*ensure that the module is out of reset*/
	reset = readl(AM3517_IP_SW_RESET);
	reset &= (~CPGMACSS_SW_RST);
	writel(reset, AM3517_IP_SW_RESET);
#endif
	return 0;
}

/*
 * Initializes on-chip ethernet controllers.
 * to override, implement board_eth_init()
 */
int cpu_eth_init(bd_t *bis)
{
#if defined(CONFIG_DRIVER_TI_EMAC)
	char mac_buf_lsb[8];
	char mac_buf_msb[16];
	char mac_id[24];
	const unsigned char separator = ':';

	printf("davinci_emac_initialize\n");
	davinci_emac_initialize();

	memset(mac_buf_lsb, '\0', sizeof(mac_buf_lsb));
	memset(mac_buf_msb, '\0', sizeof(mac_buf_msb));
	memset(mac_id, '\0', sizeof(mac_id));

	sprintf(mac_buf_msb, "%x", readl(EMACID_ADDR_MSB));
	sprintf(mac_buf_lsb, "%x", readl(EMACID_ADDR_LSB));
	strcat(mac_buf_msb, mac_buf_lsb);
	sprintf(mac_id, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
			mac_buf_msb[0], mac_buf_msb[1], separator,
			mac_buf_msb[2], mac_buf_msb[3], separator,
			mac_buf_msb[4], mac_buf_msb[5], separator,
			mac_buf_msb[6], mac_buf_msb[7], separator,
			mac_buf_msb[8], mac_buf_msb[9], separator,
			mac_buf_msb[10], mac_buf_msb[11]);

	printf("EMAC ID %s\n", mac_id);
	setenv("ethaddr", mac_id);
#endif
	return 0;
}

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
	MUX_AM35X_PIA();
}
