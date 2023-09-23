
#include "main.h"

int main(void)
{
  InitClockHSE();
  Init_TIM2_CH1_Delay();

  Enable_Disable_Clock_PortA(Enable);
  SetPinOutput(PORTA,PIN0,PushPull);
  SetPinOutput(PORTA,PIN1,PushPull);
  SetPinOutput(PORTA,PIN2,PushPull);
  SetPinOutput(PORTA,PIN3,PushPull);
  SetPinOutput(PORTA,PIN4,PushPull);
  SetPinOutput(PORTA,PIN5,PushPull);

  LCD_Init(PORTA,PIN0,PIN1,PIN2,PIN3,PIN4,PIN5);

  while (1)
  {
    LCD_Clear();
    LCD_SetCursor(0,1);
    LCD_PrintString("ABC12345",8);
    Delay_TIM2_CH1_ms(3000);

    LCD_Clear();
    LCD_SetCursor(1,5);
    LCD_PrintString("987654321",9);
    Delay_TIM2_CH1_ms(3000);

    LCD_Clear();
    LCD_SetCursor(1,0);
    LCD_PrintString("TEST LED-> MENU",15);
    Delay_TIM2_CH1_ms(3000);
  }
  
}


