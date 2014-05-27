/*
 * (C) Copyright 2001, 2002, 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 * Keith Outwater, keith_outwater@mvis.com`
 * Steven Scholz, steven.scholz@imc-berlin.de
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Date & Time support (no alarms) for Dallas Semiconductor (now Maxim)
 * DS1374 Real Time Clock (RTC).
 *
 * based on ds1337.c
 */

#include <common.h>
#include <command.h>
#include <rtc.h>
#include <i2c.h>

#if defined(CONFIG_CMD_DATE)

/*---------------------------------------------------------------------*/
#undef DEBUG_RTC
#define DEBUG_RTC

#ifdef DEBUG_RTC
#define DEBUGR(fmt,args...) printf(fmt ,##args)
#else
#define DEBUGR(fmt,args...)
#endif
/*---------------------------------------------------------------------*/

#ifndef CONFIG_SYS_I2C_RTC_ADDR
# define CONFIG_SYS_I2C_RTC_ADDR	0x68
#endif

#if defined(CONFIG_RTC_DS1374) && (CONFIG_SYS_I2C_SPEED > 400000)
# error The DS1374 is specified up to 400kHz in fast mode!
#endif

/*
 * RTC register addresses
 */
#define RTC_TOD_CNT_BYTE0_ADDR		0x00 /* TimeOfDay */
#define RTC_TOD_CNT_BYTE1_ADDR		0x01
#define RTC_TOD_CNT_BYTE2_ADDR		0x02
#define RTC_TOD_CNT_BYTE3_ADDR		0x03

#define RTC_WD_ALM_CNT_BYTE0_ADDR	0x04
#define RTC_WD_ALM_CNT_BYTE1_ADDR	0x05
#define RTC_WD_ALM_CNT_BYTE2_ADDR	0x06

#define RTC_CTL_ADDR			0x07 /* RTC-CoNTrol-register */
#define RTC_SR_ADDR			0x08 /* RTC-StatusRegister */
#define RTC_TCS_DS_ADDR			0x09 /* RTC-TrickleChargeSelect DiodeSelect-register */

#define RTC_CTL_BIT_AIE			(1<<0) /* Bit 0 - Alarm Interrupt enable */
#define RTC_CTL_BIT_RS1			(1<<1) /* Bit 1/2 - Rate Select square wave output */
#define RTC_CTL_BIT_RS2			(1<<2) /* Bit 2/2 - Rate Select square wave output */
#define RTC_CTL_BIT_WDSTR		(1<<3) /* Bit 3 - Watchdog Reset Steering */
#define RTC_CTL_BIT_BBSQW		(1<<4) /* Bit 4 - Battery-Backed Square-Wave */
#define RTC_CTL_BIT_WD_ALM		(1<<5) /* Bit 5 - Watchdoc/Alarm Counter Select */
#define RTC_CTL_BIT_WACE		(1<<6) /* Bit 6 - Watchdog/Alarm Counter Enable WACE*/
#define RTC_CTL_BIT_EN_OSC		(1<<7) /* Bit 7 - Enable Oscilator */

#define RTC_TCS_BIT_ROUT0		(1<<0) /* Bit 0 - Resistor select */
#define RTC_TCS_BIT_ROUT1		(1<<1) /* Bit 1 - Resistor select */
#define RTC_TCS_BIT_DS0			(1<<2) /* Bit 2 - Diode Select  */
#define RTC_TCS_BIT_DS1			(1<<3) /* Bit 3 - Diode Select  */
#define RTC_TCS_BIT_TCS0		(1<<4) /* Bit 4 - Trickle-charge select bit 4 */
#define RTC_TCS_BIT_TCS1		(1<<5) /* Bit 5 - Trickle-charge select bit 5 */
#define RTC_TCS_BIT_TCS2		(1<<6) /* Bit 6 - Trickle-charge select bit 6 */
#define RTC_TCS_BIT_TCS3		(1<<7) /* Bit 7 - Trickle-charge select bit 7 */

#define RTC_SR_BIT_AF			0x01 /* Bit 0 = Alarm Flag */
#define RTC_SR_BIT_OSF			0x80 /* Bit 7 - Osc Stop Flag */

#define TC_ON			// Trickle-Charge: comment out when the battery should not be charged.
//#define WATCHDOG
//#define DEBUG_ON

const char RtcTodAddr[] = {
	RTC_TOD_CNT_BYTE0_ADDR,
	RTC_TOD_CNT_BYTE1_ADDR,
	RTC_TOD_CNT_BYTE2_ADDR,
	RTC_TOD_CNT_BYTE3_ADDR
};

static uchar rtc_read (uchar reg);
static void rtc_write(uchar reg, uchar val, bool set);
static void rtc_write_raw (uchar reg, uchar val);

int ds1374_wd_enable = 0;
void rtc_init(void)
{
	printf("Starting RTC/Watchdog (DS1374) Initialization...\n");

	uchar res;

	// Read Trickle Charger config register
	res = rtc_read(RTC_TCS_DS_ADDR);

	if (ds1374_wd_enable) {
		// Read Watchdog/Alarm Counter
		int cnt = 0x0B4000; // 180 s * 4096
		uchar wd_alm_byte0, wd_alm_byte1, wd_alm_byte2;
		wd_alm_byte0 = rtc_read(RTC_WD_ALM_CNT_BYTE0_ADDR);
		wd_alm_byte1 = rtc_read(RTC_WD_ALM_CNT_BYTE1_ADDR);
		wd_alm_byte2 = rtc_read(RTC_WD_ALM_CNT_BYTE2_ADDR);
		printf("WD/ALARM Counter (Power-up): %x %x %x\n",wd_alm_byte2, wd_alm_byte1, wd_alm_byte0);

		// Periodic Alarm
		//rtc_write(RTC_CTL_ADDR, (RTC_CTL_BIT_WACE | RTC_CTL_BIT_AIE),TRUE);

		// Write to Watchdog/Alarm Counter Register
		rtc_write_raw(RTC_WD_ALM_CNT_BYTE0_ADDR, (cnt & 0xff));
		rtc_write_raw(RTC_WD_ALM_CNT_BYTE1_ADDR, ((cnt >> 8) & 0xff));
		rtc_write_raw(RTC_WD_ALM_CNT_BYTE2_ADDR, ((cnt >> 16) & 0xff));

		wd_alm_byte0 = rtc_read(RTC_WD_ALM_CNT_BYTE0_ADDR);
		wd_alm_byte1 = rtc_read(RTC_WD_ALM_CNT_BYTE1_ADDR);
		wd_alm_byte2 = rtc_read(RTC_WD_ALM_CNT_BYTE2_ADDR);
		printf("Set WD/ALARM Counter to %02x %02x %02x / 4096 s\n",
				wd_alm_byte2, wd_alm_byte1, wd_alm_byte0);

		//Enable Watchdog Interrupt
		rtc_write(RTC_CTL_ADDR, (RTC_CTL_BIT_WACE | RTC_CTL_BIT_WD_ALM),TRUE);
		res = rtc_read(RTC_CTL_ADDR);
		if((res & 0x60) == 0x60)
			printf("Watchdog enabled!\n");
	} else {
		//Disable Watchdog Interrupt
		rtc_write(RTC_CTL_ADDR, (RTC_CTL_BIT_WACE | RTC_CTL_BIT_WD_ALM),FALSE);
		res = rtc_read(RTC_CTL_ADDR);
		if(!(res & 0x60))
			printf("Watchdog disabled!\n");
	}

#ifdef TC_ON
	//Enable trickle charger (Imax = 0,65mA, bei U=3,3V)
	rtc_write_raw(RTC_TCS_DS_ADDR,
			(RTC_TCS_BIT_TCS3 | RTC_TCS_BIT_TCS1 | /* Enable Trickle-Charge */
			 RTC_TCS_BIT_DS0  |                    /* No diode */
			 RTC_TCS_BIT_ROUT1 ));                 /* 2 kOhm */

	//check if Triple Charge is enabled
	res = rtc_read(RTC_TCS_DS_ADDR);
	//printf("res: %2X\n",res);
	if((res & 0xF0) == 0xA0){
#ifdef DEBUG_ON
		printf("Trickle Charger enabled...\n NOTE: System power supply must be at 3.3V! (If not, the battery charging can damage the entire system!\n");
		switch(res){
			case 0xa5:	printf(" (Imax = 13,2mA @ U=3,3V)) \n");
						break;
			case 0xa9:	printf(" (Imax = 10,4mA @ U=3,3V)) \n"); 
						break;
			case 0xa6:	printf(" (Imax = 1,65mA @ U=3,3V)) \n");
						break;
			case 0xaa:	printf(" (Imax = 1,3mA @ U=3,3V)) \n");
						break;
			case 0xab:	printf(" (Imax = 0,65mA @ U=3,3V) \n");
						break;
			case 0xa7:	printf(" (Imax = 0,825mA @ U=3,3V)) \n"); 		 		
						break;
			default: 	printf("Warning: Maximum charging current undefined!\n");
						break;
		}
#else
		printf("Battery charging enabled.\n");
#endif	//DEBUG_ON
	}

#else	//Triple Charge not defined
	printf("Trickle Charge disabled\n");
#endif	//TC_ON
}

/*
 * Get the current time from the RTC
 */
int rtc_get (struct rtc_time *tm){
	int rel = 0;
	unsigned long time1, time2;
	unsigned int limit;
	unsigned char tmp;
	unsigned int i;

	/*
	 * Since the reads are being performed one byte at a time,
	 * there is a chance that a carry will occur during the read.
	 * To detect this, 2 reads are performed and compared.
	 */
	limit = 10;
	do {
		i = 4;
		time1 = 0;
		while (i--) {
			tmp = rtc_read(RtcTodAddr[i]);
			time1 = (time1 << 8) | (tmp & 0xff);
		}

		i = 4;
		time2 = 0;
		while (i--) {
			tmp = rtc_read(RtcTodAddr[i]);
			time2 = (time2 << 8) | (tmp & 0xff);
		}
	} while ((time1 != time2) && limit--);

	if (time1 != time2) {
		printf("can't get consistent time from rtc chip\n");
		rel = -1;
	}

	DEBUGR ("Get RTC s since 1.1.1970: %ld\n", time1);

	to_tm(time1, tm); /* To Gregorian Date */

	if (rtc_read(RTC_SR_ADDR) & RTC_SR_BIT_OSF) {
		printf ("### Warning: RTC oscillator has stopped\n");
		rel = -1;
	}

	DEBUGR ("Get DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_wday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);

#ifdef DEBUG_ON
	/* DEBUGGING */
	uchar wd_alm_byte0, wd_alm_byte1, wd_alm_byte2;
	wd_alm_byte0 = rtc_read(RTC_WD_ALM_CNT_BYTE0_ADDR);
	wd_alm_byte1 = rtc_read(RTC_WD_ALM_CNT_BYTE1_ADDR);
	wd_alm_byte2 = rtc_read(RTC_WD_ALM_CNT_BYTE2_ADDR);
	printf("WD/ALARM Counter (Get date): %x %x %x\n",wd_alm_byte2, wd_alm_byte1, wd_alm_byte0);
	/* DEBUGGING end */
#endif

	return rel;
}

/*
 * Set the RTC
 */
int rtc_set (struct rtc_time *tmp){

	unsigned long time;
	unsigned i;

	DEBUGR ("Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	if (tmp->tm_year < 1970 || tmp->tm_year > 2069)
		printf("WARNING: year should be between 1970 and 2069!\n");

	time = mktime(tmp->tm_year, tmp->tm_mon,
			tmp->tm_mday, tmp->tm_hour,
			tmp->tm_min, tmp->tm_sec);

	DEBUGR ("Set RTC s since 1.1.1970: %ld (0x%02lx)\n", time, time);

	/* write to RTC_TOD_CNT_BYTEn_ADDR */
	for (i = 0; i <= 3; i++) {
		rtc_write_raw(RtcTodAddr[i], (unsigned char)(time & 0xff));
		time = time >> 8;
	}

	/* Start clock */
	rtc_write(RTC_CTL_ADDR, RTC_CTL_BIT_EN_OSC, false);

	return 0;
}

/*
 * Reset the RTC. We setting the date back to 1970-01-01.
 * We also enable the oscillator output on the SQW/OUT pin and program
 * it for 32,768 Hz output. Note that according to the datasheet, turning
 * on the square wave output increases the current drain on the backup
 * battery to something between 480nA and 800nA.
 */
void rtc_reset (void){

	struct rtc_time tmp;

	/* clear status flags */
	rtc_write(RTC_SR_ADDR, (RTC_SR_BIT_AF|RTC_SR_BIT_OSF), false); /* clearing OSF and AF */

	/* Initialise DS1374 oriented to MPC8349E-ADS */
	rtc_write (RTC_CTL_ADDR, (RTC_CTL_BIT_EN_OSC
				 |RTC_CTL_BIT_WACE
				 |RTC_CTL_BIT_AIE), false);/* start osc, disable WACE, clear AIE
							      - set to 0 */
	rtc_write (RTC_CTL_ADDR, (RTC_CTL_BIT_WD_ALM
				|RTC_CTL_BIT_WDSTR
				|RTC_CTL_BIT_RS1
				|RTC_CTL_BIT_RS2
				|RTC_CTL_BIT_BBSQW), true);/* disable WD/ALM, WDSTR set to INT-pin,
							      set BBSQW and SQW to 32k
							      - set to 1 */
	tmp.tm_year = 1970;
	tmp.tm_mon = 1;
	tmp.tm_mday= 1;
	tmp.tm_hour = 0;
	tmp.tm_min = 0;
	tmp.tm_sec = 0;

	rtc_set(&tmp);

	printf("RTC:   %4d-%02d-%02d %2d:%02d:%02d UTC\n",
		tmp.tm_year, tmp.tm_mon, tmp.tm_mday,
		tmp.tm_hour, tmp.tm_min, tmp.tm_sec);

	rtc_write(RTC_WD_ALM_CNT_BYTE0_ADDR,0xAC, true);
	rtc_write(RTC_WD_ALM_CNT_BYTE1_ADDR, 0xDE, true);
	rtc_write(RTC_WD_ALM_CNT_BYTE2_ADDR, 0xAD, true);
}

/*
 * Helper functions
 */
static uchar rtc_read (uchar reg)
{
	return (i2c_reg_read (CONFIG_SYS_I2C_RTC_ADDR, reg));
}

static void rtc_write(uchar reg, uchar val, bool set)
{
	if (set == true) {
		val |= i2c_reg_read (CONFIG_SYS_I2C_RTC_ADDR, reg);
		i2c_reg_write (CONFIG_SYS_I2C_RTC_ADDR, reg, val);
	} else {
		val = i2c_reg_read (CONFIG_SYS_I2C_RTC_ADDR, reg) & ~val;
		i2c_reg_write (CONFIG_SYS_I2C_RTC_ADDR, reg, val);
	}
}

static void rtc_write_raw (uchar reg, uchar val)
{
		i2c_reg_write (CONFIG_SYS_I2C_RTC_ADDR, reg, val);
}
#endif
