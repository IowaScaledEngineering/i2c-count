// Command specific defines
#define STATUS_OVERFLOW    0x80
#define STATUS_UNDERFLOW   0x40

#define CONFIG_PULLUP_ENABLE 0x80
#define CONFIG_PRESET        0x40
#define CONFIG_UPDOWN        0x02
#define CONFIG_COUNT_ENABLE  0x01

#define CONFIG_ALL_PEC_ENABLE    0x80
#define CONFIG_ALL_COUNT_ENABLE  0x01

// User registers
volatile uint8_t  I2C_PAGE[1];
volatile uint8_t  I2C_STATUS_CML[1];
volatile uint8_t  I2C_STATUS[I2C_NUMPAGES];
volatile uint8_t  I2C_CONFIG[I2C_NUMPAGES];
volatile uint8_t  I2C_CONFIG_ALL[1];
volatile uint16_t I2C_REV_ID[1];
volatile uint32_t I2C_READ_COUNT[I2C_NUMPAGES];

i2cCommand i2c_registerMap[] = {
//	cmdCode, attributes,R, W, ramAddr
	{0x00, 0,           1, 1, (uint8_t *)I2C_PAGE},
	{0x03, I2C_PAGED,   0, 0, 0}, // CLEAR_FAULTS
	{0x15, 0,           0, 0, 0}, // STORE_USER_ALL
	{0x16, 0,           0, 0, 0}, // RESTORE_USER_ALL
	{0x7E, 0,           1, 0, (uint8_t *)I2C_STATUS_CML},
	{0x80, I2C_PAGED,   1, 0, (uint8_t *)I2C_STATUS},
	{0xD0, I2C_PAGED,   1, 1, (uint8_t *)I2C_CONFIG},
	{0xD1, 0,           1, 1, (uint8_t *)I2C_CONFIG_ALL},
	{0xDF, I2C_PAGED,   0, 0, 0}, // RESET_COUNT
	{0xE7, 0,           2, 0, (uint8_t *)I2C_REV_ID},
	{0xF0, I2C_PAGED,   2, 0, (uint8_t *)I2C_READ_COUNT},
};

uint8_t i2c_registerIndex[256];

