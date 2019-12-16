/* 
	Editor: https://www.visualmicro.com/
			This file is for intellisense purpose only.
			Visual micro (and the arduino ide) ignore this code during compilation. This code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			The contents of the _vm sub folder can be deleted prior to publishing a project
			All non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			Note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: LinkIt 7697, Platform=linkit_rtos, Package=LinkIt
*/

#if defined(_VMICRO_INTELLISENSE)

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define PRODUCT_VERSION 7697
#define MTK_BSPEXT_ENABLE
#define USE_HAL_DRIVER
#define MTK_NVDM_ENABLE
#define MTK_DEBUG_LEVEL_INFO
#define MTK_DEBUG_LEVEL_WARNING
#define MTK_DEBUG_LEVEL_ERROR
#define MTK_LWIP_ENABLE
#define MTK_MINISUPP_ENABLE
#define MTK_WIFI_API_TEST_CLI_ENABLE
#define MTK_WIFI_REPEATER_ENABLE
#define MTK_WIFI_WPS_ENABLE
#define SUPPORT_MBEDTLS
#define MTK_WIFI_TGN_VERIFY_ENABLE
#define PCFG_OS 2
#define _REENT_SMALL
#define F_CPU 192000000L
#define ARDUINO 108010
#define ARDUINO_linkit_7697
#define ARDUINO_ARCH_LINKIT_RTOS
#define __cplusplus 201103L
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __volatile__


#define __ICCARM__
#define __ASM
#define __INLINE
#define __builtin_va_list void
//#define _GNU_SOURCE 
//#define __GNUC__ 0
//#undef  __ICCARM__
//#define __GNU__

typedef long Pio;
typedef long Efc;
typedef long Adc;
typedef long Pwm;
typedef long Rtc;
typedef long Rtt;
typedef long pRtc;
typedef long Spi;
typedef long spi;
typedef long Ssc;
//typedef long p_scc;
typedef long Tc;
//typedef long pTc;
typedef long Twi;
typedef long Wdt;
//typedef long pTwi;
typedef long Usart;
typedef long Pdc;
typedef long Rstc;

extern const int ADC_MR_TRGEN_DIS = 0;
extern const int ADC_MR_TRGSEL_ADC_TRIG0 = 0;
extern const int ADC_MR_TRGSEL_Pos = 0;

extern const int ADC_MR_TRGSEL_Msk = 0;
extern const int ADC_MR_TRGEN = 0;
extern const int ADC_TRIG_TIO_CH_0 = 0;
extern const int ADC_MR_TRGSEL_ADC_TRIG1 = 0;
extern const int ADC_TRIG_TIO_CH_1 = 0;
extern const int ADC_MR_TRGSEL_ADC_TRIG2 = 0;
extern const int ADC_MR_TRGSEL_ADC_TRIG3 = 0;

#define __ARMCC_VERSION 400678
#define __attribute__(noinline)

#define prog_void
#define PGM_VOID_P int


            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}



#include "arduino.h"
#include <adapter_layer.h> 
#include <cmsis.h> 
#include <flash_map.h> 
#include <FreeRTOSConfig.h> 
#include <hal_feature_config.h> 
#include <log_dump.h> 
#include <lwipopts.h> 
#include <pins_arduino.h> 
#include <task_def.h> 
#include <variant.h> 
#include <variant_delay.h> 
#include <variant.cpp> 
#undef cli
#define cli()
#include "AccessPoint.ino"
#include "test.ino"
#endif
#endif
