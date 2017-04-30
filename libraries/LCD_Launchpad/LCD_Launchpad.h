
/*******************************************************************************
 *
 * LCD_Launchpad.h
 *
 * Hardware abstraction layer for the FH-1138P Segmented LCD
 * on MSP-EXP430FR6989 and MSP-EXP430FR4133
 *
 * February 2015
 * E. Chen
 *
 * June 2015 StefanSch: Adopted for Energia
 *
 * Dec 12, 2015 Rei Vilo
 * 0b00000101 mask for LCDMEM[position+1] to avoid interferences with symbols
 * Tested on MSP430 FR4133 and FR6989
 *
 ******************************************************************************/

#ifndef LCD_LAUNCHPAD_H_ 
#define LCD_LAUNCHPAD_H_ 

#include "Energia.h" 

enum LCD_ICONS {
  LCD_SEG_MARK,
  LCD_SEG_R,
  LCD_SEG_HEART,
  LCD_SEG_CLOCK,
  LCD_SEG_DOT3,
  LCD_SEG_RADIO,
  LCD_SEG_DOT2,
  LCD_SEG_COLON2,
  LCD_SEG_RX,
  LCD_SEG_TX,
  LCD_SEG_DOT1,
  LCD_SEG_MINUS1,
  LCD_SEG_BAT_POL,
  LCD_SEG_BAT1,
  LCD_SEG_BAT3,
  LCD_SEG_BAT5,
  LCD_SEG_DOT5,
  LCD_SEG_DEG5,
  LCD_SEG_BAT_ENDS,
  LCD_SEG_BAT0,
  LCD_SEG_BAT2,
  LCD_SEG_BAT4,
  LCD_SEG_DOT4,
  LCD_SEG_COLON4,
};

class LCD_LAUNCHPAD : public Print {
public:
    LCD_LAUNCHPAD();
    void init();
    void displayText(String s);
	void displayText(String s, char pos);
    void displayText(char* s, char pos);
    void displayScrollText(char* s, unsigned int wait);
    void showChar(char, int);
    void showSymbol(char symbol, int status);
    void clear(void);

    virtual size_t write(uint8_t c);
    //virtual size_t write(const uint8_t *buffer, size_t size);
    using Print::write; // pull in write(str) and write(buf, size) from Print
};

#endif /* LCD_LAUNCHPAD_H_ */
