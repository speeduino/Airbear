//#define DEBUG_SERIAL_PARSER
#define CALIBRATION_TEMPERATURE_OFFSET 40

extern uint8_t serialECURequestQueueSize;

void parseFixedSerialData();
void requestSerialData();
void initSerialData();