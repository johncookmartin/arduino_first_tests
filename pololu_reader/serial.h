void initUART(unsigned int baud);
void transmitByte(unsigned char data);
unsigned char receiveByte(void);
void transmitString(const char *str);
void printDec(int num);