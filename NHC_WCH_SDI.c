#include "NHC_WCH_SDI.h"

/*
	1-Wire WCH (CH32V003) debug protocol
	Ngo Hung Cuong
	http://vidieukhien.org
	ngohungcuong@gmail.com
	
	Based on CH32F103C8T6
*/

static uint16_t u16Capture[100];
static uint8_t u8Data[100];

#define SDI_1 GPIOB->BSRR = 0x00008000
#define SDI_0 GPIOB->BRR = 0x00008000
#define SDI_IN (GPIOB->IDR & 0x00008000)

void NHC_Delay_Us(uint16_t u16Delay);
void NHC_Delay_Ms(uint32_t u32Delay);

void NHC_Delay_Us(uint16_t u16Delay)
{
	uint16_t u16Tmp;
	
	u16Tmp = u16Delay * 72;
	TIM4->CNT = 0;
	while (TIM4->CNT < u16Tmp)
	{
	}
}

void NHC_Delay_Ms(uint32_t u32Delay)
{
	
	while (u32Delay)
	{
		NHC_Delay_Us(500);
		NHC_Delay_Us(500);
		--u32Delay;
	}
}

void NHC_WchSdi_Init(void)
{
	GPIO_InitTypeDef gpioInit;
	TIM_TimeBaseInitTypeDef timInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	timInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timInit.TIM_CounterMode = TIM_CounterMode_Up;
	timInit.TIM_Period = 0xffff;
	timInit.TIM_Prescaler = 1 - 1;
	timInit.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM4, &timInit);
	TIM_Cmd(TIM4, ENABLE);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_15;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInit);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_OD;
	gpioInit.GPIO_Pin = GPIO_Pin_15;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInit);
	
	SDI_1;
	NHC_Delay_Ms(1);
	SDI_0;
	NHC_Delay_Us(36);
	SDI_1;
}

void NHC_WchSdi_Exit(void)
{
	GPIO_InitTypeDef gpioInit;
	
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInit.GPIO_Pin = GPIO_Pin_15;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInit);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInit.GPIO_Pin = GPIO_Pin_15;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInit);
	
	TIM_DeInit(TIM4);
	TIM_Cmd(TIM4, DISABLE);
}

void NHC_WchSdi_Write(uint8_t u8Address, uint32_t u32Data)
{
	uint32_t i;
	uint8_t *p;
	
	u8Data[0] = 1;
	
	for (i = 0; i < 7; ++i)
	{
		if (u8Address & 0x40)
		{
			u8Data[i + 1] = 1;
		}
		else
		{
			u8Data[i + 1] = 0;
		}
		u8Address <<= 1;
	}
	
	u8Data[8] = 1;
	
	for (i = 0; i < 32; ++i)
	{
		if (u32Data & 0x80000000)
		{
			u8Data[i + 9] = 1;
		}
		else
		{
			u8Data[i + 9] = 0;
		}
		u32Data <<= 1;
	}
	
	p = u8Data;
	
	for (i = 0; i < 41; ++i)
	{
		if (*p++)
		{
			SDI_0;
			TIM4->CNT = 0;
			while (TIM4->CNT < 9)
			{
			}
			SDI_1;
			TIM4->CNT = 0;
			while (TIM4->CNT < 9)
			{
			}
		}
		else
		{
			SDI_0;
			TIM4->CNT = 0;
			while (TIM4->CNT < 48)
			{
			}
			SDI_1;
			TIM4->CNT = 0;
			while (TIM4->CNT < 9)
			{
			}
		}
	}
}

uint32_t NHC_WchSdi_Read(uint8_t u8Address, uint32_t *pu32Data)
{
	uint32_t i;
	uint32_t u32Tmp;
	uint8_t *p;
	uint16_t *pu16;
	
	u8Data[0] = 1;
	for (i = 0; i < 7; ++i)
	{
		if (u8Address & 0x40)
		{
			u8Data[i + 1] = 1;
		}
		else
		{
			u8Data[i + 1] = 0;
		}
		u8Address <<= 1;
	}
	u8Data[8] = 0;
	
	p = u8Data;
	
	for (i = 0; i < 9; ++i)
	{
		if (*p++)
		{
			SDI_0;
			TIM4->CNT = 0;
			while (TIM4->CNT < 9)
			{
			}
			SDI_1;
			TIM4->CNT = 0;
			while (TIM4->CNT < 9)
			{
			}
		}
		else
		{
			SDI_0;
			TIM4->CNT = 0;
			while (TIM4->CNT < 48)
			{
			}
			SDI_1;
			TIM4->CNT = 0;
			while (TIM4->CNT < 9)
			{
			}
		}
	}
	
	pu16 = u16Capture;
	
	for (i = 0; i < 32; ++i)
	{
		SDI_0;
		TIM4->CNT = 0;
		while (TIM4->CNT < 9)
		{
		}
		SDI_1;
		TIM4->CNT = 0;
		while (1)
		{
			if (SDI_IN)
			{
				*pu16++ = TIM4->CNT;
				break;
			}
			if (TIM4->CNT > 144)
			{
				return 0;
			}
		}
	}
	
	u32Tmp = 0;
	
	for (i = 0; i < 32; ++i)
	{
		u32Tmp <<= 1;
		if (u16Capture[i] < 8)
		{
			u32Tmp |= 0x01;
		}
	}
	
	*pu32Data = u32Tmp;
	
	return 1;
}
