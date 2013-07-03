/*
 * am35xpia.h - Header file for the piA AM35x.
 *
 * Author: Bjoern Krombholz <b.krombholz@pironex.de>
 *
 * Based on ti/am3517evm/am3517evm.h
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

#ifndef _AM35XPIA_H_
#define _AM35XPIA_H_

const omap3_sysinfo sysinfo = {
	DDR_DISCRETE,
	"piA_AM35X",
	"NAND",
};

/*
 * IEN  - Input Enable
 * IDIS - Input Disable
 * PTD  - Pull type Down
 * PTU  - Pull type Up
 * DIS  - Pull type selection is inactive
 * EN   - Pull type selection is active
 * M0   - Mode 0
 * The commented string gives the final mux configuration for that pin
 */
#define MUX_AM35X_PIA() \
/* SDRC */ \
	MUX_VAL(CP(SDRC_D0),		(IEN  | PTD | DIS | M0)) /* sdrc_d0 */\
	MUX_VAL(CP(SDRC_D1),		(IEN  | PTD | DIS | M0)) /* sdrc_d1 */\
	MUX_VAL(CP(SDRC_D2),		(IEN  | PTD | DIS | M0)) /* sdrc_d2 */\
	MUX_VAL(CP(SDRC_D3),		(IEN  | PTD | DIS | M0)) /* sdrc_d3 */\
	MUX_VAL(CP(SDRC_D4),		(IEN  | PTD | DIS | M0)) /* sdrc_d4 */\
	MUX_VAL(CP(SDRC_D5),		(IEN  | PTD | DIS | M0)) /* sdrc_d5 */\
	MUX_VAL(CP(SDRC_D6),		(IEN  | PTD | DIS | M0)) /* sdrc_d6 */\
	MUX_VAL(CP(SDRC_D7),		(IEN  | PTD | DIS | M0)) /* sdrc_d7 */\
	MUX_VAL(CP(SDRC_D8),		(IEN  | PTD | DIS | M0)) /* sdrc_d8 */\
	MUX_VAL(CP(SDRC_D9),		(IEN  | PTD | DIS | M0)) /* sdrc_d9 */\
	MUX_VAL(CP(SDRC_D10),		(IEN  | PTD | DIS | M0)) /* sdrc_d10 */\
	MUX_VAL(CP(SDRC_D11),		(IEN  | PTD | DIS | M0)) /* sdrc_d11 */\
	MUX_VAL(CP(SDRC_D12),		(IEN  | PTD | DIS | M0)) /* sdrc_d12 */\
	MUX_VAL(CP(SDRC_D13),		(IEN  | PTD | DIS | M0)) /* sdrc_d13 */\
	MUX_VAL(CP(SDRC_D14),		(IEN  | PTD | DIS | M0)) /* sdrc_d14 */\
	MUX_VAL(CP(SDRC_D15),		(IEN  | PTD | DIS | M0)) /* sdrc_d15 */\
	MUX_VAL(CP(SDRC_D16),		(IEN  | PTD | DIS | M0)) /* sdrc_d16 */\
	MUX_VAL(CP(SDRC_D17),		(IEN  | PTD | DIS | M0)) /* sdrc_d17 */\
	MUX_VAL(CP(SDRC_D18),		(IEN  | PTD | DIS | M0)) /* sdrc_d18 */\
	MUX_VAL(CP(SDRC_D19),		(IEN  | PTD | DIS | M0)) /* sdrc_d19 */\
	MUX_VAL(CP(SDRC_D20),		(IEN  | PTD | DIS | M0)) /* sdrc_d20 */\
	MUX_VAL(CP(SDRC_D21),		(IEN  | PTD | DIS | M0)) /* sdrc_d21 */\
	MUX_VAL(CP(SDRC_D22),		(IEN  | PTD | DIS | M0)) /* sdrc_d22 */\
	MUX_VAL(CP(SDRC_D23),		(IEN  | PTD | DIS | M0)) /* sdrc_d23 */\
	MUX_VAL(CP(SDRC_D24),		(IEN  | PTD | DIS | M0)) /* sdrc_d24 */\
	MUX_VAL(CP(SDRC_D25),		(IEN  | PTD | DIS | M0)) /* sdrc_d25 */\
	MUX_VAL(CP(SDRC_D26),		(IEN  | PTD | DIS | M0)) /* sdrc_d26 */\
	MUX_VAL(CP(SDRC_D27),		(IEN  | PTD | DIS | M0)) /* sdrc_d27 */\
	MUX_VAL(CP(SDRC_D28),		(IEN  | PTD | DIS | M0)) /* sdrc_d28 */\
	MUX_VAL(CP(SDRC_D29),		(IEN  | PTD | DIS | M0)) /* sdrc_d29 */\
	MUX_VAL(CP(SDRC_D30),		(IEN  | PTD | DIS | M0)) /* sdrc_d30 */\
	MUX_VAL(CP(SDRC_D31),		(IEN  | PTD | DIS | M0)) /* sdrc_d31 */\
	MUX_VAL(CP(SDRC_CLK),		(IEN  | PTD | DIS | M0)) /* sdrc_clk */\
	MUX_VAL(CP(SDRC_DQS0),		(IEN  | PTD | DIS | M0)) /* sdrc_dqs0p */\
	MUX_VAL(CP(SDRC_CKE0),		(M0))                    /* sdrc_cke0_safe */\
	MUX_VAL(CP(SDRC_DQS1),		(IEN  | PTD | DIS | M0)) /* sdrc_dqs1p */\
	MUX_VAL(CP(SDRC_DQS2),		(IEN  | PTD | DIS | M0)) /* sdrc_dqs2p */\
	MUX_VAL(CP(SDRC_DQS3),		(IEN  | PTD | DIS | M0)) /* sdrc_dqs3p */\
	MUX_VAL(CP(SDRC_DQS0N),		(IEN  | PTD | EN  | M0)) /* sdrc_dqs0n */\
	MUX_VAL(CP(SDRC_DQS1N),		(IEN  | PTD | EN  | M0)) /* sdrc_dqs1n */\
	MUX_VAL(CP(SDRC_DQS2N),		(IEN  | PTD | EN  | M0)) /* sdrc_dqs2n */\
	MUX_VAL(CP(SDRC_DQS3N),		(IEN  | PTD | EN  | M0)) /* sdrc_dqs3n */\
	MUX_VAL(CP(STRBEN_DLY0),	(IEN  | PTD | EN  | M0)) /* sdrc_strben_dly0 */\
	MUX_VAL(CP(STRBEN_DLY1),	(IEN  | PTD | EN  | M0)) /* sdrc_strben_dly1 */\
/* GPMC */\
	MUX_VAL(CP(GPMC_A1),		(IEN  | PTD | EN  | M4)) /* TTL.DIR_0 gpio_34 */\
	MUX_VAL(CP(GPMC_A2),		(IEN  | PTD | EN  | M4)) /* TTL.IO_0  gpio_35 */\
	MUX_VAL(CP(GPMC_A3),		(IEN  | PTD | EN  | M4)) /* TTL.DIR_1 gpio_36 */\
	MUX_VAL(CP(GPMC_A4),		(IEN  | PTD | EN  | M4)) /* TTL.IO_1  gpio_37 */\
	MUX_VAL(CP(GPMC_A5),		(IEN  | PTD | EN  | M4)) /* TTL.DIR_2 gpio_38 */\
	MUX_VAL(CP(GPMC_A6),		(IEN  | PTU | EN  | M4)) /* TTL.IO_2  gpio_39 */\
	MUX_VAL(CP(GPMC_A7),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_A8),		(IEN  | PTU | EN  | M4)) /* MMC1.CD   gpio_41 */\
	MUX_VAL(CP(GPMC_A9),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_A10),		(IEN  | PTU | EN  | M4)) /* USB0_OC   gpio_43 */\
	MUX_VAL(CP(GPMC_D0),		(IEN  | PTU | EN  | M0)) /* GPMC_D0 */\
	MUX_VAL(CP(GPMC_D1),		(IEN  | PTU | EN  | M0)) /* GPMC_D1 */\
	MUX_VAL(CP(GPMC_D2),		(IEN  | PTU | EN  | M0)) /* GPMC_D2 */\
	MUX_VAL(CP(GPMC_D3),		(IEN  | PTU | EN  | M0)) /* GPMC_D3 */\
	MUX_VAL(CP(GPMC_D4),		(IEN  | PTU | EN  | M0)) /* GPMC_D4 */\
	MUX_VAL(CP(GPMC_D5),		(IEN  | PTU | EN  | M0)) /* GPMC_D5 */\
	MUX_VAL(CP(GPMC_D6),		(IEN  | PTU | EN  | M0)) /* GPMC_D6 */\
	MUX_VAL(CP(GPMC_D7),		(IEN  | PTU | EN  | M0)) /* GPMC_D7 */\
	MUX_VAL(CP(GPMC_D8),		(IEN  | PTU | EN  | M0)) /* GPMC_D8 */\
	MUX_VAL(CP(GPMC_D9),		(IEN  | PTU | EN  | M0)) /* GPMC_D9 */\
	MUX_VAL(CP(GPMC_D10),		(IEN  | PTU | EN  | M0)) /* GPMC_D10 */\
	MUX_VAL(CP(GPMC_D11),		(IEN  | PTU | EN  | M0)) /* GPMC_D11 */\
	MUX_VAL(CP(GPMC_D12),		(IEN  | PTU | EN  | M0)) /* GPMC_D12 */\
	MUX_VAL(CP(GPMC_D13),		(IEN  | PTU | EN  | M0)) /* GPMC_D13 */\
	MUX_VAL(CP(GPMC_D14),		(IEN  | PTU | EN  | M0)) /* GPMC_D14 */\
	MUX_VAL(CP(GPMC_D15),		(IEN  | PTU | EN  | M0)) /* GPMC_D15 */\
	MUX_VAL(CP(GPMC_NCS0),		(IDIS | PTU | EN  | M0)) /* GPMC_NCS0 - NAND CS# */\
	MUX_VAL(CP(GPMC_NCS1),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_NCS2),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_NCS3),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_NCS4),		(IEN  | PTD | DIS | M4)) /* INPUT_GPIO1 gpio_55 */\
	MUX_VAL(CP(GPMC_NCS5),		(IEN  | PTD | DIS | M4)) /* INPUT_GPIO2 gpio_56 */\
	MUX_VAL(CP(GPMC_NCS6),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_NCS7),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_CLK),		(IDIS | PTU | EN  | M0)) /* TP */\
	MUX_VAL(CP(GPMC_NADV_ALE),	(IDIS | PTD | DIS | M0)) /* gpmc_nadv_ale */\
	MUX_VAL(CP(GPMC_NOE),		(IDIS | PTD | DIS | M0)) /* gpmc_noe */\
	MUX_VAL(CP(GPMC_NWE),		(IDIS | PTD | DIS | M0)) /* gpmc_nwe */\
	MUX_VAL(CP(GPMC_NBE0_CLE),	(IDIS | PTU | EN  | M0)) /* gpmc_nbe0_cle */\
	MUX_VAL(CP(GPMC_NBE1),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_NWP),		(IEN  | PTD | DIS | M0)) /* gpmc_nwp */\
	MUX_VAL(CP(GPMC_WAIT0),		(IEN  | PTD | DIS | M0)) /* gpmc_wait0 */\
	MUX_VAL(CP(GPMC_WAIT1),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_WAIT2),		(M7))                    /* NC */\
	MUX_VAL(CP(GPMC_WAIT3),		(IDIS | PTU | EN  | M4 ))/* ETHERNET_nRST gpio_65 */\
/* DSS */ \
	MUX_VAL(CP(DSS_PCLK),		(IDIS | PTD | DIS | M0 )) /* dss_pclk */\
	MUX_VAL(CP(DSS_HSYNC),		(IDIS | PTD | DIS | M0 )) /* dss_hsync */\
	MUX_VAL(CP(DSS_VSYNC),		(IDIS | PTD | DIS | M0 )) /* dss_vsync */\
	MUX_VAL(CP(DSS_ACBIAS),		(IDIS | PTD | DIS | M0 )) /* dss_acbias */\
	MUX_VAL(CP(DSS_DATA0),		(IDIS | PTD | DIS | M0 )) /* dss_data0 */\
	MUX_VAL(CP(DSS_DATA1),		(IDIS | PTD | DIS | M0 )) /* dss_data1 */\
	MUX_VAL(CP(DSS_DATA2),		(IDIS | PTD | DIS | M0 )) /* dss_data2 */\
	MUX_VAL(CP(DSS_DATA3),		(IDIS | PTD | DIS | M0 )) /* dss_data3 */\
	MUX_VAL(CP(DSS_DATA4),		(IDIS | PTD | DIS | M0 )) /* dss_data4 */\
	MUX_VAL(CP(DSS_DATA5),		(IDIS | PTD | DIS | M0 )) /* dss_data5 */\
	MUX_VAL(CP(DSS_DATA6),		(IDIS | PTD | DIS | M0 )) /* dss_data6 */\
	MUX_VAL(CP(DSS_DATA7),		(IDIS | PTD | DIS | M0 )) /* dss_data7 */\
	MUX_VAL(CP(DSS_DATA8),		(IDIS | PTD | DIS | M0 )) /* dss_data8 */\
	MUX_VAL(CP(DSS_DATA9),		(IDIS | PTD | DIS | M0 )) /* dss_data9 */\
	MUX_VAL(CP(DSS_DATA10),		(IDIS | PTD | DIS | M0 )) /* dss_data10 */\
	MUX_VAL(CP(DSS_DATA11),		(IDIS | PTD | DIS | M0 )) /* dss_data11 */\
	MUX_VAL(CP(DSS_DATA12),		(IDIS | PTD | DIS | M0 )) /* dss_data12 */\
	MUX_VAL(CP(DSS_DATA13),		(IDIS | PTD | DIS | M0 )) /* dss_data13 */\
	MUX_VAL(CP(DSS_DATA14),		(IDIS | PTD | DIS | M0 )) /* dss_data14 */\
	MUX_VAL(CP(DSS_DATA15),		(IDIS | PTD | DIS | M0 )) /* dss_data15 */\
	MUX_VAL(CP(DSS_DATA16),		(IDIS | PTD | DIS | M0 )) /* dss_data16 */\
	MUX_VAL(CP(DSS_DATA17),		(IDIS | PTD | DIS | M0 )) /* dss_data17 */\
	MUX_VAL(CP(DSS_DATA18),		(IDIS | PTD | DIS | M0 )) /* dss_data18 */\
	MUX_VAL(CP(DSS_DATA19),		(IDIS | PTD | DIS | M0 )) /* dss_data19 */\
	MUX_VAL(CP(DSS_DATA20),		(IDIS | PTD | DIS | M0 )) /* dss_data20 */\
	MUX_VAL(CP(DSS_DATA21),		(IDIS | PTD | DIS | M0 )) /* dss_data21 */\
	MUX_VAL(CP(DSS_DATA22),		(IDIS | PTD | DIS | M0 )) /* dss_data22 */\
	MUX_VAL(CP(DSS_DATA23),		(IDIS | PTD | DIS | M0 )) /* dss_data23 */\
/*CCDC */ \
	MUX_VAL(CP(CCDC_PCLK),		(IEN  | PTD | DIS | M4)) /* CCDC.PCLK gpio_94 */\
/* UART4 */ \
	MUX_VAL(CP(CCDC_FIELD),		(IDIS | PTD | DIS | M2)) /* UART4_TX  gpio_95 H2-21 */\
	MUX_VAL(CP(CCDC_HD),		(IDIS | PTD | DIS | M2)) /* UART4_RTS gpio_96 H2-23 */\
	MUX_VAL(CP(CCDC_VD),		(IEN  | PTD | DIS | M2)) /* UART4_CTS gpio_97 H2-25 */\
	MUX_VAL(CP(CCDC_WEN),		(IEN  | PTD | DIS | M2)) /* UART4_RX  gpio_98 H2-27 */\
	MUX_VAL(CP(CCDC_DATA0),		(IEN  | PTD | EN  | M4)) /* gpio_99 */\
	MUX_VAL(CP(CCDC_DATA1),		(IEN  | PTD | EN  | M4)) /* gpio_100 */\
	MUX_VAL(CP(CCDC_DATA2),		(IEN  | PTD | DIS | M4)) /* gpio_101 */\
	MUX_VAL(CP(CCDC_DATA3),		(IEN  | PTD | DIS | M4)) /* gpio_102 */\
	MUX_VAL(CP(CCDC_DATA4),		(IEN  | PTD | DIS | M4)) /* gpio_103 */\
	MUX_VAL(CP(CCDC_DATA5),		(IEN  | PTD | DIS | M4)) /* gpio_104 */\
	MUX_VAL(CP(CCDC_DATA6),		(IEN  | PTD | DIS | M4)) /* gpio_105 H1-32 */\
	MUX_VAL(CP(CCDC_DATA7),		(IEN  | PTD | DIS | M4)) /* gpio_106 H1-11 */\
/* RMII / ETH */\
	MUX_VAL(CP(RMII_MDIO_DATA),	(IEN  |             M0)) /* rmii_mdio_data   gpio_107 */\
	MUX_VAL(CP(RMII_MDIO_CLK),	(M0))                    /* rmii_mdio_clk    gpio_108 */\
	MUX_VAL(CP(RMII_RXD0),		(IEN  | PTD |       M0)) /* rmii_rxd0        gpio_109 */\
	MUX_VAL(CP(RMII_RXD1),		(IEN  | PTD |       M0)) /* rmii_rxd1        gpio_110 */\
	MUX_VAL(CP(RMII_CRS_DV),	(IEN  | PTD |       M0)) /* rmii_crs_dv      gpio_111 */\
	MUX_VAL(CP(RMII_RXER),		(       PTD |       M0)) /* rmii_rxer        gpio_167 */\
	MUX_VAL(CP(RMII_TXD0),		(       PTD |       M0)) /* rmii_txd0        gpio_126 */\
	MUX_VAL(CP(RMII_TXD1),		(       PTD |       M0)) /* rmii_txd1        gpio_112 */\
	MUX_VAL(CP(RMII_TXEN),		(       PTD |       M0)) /* rmii_txen        gpio_113 */\
	MUX_VAL(CP(RMII_50MHZ_CLK),	(IEN  | PTD | EN  | M0)) /* rmii_50mhz_clk   gpio_114 */\
/* MCBSP2 */\
	MUX_VAL(CP(MCBSP2_CLKX),	(M7))                    /* NC */\
	MUX_VAL(CP(MCBSP2_DR),		(M7))                    /* NC */\
	MUX_VAL(CP(MCBSP2_DX),		(M7))                    /* NC */\
	MUX_VAL(CP(MCBSP2_FSX),		(M7))                    /* NC */\
/* MMC1 */\
	MUX_VAL(CP(MMC1_CLK),		(IEN  | PTU | EN  | M0)) /* mmc1_clk  */\
	MUX_VAL(CP(MMC1_CMD),		(IEN  | PTU | DIS | M0)) /* mmc1_cmd  */\
	MUX_VAL(CP(MMC1_DAT0),		(IEN  | PTU | DIS | M0)) /* mmc1_dat0 */\
	MUX_VAL(CP(MMC1_DAT1),		(IEN  | PTU | DIS | M0)) /* mmc1_dat1 */\
	MUX_VAL(CP(MMC1_DAT2),		(IEN  | PTU | DIS | M0)) /* mmc1_dat2 */\
	MUX_VAL(CP(MMC1_DAT3),		(IEN  | PTU | DIS | M0)) /* mmc1_dat3 */\
	MUX_VAL(CP(MMC1_DAT4),		(IDIS | PTU | EN  | M4)) /* GSM_nRESET       gpio_126 */\
	MUX_VAL(CP(MMC1_DAT5),		(IDIS | PTU | EN  | M4)) /* nGSM_ON/OFF      gpio_127 */\
	MUX_VAL(CP(MMC1_DAT6),		(IDIS | PTD | EN  | M4)) /* UART2.485/#232   gpio_128 */\
	MUX_VAL(CP(MMC1_DAT7),		(IDIS | PTD | EN  | M4)) /* UART2.SLEW       gpio_129 */\
/* MMC2 / WIFI */\
	MUX_VAL(CP(MMC2_CLK),		(IEN  | PTU | EN  | M0)) /* mmc2_clk            H1-02 */\
	MUX_VAL(CP(MMC2_CMD),		(IEN  | PTU | DIS | M0)) /* mmc2_cmd            H1-04 */\
	MUX_VAL(CP(MMC2_DAT0),		(IEN  | PTU | DIS | M0)) /* mmc2_dat0           H1-06 */\
	MUX_VAL(CP(MMC2_DAT1),		(IEN  | PTU | DIS | M0)) /* mmc2_dat1           H1-08 */\
	MUX_VAL(CP(MMC2_DAT2),		(IEN  | PTU | DIS | M0)) /* mmc2_dat2           H1-10 */\
	MUX_VAL(CP(MMC2_DAT3),		(IEN  | PTU | DIS | M0)) /* mmc2_dat3           H1-12 */\
	MUX_VAL(CP(MMC2_DAT4),		(IEN  | PTD | EN  | M4)) /* BT_PCM.WU  gpio_136 H1-14 */\
	MUX_VAL(CP(MMC2_DAT5),		(IEN  | PTD | EN  | M4)) /* WLAN.IRQ   gpio_137 H1-16 */\
	MUX_VAL(CP(MMC2_DAT6),		(IDIS | PTD | EN  | M4)) /* BT_HCI.EN  gpio_138 H1-18 */\
	MUX_VAL(CP(MMC2_DAT7),		(IDIS | PTD | EN  | M4)) /* WLAN.EN    gpio_139 H1-20 */\
/* MCBSP3 */\
	MUX_VAL(CP(MCBSP3_CLKX),	(M7))                    /* NC */\
	MUX_VAL(CP(MCBSP3_DR),		(M7))                    /* NC */\
	MUX_VAL(CP(MCBSP3_DX),		(IEN | PTU | EN  | M4)) /* DVI.LCDPD gpio_140 */\
	MUX_VAL(CP(MCBSP3_FSX),		(M7))                    /* NC */\
/* UART2 */\
	MUX_VAL(CP(UART2_CTS),		(IEN  | PTD | DIS | M4)) /* UART2.RXEN   gpio_144 */\
	MUX_VAL(CP(UART2_RTS),		(IEN  | PTD | DIS | M4)) /* UART2.DEN    gpio_145 */\
	MUX_VAL(CP(UART2_TX),		(IDIS | PTD | DIS | M0)) /* UART2_TX */\
	MUX_VAL(CP(UART2_RX),		(IEN  | PTD | DIS | M0)) /* UART2_RX */\
/* UART1 */\
	MUX_VAL(CP(UART1_CTS),		(M7))                    /* NC */\
	MUX_VAL(CP(UART1_RTS),		(IEN  | PTD | DIS | M4)) /* uart1_rts    gpio_149 */\
	MUX_VAL(CP(UART1_TX),		(IDIS | PTD | DIS | M0)) /* uart1_tx */\
	MUX_VAL(CP(UART1_RX),		(IEN  | PTD | DIS | M0)) /* uart1_rx */\
/* MCBSP4 */ \
	MUX_VAL(CP(MCBSP4_CLKX),	(M7))                    /* NC */\
	MUX_VAL(CP(MCBSP4_DR),		(M7))                    /* NC */\
	MUX_VAL(CP(MCBSP4_DX),		(M7))                    /* NC */\
	MUX_VAL(CP(MCBSP4_FSX),		(M7))                    /* NC */\
/* MCBSP1 */ \
	MUX_VAL(CP(MCBSP_CLKS),		(IEN  | PTU | DIS | M0)) /* mcbsp_clks */\
	MUX_VAL(CP(MCBSP1_CLKR),	(IEN  | PTD | DIS | M0)) /* mcbsp1_clkr */\
	MUX_VAL(CP(MCBSP1_FSR),		(IDIS | PTU | EN  | M0)) /* mcbsp1_fsr */\
	MUX_VAL(CP(MCBSP1_DX),		(IDIS | PTD | DIS | M0)) /* mcbsp1_dx */\
	MUX_VAL(CP(MCBSP1_DR),		(IEN  | PTD | DIS | M0)) /* mcbsp1_dr */\
	MUX_VAL(CP(MCBSP1_FSX),		(IEN  | PTD | DIS | M0)) /* mcbsp1_fsx */\
	MUX_VAL(CP(MCBSP1_CLKX),	(IEN  | PTD | DIS | M0)) /* mcbsp1_clkx */\
/* UART3 */\
	MUX_VAL(CP(UART3_CTS_RCTX),	(IEN  | PTU | DIS | M0)) /* uart3_cts_rctx */\
	MUX_VAL(CP(UART3_RTS_SD),	(IDIS | PTD | DIS | M0)) /* uart3_rts_sd */\
	MUX_VAL(CP(UART3_RX_IRRX),	(IEN  | PTD | DIS | M0)) /* uart3_rx_irrx */\
	MUX_VAL(CP(UART3_TX_IRTX),	(IDIS | PTD | DIS | M0)) /* uart3_tx_irtx */\
/* HSUSB / USB0 */ \
	MUX_VAL(CP(USB0_DRVBUS),	(IEN  | PTD | EN  | M0)) /* usb0_drvvbus */\
/* HECC / CAN */ \
	MUX_VAL(CP(HECC1_RXD),		(IEN  | PTU | EN  | M0)) /* hecc1_rxd */\
	MUX_VAL(CP(HECC1_TXD),		(IEN  | PTU | EN  | M0)) /* hecc1_txd */\
/* I2C1,2,3 */\
	MUX_VAL(CP(I2C1_SCL),		(IEN  | PTU | EN  | M0)) /* i2c1_scl */\
	MUX_VAL(CP(I2C1_SDA),		(IEN  | PTU | EN  | M0)) /* i2c1_sda */\
	MUX_VAL(CP(I2C2_SCL),		(IEN  | PTU | EN  | M0)) /* i2c2_scl H2-15 */\
	MUX_VAL(CP(I2C2_SDA),		(IEN  | PTU | EN  | M0)) /* i2c2_sda H2-17 */\
	MUX_VAL(CP(I2C3_SCL),		(IEN  | PTU | EN  | M0)) /* i2c3_scl H1-01 */\
	MUX_VAL(CP(I2C3_SDA),		(IEN  | PTU | EN  | M0)) /* i2c3_sda H1-03 */\
/* HDQ,	MCSPI1 */\
	MUX_VAL(CP(HDQ_SIO),		(IEN  | PTU | EN  | M0)) /* hdq_sio     H2-19 */\
	MUX_VAL(CP(MCSPI1_CLK),		(IEN  | PTU | EN  | M0)) /* mcspi1_clk  H2-29 */\
	MUX_VAL(CP(MCSPI1_SIMO),	(IEN  | PTU | EN  | M0)) /* mcspi1_simo H2-31 */\
	MUX_VAL(CP(MCSPI1_SOMI),	(IEN  | PTU | EN  | M0)) /* mcspi1_somi H2-33 */\
	MUX_VAL(CP(MCSPI1_CS0),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs0  H2-35 */\
	MUX_VAL(CP(MCSPI1_CS1),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs1  H2-37 */\
	MUX_VAL(CP(MCSPI1_CS2),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs2  H2-39 */\
	MUX_VAL(CP(MCSPI1_CS3),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs3  H2-32 */\
/* MCSPI2 */\
	MUX_VAL(CP(MCSPI2_CLK),		(IEN  | PTU | EN  | M0)) /* mcspi2_clk  H1-22 */\
	MUX_VAL(CP(MCSPI2_SIMO),	(IEN  | PTU | EN  | M0)) /* mcspi2_simo H1-24 */\
	MUX_VAL(CP(MCSPI2_SOMI),	(IEN  | PTU | EN  | M0)) /* mcspi2_somi H1-26 */\
	MUX_VAL(CP(MCSPI2_CS0),		(IDIS | PTU | EN  | M0)) /* mcspi2_cs0  H1-28 */\
	MUX_VAL(CP(MCSPI2_CS1),		(IDIS | PTU | EN  | M0)) /* mcspi2_cs1  H1-30 */\
	MUX_VAL(CP(SYS_NIRQ),		(IEN  | PTU | EN  | M0)) /* TPS_INT1 SYS_nIRQ */\
	MUX_VAL(CP(SYS_CLKOUT2),	(IEN  | PTU | EN  | M0)) /* unused sys_clkout2  H1-15 */\
/* ETK / HSUSB1/2 */\
	MUX_VAL(CP(ETK_CLK_ES2),	(IEN  | PTU | EN  | M3)) /* HSUSB1_STP   gpio_12  H2-22 */\
	MUX_VAL(CP(ETK_CTL_ES2),	(IDIS | PTD | DIS | M3)) /* HSUSB1_CLK   gpio_13  H2-24 */\
	MUX_VAL(CP(ETK_D0_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DATA0 gpio_14  H2-26 */\
	MUX_VAL(CP(ETK_D1_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DATA1 gpio_15  H2-12 */\
	MUX_VAL(CP(ETK_D2_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DATA2 gpio_16  H2-14 */\
	MUX_VAL(CP(ETK_D3_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DATA7 gpio_17  H2-16 */\
	MUX_VAL(CP(ETK_D4_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DATA4 gpio_18  H2-20 */\
	MUX_VAL(CP(ETK_D5_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DATA5 gpio_19  H2-28 */\
	MUX_VAL(CP(ETK_D6_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DATA6 gpio_20  H2-30 */\
	MUX_VAL(CP(ETK_D7_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DATA3 gpio_21  H2-18 */\
	MUX_VAL(CP(ETK_D8_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_DIR   gpio_22  H1-05 */\
	MUX_VAL(CP(ETK_D9_ES2),		(IEN  | PTD | DIS | M3)) /* HSUSB1_NXT   gpio_23  H1-07 */\
	MUX_VAL(CP(ETK_D10_ES2),	(M7))                    /* NC */\
	MUX_VAL(CP(ETK_D11_ES2),	(M7))                    /* NC */\
	MUX_VAL(CP(ETK_D12_ES2),	(M7))                    /* NC */\
	MUX_VAL(CP(ETK_D13_ES2),	(M7))                    /* NC */\
	MUX_VAL(CP(ETK_D14_ES2),	(IEN | PTD | EN  | M4))	 /* EN_VCC_5V_Per gpio_28 H1-13 FIXME PTU/PTD? */\
	MUX_VAL(CP(ETK_D15_ES2),	(IDIS | PTU | EN  | M4)) /* EN_GSM_POWER  gpio_29 */\
/*SYS */\
	MUX_VAL(CP(SYS_32K),		(IEN  | PTD | DIS | M0)) /* sys_32k */\
	MUX_VAL(CP(SYS_CLKREQ),		(IEN  | PTD | DIS | M4)) /* gpio_01 sys_clkreq    H1-09 */\
	MUX_VAL(CP(SYS_NRESWARM),	(IEN  | PTU | EN  | M0)) /* sys_nreswarm */\
	MUX_VAL(CP(SYS_BOOT0),		(IEN  | PTD | DIS | M0)) /* sys_boot0 */\
	MUX_VAL(CP(SYS_BOOT1),		(IEN  | PTD | DIS | M0)) /* sys_boot1 */\
	MUX_VAL(CP(SYS_BOOT2),		(IEN  | PTD | DIS | M0)) /* sys_boot2 */\
	MUX_VAL(CP(SYS_BOOT3),		(IEN  | PTD | DIS | M0)) /* sys_boot3 */\
	MUX_VAL(CP(SYS_BOOT4),		(IEN  | PTD | DIS | M0)) /* sys_boot4 */\
	MUX_VAL(CP(SYS_BOOT5),		(IEN  | PTD | DIS | M0)) /* sys_boot5 */\
	MUX_VAL(CP(SYS_BOOT6),		(IEN  | PTD | DIS | M0)) /* sys_boot6 */\
	MUX_VAL(CP(SYS_BOOT7),		(IEN  | PTD | DIS | M0)) /* sys_boot7 */\
	MUX_VAL(CP(SYS_BOOT8),		(IEN  | PTD | DIS | M0)) /* sys_boot8 */\
	MUX_VAL(CP(SYS_CLKOUT1),	(IEN  | PTD | DIS | M0)) /* gpio_10 sys_clkout1  H1-17*/\
/* JTAG */\
	MUX_VAL(CP(JTAG_nTRST),		(IEN  | PTD | DIS | M0)) /* jtag_ntrst */\
	MUX_VAL(CP(JTAG_TCK),		(IEN  | PTD | DIS | M0)) /* jtag_tck */\
	MUX_VAL(CP(JTAG_TDI),		(IEN  | PTD | DIS | M0)) /* jtag_tdi */\
	MUX_VAL(CP(JTAG_EMU0),		(IEN  | PTD | DIS | M0)) /* jtag_emu0 */\
	MUX_VAL(CP(JTAG_EMU1),		(IEN  | PTD | DIS | M0)) /* jtag_emu1 */\
	MUX_VAL(CP(JTAG_TMS),		(IEN  | PTD | DIS | M0)) /* jtag_tms_tmsc */\

/* Expansion Board */
#define MUX_PIA_WIFI() \
	//define mux here

#define MUX_PIA_LCD() \
	/* nothing yet */

#define MUX_PIA_LCD_DT() \
	MUX_VAL(CP(DSS_DATA18),		(IEN  | PTU | EN  | M2 )) /* mcspi3_clk */\
	MUX_VAL(CP(DSS_DATA19),		(IEN  | PTU | EN  | M2 )) /* mcspi3_simo */\
	MUX_VAL(CP(DSS_DATA20),		(IEN  | PTU | EN  | M2 )) /* mcspi3_somi */\
	MUX_VAL(CP(DSS_DATA21),		(IDIS | PTU | EN  | M2 )) /* mcspi3_cs0 */\
	MUX_VAL(CP(DSS_DATA22),		(IDIS | PTU | EN  | M2 )) /* mcspi3_cs1 */\
	MUX_VAL(CP(CCDC_DATA3),		(IEN  | PTU | DIS | M4)) /* gpio_102, display_reset */\


#define MUX_PIA_MC() \
	//define mux here

#define MUX_PIA_CC() \
	//define mux here

#define MUX_PIA_IO() \
	//define mux here

#define MUX_PIA_EMS_IO() \
	MUX_VAL(CP(MMC2_DAT0),		(IEN  | PTU | EN  | M4)) /* CAN_INT1   gpio_132 H1-35 */\
	MUX_VAL(CP(MMC2_DAT1),		(IEN  | PTU | EN  | M4)) /* CAN_INT2   gpio_133 H1-33 */\
	MUX_VAL(CP(MMC2_DAT2),		(IEN  | PTU | EN  | M4)) /* CAN_INT3   gpio_134 H1-31 */\
	MUX_VAL(CP(MMC2_DAT3),		(IEN  | PTU | EN  | M4)) /* RS485_INT1 gpio_135 H1-29 */\
	MUX_VAL(CP(MMC2_DAT4),		(IEN  | PTU | EN  | M4)) /* RS485_INT2 gpio_136 H1-27 */\
	MUX_VAL(CP(MMC2_DAT5),		(IEN  | PTU | EN  | M4)) /* RS485_INT3 gpio_137 H1-25 */\
	MUX_VAL(CP(MMC2_DAT6),		(IEN  | PTU | EN  | M4)) /* RS485_INT4 gpio_138 H1-23 */\
	MUX_VAL(CP(MMC2_DAT7),		(M7))                    /* NC                  H1-21 */\
	MUX_VAL(CP(HDQ_SIO),		(M7))                    /* hdq_sio             H2-22 */\
	/* MCSPI1 */ \
	MUX_VAL(CP(MCSPI1_CLK),		(IEN  | PTU | DIS | M0)) /* mcspi1_clk  H2-12 */\
	MUX_VAL(CP(MCSPI1_SIMO),	(IDIS | PTU | DIS | M0))/* mcspi1_simo H2-10 */\
	MUX_VAL(CP(MCSPI1_SOMI),	(IEN  | PTU | DIS | M0)) /* mcspi1_somi H2-08 */\
	MUX_VAL(CP(MCSPI1_CS0),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs0  H2-06 */\
	MUX_VAL(CP(MCSPI1_CS1),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs1  H2-04 */\
	MUX_VAL(CP(MCSPI1_CS2),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs2  H2-02 */\
	MUX_VAL(CP(MCSPI1_CS3),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs3  H2-09 */\
	/* MCSPI2 */\
	MUX_VAL(CP(MCSPI2_CLK),		(IEN  | PTU | DIS | M0)) /* mcspi2_clk  H1-19 */\
	MUX_VAL(CP(MCSPI2_SIMO),	(IDIS | PTU | DIS | M0))/* mcspi2_simo H1-17 */\
	MUX_VAL(CP(MCSPI2_SOMI),	(IEN  | PTU | DIS | M0))/* mcspi2_somi H1-15 */\
	MUX_VAL(CP(MCSPI2_CS0),		(IDIS | PTU | EN  | M4)) /* mcspi2_cs0 gpio_181 H1-13 */\
	MUX_VAL(CP(MCSPI2_CS1),		(IDIS | PTU | EN  | M4)) /* mcspi2_cs1 gpio_182 H1-11 */\
	MUX_VAL(CP(ETK_CLK_ES2),	(IDIS | PTU | EN  | M4)) /* mcspi2_cs2 gpio_12  H2-22 */\
	MUX_VAL(CP(ETK_D0_ES2),		(IDIS | PTU | EN  | M4)) /* RESET      gpio_14  H2-15 */\
	MUX_VAL(CP(ETK_D3_ES2),		(IEN  | PTU | EN  | M4)) /* DISP_INT   gpio_17  H2-25 */\
	MUX_VAL(CP(ETK_D5_ES2),		(IEN  | PTU | EN  | M4)) /* DIN_INT1   gpio_19  H2-13 */\
	MUX_VAL(CP(ETK_D7_ES2),		(IEN  | PTU | EN  | M4)) /* DIN_INT2   gpio_21  H2-23 */\

#define MUX_PIA_EMS_IO_V3() \
	MUX_VAL(CP(MMC2_CLK),		(IDIS | PTU | DIS | M4)) /* USB_Reset  gpio_130 H1-39 */\
	MUX_VAL(CP(MMC2_DAT0),		(IEN  | PTU | EN  | M4)) /* CAN_INT1   gpio_132 H1-35 */\
	MUX_VAL(CP(MMC2_DAT1),		(IEN  | PTU | EN  | M4)) /* CAN_INT2   gpio_133 H1-33 */\
	MUX_VAL(CP(MMC2_DAT2),		(IEN  | PTU | EN  | M4)) /* CAN_INT3   gpio_134 H1-31 */\
	MUX_VAL(CP(MMC2_DAT3),		(IDIS | PTU | DIS | M4)) /* <io>_RESET gpio_135 H1-29 */\
	MUX_VAL(CP(MMC2_DAT4),		(IEN  | PTU | EN  | M4)) /* DISP_INT   gpio_136 H1-27 */\
	MUX_VAL(CP(MMC2_DAT5),		(IEN  | PTU | EN  | M4)) /* DIN_INT1   gpio_137 H1-25 */\
	MUX_VAL(CP(MMC2_DAT6),		(IEN  | PTU | EN  | M4)) /* DIN_INT2   gpio_138 H1-23 */\
	MUX_VAL(CP(MMC2_DAT7),		(M7))                    /* NC                  H1-21 */\
	MUX_VAL(CP(HDQ_SIO),		(M7))                    /* hdq_sio             H2-22 */\
	/* MCSPI1 */ \
	MUX_VAL(CP(MCSPI1_CLK),		(IEN  | PTU | DIS | M0)) /* mcspi1_clk  H2-12 */\
	MUX_VAL(CP(MCSPI1_SIMO),	(IDIS | PTU | DIS | M0)) /* mcspi1_simo H2-10 */\
	MUX_VAL(CP(MCSPI1_SOMI),	(IEN  | PTU | DIS | M0)) /* mcspi1_somi H2-08 */\
	MUX_VAL(CP(MCSPI1_CS0),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs0  H2-06 */\
	MUX_VAL(CP(MCSPI1_CS1),		(IDIS | PTU | EN  | M0)) /* NC mcspi1_cs1  H2-04 */\
	MUX_VAL(CP(MCSPI1_CS2),		(IDIS | PTU | EN  | M0)) /* mcspi1_cs2  H2-02 */\
	MUX_VAL(CP(MCSPI1_CS3),		(IDIS | PTU | EN  | M0)) /* NC mcspi1_cs3  H2-09 */\
	/* MCSPI2 */\
	MUX_VAL(CP(MCSPI2_CLK),		(IEN  | PTU | DIS | M0)) /* mcspi2_clk  H1-19 */\
	MUX_VAL(CP(MCSPI2_SIMO),	(IDIS | PTU | DIS | M0)) /* mcspi2_simo H1-17 */\
	MUX_VAL(CP(MCSPI2_SOMI),	(IEN  | PTU | DIS | M0)) /* mcspi2_somi H1-15 */\
	MUX_VAL(CP(MCSPI2_CS0),		(IDIS | PTU | EN  | M0)) /* mcspi2_cs0 gpio_181 H1-13 */\
	MUX_VAL(CP(MCSPI2_CS1),		(IDIS | PTU | EN  | M0)) /* NC   gpio_182 H1-11 */\
	MUX_VAL(CP(ETK_CLK_ES2),	(IDIS | PTU | EN  | M3)) /* HSUSB1.STP   gpio_12  H2-19 */\
	MUX_VAL(CP(ETK_CTL_ES2),	(IDIS | PTU | DIS | M3)) /* HSUSB1.CLK   gpio_13  H2-17 */\
	MUX_VAL(CP(ETK_D0_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1.DATA0 gpio_14  H2-15 */\
	MUX_VAL(CP(ETK_D1_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1_DATA1 gpio_15  H2-29 */\
	MUX_VAL(CP(ETK_D2_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1_DATA2 gpio_16  H2-27 */\
	MUX_VAL(CP(ETK_D3_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1.DATA3 gpio_17  H2-25 */\
	MUX_VAL(CP(ETK_D4_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1.DATA4 gpio_18  H2-21 */\
	MUX_VAL(CP(ETK_D5_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1.DATA5 gpio_19  H2-13 */\
	MUX_VAL(CP(ETK_D6_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1.DATA6 gpio_20  H2-11 */\
	MUX_VAL(CP(ETK_D7_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1.DATA7 gpio_21  H2-23 */\
	MUX_VAL(CP(ETK_D8_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1.DIR   gpio_22  H1-36 */\
	MUX_VAL(CP(ETK_D9_ES2),		(IEN  | PTU | DIS | M3)) /* HSUSB1.NXT   gpio_23  H1-34 */\

#endif /* _AM3517CRANE_H_ */
