
#ifndef __LCD_1602__
#define __LCD_1602__

#include "main.h"

void LCD_Init(char PORT, char RS, char EN, char D4, char D5, char D6, char D7);
void LCD_SetCursor(char ROW, char COL);
void LCD_Clear(void);
void LCD_PrintChar(char *Data);
void LCD_PrintString(char *Data, const unsigned char Length);

#endif /*__LCD_1602__*/


