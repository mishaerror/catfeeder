//LCD Functions Developed by electroSome

#define LCD_I2C

void lcdWriteChar(const unsigned char a);

void lcdWriteString(const unsigned char *a);

void lcdClear();

void lcdHome();

void lcdSetCursor(char row, char col);

void lcdInit();

void lcdOn(unsigned on);

void lcdCursorBlink(unsigned on);
