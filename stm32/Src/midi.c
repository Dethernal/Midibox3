/*
 *  Copyright (C) 2002-2013  The DOSBox Team
 *  Copyright (C) 2013-2014  bjt, elianda
 *  Copyright (C) 2015		 ab0tj
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * --------------------------------------------
 * HardMPU by ab0tj - Hardware MPU-401 Emulator
 * --------------------------------------------
 * 
 * Based on midi.c from SoftMPU by bjt which was
 * based on original midi.c from DOSBox
 *
 */

/* SOFTMPU: Moved exported functions & types to header */
#include "export.h"
#include <stdint.h>
#include "utils.h"
#include "stm32f1xx_ll_usart.h"
/* HardMPU includes */
#include "config.h"
#include "utils.h"
#include "midi.h"
#include "iobuffers.h"

ring_buffer midi_out_buff = { {0}, 0, 0 };

const uint8_t MIDI_evt_len[256] = {
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  // 0x00
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  // 0x10
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  // 0x20
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  // 0x30
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  // 0x40
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  // 0x50
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  // 0x60
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,  // 0x70

 3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3,  // 0x80
 3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3,  // 0x90
 3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3,  // 0xa0
 3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3,  // 0xb0

 2,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2,  // 0xc0
 2,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2,  // 0xd0

 3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3,  // 0xe0

 0,2,3,2, 0,0,1,0, 1,0,1,1, 1,0,1,0   // 0xf0
};

/* SOFTMPU: Note tracking for RA-50 */
#define MAX_TRACKED_CHANNELS 16
#define MAX_TRACKED_NOTES 8

static const uint8_t MIDI_welcome_msg[30] = "\xf0\x41\x10\x16\x12\x20\x00\x00  *** HardMPU ***   \x13\xf7";	// message to show on MT-32 display

static uint8_t MIDI_note_off[3] = { 0x80,0x00,0x00 }; /* SOFTMPU */

/* SOFTMPU: Note tracking for RA-50 */
typedef struct {
    uint8_t used;
    uint8_t next;
    uint8_t notes[MAX_TRACKED_NOTES];
} channel;

channel tracked_channels[MAX_TRACKED_CHANNELS];

static struct {
    uint8_t status;
    uint8_t cmd_len;
    uint8_t cmd_pos;
    uint8_t cmd_buf[8];
    uint8_t rt_buf[8];
	struct {
        uint8_t buf[SYSEX_SIZE];
        uint16_t used;
        uint8_t usedbufs;
        //uint16_t delay;
		bool delay;
        uint8_t status;
	} sysex;
    bool fakeallnotesoff;
	bool available;
	/*MidiHandler * handler;*/ /* SOFTMPU */
} midi;

/* SOFTMPU: Sysex delay is decremented from PIC_Update */
volatile uint16_t MIDI_sysex_delaytime;


static void PlayMsg(uint8_t* msg, uint16_t len)
{
    __disable_irq();
    // Enqueue data and start transmission if it is not already started
    ring_push(&midi_out_buff, (char*) msg, len);
    if (LL_USART_IsActiveFlag_TXE(USART2)) {
        uint8_t b = ring_popb(&midi_out_buff);
        LL_USART_TransmitData8(USART2, b);
        LL_USART_EnableIT_TXE(USART2);
    }
    __enable_irq();

}

static void send_midi_byte_now(uint8_t byte) {
    LL_USART_Enable(USART2);
    while (!LL_USART_IsActiveFlag_TXE(USART2)) {
    }
    LL_USART_TransmitData8(USART2, byte);
    while (!LL_USART_IsActiveFlag_TXE(USART2)){
    }
}

/* SOFTMPU: Fake "All Notes Off" for Roland RA-50 */
static void FakeAllNotesOff(uint8_t chan)
{
    uint8_t note;
	channel* pChan;

	MIDI_note_off[0] &= 0xf0;
	MIDI_note_off[0] |= chan;

	pChan=&tracked_channels[chan];

	for (note=0;note<pChan->used;note++)
	{
        MIDI_note_off[1]=pChan->notes[note];
        PlayMsg(MIDI_note_off,3);
	}

	pChan->used=0;
	pChan->next=0;
}

void MIDI_RawOutByte(uint8_t data) {
    channel* pChan; /* SOFTMPU */

	/* Test for a realtime MIDI message */
	if (data>=0xf8) {
		midi.rt_buf[0]=data;
		PlayMsg(midi.rt_buf,1);
		return;
	}        
	if (data&0x80) {
		midi.status=data;
		midi.cmd_pos=0;
		midi.cmd_len=MIDI_evt_len[data];
	}
	if (midi.cmd_len) {
		midi.cmd_buf[midi.cmd_pos++]=data;
		if (midi.cmd_pos >= midi.cmd_len) {
			/*if (CaptureState & CAPTURE_MIDI) {
				CAPTURE_AddMidi(false, midi.cmd_len, midi.cmd_buf);
                        }*/ /* SOFTMPU */

                        if (midi.fakeallnotesoff)
                        {
                                /* SOFTMPU: Test for "Note On" */
                                if ((midi.status&0xf0)==0x90)
                                {
                                        if (midi.cmd_buf[2]>0)
                                        {
                                                pChan=&tracked_channels[midi.status&0x0f];
                                                pChan->notes[pChan->next++]=midi.cmd_buf[1];
                                                if (pChan->next==MAX_TRACKED_NOTES) pChan->next=0;
                                                if (pChan->used<MAX_TRACKED_NOTES) pChan->used++;
                                        }

                                        PlayMsg(midi.cmd_buf,midi.cmd_len);
                                }
                                /* SOFTMPU: Test for "All Notes Off" */
                                else if (((midi.status&0xf0)==0xb0) &&
                                         (midi.cmd_buf[1]>=0x7b) &&
                                         (midi.cmd_buf[1]<=0x7f))
                                {
                                        FakeAllNotesOff(midi.status&0x0f);
                                }
                                else
                                {
                                        PlayMsg(midi.cmd_buf,midi.cmd_len);
                                }
                        }
                        else
                        {
                                PlayMsg(midi.cmd_buf,midi.cmd_len);
                        }
                        midi.cmd_pos=1;         //Use Running status
		}
	}
	else 
	{
		midi.rt_buf[0] = data;
		PlayMsg(midi.rt_buf,1);
	}
}

bool MIDI_Available(void)  {
	return midi.available;
}

/* SOFTMPU: Initialization */
void MIDI_Init(bool delaysysex,bool fakeallnotesoff){
    uint8_t i; /* SOFTMPU */
	MIDI_sysex_delaytime = 0; /* SOFTMPU */
	midi.sysex.delay = delaysysex;
	
	midi.status=0x00;
	midi.sysex.status=0x00;
	midi.cmd_pos=0;
	midi.cmd_len=0;
    midi.fakeallnotesoff=fakeallnotesoff>0;
    midi.available=true;
    ring_init(&midi_out_buff);
		
    /* SOFTMPU: Display welcome message on MT-32 */
    __disable_irq();
    for (i=0;i<sizeof(MIDI_welcome_msg);i++)
    {
        send_midi_byte_now(MIDI_welcome_msg[i]);
    }

    //PlayMsg((uint8_t *) MIDI_welcome_msg, sizeof(MIDI_welcome_msg));

	/* HardMPU: Turn off any stuck notes */
    for (i=0xb0;i<0xc0;i++)
    {
        send_midi_byte_now(i);
        send_midi_byte_now(0x7b);
        send_midi_byte_now(0);
    }
		
    /* SOFTMPU: Init note tracking */
    for (i=0;i<MAX_TRACKED_CHANNELS;i++)
    {
		tracked_channels[i].used=0;
		tracked_channels[i].next=0;
    }

    while(!LL_USART_IsActiveFlag_TC(USART2));
    LL_USART_ClearFlag_TC(USART2);

    __enable_irq();
}
