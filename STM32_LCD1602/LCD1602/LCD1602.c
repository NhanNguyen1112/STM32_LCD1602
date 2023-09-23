/*====================================================================
*                          INCLUDE FILES
======================================================================*/
#include "LCD1602.h"
/*====================================================================*/

/*====================================================================
*                          LOCAL MACROS
======================================================================*/
#define CMD_CLEAR     (0x01UL)
#define CMD_INIT_8BIT (0x38UL)
#define CMD_INIT_4BIT (0x28UL)
#define CMD_INPUT_SET (0x06UL)
#define CMD_SHIFT     (0x0CUL)
/*====================================================================*/

/*====================================================================
*                          LOCAL TYPEDEFS
======================================================================*/
typedef enum 
{
  COMMAND,
  DATA
}DataCommand_enum;
/*====================================================================*/

/*====================================================================
*                          LOCAL VARIABLES
======================================================================*/
static char LCD_PORT;
static char LCD_RS;
static char LCD_EN;
static char LCD_D4;
static char LCD_D5;
static char LCD_D6;
static char LCD_D7;
/*====================================================================*/

/*====================================================================
*                          LOCAL FUNCTIONS 0.00066040 0.63117040
======================================================================*/

static void SendToLCD(char *Data, DataCommand_enum RS)
{
  char Value=0;

  WritePin(LCD_PORT, LCD_RS, RS);

  Value = ((*Data>>3)&1UL);
  WritePin(LCD_PORT, LCD_D7, Value );
  Value = ((*Data>>2)&1UL);
  WritePin(LCD_PORT, LCD_D6, Value );
  Value = ((*Data>>1)&1UL);
  WritePin(LCD_PORT, LCD_D5, Value );
  Value = ((*Data>>0)&1UL);
  WritePin(LCD_PORT, LCD_D4, Value );
  Delay_TIM2_CH1_us(50);

  WritePin(LCD_PORT, LCD_EN, 1);
  Delay_TIM2_CH1_us(50);
  WritePin(LCD_PORT, LCD_EN, 0);
  Delay_TIM2_CH1_us(50);
}

static void SendCMD(char CMD)
{
  char DataSend;

  DataSend = ( (CMD>>4)&0x0F ); /* 4bit high */
  SendToLCD(&DataSend, COMMAND);

  DataSend = ( CMD&0x0F ); /* 4bit LOW */
  SendToLCD(&DataSend, COMMAND);  
}

static void SendDATA(char *Data)
{
  char DataSend;

  DataSend = ( (*Data>>4)&0x0F ); /* 4bit high */
  SendToLCD(&DataSend, DATA);

  DataSend = ( ((*Data)&0x0F) ); /* 4bit LOW */
  SendToLCD(&DataSend, DATA); 
}

/*====================================================================*/

/*====================================================================
*                          GLOBAL FUNCTIONS
======================================================================*/

void LCD_Init(char PORT, char RS, char EN, 
              char D4, char D5, char D6, char D7)
{
  LCD_PORT = PORT;
  LCD_RS = RS;
  LCD_EN = EN;
  LCD_D4 = D4;
  LCD_D5 = D5;
  LCD_D6 = D6;
  LCD_D7 = D7;

  Delay_TIM2_CH1_ms(50);
  SendCMD(0x30);
  Delay_TIM2_CH1_ms(5);
  SendCMD(0x30);
  Delay_TIM2_CH1_ms(1);
  SendCMD(0x30);
  Delay_TIM2_CH1_ms(10);
  SendCMD(0x20);
  Delay_TIM2_CH1_ms(10);

  SendCMD(CMD_INIT_4BIT);
  Delay_TIM2_CH1_us(1000);
  SendCMD(CMD_CLEAR);
  Delay_TIM2_CH1_ms(5);
  SendCMD(CMD_INPUT_SET);
  Delay_TIM2_CH1_us(1000);
  SendCMD(CMD_SHIFT);
  Delay_TIM2_CH1_us(1000);  
}

void LCD_SetCursor(char ROW, char COL)
{
  switch (ROW)
  {
    case 0:
      COL |= 0x80;
      break;
    case 1:
      COL |= 0xC0;
      break;
  }
  SendCMD(COL);
  Delay_TIM2_CH1_us(1000);
}

void LCD_Clear(void)
{
  SendCMD(CMD_CLEAR);
  Delay_TIM2_CH1_us(2000);
}

void LCD_PrintChar(char *Data)
{
  SendDATA(Data);
  Delay_TIM2_CH1_us(1000);
}

void LCD_PrintString(char *Data, const unsigned char Length)
{
  char i=0;
  for(i=0; i<Length; i++)
  {
    SendDATA( Data+i );
    Delay_TIM2_CH1_us(1000);
  }
}

/*====================================================================*/






