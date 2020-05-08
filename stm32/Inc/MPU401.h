#ifndef MPU401_H
#define MPU401_H

#include "export.h"
#include <stdint.h>

void MIDI_Init(bool delaysysex,bool fakeallnotesoff);
void MIDI_RawOutByte(uint8_t data);
bool MIDI_Available(void);

void MPU401_Event(void);
void MPU401_ResetDone(void); /* SOFTMPU */
void MPU401_EOIHandler(void);
void MPU401_Reset(void);
void MPU401_EOIHandlerDispatch(void);

#define MPU401_VERSION  0x15
#define MPU401_REVISION 0x01
#define MPU401_QUEUE 32
#define MPU401_TIMECONSTANT 58000 /* SOFTMPU: Was 60000 */
#define MPU401_RESETBUSY 27*(RTCFREQ/1000) /* SOFTMPU */

enum MpuMode { M_UART,M_INTELLIGENT };
typedef enum MpuMode MpuMode; /* SOFTMPU */
enum MpuDataType {T_OVERFLOW,T_MARK,T_MIDI_SYS,T_MIDI_NORM,T_COMMAND};
typedef enum MpuDataType MpuDataType; /* SOFTMPU */

/* Messages sent to MPU-401 from host */
#define MSG_EOX					0xf7
#define MSG_OVERFLOW			0xf8
#define MSG_MARK				0xfc

/* Messages sent to host from MPU-401 */
#define MSG_MPU_OVERFLOW		0xf8
#define MSG_MPU_COMMAND_REQ		0xf9
#define MSG_MPU_END				0xfc
#define MSG_MPU_CLOCK			0xfd
#define MSG_MPU_ACK				0xfe

/* HardMPU: MPU config int16_t */
#define CONFIG_SYSEXDELAY		(mpu.config & 0x80)
#define CONFIG_FAKEALLNOTESOFF	(mpu.config & 0x40)
#define CONFIG_VERSIONFIX		(mpu.config & 0x20)

typedef struct {
    bool intelligent;
        uint8_t config;
        MpuMode mode;
    struct track {
        int16_t counter;
        uint8_t value[8],sys_val;
        uint8_t vlength,length;
        MpuDataType type;
    } playbuf[8],condbuf;
    struct {
        bool conductor,cond_req,cond_set, block_ack;
        bool playing,reset;
        bool wsd,wsm,wsd_start;
        bool run_irq,irq_pending;
        bool send_now;
        bool eoi_scheduled;
        int8_t data_onoff;
        uint16_t command_byte,cmd_pending;
        uint8_t tmask,cmask,amask;
        uint16_t midi_mask;
        uint16_t req_mask;
        uint8_t channel,old_chan;
    } state;
    struct {
        uint8_t timebase,old_timebase;
        uint8_t tempo,old_tempo;
        uint8_t tempo_rel,old_tempo_rel;
        uint8_t tempo_grad;
        uint8_t cth_rate,cth_counter;
        bool clock_to_host,cth_active;
    } clock;
} mpu_t;

#endif // MPU401_H
