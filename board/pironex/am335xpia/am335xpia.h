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

#define PIA_I2C1_MUX_DEFAULT	0
#define PIA_I2C1_MUX_ALT1	1
#define PIA_I2C1_MUX_ALT2	2

struct am335x_baseboard_id {
	unsigned int  magic;		// 0..3
	char name[HDR_NAME_LEN];	// 4..11
	char version[4];		// 12..15
	char serial[12];		// 16..27
	char config[32];		// 28..59
	char mac_addr[HDR_NO_OF_MAC_ADDR][HDR_ETH_ALEN];	// 60..78
};

static inline int board_is_cantft2(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "P335BCT2", 8) == 0);
}

static inline int board_is_dr(struct am335x_baseboard_id *header)
{
	return ((strncmp(header->name, "P335BAPC", 8) == 0) ||
		(strncmp(header->name, "P335BDR", 7) == 0));
}

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

static inline int board_is_sk(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "P335BSK", 7) == 0);
}

static inline int board_is_sf(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "P335BSF", 7) == 0);
}

static inline int board_is_sf2(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "P335BSF2", 8) == 0);
}

static inline int board_is_pia(struct am335x_baseboard_id *header)
{
	return (strncmp(header->name, "P335BPIA", 8) == 0);
}

static inline int board_is_pm(struct am335x_baseboard_id *header)
{
	return ((strncmp(header->name, "PIA335PM", 8) == 0) ||
		board_is_sk(header) || board_is_dr(header) ||
		board_is_ebtft(header) || board_is_pia(header) ||
		board_is_sf(header)
		);
}

void enable_uart0_pin_mux(void);
void enable_i2c0_pin_mux(void);
void enable_board_pin_mux(struct am335x_baseboard_id *header);

#endif
