#include "tusb.h"


// Endpoint addresses
#define EPNUM_CDC_NOTIF  0x81
#define EPNUM_CDC_OUT    0x02
#define EPNUM_CDC_IN     0x82
#define EPNUM_MIDI_OUT   0x03
#define EPNUM_MIDI_IN    0x83

// Device Descriptor
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB            = 0x0200,
    .bDeviceClass       = 0xef, // Composite device
    .bDeviceSubClass    = 0x02,
    .bDeviceProtocol    = 0x01,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = 0xCafe,
    .idProduct          = 0x4002,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 1
};

// Configuration Descriptor
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_MIDI_DESC_LEN)

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, 3, 0, CONFIG_TOTAL_LEN, 0, 100),

    // CDC (Virtual Serial Port)
    TUD_CDC_DESCRIPTOR(0, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),
    TUD_CDC_DATA_DESCRIPTOR(1, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),

    // MIDI (USB Audio Class v1)
    TUD_MIDI_DESCRIPTOR(2, 5, EPNUM_MIDI_OUT, EPNUM_MIDI_IN, 64),
};

// String Descriptors
char const* string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 },  // 0: Language (English US)
    "Your Manufacturer",            // 1: Manufacturer
    "CDC + MIDI Device",            // 2: Product
    "123456",                        // 3: Serial Number
    "USB CDC",                       // 4: CDC Interface Name
    "USB MIDI"                       // 5: MIDI Interface Name
};

uint16_t _desc_str[32];

uint8_t const* tud_descriptor_device_cb(void) {
    return (uint8_t const*)&desc_device;
}

uint8_t const* tud_descriptor_configuration_cb(uint8_t index) {
    (void)index;
    return desc_configuration;
}

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;  
    uint8_t chr_count;

    if (index == 0) {
        _desc_str[0] = (TUSB_DESC_STRING << 8) | 4;
        _desc_str[1] = 0x09;  // Language ID
        _desc_str[2] = 0x04;  // Language code for English (US)
        return _desc_str;
    } else {
        if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0]))) return NULL;
        const char* str = string_desc_arr[index];
        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;  // Ensure we don't exceed 31 characters

        _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);  // Length of the string descriptor
        for (uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];  // Copy characters into the descriptor buffer
        }
        return _desc_str;
    }
}
