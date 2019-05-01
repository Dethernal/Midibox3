#include "midiControl.h"

uint8_t midiRxBuffer[MIDIRXBUFFER_SIZE];
int midiRxBufferStartIndex = 0;
int midiRxBufferEndIndex = 0;
