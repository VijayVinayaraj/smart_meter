#ifndef __M90E26_H__
#define __M90E20_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

#define	VSPI_CS1				GPIO_NUM_5			// D8
#define	VSPI_SCLK				GPIO_NUM_18
#define	VSPI_MISO				GPIO_NUM_19
#define	VSPI_MOSI				GPIO_NUM_23

#define SOFTRESET			0x00	// Software Reset
#define SYS_STATUS			0x01	// System Status
#define FUNC_ENAB			0x02	// Function Enable
#define V_SAG_THR			0x03 	// Voltage Sag Threshold
#define POWER_MODE			0x04 	// Small-Power Mode
#define LASTDATA			0x06	// Last Read/Write SPI/UART Value
#define LSB					0x08 	// RMS/Power 16-bit LSB

#define CALSTART			0x20 	// Calibration Start Command
#define PLconstH			0x21 	// High Word of PL_Constant
#define PLconstL			0x22 	// Low Word of PL_Constant
#define L_GAIN				0x23 	// L Line Calibration Gain
#define L_PHI				0x24 	// L Line Calibration Angle
#define N_GAIN				0x25 	// N Line Calibration Gain
#define N_PHI				0x26 	// N Line Calibration Angle
#define P_SUP_TH			0x27 	// Active Startup Power Threshold
#define P_NOL_TH			0x28 	// Active No-Load Power Threshold
#define Q_SUP_TH			0x29 	// Reactive Startup Power Threshold
#define Q_NOL_TH			0x2A 	// Reactive No-Load Power Threshold
#define MET_MODE			0x2B 	// Metering Mode Configuration
#define CRC_1				0x2C 	// Checksum 1

#define ADJSTART			0x30 	// Measurement Calibration Start Command
#define U_GAIN				0x31 	// Voltage rms Gain
#define I_GAIN_L			0x32 	// L Line Current rms Gain
#define I_GAIN_N			0x33 	// N Line Current rms Gain
#define V_OFFSET			0x34 	// Voltage Offset
#define I_OFST_L			0x35 	// L Line Current Offset
#define I_OFST_N			0x36 	// N Line Current Offset
#define P_OFST_L			0x37 	// L Line Active Power Offset
#define Q_OFST_L 			0x38 	// L Line Reactive Power Offset
#define P_OFST_N 			0x39 	// N Line Active Power Offset
#define Q_OFST_N 			0x3A 	// N Line Reactive Power Offset
#define CRC_2 				0x3B 	// Checksum 2

// Energy registers
#define E_ACT_FWD 			0x40 	// Forward Active Energy
#define E_ACT_REV 			0x41 	// Reverse Active Energy
#define E_ACT_ABS 			0x42 	// Absolute Active Energy
#define E_REACT_FWD			0x43 	// Forward (Inductive) Reactive Energy
#define E_REACT_REV 		0x44 	// Reverse (Capacitive) Reactive Energy
#define E_REACT_ABS 		0x45 	// Absolute Reactive Energy

#define MET_STATUS 			0x46 	// Metering Status

#define I_RMS_L				0x48 	// L Line Current rms
#define V_RMS				0x49 	// Voltage rms
#define P_ACT_L				0x4A 	// L Line Mean Active Power
#define P_REACT_L			0x4B 	// L Line Mean Reactive Power
#define FREQ				0x4C 	// Voltage Frequency
#define P_FACTOR_L			0x4D 	// L Line Power Factor
#define P_ANGLE_L			0x4E 	// L Line Phase Angle between Voltage and Current
#define P_APP_L 			0x4F 	// L Line Mean Apparent Power

#define I_RMS_N 			0x68 	// N Line Current rms
#define P_ACT_N 			0x6A 	// N Line Mean Active Power
#define P_REACT_N 			0x6B 	// N Line Mean Reactive Power
#define P_FACTOR_N 			0x6D 	// N Line Power Factor
#define P_ANGLE_N 			0x6E 	// N Line Phase Angle between Voltage and Current
#define P_APP_N 			0x6F 	// N Line Mean Apparent Power

#define	CODE_DFALT			0x6886	// indicates default Power On status, not measuring
#define	CODE_START			0x5678	// Reset to defaults, start metering, allow calibration/adjustment
#define	CODE_CHECK			0x8765	// check calibration/adjustment, continue measurement if all OK
#define	CODE_RESET			0x789A	// trigger software reset
#define	CODE_POWER			0xA987	// Set small power mode


#define Ugain_Default 0x6720
#define IgainL_Default 0x7A13
#define IgainN_Default 0x7530
#define PLconstL_Default 0xD174
#define PLconstH_Default 0x0015
#define PStartTh_Default 0x08BD
#define QStartTh_Default 0x0AEC
#define MMode_Default 0x9422

 //uint16_t _crc2;



void spi_init();
uint16_t m90e26ReadU16(uint8_t address);
void m90e26WriteU16( uint8_t address, uint16_t val);
uint16_t checksumCalc(uint8_t id);
void calibrateIC();

uint16_t getSystemStatus();
uint16_t getMeterStatus();
double getLineVoltage();
double getLineCurrent();
double getActivePower();
double getPowerFactor ();
double getEnergy();
double getEnergyRev();

#endif