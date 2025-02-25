#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// Enable device mode
#define CFG_TUD_ENABLED              1
#define CFG_TUSB_MCU                 OPT_MCU_RP2040
#define CFG_TUSB_RHPORT0_MODE        OPT_MODE_DEVICE

// Endpoint buffer size (important for Windows compatibility)
#define CFG_TUD_ENDPOINT0_SIZE       64

// Enable composite device with CDC and MIDI
#define CFG_TUD_COMPOSITE            1
#define CFG_TUD_CDC                  1  // Enable 1 CDC interface
#define CFG_TUD_MIDI                 1  // Enable 1 MIDI interface

// Buffer sizes
#define CFG_TUD_CDC_RX_BUFSIZE       64
#define CFG_TUD_CDC_TX_BUFSIZE       64
#define CFG_TUD_MIDI_RX_BUFSIZE      64
#define CFG_TUD_MIDI_TX_BUFSIZE      64


#endif /* _TUSB_CONFIG_H_ */
