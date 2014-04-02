// Command specific defines

// User registers
volatile uint8_t  I2C_PAGE[1];
volatile uint8_t  I2C_STATUS_CML[1];
volatile uint8_t  I2C_CONFIG[2];
volatile uint16_t I2C_REV_ID[1];
volatile uint32_t I2C_READ_COUNT[2];

i2cCommand i2c_registerMap[] = {
//	cmdCode, attributes,R, W, ramAddr
	{0x00, 0,           1, 1, (uint8_t *)I2C_PAGE},
	{0x03, I2C_PAGED,   0, 0, 0}, // CLEAR
	{0x7E, 0,           1, 0, (uint8_t *)I2C_STATUS_CML},
	{0xD0, I2C_PAGED,   1, 1, (uint8_t *)I2C_CONFIG},
	{0xE7, 0,           2, 0, (uint8_t *)I2C_REV_ID},
	{0xF0, I2C_PAGED,   2, 0, (uint8_t *)I2C_READ_COUNT},
};

uint8_t i2c_registerIndex[256];

