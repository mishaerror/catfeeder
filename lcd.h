//LCD Functions Developed by electroSome

#define LCD_I2C

void LcdWriteChar(const unsigned char a);

void LcdWriteString(const unsigned char *a);

void LcdClear();

void LcdHome();

void LcdSetCursor(char row, char col);

void LcdInit();

