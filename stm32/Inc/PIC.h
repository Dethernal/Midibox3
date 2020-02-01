#ifndef PIC_H
#define PIC_H

#include "export.h"
#include "config.h"

void PIC_Update(bool blocking);
void PIC_Init(void); /* SOFTMPU */
void PIC_AddEvent(EventID event,uint16_t delay);
void PIC_RemoveEvents(EventID event);

#endif
