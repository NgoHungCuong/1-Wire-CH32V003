#ifndef NHC_WCH_SDI_H_
#define NHC_WCH_SDI_H_

#include "stm32f10x.h"

/*
	1-Wire WCH (CH32V003) debug protocol
	Ngo Hung Cuong
	http://vidieukhien.org
	ngohungcuong@gmail.com
	
	Based on CH32F103C8T6
*/

#define WCH_SDI_DATA0 0x04
#define WCH_SDI_DATA1 0x05
#define WCH_SDI_DMCONTROL 0x10
#define WCH_SDI_DMSTATUS 0x11
#define WCH_SDI_HARTINFO 0x12
#define WCH_SDI_ABSTRACTCS 0x16
#define WCH_SDI_COMMAND 0x17
#define WCH_SDI_ABSTRACTAUTO 0x18
#define WCH_SDI_PROGBUFF0 0x20
#define WCH_SDI_PROGBUFF1 0x21
#define WCH_SDI_PROGBUFF2 0x22
#define WCH_SDI_PROGBUFF3 0x23
#define WCH_SDI_PROGBUFF4 0x24
#define WCH_SDI_PROGBUFF5 0x25
#define WCH_SDI_PROGBUFF6 0x26
#define WCH_SDI_PROGBUFF7 0x27
#define WCH_SDI_HALTSUM0 0x40

#define WCH_SDI_CPBR 0x7C
#define WCH_SDI_CFGR 0x7D
#define WCH_SDI_SHDWCFGR 0x7E

void NHC_WchSdi_Init(void);
void NHC_WchSdi_Exit(void);
void NHC_WchSdi_Write(uint8_t u8Address, uint32_t u32Data);
uint32_t NHC_WchSdi_Read(uint8_t u8Address, uint32_t *pu32Data);

#endif
