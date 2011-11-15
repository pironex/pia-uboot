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
#include <rtc.h>
#include <asm/mach-types.h>
#include "am35xpia.h"

#if defined(CONFIG_DRIVER_TI_EMAC)
#define AM3517_IP_SW_RESET  0x48002598
#define CPGMACSS_SW_RST     (1 << 1)
#define ETHERNET_NRST       65  /* GPIO for ETHERNET nRST */
#define EMACID_ADDR_LSB     0x48002380
#define EMACID_ADDR_MSB     0x48002384
#endif

#define EXPANSION_EEPROM_I2C_BUS     1
#define EXPANSION_EEPROM_I2C_ADDRESS 0x50

#define LCD_EXPANSION_EEPROM_I2C_BUS     2
#define LCD_EXPANSION_EEPROM_I2C_ADDRESS 0x50

/*
 * EEPROM-Content:
 * Byte | 0 - 1  |   2 - 3   |    4     |  5  |
 * --------------------------------------------
 * Usage| Dev-ID | Vendor-ID | Revision | CRC |
 */
#define PIA_WIFI       0x00010001	//Wireless Board
#define PIA_MC         0x00010002	//MotorControl
#define PIA_CC         0x00010003	//ChargeControl
#define PIA_IO         0x00010004	//IO-Board
#define PIA_LCD        0x00010005	//LCD Board
#define PIA_NEW_EEPROM 0xffffffff
#define PIA_NO_EEPROM  0x00000000

DECLARE_GLOBAL_DATA_PTR;

#define AS(x) (sizeof(x)/sizeof(x[0]))

static struct {
	unsigned int device_vendor;
	unsigned char revision;
	unsigned char crc;
} expansion_config;

/*
 * Routine: sum_of_digits
 * Description: Calculate digit sum (Quersumme)
 */
int sum_of_digits(__u8 *data, int len)
{
	int sum = 0;
	int i;

	for(i = 0; i < len; i++) {
		while (data[i] > 0) {
		sum += data[i] % 10;
		data[i] /= 10;
		}
	}

	return sum;
}

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
 * Routine: get_expansion_id
 * Description: This function checks for expansion board by checking I2C
 *		bus 1 for the availability of an AT24C01B serial EEPROM.
 *		returns the device_vendor field from the EEPROM
 */
unsigned int get_expansion_id(int i2c_bus, int i2c_addr)
{
	int i;
	__u8 data[5] = {0};				/* 5 bytes: 2*Dev-ID,2*Vend-ID,1*Rev */
	__u8 *p_data = (__u8 *)&expansion_config;

	/* save current i2c bus number */
	unsigned int old_bus = i2c_get_bus_num();
	
	/* switch to expansion_eeprom i2c bus */
	i2c_set_bus_num(i2c_bus);

	/* return PIA_NO_EEPROM if eeprom doesn't respond */
	if (i2c_probe(i2c_addr)) {
		/* restore previous i2c bus number */
		i2c_set_bus_num(old_bus);	
		return PIA_NO_EEPROM;
	}

	/* read configuration data */
	i2c_read(i2c_addr, 0, 1, (u8 *)&expansion_config,
		 sizeof(expansion_config));

	/* Check sum of digits */
	for(i = 0; i < AS(data); i++)
		data[i] = *p_data++;

	if(sum_of_digits(data, AS(data)) != expansion_config.crc)
		printf("Warning: eeprom data not valid!\n");

	/* restore previous i2c bus number */
	i2c_set_bus_num(old_bus);

	return expansion_config.device_vendor;
}

#if 0
/*
 * Routine: lcd_probe
 * Description: This function checks for expansion board piA-LCD by checking I2C
 *		bus 2 for the availability of an AT24C01B serial EEPROM.
 *		Returns positive value if expansion board is connected, otherwise 0.
 */
static int lcd_probe(void)
{
	/* save current i2c bus number */
	unsigned int old_bus = i2c_get_bus_num();

	/* switch to lcd_expansion_eeprom i2c bus */
	i2c_set_bus_num(LCD_EXPANSION_EEPROM_I2C_BUS);

	/* return PIA_NO_EEPROM if eeprom doesn't respond */
	if (i2c_probe(LCD_EXPANSION_EEPROM_I2C_ADDRESS)) {
		/* restore previous i2c bus number */
		i2c_set_bus_num(old_bus);
		return PIA_NO_EEPROM;
	}

	/* read configuration data */
	i2c_read(LCD_EXPANSION_EEPROM_I2C_ADDRESS, 0, 1, (u8 *)&expansion_config,
		 sizeof(expansion_config));

	return PIA_LCD;
}
#endif

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

	/* RTC Initialization */
#ifdef CONFIG_CMD_DATE
	rtc_init();
#endif

	switch (get_expansion_id(EXPANSION_EEPROM_I2C_BUS, EXPANSION_EEPROM_I2C_ADDRESS)) {
	case PIA_WIFI:
		printf("Expansion Board: piA-Wireless (rev %d)\n", expansion_config.revision);
		MUX_PIA_WIFI();
		setenv("buddy", "pia_wifi");
		break;
	case PIA_MC:
		printf("Expansion Board: piA-Motor (rev %d)\n", expansion_config.revision);
		MUX_PIA_MC();
		setenv("buddy", "pia_motorcontrol");
		break;
	case PIA_CC:
		printf("Expansion Board: piA-ChargeControl (rev %d)\n", expansion_config.revision);
		MUX_PIA_CC();
		setenv("buddy", "pia_chargecontrol");
		break;
	case PIA_IO:
		printf("Expansion Board: piA-IO (rev %d)\n", expansion_config.revision);
		MUX_PIA_IO();
		setenv("buddy", "pia_io");
		break;
	case PIA_NEW_EEPROM:
		printf("Expansion Board: unknown expansion board (vendor-id: %x)\n", expansion_config.device_vendor);
		setenv("buddy", "unknown");
		break;
	default:
		setenv("buddy", "none");
		break;
	}

	switch(get_expansion_id(LCD_EXPANSION_EEPROM_I2C_BUS, LCD_EXPANSION_EEPROM_I2C_ADDRESS)){
	case PIA_LCD:
		printf("Expansion Board: piA-LCD (rev %d)\n", expansion_config.revision);
		MUX_PIA_LCD();
		setenv("buddy_lcd", "pia_lcd");
		/* Overwrite default display variable if lcd is connected */
		setenv("display","lcd");
		break;
	case PIA_NEW_EEPROM:
		printf("Expansion Board: unknown LCD board (vendor-id: %x)\n", expansion_config.device_vendor);
		setenv("buddy_lcd", "unknown");
		break;
	default:
		setenv("buddy_lcd", "no_lcd");
		/* Set default display kernel parameter */
		setenv("display","dvi");
		break;
	}

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
