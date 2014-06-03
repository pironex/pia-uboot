/*
 * am335xpia.h
 *
 * Copyright (C) 2013 pironex GmbH -- http://www.pironex.de
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
#ifndef AM335XPIA_h
#define AM335XPIA_H

#include <asm/arch/omap.h>

/* taken from ti evm
 * TI AM335x parts define a system EEPROM that defines certain sub-fields.
 * We use these fields to in turn see what board we are on, and what
 * that might require us to set or not set.
 */
#define HDR_NO_OF_MAC_ADDR	3
#define HDR_ETH_ALEN		6
#define HDR_NAME_LEN		8

struct am335x_baseboard_id {
	unsigned int  magic;
	char name[HDR_NAME_LEN];
	char version[4];
	char serial[12];
	char config[32];
	char mac_addr[HDR_NO_OF_MAC_ADDR][HDR_ETH_ALEN];
};

/* .bss is defined in SDRAM, so we don't have a global area to store
 * the EEPROM header data in SPL. Use Scratch space in SRAM instead.
 */
#ifdef CONFIG_SPL_BUILD
static struct am335x_baseboard_id *const header =
	(struct am335x_baseboard_id *)(SRAM_SCRATCH_SPACE_ADDR + sizeof(struct omap_boot_parameters));
#else
static struct am335x_baseboard_id header_;
static struct am335x_baseboard_id *header = &header_;
#endif

static inline int board_is_e2(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "PIA335E2", 8) == 0);
}

static inline int board_is_ebtft(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "P335BEBT", 8) == 0);
}

static inline int board_is_em(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "PIA335EM", 8) == 0);
}

static inline int board_is_mmi(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "PIA335MI", 8) == 0);
}

void enable_uart0_pin_mux(void);
void enable_i2c0_pin_mux(void);
void enable_board_pin_mux(struct am335x_baseboard_id *header);

#endif
