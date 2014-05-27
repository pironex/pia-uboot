/*
 * mux.c
 *
 * Copyright (C) 2012 pironex GmbH
 * based on am335x/mux.c by Texas Instruments Incorporated - http://www.ti.com/
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
#include <asm/arch/sys_proto.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <i2c.h>

#include "am335xpia.h"

#define MUX_CFG(value, offset)	\
	__raw_writel(value, (CTRL_BASE + offset));

/* PAD Control Fields */
#define SLEWCTRL	(0x1 << 6)
#define IEN    (0x1 << 5)
#define P_UP   (0x1 << 4) /* Pull UP Selection */
#define P_DOWN (0x0 << 4) /* Pull DOWN Selection */
#define P_EN   (0x0 << 3) /* Pull enabled */
#define P_DIS   (0x1 << 3) /* Pull up disabled */

#if 0
#define RXACTIVE (0x1 << 5)
#define PULLUP_EN	(0x1 << 4) /* Pull UP Selection */
#define PULLUDEN	(0x0 << 3) /* Pull up enabled */
#define PULLUDDIS	(0x1 << 3) /* Pull up disabled */
#define MODE(val)	val	/* used for Readability */
#endif
#define	PIN_OUTPUT          (0)
#define	PIN_OUTPUT_PULLUP   (P_UP)
#define	PIN_INPUT           (IEN | P_DIS)
#define	PIN_INPUT_PULLUP    (IEN | P_UP | P_EN)
#define	PIN_INPUT_PULLDOWN	(IEN | P_DOWN | P_EN)

/* mux modes */
#define M0 0
#define M1 1
#define M2 2
#define M3 3
#define M4 4
#define M5 5
#define M6 6
#define M7 7

/*
 * PAD CONTROL OFFSETS
 * Field names corresponds to the pad signal name
 */
struct pad_signals {
	int gpmc_ad0;
	int gpmc_ad1;
	int gpmc_ad2;
	int gpmc_ad3;
	int gpmc_ad4;
	int gpmc_ad5;
	int gpmc_ad6;
	int gpmc_ad7;
	int gpmc_ad8;
	int gpmc_ad9;
	int gpmc_ad10;
	int gpmc_ad11;
	int gpmc_ad12;
	int gpmc_ad13;
	int gpmc_ad14;
	int gpmc_ad15;
	int gpmc_a0;
	int gpmc_a1;
	int gpmc_a2;
	int gpmc_a3;
	int gpmc_a4;
	int gpmc_a5;
	int gpmc_a6;
	int gpmc_a7;
	int gpmc_a8;
	int gpmc_a9;
	int gpmc_a10;
	int gpmc_a11;
	int gpmc_wait0;
	int gpmc_wpn;
	int gpmc_be1n;
	int gpmc_csn0;
	int gpmc_csn1;
	int gpmc_csn2;
	int gpmc_csn3;
	int gpmc_clk;
	int gpmc_advn_ale;
	int gpmc_oen_ren;
	int gpmc_wen;
	int gpmc_be0n_cle;
	int lcd_data0;
	int lcd_data1;
	int lcd_data2;
	int lcd_data3;
	int lcd_data4;
	int lcd_data5;
	int lcd_data6;
	int lcd_data7;
	int lcd_data8;
	int lcd_data9;
	int lcd_data10;
	int lcd_data11;
	int lcd_data12;
	int lcd_data13;
	int lcd_data14;
	int lcd_data15;
	int lcd_vsync;
	int lcd_hsync;
	int lcd_pclk;
	int lcd_ac_bias_en;
	int mmc0_dat3;
	int mmc0_dat2;
	int mmc0_dat1;
	int mmc0_dat0;
	int mmc0_clk;
	int mmc0_cmd;
	int mii1_col;
	int mii1_crs;
	int mii1_rxerr;
	int mii1_txen;
	int mii1_rxdv;
	int mii1_txd3;
	int mii1_txd2;
	int mii1_txd1;
	int mii1_txd0;
	int mii1_txclk;
	int mii1_rxclk;
	int mii1_rxd3;
	int mii1_rxd2;
	int mii1_rxd1;
	int mii1_rxd0;
	int rmii1_refclk;
	int mdio_data;
	int mdio_clk;
	int spi0_sclk;
	int spi0_d0;
	int spi0_d1;
	int spi0_cs0;
	int spi0_cs1;
	int ecap0_in_pwm0_out;
	int uart0_ctsn;
	int uart0_rtsn;
	int uart0_rxd;
	int uart0_txd;
	int uart1_ctsn;
	int uart1_rtsn;
	int uart1_rxd;
	int uart1_txd;
	int i2c0_sda;
	int i2c0_scl;
	int mcasp0_aclkx;
	int mcasp0_fsx;
	int mcasp0_axr0;
	int mcasp0_ahclkr;
	int mcasp0_aclkr;
	int mcasp0_fsr;
	int mcasp0_axr1;
	int mcasp0_ahclkx;
	int xdma_event_intr0;
	int xdma_event_intr1;
	int nresetin_out;
	int porz;
	int nnmi;
	int osc0_in;
	int osc0_out;
	int rsvd1;
	int tms;
	int tdi;
	int tdo;
	int tck;
	int ntrst;
	int emu0;
	int emu1;
	int osc1_in;
	int osc1_out;
	int pmic_power_en;
	int rtc_porz;
	int rsvd2;
	int ext_wakeup;
	int enz_kaldo_1p8v;
	int usb0_dm;
	int usb0_dp;
	int usb0_ce;
	int usb0_id;
	int usb0_vbus;
	int usb0_drvvbus;
	int usb1_dm;
	int usb1_dp;
	int usb1_ce;
	int usb1_id;
	int usb1_vbus;
	int usb1_drvvbus;
	int ddr_resetn;
	int ddr_csn0;
	int ddr_cke;
	int ddr_ck;
	int ddr_nck;
	int ddr_casn;
	int ddr_rasn;
	int ddr_wen;
	int ddr_ba0;
	int ddr_ba1;
	int ddr_ba2;
	int ddr_a0;
	int ddr_a1;
	int ddr_a2;
	int ddr_a3;
	int ddr_a4;
	int ddr_a5;
	int ddr_a6;
	int ddr_a7;
	int ddr_a8;
	int ddr_a9;
	int ddr_a10;
	int ddr_a11;
	int ddr_a12;
	int ddr_a13;
	int ddr_a14;
	int ddr_a15;
	int ddr_odt;
	int ddr_d0;
	int ddr_d1;
	int ddr_d2;
	int ddr_d3;
	int ddr_d4;
	int ddr_d5;
	int ddr_d6;
	int ddr_d7;
	int ddr_d8;
	int ddr_d9;
	int ddr_d10;
	int ddr_d11;
	int ddr_d12;
	int ddr_d13;
	int ddr_d14;
	int ddr_d15;
	int ddr_dqm0;
	int ddr_dqm1;
	int ddr_dqs0;
	int ddr_dqsn0;
	int ddr_dqs1;
	int ddr_dqsn1;
	int ddr_vref;
	int ddr_vtp;
	int ddr_strben0;
	int ddr_strben1;
	int ain7;
	int ain6;
	int ain5;
	int ain4;
	int ain3;
	int ain2;
	int ain1;
	int ain0;
	int vrefp;
	int vrefn;
};

struct module_pin_mux {
	short reg_offset;
	unsigned char val;
};

/* Pad control register offset */
#define PAD_CTRL_BASE	0x800
#define OFFSET(x)	(unsigned int) (&((struct pad_signals *) \
				(PAD_CTRL_BASE))->x)

/* piA-AM335x-PM base configuration for fixed and "most likely" fixed pin
 * configs. This includes I2C, SPI busses, MII */
static struct module_pin_mux pia335x_pm[] = {
	{ OFFSET(gpmc_ad0),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D0 */
	{ OFFSET(gpmc_ad1),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D1 */
	{ OFFSET(gpmc_ad2),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D2*/
	{ OFFSET(gpmc_ad3),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D3 */
	{ OFFSET(gpmc_ad4),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D4 */
	{ OFFSET(gpmc_ad5),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D5 */
	{ OFFSET(gpmc_ad6),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D6 */
	{ OFFSET(gpmc_ad7),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D7 */
	{ OFFSET(gpmc_ad8),         (M7 | PIN_INPUT) }, /* GPIO 0_22 */
	{ OFFSET(gpmc_ad9),         (M7 | PIN_INPUT) }, /* GPIO 0_23 */
	{ OFFSET(gpmc_ad10),        (M7 | PIN_INPUT) }, /* GPIO 0_26 */
	{ OFFSET(gpmc_ad11),        (M7 | PIN_INPUT) }, /* GPIO 0_27 */
	{ OFFSET(gpmc_ad12),        (M7 | PIN_INPUT) }, /* GPIO 1_12 */
	{ OFFSET(gpmc_ad13),        (M7 | PIN_INPUT) }, /* GPIO 1_13 */
	{ OFFSET(gpmc_ad14),        (M7 | PIN_INPUT) }, /* GPIO 1_14 */
	{ OFFSET(gpmc_ad15),        (M7 | PIN_INPUT) }, /* GPIO 1_15 */
	{ OFFSET(gpmc_a0),          (M1 | PIN_OUTPUT) }, /* MII2 TXEN */
	{ OFFSET(gpmc_a1),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXDV */
	{ OFFSET(gpmc_a2),          (M1 | PIN_OUTPUT) }, /* MII2 TXD3 */
	{ OFFSET(gpmc_a3),          (M1 | PIN_OUTPUT) }, /* MII2 TXD2 */
	{ OFFSET(gpmc_a4),          (M1 | PIN_OUTPUT) }, /* MII2 TXD1 */
	{ OFFSET(gpmc_a5),          (M1 | PIN_OUTPUT) }, /* MII2 TXD0 */
	{ OFFSET(gpmc_a6),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 TXCLK */
	{ OFFSET(gpmc_a7),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXCLK */
	{ OFFSET(gpmc_a8),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXD3 */
	{ OFFSET(gpmc_a9),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXD2 */
	{ OFFSET(gpmc_a10),         (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXD1 */
	{ OFFSET(gpmc_a11),         (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXD0 */
	{ OFFSET(gpmc_wait0),       (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 CRS */
	{ OFFSET(gpmc_wpn),         (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXER */
	{ OFFSET(gpmc_be1n),        (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 COL */
	{ OFFSET(gpmc_csn0),        (M7 | PIN_INPUT) }, /* GPIO 1_29 LED1*/
	{ OFFSET(gpmc_csn1),        (M2 | PIN_INPUT_PULLUP) }, /* MMC1 CLK */
	{ OFFSET(gpmc_csn2),        (M2 | PIN_INPUT_PULLUP) }, /* MMC1 CMD */
	{ OFFSET(gpmc_csn3),        (M7 | PIN_INPUT) }, /* GPIO 2_0 */
	{ OFFSET(gpmc_clk),         (M7 | PIN_INPUT_PULLUP) }, /* optional Cap Touch Int */
	{ OFFSET(gpmc_advn_ale),    (M7 | PIN_INPUT) }, /* GPIO 2_2 */
	{ OFFSET(gpmc_oen_ren),     (M7 | PIN_INPUT) }, /* GPIO 2_3 */
	{ OFFSET(gpmc_wen),         (M7 | PIN_INPUT) }, /* GPIO 2_4 */
	{ OFFSET(gpmc_be0n_cle),    (M7 | PIN_INPUT) }, /* GPIO 2_5 */
	{ OFFSET(lcd_data0),        (M7 | PIN_INPUT) }, /* GPIO 2_6 */
	{ OFFSET(lcd_data1),        (M7 | PIN_INPUT) }, /* GPIO 2_7 */
	{ OFFSET(lcd_data2),        (M7 | PIN_INPUT) }, /* GPIO 2_8 */
	{ OFFSET(lcd_data3),        (M7 | PIN_INPUT) }, /* GPIO 2_9 */
	{ OFFSET(lcd_data4),        (M7 | PIN_INPUT) }, /* GPIO 2_10 */
	{ OFFSET(lcd_data5),        (M7 | PIN_INPUT) }, /* GPIO 2_11 */
	{ OFFSET(lcd_data6),        (M7 | PIN_INPUT) }, /* GPIO 2_12 */
	{ OFFSET(lcd_data7),        (M7 | PIN_INPUT) }, /* GPIO 2_13 */
	{ OFFSET(lcd_data8),        (M7 | PIN_INPUT) }, /* GPIO 2_14 */
	{ OFFSET(lcd_data9),        (M7 | PIN_INPUT) }, /* GPIO 2_15 */
	{ OFFSET(lcd_data10),       (M7 | PIN_INPUT) }, /* GPIO 2_16 */
	{ OFFSET(lcd_data11),       (M7 | PIN_INPUT) }, /* GPIO 2_17 */
	{ OFFSET(lcd_data12),       (M7 | PIN_INPUT) }, /* GPIO 0_8 */
	{ OFFSET(lcd_data13),       (M7 | PIN_INPUT) }, /* GPIO 0_9 */
	{ OFFSET(lcd_data14),       (M7 | PIN_INPUT) }, /* GPIO 0_10 */
	{ OFFSET(lcd_data15),       (M7 | PIN_INPUT) }, /* GPIO 0_11 */
	{ OFFSET(lcd_vsync),        (M7 | PIN_INPUT) }, /* GPIO 2_22 */
	{ OFFSET(lcd_hsync),        (M7 | PIN_INPUT) }, /* GPIO 2_24 */
	{ OFFSET(lcd_pclk),         (M7 | PIN_INPUT) }, /* GPIO 2_24 */
	{ OFFSET(lcd_ac_bias_en),   (M7 | PIN_INPUT) }, /* GPIO 2_25 */
	{ OFFSET(mmc0_dat3),        (M7 | PIN_INPUT) }, /* GPIO 2_26 */
	{ OFFSET(mmc0_dat2),        (M7 | PIN_INPUT) }, /* GPIO 2_27 */
	{ OFFSET(mmc0_dat1),        (M7 | PIN_INPUT) }, /* GPIO 2_28 */
	{ OFFSET(mmc0_dat0),        (M7 | PIN_INPUT) }, /* GPIO 2_29 */
	{ OFFSET(mmc0_clk),         (M7 | PIN_INPUT) }, /* GPIO 2_30 */
	{ OFFSET(mmc0_cmd),         (M7 | PIN_INPUT) }, /* GPIO 2_31 */
	{ OFFSET(mii1_col),         (M7 | PIN_INPUT) }, /* GPIO 3_0 */
	{ OFFSET(mii1_crs),         (M3 | PIN_INPUT_PULLUP) }, /* I2C1 SDA */
	{ OFFSET(mii1_rxerr),       (M3 | PIN_INPUT_PULLUP) }, /* I2C1 SCL */
	{ OFFSET(mii1_txen),        (M7 | PIN_INPUT) }, /* GPIO 3_3 */
	{ OFFSET(mii1_rxdv),        (M7 | PIN_INPUT) }, /* GPIO 3_4 */
	{ OFFSET(mii1_txd3),        (M7 | PIN_INPUT) }, /* GPIO 0_16 */
	{ OFFSET(mii1_txd2),        (M7 | PIN_INPUT) }, /* GPIO 0_17 */
	{ OFFSET(mii1_txd1),        (M7 | PIN_INPUT) }, /* GPIO 0_21 */
	{ OFFSET(mii1_txd0),        (M7 | PIN_INPUT) }, /* GPIO 0_28 */
	{ OFFSET(mii1_txclk),       (M7 | PIN_INPUT) }, /* GPIO 3_9 */
	{ OFFSET(mii1_rxclk),       (M7 | PIN_INPUT) }, /* GPIO 3_10 */
	{ OFFSET(mii1_rxd3),        (M7 | PIN_INPUT) }, /* GPIO 2_18 */
	{ OFFSET(mii1_rxd2),        (M7 | PIN_INPUT) }, /* GPIO 2_19 */
	{ OFFSET(mii1_rxd1),        (M7 | PIN_INPUT) }, /* GPIO 2_20 */
	{ OFFSET(mii1_rxd0),        (M7 | PIN_INPUT) }, /* GPIO 2_21 */
	{ OFFSET(rmii1_refclk),     (M7 | PIN_INPUT) }, /* GPIO 0_29 */
	{ OFFSET(mdio_data),        (M0 | PIN_INPUT_PULLUP) }, /* MDIO data */
	{ OFFSET(mdio_clk),         (M0 | PIN_OUTPUT_PULLUP) }, /* MDIO CLK */
	{ OFFSET(spi0_sclk),        (M0 | PIN_INPUT_PULLUP) }, /* SPI0 CLK */
	{ OFFSET(spi0_d0),          (M0 | PIN_INPUT_PULLUP) }, /* SPI0 D0 */
	{ OFFSET(spi0_d1),          (M0 | PIN_INPUT_PULLUP) }, /* SPI0 D1 */
	{ OFFSET(spi0_cs0),         (M0 | PIN_INPUT_PULLUP) }, /* SPI0 CS0 */
	{ OFFSET(spi0_cs1),         (M7 | PIN_INPUT) }, /* GPIO 0_6 */
	{ OFFSET(ecap0_in_pwm0_out),(M7 | PIN_INPUT) }, /* GPIO 0_7 */
	{ OFFSET(uart0_ctsn),       (M7 | PIN_INPUT) }, /* GPIO 1_8 */
	{ OFFSET(uart0_rtsn),       (M7 | PIN_INPUT) }, /* GPIO 1_9 */
	{ OFFSET(uart0_rxd),        (M0 | PIN_INPUT) }, /* UART0 RX */
	{ OFFSET(uart0_txd),        (M0 | PIN_OUTPUT) },/* UART0 TX */
	{ OFFSET(uart1_ctsn),       (M7 | PIN_INPUT) }, /* GPIO 0_12 */
	{ OFFSET(uart1_rtsn),       (M7 | PIN_INPUT) }, /* GPIO 0_13 */
	{ OFFSET(uart1_rxd),        (M7 | PIN_INPUT) }, /* GPIO 0_14 */
	{ OFFSET(uart1_txd),        (M7 | PIN_INPUT) }, /* GPIO 0_15 */
	{ OFFSET(i2c0_sda),         (M0 | PIN_INPUT_PULLUP) }, /* I2C0 SDA */
	{ OFFSET(i2c0_scl),         (M0 | PIN_INPUT_PULLUP) }, /* I2C0 SCL */
	{ OFFSET(mcasp0_aclkx),     (M7 | PIN_INPUT) }, /* GPIO 3_14 */
	{ OFFSET(mcasp0_fsx),       (M7 | PIN_INPUT) }, /* GPIO 3_15 */
	{ OFFSET(mcasp0_axr0),      (M7 | PIN_INPUT) }, /* GPIO 3_16 */
	{ OFFSET(mcasp0_ahclkr),    (M7 | PIN_INPUT) }, /* GPIO 3_17 LED1 */
	{ OFFSET(mcasp0_aclkr),     (M7 | PIN_INPUT) }, /* GPIO 3_18 */
	{ OFFSET(mcasp0_fsr),       (M7 | PIN_INPUT) }, /* GPIO 3_19 */
	{ OFFSET(mcasp0_axr1),      (M7 | PIN_INPUT) }, /* GPIO 3_20 */
	{ OFFSET(mcasp0_ahclkx),    (M7 | PIN_INPUT) }, /* GPIO 3_21 */
	{ OFFSET(xdma_event_intr0), (M7 | PIN_INPUT) }, /* GPIO 0_19 */
	{ OFFSET(xdma_event_intr1), (M7 | PIN_INPUT) }, /* GPIO 0_20 */
	{ OFFSET(emu0),             (M7 | PIN_INPUT) }, /* GPIO 3_7 */
	{ OFFSET(emu1),             (M7 | PIN_INPUT) }, /* GPIO 3_8 */
	{ OFFSET(usb0_drvvbus),     (M7 | PIN_INPUT) }, /* GPIO 0_18 */
	{ OFFSET(usb1_drvvbus),     (M7 | PIN_INPUT) }, /* GPIO 3_13 */
	{ -1 },
};

/* EB_TFT_Baseboard: only changes based on pia335x_pm */
static struct module_pin_mux pia335x_eb_tft[] = {
	{ OFFSET(gpmc_csn3),        (M7 | PIN_INPUT_PULLDOWN) }, /* LCD BACKLIGHT EN */
	{ OFFSET(gpmc_wen),         (M7 | PIN_INPUT_PULLDOWN) }, /* LCD ON/OFF */
	{ OFFSET(gpmc_ad8),         (M1 | PIN_OUTPUT) }, /* LCD D23 */
	{ OFFSET(gpmc_ad9),         (M1 | PIN_OUTPUT) }, /* LCD D22 */
	{ OFFSET(gpmc_ad10),        (M1 | PIN_OUTPUT) }, /* LCD D21 */
	{ OFFSET(gpmc_ad11),        (M1 | PIN_OUTPUT) }, /* LCD D20 */
	{ OFFSET(gpmc_ad12),        (M1 | PIN_OUTPUT) }, /* LCD D19 */
	{ OFFSET(gpmc_ad13),        (M1 | PIN_OUTPUT) }, /* LCD D18 */
	{ OFFSET(gpmc_ad14),        (M1 | PIN_OUTPUT) }, /* LCD D17 */
	{ OFFSET(gpmc_ad15),        (M1 | PIN_OUTPUT) }, /* LCD D16 */
	{ OFFSET(lcd_data0),        (M0 | PIN_OUTPUT) }, /* LCD D0 */
	{ OFFSET(lcd_data1),        (M0 | PIN_OUTPUT) }, /* LCD D1 */
	{ OFFSET(lcd_data2),        (M0 | PIN_OUTPUT) }, /* LCD D2 */
	{ OFFSET(lcd_data3),        (M0 | PIN_OUTPUT) }, /* LCD D3 */
	{ OFFSET(lcd_data4),        (M0 | PIN_OUTPUT) }, /* LCD D4 */
	{ OFFSET(lcd_data5),        (M0 | PIN_OUTPUT) }, /* LCD D5 */
	{ OFFSET(lcd_data6),        (M0 | PIN_OUTPUT) }, /* LCD D6 */
	{ OFFSET(lcd_data7),        (M0 | PIN_OUTPUT) }, /* LCD D7 */
	{ OFFSET(lcd_data8),        (M0 | PIN_OUTPUT) }, /* LCD D8 */
	{ OFFSET(lcd_data9),        (M0 | PIN_OUTPUT) }, /* LCD D9 */
	{ OFFSET(lcd_data10),       (M0 | PIN_OUTPUT) }, /* LCD D10 */
	{ OFFSET(lcd_data11),       (M0 | PIN_OUTPUT) }, /* LCD D11 */
	{ OFFSET(lcd_data12),       (M0 | PIN_OUTPUT) }, /* LCD D12 */
	{ OFFSET(lcd_data13),       (M0 | PIN_OUTPUT) }, /* LCD D13 */
	{ OFFSET(lcd_data14),       (M0 | PIN_OUTPUT) }, /* LCD D14 */
	{ OFFSET(lcd_data15),       (M0 | PIN_OUTPUT) }, /* LCD D15 */
	{ OFFSET(lcd_vsync),        (M0 | PIN_OUTPUT) }, /* LCD VSYNC */
	{ OFFSET(lcd_hsync),        (M0 | PIN_OUTPUT) }, /* LCD HSYNC */
	{ OFFSET(lcd_pclk),         (M0 | PIN_OUTPUT) }, /* LCD PCLK */
	{ OFFSET(lcd_ac_bias_en),   (M0 | PIN_OUTPUT) }, /* LCD DATA EN */
	{ OFFSET(mmc0_dat3),        (M0 | PIN_INPUT_PULLUP) }, /* MMC0 D3 */
	{ OFFSET(mmc0_dat2),        (M0 | PIN_INPUT_PULLUP) }, /* MMC0 D2 */
	{ OFFSET(mmc0_dat1),        (M0 | PIN_INPUT_PULLUP) }, /* MMC0 D1 */
	{ OFFSET(mmc0_dat0),        (M0 | PIN_INPUT_PULLUP) }, /* MMC0 D0 */
	{ OFFSET(mmc0_clk),         (M0 | PIN_INPUT_PULLUP) }, /* MMC0 CLK */
	{ OFFSET(mmc0_cmd),         (M0 | PIN_INPUT_PULLUP) }, /* MMC0 CMD */
	{ OFFSET(mii1_col),         (M7 | PIN_INPUT_PULLDOWN) }, /* RFID.POW_EN */
	{ OFFSET(mii1_rxdv),        (M7 | PIN_INPUT_PULLUP) }, /* RFID.IRQ */
	{ OFFSET(mii1_txd3),        (M1 | PIN_OUTPUT) }, /* CAN0 TX */
	{ OFFSET(mii1_txd2),        (M1 | PIN_INPUT) },  /* CAN0 RX */
	{ OFFSET(mii1_rxd3),        (M7 | PIN_INPUT_PULLUP) }, /* CAN0 TERM */
	{ OFFSET(rmii1_refclk),     (M2 | PIN_INPUT_PULLUP) }, /* SPI1 CS0 (RFID) */
	{ OFFSET(spi0_cs1),         (M2 | PIN_OUTPUT) }, /* Buzzer O */
	{ OFFSET(mcasp0_aclkx),     (M3 | PIN_INPUT_PULLUP) }, /* SPI1 CLK */
	{ OFFSET(mcasp0_ahclkr),    (M7 | PIN_INPUT_PULLUP) }, /* LED1 GPIO 3_17 */
	{ OFFSET(mcasp0_fsx),       (M3 | PIN_INPUT_PULLUP) }, /* SPI1 MOSI */
	{ OFFSET(mcasp0_axr0),      (M3 | PIN_INPUT_PULLUP) }, /* SPI1 MISO */
	{ OFFSET(xdma_event_intr0), (M6 | PIN_INPUT) }, /* JTAG.EMU2 */
	{ OFFSET(xdma_event_intr1), (M6 | PIN_INPUT) }, /* JTAG.EMU3 */
	{ OFFSET(emu0),             (M0 | PIN_INPUT) }, /* JTAG.EMU0 */
	{ OFFSET(emu1),             (M0 | PIN_INPUT) }, /* JTAG.EMU1 */
	{ OFFSET(usb0_drvvbus),     (M0 | PIN_OUTPUT) }, /* USB0 DRVVBUS */
	{ OFFSET(usb1_drvvbus),     (M0 | PIN_OUTPUT) }, /* USB1 DRVVBUS */
	{ -1 },
};

/* LOKISA EnergyManager */
static struct module_pin_mux lokisa_em[] = {
	{ OFFSET(gpmc_ad0),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D0 */
	{ OFFSET(gpmc_ad1),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D1 */
	{ OFFSET(gpmc_ad2),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D2*/
	{ OFFSET(gpmc_ad3),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D3 */
	{ OFFSET(gpmc_ad4),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D4 */
	{ OFFSET(gpmc_ad5),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D5 */
	{ OFFSET(gpmc_ad6),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D6 */
	{ OFFSET(gpmc_ad7),         (M1 | PIN_INPUT_PULLUP) }, /* MMC1 D7 */
	{ OFFSET(gpmc_ad8),         (M7 | PIN_INPUT) }, /* GPIO 0_22 */
	{ OFFSET(gpmc_ad9),         (M7 | PIN_INPUT) }, /* GPIO 0_23 */
	{ OFFSET(gpmc_ad10),        (M7 | PIN_INPUT) }, /* GPIO 0_26 */
	{ OFFSET(gpmc_ad11),        (M7 | PIN_INPUT) }, /* GPIO 0_27 */
	{ OFFSET(gpmc_ad12),        (M7 | PIN_INPUT) }, /* GPIO 1_12 */
	{ OFFSET(gpmc_ad13),        (M7 | PIN_INPUT) }, /* GPIO 1_13 */
	{ OFFSET(gpmc_ad14),        (M7 | PIN_INPUT) }, /* GPIO 1_14 */
	{ OFFSET(gpmc_ad15),        (M7 | PIN_INPUT) }, /* GPIO 1_15 */
	{ OFFSET(gpmc_a0),          (M1 | PIN_OUTPUT) }, /* MII2 TXEN */
	{ OFFSET(gpmc_a1),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXDV */
	{ OFFSET(gpmc_a2),          (M1 | PIN_OUTPUT) }, /* MII2 TXD3 */
	{ OFFSET(gpmc_a3),          (M1 | PIN_OUTPUT) }, /* MII2 TXD2 */
	{ OFFSET(gpmc_a4),          (M1 | PIN_OUTPUT) }, /* MII2 TXD1 */
	{ OFFSET(gpmc_a5),          (M1 | PIN_OUTPUT) }, /* MII2 TXD0 */
	{ OFFSET(gpmc_a6),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 TXCLK */
	{ OFFSET(gpmc_a7),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXCLK */
	{ OFFSET(gpmc_a8),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXD3 */
	{ OFFSET(gpmc_a9),          (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXD2 */
	{ OFFSET(gpmc_a10),         (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXD1 */
	{ OFFSET(gpmc_a11),         (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXD0 */
	{ OFFSET(gpmc_wait0),       (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 CRS */
	{ OFFSET(gpmc_wpn),         (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 RXER */
	{ OFFSET(gpmc_be1n),        (M1 | PIN_INPUT_PULLDOWN) }, /* MII2 COL */
	{ OFFSET(gpmc_csn0),        (M7 | PIN_INPUT) }, /* GPIO 1_29 LED1*/
	{ OFFSET(gpmc_csn1),        (M2 | PIN_INPUT_PULLUP) }, /* MMC1 CLK */
	{ OFFSET(gpmc_csn2),        (M2 | PIN_INPUT_PULLUP) }, /* MMC1 CMD */
	{ OFFSET(gpmc_csn3),        (M7 | PIN_INPUT) }, /* GPIO 2_0 */
	{ OFFSET(gpmc_clk),         (M7 | PIN_INPUT_PULLUP) }, /* optional Cap Touch Int */
	{ OFFSET(gpmc_advn_ale),    (M7 | PIN_INPUT) }, /* GPIO 2_2 */
	{ OFFSET(gpmc_oen_ren),     (M7 | PIN_INPUT) }, /* GPIO 2_3 */
	{ OFFSET(gpmc_wen),         (M7 | PIN_INPUT) }, /* GPIO 2_4 */
	{ OFFSET(gpmc_be0n_cle),    (M7 | PIN_INPUT) }, /* GPIO 2_5 */
	{ OFFSET(lcd_data0),        (M7 | PIN_INPUT) }, /* GPIO 2_6 */
	{ OFFSET(lcd_data1),        (M7 | PIN_INPUT) }, /* GPIO 2_7 */
	{ OFFSET(lcd_data2),        (M7 | PIN_INPUT) }, /* GPIO 2_8 */
	{ OFFSET(lcd_data3),        (M7 | PIN_INPUT) }, /* GPIO 2_9 */
	{ OFFSET(lcd_data4),        (M7 | PIN_INPUT) }, /* GPIO 2_10 */
	{ OFFSET(lcd_data5),        (M7 | PIN_INPUT) }, /* GPIO 2_11 */
	{ OFFSET(lcd_data6),        (M7 | PIN_INPUT) }, /* GPIO 2_12 */
	{ OFFSET(lcd_data7),        (M7 | PIN_INPUT) }, /* GPIO 2_13 */
	{ OFFSET(lcd_data8),        (M7 | PIN_INPUT) }, /* GPIO 2_14 */
	{ OFFSET(lcd_data9),        (M7 | PIN_INPUT) }, /* GPIO 2_15 */
	{ OFFSET(lcd_data10),       (M7 | PIN_INPUT) }, /* GPIO 2_16 */
	{ OFFSET(lcd_data11),       (M7 | PIN_INPUT) }, /* GPIO 2_17 */
	{ OFFSET(lcd_data12),       (M7 | PIN_INPUT) }, /* GPIO 0_8 */
	{ OFFSET(lcd_data13),       (M7 | PIN_INPUT) }, /* GPIO 0_9 */
	{ OFFSET(lcd_data14),       (M7 | PIN_INPUT) }, /* GPIO 0_10 */
	{ OFFSET(lcd_data15),       (M7 | PIN_INPUT) }, /* GPIO 0_11 */
	{ OFFSET(lcd_vsync),        (M7 | PIN_INPUT) }, /* GPIO 2_22 */
	{ OFFSET(lcd_hsync),        (M7 | PIN_INPUT) }, /* GPIO 2_24 */
	{ OFFSET(lcd_pclk),         (M7 | PIN_INPUT) }, /* GPIO 2_24 */
	{ OFFSET(lcd_ac_bias_en),   (M7 | PIN_INPUT) }, /* GPIO 2_25 */
	{ OFFSET(mmc0_dat3),        (M7 | PIN_INPUT) }, /* GPIO 2_26 */
	{ OFFSET(mmc0_dat2),        (M7 | PIN_INPUT) }, /* GPIO 2_27 */
	{ OFFSET(mmc0_dat1),        (M7 | PIN_INPUT) }, /* GPIO 2_28 */
	{ OFFSET(mmc0_dat0),        (M7 | PIN_INPUT) }, /* GPIO 2_29 */
	{ OFFSET(mmc0_clk),         (M7 | PIN_INPUT) }, /* GPIO 2_30 */
	{ OFFSET(mmc0_cmd),         (M7 | PIN_INPUT) }, /* GPIO 2_31 */
	{ OFFSET(mii1_col),         (M7 | PIN_INPUT) }, /* GPIO 3_0 */
	{ OFFSET(mii1_crs),         (M3 | PIN_INPUT_PULLUP) }, /* I2C1 SDA */
	{ OFFSET(mii1_rxerr),       (M3 | PIN_INPUT_PULLUP) }, /* I2C1 SCL */
	{ OFFSET(mii1_txen),        (M7 | PIN_INPUT) }, /* GPIO 3_3 */
	{ OFFSET(mii1_rxdv),        (M7 | PIN_INPUT) }, /* GPIO 3_4 */
	{ OFFSET(mii1_txd3),        (M7 | PIN_INPUT) }, /* GPIO 0_16 */
	{ OFFSET(mii1_txd2),        (M7 | PIN_INPUT) }, /* GPIO 0_17 */
	{ OFFSET(mii1_txd1),        (M7 | PIN_INPUT) }, /* GPIO 0_21 */
	{ OFFSET(mii1_txd0),        (M7 | PIN_INPUT) }, /* GPIO 0_28 */
	{ OFFSET(mii1_txclk),       (M7 | PIN_INPUT) }, /* GPIO 3_9 */
	{ OFFSET(mii1_rxclk),       (M7 | PIN_INPUT) }, /* GPIO 3_10 */
	{ OFFSET(mii1_rxd3),        (M7 | PIN_INPUT) }, /* GPIO 2_18 */
	{ OFFSET(mii1_rxd2),        (M7 | PIN_INPUT) }, /* GPIO 2_19 */
	{ OFFSET(mii1_rxd1),        (M7 | PIN_INPUT) }, /* GPIO 2_20 */
	{ OFFSET(mii1_rxd0),        (M7 | PIN_INPUT) }, /* GPIO 2_21 */
	{ OFFSET(rmii1_refclk),     (M7 | PIN_INPUT) }, /* GPIO 0_29 */
	{ OFFSET(mdio_data),        (M0 | PIN_INPUT_PULLUP) }, /* MDIO data */
	{ OFFSET(mdio_clk),         (M0 | PIN_OUTPUT_PULLUP) }, /* MDIO CLK */
	{ OFFSET(spi0_sclk),        (M0 | PIN_INPUT_PULLUP) }, /* SPI0 CLK */
	{ OFFSET(spi0_d0),          (M0 | PIN_INPUT_PULLUP) }, /* SPI0 D0 */
	{ OFFSET(spi0_d1),          (M0 | PIN_INPUT_PULLUP) }, /* SPI0 D1 */
	{ OFFSET(spi0_cs0),         (M0 | PIN_INPUT_PULLUP) }, /* SPI0 CS0 */
	{ OFFSET(spi0_cs1),         (M7 | PIN_INPUT) }, /* GPIO 0_6 */
	{ OFFSET(ecap0_in_pwm0_out),(M7 | PIN_INPUT) }, /* GPIO 0_7 */
	{ OFFSET(uart0_ctsn),       (M7 | PIN_INPUT) }, /* GPIO 1_8 */
	{ OFFSET(uart0_rtsn),       (M7 | PIN_INPUT) }, /* GPIO 1_9 */
	{ OFFSET(uart0_rxd),        (M0 | PIN_INPUT) }, /* UART0 RX */
	{ OFFSET(uart0_txd),        (M0 | PIN_OUTPUT) },/* UART0 TX */
	{ OFFSET(uart1_ctsn),       (M7 | PIN_INPUT) }, /* GPIO 0_12 */
	{ OFFSET(uart1_rtsn),       (M7 | PIN_INPUT) }, /* GPIO 0_13 */
	{ OFFSET(uart1_rxd),        (M7 | PIN_INPUT) }, /* GPIO 0_14 */
	{ OFFSET(uart1_txd),        (M7 | PIN_INPUT) }, /* GPIO 0_15 */
	{ OFFSET(i2c0_sda),         (M0 | PIN_INPUT_PULLUP) }, /* I2C0 SDA */
	{ OFFSET(i2c0_scl),         (M0 | PIN_INPUT_PULLUP) }, /* I2C0 SCL */
	{ OFFSET(mcasp0_aclkx),     (M7 | PIN_INPUT) }, /* GPIO 3_14 */
	{ OFFSET(mcasp0_fsx),       (M7 | PIN_INPUT) }, /* GPIO 3_15 */
	{ OFFSET(mcasp0_axr0),      (M7 | PIN_INPUT) }, /* GPIO 3_16 */
	{ OFFSET(mcasp0_ahclkr),    (M7 | PIN_INPUT) }, /* GPIO 3_17 LED1 */
	{ OFFSET(mcasp0_aclkr),     (M7 | PIN_INPUT) }, /* GPIO 3_18 */
	{ OFFSET(mcasp0_fsr),       (M7 | PIN_INPUT) }, /* GPIO 3_19 */
	{ OFFSET(mcasp0_axr1),      (M7 | PIN_INPUT) }, /* GPIO 3_20 */
	{ OFFSET(mcasp0_ahclkx),    (M7 | PIN_INPUT) }, /* GPIO 3_21 */
	{ OFFSET(xdma_event_intr0), (M7 | PIN_INPUT) }, /* GPIO 0_19 */
	{ OFFSET(xdma_event_intr1), (M7 | PIN_INPUT) }, /* GPIO 0_20 */
	{ OFFSET(emu0),             (M7 | PIN_INPUT) }, /* GPIO 3_7 */
	{ OFFSET(emu1),             (M7 | PIN_INPUT) }, /* GPIO 3_8 */
	{ OFFSET(usb0_drvvbus),     (M7 | PIN_INPUT) }, /* GPIO 0_18 */
	{ OFFSET(usb1_drvvbus),     (M7 | PIN_INPUT) }, /* GPIO 3_13 */
	{ -1 },
};

/* Module pin mux for LCDC */
static struct module_pin_mux lcdc_pin_mux[] = {
	{OFFSET(lcd_data0),		(M0 | PIN_OUTPUT)},	/* LCD.DATA0 */
	{OFFSET(lcd_data1),		(M0 | PIN_OUTPUT)}, /* LCD.DATA1 */
	{OFFSET(lcd_data2),		(M0 | PIN_OUTPUT)}, /* LCD.DATA2 */
	{OFFSET(lcd_data3),		(M0 | PIN_OUTPUT)}, /* LCD.DATA3 */
	{OFFSET(lcd_data4),		(M0 | PIN_OUTPUT)}, /* LCD.DATA4 */
	{OFFSET(lcd_data5),		(M0 | PIN_OUTPUT)}, /* LCD.DATA5 */
	{OFFSET(lcd_data6),		(M0 | PIN_OUTPUT)}, /* LCD.DATA6 */
	{OFFSET(lcd_data7),		(M0 | PIN_OUTPUT)}, /* LCD.DATA7 */
	{OFFSET(lcd_data8),		(M0 | PIN_OUTPUT)}, /* LCD.DATA8 */
	{OFFSET(lcd_data9),		(M0 | PIN_OUTPUT)}, /* LCD.DATA9 */
	{OFFSET(lcd_data10),	(M0 | PIN_OUTPUT)}, /* LCD.DATA10 */
	{OFFSET(lcd_data11),	(M0 | PIN_OUTPUT)}, /* LCD.DATA11 */
	{OFFSET(lcd_data12),	(M0 | PIN_OUTPUT)}, /* LCD.DATA12 */
	{OFFSET(lcd_data13),	(M0 | PIN_OUTPUT)}, /* LCD.DATA13 */
	{OFFSET(lcd_data14),	(M0 | PIN_OUTPUT)}, /* LCD.DATA14 */
	{OFFSET(lcd_data15),	(M0 | PIN_OUTPUT)}, /* LCD.DATA15 */
	{OFFSET(gpmc_ad8),		(M1 | PIN_OUTPUT)}, /* LCD.DATA23 */
	{OFFSET(gpmc_ad9),		(M1 | PIN_OUTPUT)}, /* LCD.DATA22 */
	{OFFSET(gpmc_ad10),		(M1 | PIN_OUTPUT)}, /* LCD.DATA21 */
	{OFFSET(gpmc_ad11),		(M1 | PIN_OUTPUT)}, /* LCD.DATA20 */
	{OFFSET(gpmc_ad12),		(M1 | PIN_OUTPUT)}, /* LCD.DATA19 */
	{OFFSET(gpmc_ad13),		(M1 | PIN_OUTPUT)}, /* LCD.DATA18 */
	{OFFSET(gpmc_ad14),		(M1 | PIN_OUTPUT)}, /* LCD.DATA17 */
	{OFFSET(gpmc_ad15),		(M1 | PIN_OUTPUT)}, /* LCD.DATA16 */
	{OFFSET(lcd_vsync),		(M0 | PIN_OUTPUT)}, /* LCD.VSYNC */
	{OFFSET(lcd_hsync),		(M0 | PIN_OUTPUT)}, /* LCD.HSYNC */
	{OFFSET(lcd_pclk),		(M0 | PIN_OUTPUT)}, /* LCD.PCLK */
	{OFFSET(lcd_ac_bias_en),(M0 | PIN_OUTPUT)}, /* LCD.AC_BIAS_EN */
	{OFFSET(gpmc_be1n), 	(M7 | PIN_OUTPUT)},	/* LCD.DISEN */
	{OFFSET(mcasp0_ahclkr), (M7 | PIN_OUTPUT)},	/* LCD.BACKLIGHTEN */
	{-1},
};


static struct module_pin_mux uart0_pin_mux[] = {
	{OFFSET(uart0_rxd), (M0 | PIN_INPUT_PULLUP)},  /* UART0_RXD */
	{OFFSET(uart0_txd), (M0 | PIN_OUTPUT_PULLUP)},  /* UART0_TXD */
	{-1},
};

/* MII pinmux for board PIA-AM335x-MMI */
static struct module_pin_mux mmi_mii1_pin_mux[] = {
	{OFFSET(mii1_rxerr),  M0 | PIN_INPUT_PULLDOWN},/* MII1_RXERR */
	{OFFSET(mii1_txen),  M0 | PIN_OUTPUT},         /* MII1_TXEN */
	{OFFSET(mii1_rxdv),  M0 | PIN_INPUT_PULLDOWN}, /* MII1_RXDV */
	{OFFSET(mii1_txd3),  M0 | PIN_OUTPUT},         /* MII1_TXD3 */
	{OFFSET(mii1_txd2),  M0 | PIN_OUTPUT},         /* MII1_TXD2 */
	{OFFSET(mii1_txd1),  M0 | PIN_OUTPUT},         /* MII1_TXD1 */
	{OFFSET(mii1_txd0),  M0 | PIN_OUTPUT},         /* MII1_TXD0 */
	{OFFSET(mii1_txclk),  M0 | PIN_INPUT_PULLDOWN}, /* MII1_TXCLK */
	{OFFSET(mii1_rxclk),  M0 | PIN_INPUT_PULLDOWN}, /* MII1_RXCLK */
	{OFFSET(mii1_rxd3),  M0 | PIN_INPUT_PULLDOWN},  /* MII1_RXD3 */
	{OFFSET(mii1_rxd2),  M0 | PIN_INPUT_PULLDOWN},  /* MII1_RXD2 */
	{OFFSET(mii1_rxd1),  M0 | PIN_INPUT_PULLDOWN},  /* MII1_RXD1 */
	{OFFSET(mii1_rxd0),  M0 | PIN_INPUT_PULLDOWN},  /* MII1_RXD0 */
	{OFFSET(mdio_data),M0 | PIN_INPUT_PULLUP},   /* MDIO_DATA */
	{OFFSET(mdio_clk), M0 | PIN_OUTPUT_PULLUP},  /* MDIO_CLK */
	{OFFSET(mii1_col),  M0 | PIN_INPUT_PULLDOWN},  /* MII1_COL */
	{OFFSET(mii1_crs),  M0 | PIN_INPUT_PULLDOWN},  /* MII1_CRS DV */
	{-1},
};

static struct module_pin_mux mii2_pin_mux[] = {
	/*{OFFSET(gpmc_wpn), M1 | PIN_INPUT_PULLDOWN},  RXER not used! is GPMC.WPn */
	{OFFSET(gpmc_a0),  M1 | PIN_OUTPUT},         /* RGMII2_TCTL / TXEN */
	{OFFSET(gpmc_a1),  M1 | PIN_INPUT_PULLDOWN}, /* RGMII2_RCTL / RXDV */
	{OFFSET(gpmc_a2),  M1 | PIN_OUTPUT},         /* RGMII2_TXD3 */
	{OFFSET(gpmc_a3),  M1 | PIN_OUTPUT},         /* RGMII2_TXD2 */
	{OFFSET(gpmc_a4),  M1 | PIN_OUTPUT},         /* RGMII2_TXD1 */
	{OFFSET(gpmc_a5),  M1 | PIN_OUTPUT},         /* RGMII2_TXD0 */
	{OFFSET(gpmc_a6),  M1 | PIN_INPUT_PULLDOWN}, /* RGMII2_TCLK */
	{OFFSET(gpmc_a7),  M1 | PIN_INPUT_PULLDOWN}, /* RGMII2_RCLK */
	{OFFSET(gpmc_a8),  M1 | PIN_INPUT_PULLDOWN}, /* RGMII2_RXD3 */
	{OFFSET(gpmc_a9),  M1 | PIN_INPUT_PULLDOWN}, /* RGMII2_RXD2 */
	{OFFSET(gpmc_a10), M1 | PIN_INPUT_PULLDOWN}, /* RGMII2_RXD1 */
	{OFFSET(gpmc_a11), M1 | PIN_INPUT_PULLDOWN}, /* RGMII2_RXD0 */
	{OFFSET(mdio_data),M0 | PIN_INPUT_PULLUP},   /* MDIO_DATA */
	{OFFSET(mdio_clk), M0 | PIN_OUTPUT_PULLUP},  /* MDIO_CLK */
	{-1},
};

#ifdef CONFIG_MMC
#ifndef PIA_ON_BONE
/* configuration for KM_E2 */
static struct module_pin_mux mmc0_pin_mux[] = {
	{OFFSET(mmc0_dat3), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT3 */
	{OFFSET(mmc0_dat2), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT2 */
	{OFFSET(mmc0_dat1), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT1 */
	{OFFSET(mmc0_dat0), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT0 */
	{OFFSET(mmc0_clk),  M0 | PIN_INPUT_PULLUP},	/* MMC0_CLK */
	{OFFSET(mmc0_cmd),  M0 | PIN_INPUT_PULLUP},	/* MMC0_CMD */
	{-1},
};
#else
/* configuration for Beaglebone */
static struct module_pin_mux mmc0_pin_mux[] = {
	{OFFSET(mmc0_dat3), (M0 | IEN | P_UP)},	/* MMC0_DAT3 */
	{OFFSET(mmc0_dat2), (M0 | IEN | P_UP)},	/* MMC0_DAT2 */
	{OFFSET(mmc0_dat1), (M0 | IEN | P_UP)},	/* MMC0_DAT1 */
	{OFFSET(mmc0_dat0), (M0 | IEN | P_UP)},	/* MMC0_DAT0 */
	{OFFSET(mmc0_clk), (M0 | IEN | P_UP)},	/* MMC0_CLK */
	{OFFSET(mmc0_cmd), (M0 | IEN | P_UP)},	/* MMC0_CMD */
	{OFFSET(spi0_cs1), (M5 | IEN | P_UP)},	/* MMC0_CD */
	{-1},
};
#endif /* PIA_ON_BONE */

static struct module_pin_mux e2_mmc0_pin_mux[] = {
	{OFFSET(mmc0_dat3), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT3 */
	{OFFSET(mmc0_dat2), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT2 */
	{OFFSET(mmc0_dat1), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT1 */
	{OFFSET(mmc0_dat0), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT0 */
	{OFFSET(mmc0_clk),  M0 | PIN_INPUT_PULLUP},	/* MMC0_CLK */
	{OFFSET(mmc0_cmd),  M0 | PIN_INPUT_PULLUP},	/* MMC0_CMD */
	// dedicated CD pin was not used in Rev 0.01 and is not present in newer
	// hw revisions
	//{OFFSET(mii1_txd2), M7| PIN_INPUT_PULLUP},	/* MMC0_CD */
	{-1},
};

static struct module_pin_mux mmi_mmc0_pin_mux[] = {
	{OFFSET(mmc0_dat3), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT3 */
	{OFFSET(mmc0_dat2), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT2 */
	{OFFSET(mmc0_dat1), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT1 */
	{OFFSET(mmc0_dat0), M0 | PIN_INPUT_PULLUP},	/* MMC0_DAT0 */
	{OFFSET(mmc0_clk),  M0 | PIN_INPUT_PULLUP},	/* MMC0_CLK */
	{OFFSET(mmc0_cmd),  M0 | PIN_INPUT_PULLUP},	/* MMC0_CMD */
	//{OFFSET(spi0_d0), M7 | PIN_INPUT_PULLUP},	/* MMC0_CD */	//not used on MMI board
	{-1},
};
#endif

/* I2C0 - piA-AM335x-KM-E2 and piA-AM335x-KM-MMI */
static struct module_pin_mux i2c0_pin_mux[] = {
	{OFFSET(i2c0_sda), (M0 | IEN |
			P_UP | P_EN | SLEWCTRL)}, /* I2C_DATA */
	{OFFSET(i2c0_scl), (M0 | IEN |
			P_UP | P_EN | SLEWCTRL)}, /* I2C_SCLK */
	{-1},
};

/* I2C1 - piA-AM335x-KM-E2 */
static struct module_pin_mux i2c1_pin_mux[] = {
	{OFFSET(uart0_ctsn), M3 | PIN_INPUT_PULLUP | SLEWCTRL},	/* I2C_DATA */
	{OFFSET(uart0_rtsn), M3 | PIN_INPUT_PULLUP | SLEWCTRL},	/* I2C_SCLK */
	{-1},
};

/* I2C1 - piA-AM335x-KM-MMI */
static struct module_pin_mux mmi_i2c1_pin_mux[] = {
	{OFFSET(uart1_rxd), M3 | PIN_INPUT_PULLUP | SLEWCTRL},	/* I2C_DATA */
	{OFFSET(uart1_txd), M3 | PIN_INPUT_PULLUP | SLEWCTRL},	/* I2C_SCLK */
	{-1},
};

/* I2C1 - piA-AM335x-PM with baseboard */
static struct module_pin_mux pm_i2c1_pin_mux[] = {
	{OFFSET(mii1_crs), M3 | PIN_INPUT_PULLUP | SLEWCTRL},	/* I2C_DATA */
	{OFFSET(mii1_rxerr), M3 | PIN_INPUT_PULLUP | SLEWCTRL},	/* I2C_SCLK */
	{-1},
};

/* Supervisor - piA-AM335x-KM-E2 */
static struct module_pin_mux e2_supervisor_pin_mux[] = {
	{OFFSET(lcd_data3), (M7 | PIN_INPUT_PULLDOWN)}, /* FlipFlop Clock 2_09 */
	{OFFSET(gpmc_ad14), (M7 | PIN_INPUT_PULLDOWN)}, /* WD_RESET 1_14 */
	{OFFSET(mii1_col),  (M7 | PIN_INPUT_PULLDOWN)}, /* PB_RESET 3_00 */
	/* SET0 = HIGH, SET1 = HIGH, SET2 = LOW default off for Watchdog */
	{OFFSET(lcd_vsync), (M7 | PIN_INPUT_PULLUP)},   /* WD_SET1  2_22 */
	{OFFSET(lcd_hsync), (M7 | PIN_INPUT_PULLDOWN)},   /* WD_SET2  2_23 */
	{OFFSET(lcd_ac_bias_en), (M7 | PIN_INPUT_PULLUP)},   /* 24V_Fail  2_25 */
	{-1},
};
static struct module_pin_mux e2_r2_supervisor_pin_mux[] = {
	{OFFSET(mii1_rxclk), (M7 | PIN_INPUT_PULLUP)}, /* FlipFlop Clock 3_10 */
	{OFFSET(gpmc_ad14), (M7 | PIN_INPUT_PULLDOWN)}, /* WD_RESET 1_14 */
	{OFFSET(mii1_col),  (M7 | PIN_INPUT_PULLDOWN)}, /* PB_RESET 3_00 */
	/* SET0 = HIGH, SET1 = HIGH, SET2 = LOW default off for Watchdog */
	{OFFSET(lcd_vsync), (M7 | PIN_INPUT_PULLUP)},   /* WD_SET1  2_22 */
	{OFFSET(lcd_hsync), (M7 | PIN_INPUT_PULLDOWN)},   /* WD_SET2  2_23 */
	{OFFSET(lcd_ac_bias_en), (M7 | PIN_INPUT_PULLUP)},   /* 24V_Fail  2_25 */
	{-1},
};
/* additional PoE pins - piA-AM335x-KM-E2 Rev 0.3 */
static struct module_pin_mux e2_r3_poe_pin_mux[] = {
	{OFFSET(mii1_txclk),  (M7 | PIN_INPUT_PULLDOWN)},   /* PoE_PS_Shutdown 3_09 */
	{OFFSET(mcasp0_axr1), (M7 | PIN_INPUT_PULLDOWN)}, /* PSE_Shutdown 3_20 */
	{-1},
};

/* Supervisor - piA-AM335x-KM-MMI */
static struct module_pin_mux mmi_supervisor_pin_mux[] = {
	/* SET0 = HIGH, SET1 = HIGH, SET2 = LOW default off for Watchdog */
	{OFFSET(gpmc_ad0), (M7 | PIN_OUTPUT)},   		/* WDI   	1_0 */
	{OFFSET(gpmc_ad1), (M7 | PIN_INPUT_PULLUP)},   	/* WD_SET1  1_1 */
	{OFFSET(gpmc_ad2), (M7 | PIN_INPUT_PULLDOWN)},	/* WD_SET2	1_2 */
	{OFFSET(mcasp0_axr1), (M7 | PIN_INPUT_PULLUP)},	/* 3.3V_Fail 3_20 */
	/* Event_INTR0 as GPIO? */
	{OFFSET(xdma_event_intr0), (M7 | PIN_INPUT_PULLUP)},	/* XDMA_EVENT_INTR0 0_19 */
	{OFFSET(xdma_event_intr1), (M3 | PIN_OUTPUT)},			/* XDMA_EVENT_INTR1 CLKOUT2 */
	{-1},
};

/* PMIC - piA-AM335x-KM-MMI */
static struct module_pin_mux mmi_pmic_pin_mux[] = {
	{OFFSET(gpmc_clk), (M7 | PIN_OUTPUT)},   				/* PMIC INT1 2_1 */
	{OFFSET(mcasp0_axr0), (M7 | PIN_INPUT_PULLUP)},			/* PMIC SLEEP 3_16 */
	{-1},
};

/* Acceleration - piA-AM335x-KM-MMI */
static struct module_pin_mux mmi_accl_pin_mux[] = {
	{OFFSET(ecap0_in_pwm0_out), (M7 | PIN_INPUT_PULLUP)},	/* ACC_INT2  0_7 */
	{OFFSET(mcasp0_fsr), (M7 | PIN_INPUT_PULLUP)},			/* ACC_INT1  3_19 */
	{OFFSET(gpmc_wait0), (M7 | PIN_OUTPUT)},				/* LED1 	0_30 */
	{OFFSET(gpmc_wpn), (M7 | PIN_OUTPUT)},					/* LED2 	0_31*/
	{-1},
};

/* Audio - piA-AM335x-KM-MMI */
static struct module_pin_mux mmi_audio_pin_mux[] = {
	// temp. GPIOs for testing purpose only
	{OFFSET(mcasp0_aclkx), (M7 | PIN_INPUT_PULLUP)},   		/* Audio.BCLK 3_14 */
	{OFFSET(mcasp0_fsx), (M7 | PIN_INPUT_PULLUP)},			/* Audio.FSX  3_15 */
	{OFFSET(mcasp0_aclkr), (M7 | PIN_INPUT_PULLUP)},			/* Audio.DIN  3_18 */
	{OFFSET(mcasp0_ahclkx), (M7 | PIN_INPUT_PULLUP)},	/* Audio.DOUT 3_32 */
#if 0
	{OFFSET(mcasp0_aclkx), (M0 | PIN_OUTPUT)},   		/* Audio.BCLK 3_14 */
	{OFFSET(mcasp0_fsx), (M0 | PIN_OUTPUT)},			/* Audio.FSX  3_15 */
	{OFFSET(mcasp0_aclkr), (M7 | PIN_OUTPUT)},			/* Audio.DIN  3_18 */
	{OFFSET(mcasp0_ahclkx), (M7 | PIN_INPUT_PULLUP)},	/* Audio.DOUT 3_32 */
#endif
	{-1},
};


/*
 * Configure the pin mux for the module
 */
static void configure_module_pin_mux(struct module_pin_mux *mod_pin_mux)
{
	int i;

	if (!mod_pin_mux)
		return;

	for (i = 0; mod_pin_mux[i].reg_offset != -1; i++)
		MUX_CFG(mod_pin_mux[i].val, mod_pin_mux[i].reg_offset);
}

void enable_uart0_pin_mux(void)
{
	configure_module_pin_mux(uart0_pin_mux);
}

#ifdef CONFIG_MMC
void enable_mmc0_pin_mux(struct am335x_baseboard_id *header)
{
	debug(">>pia:enable_mmc0_pin_mux()\n");
	if (board_is_e2(header)) {
		configure_module_pin_mux(mmc0_pin_mux);
	} else if (board_is_mmi(header)) {
		configure_module_pin_mux(mmi_mmc0_pin_mux);
	}
}
#endif

void enable_i2c0_pin_mux(void)
{
	debug(">>pia:enable_i2c0_pin_mux()\n");
	configure_module_pin_mux(i2c0_pin_mux);
}

void enable_i2c1_pin_mux(void)
{
	debug(">>pia:enable_i2c1_pin_mux()\n");
#if defined CONFIG_PIA_FIRSTSTART && !defined(CONFIG_PIA_EBTFT)
	/* for first start config, we need the second i2c bus in MLO */
#if defined(CONFIG_PIA_E2)
	configure_module_pin_mux(i2c1_pin_mux);
#elif defined(CONFIG_PIA_MMI)
	configure_module_pin_mux(mmi_i2c1_pin_mux);
#endif
#else
	/* special case for PM without EEPROM */
	configure_module_pin_mux(pm_i2c1_pin_mux);
#endif
}

static void init_pia_e2_gpios(struct am335x_baseboard_id *header)
{
	unsigned int gpio_ffclock = 0;

	debug(">>pia:init_pia_e2_gpios()\n");
#if defined(CONFIG_MMC) && defined(CONFIG_E2_MMC_CD_GPIO)
	gpio_request(CONFIG_E2_MMC_CD_GPIO, "mmc0_cd");
	gpio_direction_input(CONFIG_E2_MMC_CD_GPIO);
	debug("MMC CD: %d\n", gpio_get_value(CONFIG_E2_MMC_CD_GPIO));
#endif

	/* Supervisor */
	gpio_request(CONFIG_E2_PB_RESET_GPIO, "pb_reset");
	gpio_direction_input(CONFIG_E2_PB_RESET_GPIO);
	gpio_request(CONFIG_E2_WD_RESET_GPIO, "wd_reset");
	gpio_direction_input(CONFIG_E2_WD_RESET_GPIO);
	/* reset clock for supervisor flip flops */
	if (strncmp(header->version, "0.01", 4) == 0) {
		gpio_ffclock = (2 * 32) + 9;
	} else {
		gpio_ffclock = (3 * 32) + 10;
	}
	gpio_request(gpio_ffclock, "ff_clock");
	gpio_direction_output(gpio_ffclock, 1);

	/* Watchdog config, both high = WD disabled */
	gpio_request(CONFIG_E2_WD_SET1_GPIO, "wd_set1");
	gpio_direction_output(CONFIG_E2_WD_SET1_GPIO, 1);
	gpio_request(CONFIG_E2_WD_SET2_GPIO, "wd_set2");
	gpio_direction_output(CONFIG_E2_WD_SET2_GPIO, 0);
	gpio_request(CONFIG_E2_24V_FAIL_GPIO, "fail_24v");
	gpio_direction_input(CONFIG_E2_24V_FAIL_GPIO);

	/* PoE - disable DCDC and outputs */
	if (header && 0 == strncmp(header->version, "0.03", 4)) {
		gpio_request(CONFIG_E2_POE_POE_PS_SD_GPIO, "poe_ps_shutdown");
		// shutdown
		gpio_direction_output(CONFIG_E2_POE_POE_PS_SD_GPIO, 0);
		gpio_request(CONFIG_E2_POE_PSE_SD_GPIO, "pse_shutdown");
		gpio_direction_output(CONFIG_E2_POE_PSE_SD_GPIO, 0);
	}
}

static void init_pia_mmi_gpios(void)
{
	debug(">>pia:init_pia_mmi_gpios()\n");
#if 0
	gpio_request(CONFIG_MMI_MMC_CD_GPIO, "mmc0_cd");
	gpio_direction_input(CONFIG_MMI_MMC_CD_GPIO);
	debug("MMC CD: %d\n", gpio_get_value(CONFIG_MMI_MMC_CD_GPIO));	//MMC_CD is unused
endif
#endif

	/* Watchdog config, SET0=SET1=high and SET2=low == WD disabled */
	gpio_request(CONFIG_MMI_WD_SET1_GPIO, "wd_set1");
	gpio_direction_output(CONFIG_MMI_WD_SET1_GPIO, 1);
	gpio_request(CONFIG_MMI_WD_SET2_GPIO, "wd_set2");
	gpio_direction_output(CONFIG_MMI_WD_SET2_GPIO, 0);

	gpio_request(CONFIG_MMI_LED1_GPIO, "LED1");
	gpio_direction_output(CONFIG_MMI_LED1_GPIO, 0);
	gpio_request(CONFIG_MMI_LED2_GPIO, "LED2");
	gpio_direction_output(CONFIG_MMI_LED2_GPIO, 0);

	gpio_request(CONFIG_MMI_ACC_INT1_GPIO, "ACC_INT1");
	gpio_direction_input(CONFIG_MMI_ACC_INT1_GPIO);
	gpio_request(CONFIG_MMI_ACC_INT2_GPIO, "ACC_INT2");
	gpio_direction_input(CONFIG_MMI_ACC_INT2_GPIO);

	gpio_request(CONFIG_MMI_XDMA_EVENT_INTR0_GPIO, "XDMA_EVENT_INTR0");
	gpio_direction_input(CONFIG_MMI_XDMA_EVENT_INTR0_GPIO);
}

static void init_pia_ebtft_gpios(void)
{
	gpio_request(69, "emmc_reset");
	gpio_direction_output(69, 1);
}

void enable_board_pin_mux(struct am335x_baseboard_id *header)
{
	// TODO use eeprom header spec
	debug(">>pia:enable_board_pin_mux() for board %.8s\n",header->name);
	if (board_is_e2(header)) {
		configure_module_pin_mux(i2c1_pin_mux);
		configure_module_pin_mux(mii2_pin_mux);
		configure_module_pin_mux(e2_mmc0_pin_mux);
		if (0 == strncmp(header->version, "0.01", 4))
			configure_module_pin_mux(e2_supervisor_pin_mux);
		else
			configure_module_pin_mux(e2_r2_supervisor_pin_mux);
		if (0 == strncmp(header->version, "0.03", 4))
			configure_module_pin_mux(e2_r3_poe_pin_mux);
		init_pia_e2_gpios(header);
	} else if (board_is_mmi(header)) {
		configure_module_pin_mux(mmi_i2c1_pin_mux);
		configure_module_pin_mux(mmi_mii1_pin_mux);
		configure_module_pin_mux(mmi_mmc0_pin_mux);
		configure_module_pin_mux(mmi_supervisor_pin_mux);
		configure_module_pin_mux(mmi_pmic_pin_mux);
		configure_module_pin_mux(mmi_accl_pin_mux);
		configure_module_pin_mux(mmi_audio_pin_mux);
		configure_module_pin_mux(lcdc_pin_mux);
		init_pia_mmi_gpios();
	} else if (board_is_ebtft(header)) {
		configure_module_pin_mux(pia335x_pm);
		configure_module_pin_mux(pia335x_eb_tft);
		init_pia_ebtft_gpios();
	}

	/* There is no hook for additional GPIO initialization */
	//init_pia_gpios();
}
