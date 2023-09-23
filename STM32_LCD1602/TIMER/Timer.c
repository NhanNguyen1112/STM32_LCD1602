
#include "Timer.h"

/*======================================================================================================*/
/*                              TIMER BASE                                                           */
/*======================================================================================================*/
static unsigned int Count=0;

void Function_Test(void)
{
  TogglePin(PORTC,PIN13);
}

void Main_TIM2_BASE(void)
{
  InitClockHSE();

	Enable_Disable_Clock_PortC(Enable);
	SetPinOutput(PORTC,PIN13,PushPull);
	WritePin(PORTC,PIN13,LOW);

	Init_TIM2_CH1_Delay();

	while (1)
	{
    Function_Test();
    Delay_TIM2_CH1_us(5);
    //Delay_TIM2_ms(5000);
	}
}

void Init_TIM2_CH1_Delay(void)
{
  unsigned int CR1=0;

  TIM2_EnableClock(Enable); /* Enable clock TIM2 */

  TIM2->PSC = PSC_TIM2_BASE; /* Prescaler = (0-1)+1 = 0 -> 72mhz/0 = 72mhz */

  /* SYSCLK=72Mhz -> ARR=1us -> 72.000.000*0,000001= 72 tick */
  TIM2->ARR = ARR_TIM2_BASE;

  TIM2->CNT = 0; /* Reset counter */

  TIM2->DIER |= (1<<0);  /* UIE: Update interrupt enable */
  TIM2->SR &= ~(1u<<0);  /* UIF: Clear UIF update interrupt flag */

  NVIC_ClearPendingFlag(TIM2_IRQn); /* Clear Pending */
  NVIC_SetPriority(TIM2_IRQn,1);
  NVIC_EnableInterrupt(TIM2_IRQn); 	/* Enable interupt */

  CR1 &= ~(1u<<4); 	/* DIR set Up counter */
  CR1 |= (1u<<0); 	/* enable counter */
  TIM2->CR1 = CR1;  

  TIM2->EGR = 0x01u; /* UG Re-initialize the counter and generates an update of the registers */
}

void Delay_TIM2_CH1_ms(const unsigned int MiliSeconds)
{
  unsigned int MicroSeconds = MiliSeconds*1000;
  while(Count<=MicroSeconds);
  Count=0;
}

void Delay_TIM2_CH1_us(const unsigned int MicroSeconds)
{
  while(Count<=(MicroSeconds-1));
  Count=0;
}

void TIM2_IRQHandler(void)
{
  TIM2->SR &= ~(1u<<0);  /* UIF: Clear UIF update interrupt flag */
	Count+=1;
}
/*======================================================================================================*/



/*======================================================================================================*/
/*                              TIMER PWM OUTPUT                                                           */
/*======================================================================================================*/

/* 
  F_timer qua Prescaler = F_timer / Prescaler = 84000000/840 = 100000Hz.

  Set Auto reload = 100 (Bằng độ phân giải PWM)
  -> F_output = F_timer qua Prescaler / 100 = 1000Hz = 1Khz

=> F_output = (F_timer / Prescaler) / Auto reload

*/

void Main_TIM2_PWM(void)
{
  InitClockHSE();

  Enable_Disable_Clock_PortA(Enable);
  SetPinOutput(PORTA,PIN0,Alternate_PushPull);
	
  Init_TIM2_PWM_CH1_PA0();

  while(1)
  {
    for( unsigned int i=0; i<=99; i++)
    {
      TIM2_PWM_OUTPUT(i);
      for( unsigned int d=0; d<=100000; d++);
    }
		
    for( unsigned int n=99; n>0; n--)
    {
      TIM2_PWM_OUTPUT(n);
      for( unsigned int j=0; j<=100000; j++);
    }
    
  }
}

void Init_TIM2_PWM_CH1_PA0(void)
{
  unsigned int CR1=0;

  TIM2_EnableClock(Enable); /* Enable clock TIM2 */

  TIM2->PSC = PSC_TIM2_PWM - 1u;  					/* Prescaler = (72.000.000hz/1000hz)/100 = 720hz*/
  TIM2->ARR = (unsigned short)(ARR_TIM2_PWM - 1u); 	/* 100 */

  TIM2->CNT = 0;

  TIM2->CCMR1 |= (6u<<4); /* OC1M Mode PWM 1 */
  TIM2->CCMR1 |= (1u<<3); /* OC1PE Output preload Disable */

  TIM2->CCR1 = 0;

  TIM2->CCER &= ~(1u<<1); /* CC1P:0 -> OC1 active high */
  TIM2->CCER |= (1u<<0);  /* On - OC1 signal is output on the corresponding output pin */

  CR1 &= ~(1u<<4); 	/* DIR set Up counter */
  CR1 |= (1u<<7);   /* Auto reload enable */
  CR1 |= (1u<<0); 	/* enable counter */
  TIM2->CR1 = CR1;  

  TIM2->EGR = 0x01u; /* UG Re-initialize the counter and generates an update of the registers */
}

void TIM2_PWM_OUTPUT(const unsigned int Value)
{
  TIM2->CCR1 = (unsigned int)((Value*ARR_TIM2_PWM)/100u);
}

void Init_TIM3_PWM_CH3_PB0(void)
{
  unsigned int CR1=0;

  TIM3_EnableClock(Enable); /* Enable clock TIM2 */

  TIM3->PSC = 720u - 1u;  					/* Prescaler = (72.000.000hz/1000hz)/100 = 720hz*/
  TIM3->ARR = (unsigned short)(100u - 1u); 	/* 100 */

  TIM3->CNT = 0;

  TIM3->CCMR2 |= (6u<<4); /* OC3M Mode PWM 1 */
  TIM3->CCMR2 |= (1u<<3); /* OC3PE Output preload Disable */

  TIM3->CCR3 = 0;

  TIM3->CCER &= ~(1u<<9); /* CC3P:0 -> OC3 active high */
  TIM3->CCER |= (1u<<8);  /* On - OC3 signal is output on the corresponding output pin */

  CR1 &= ~(1u<<4); 	/* DIR set Up counter */
  CR1 |= (1u<<7);   /* Auto reload enable */
  CR1 |= (1u<<0); 	/* enable counter */
  TIM3->CR1 = CR1;  

  TIM3->EGR = 0x01u; /* UG Re-initialize the counter and generates an update of the registers */
}

void TIM3_CH3_PWM_OUTPUT(const unsigned int Value)
{
  TIM3->CCR3 = (unsigned int)(Value);
}


/*======================================================================================================*/

