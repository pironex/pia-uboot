/*
 * cmd_readmacid.c
 *
 * Displays Craneboard's EMAC ID
 *
 * Author : Ulaganathan.V  <ulaganathan@mistralsolutions.com>
 *
 * Copyright (C) 2010 Mistral Solutions Pvt Ltd  <www.mistralsolutions.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */
/*
 * modification history
 * --------------------
 * 23 Aug 2010 - Created
 *
 */
#include <common.h>

#if defined(CONFIG_DRIVER_TI_EMAC)
/* MAC ID */
#define EMAC_MACID_ADDR_LSB 0x48002380
#define EMAC_MACID_ADDR_MSB 0x48002384

int read_mac_id(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	run_command("print ethaddr", 0);
	return 0;
}
U_BOOT_CMD(readmacid, 10, 1, read_mac_id, "Display the EMAC ID of the board\n",
		"\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif





