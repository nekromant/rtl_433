/**
 * file nice_flors.c
 * Andy 'Necromant' Andrianov
 * brief 
 * version 0.1
 * date 2019-04-04
 *
 * copyright Copyright (c) 2019
 * 
 */
#include "decoder.h"


uint8_t halfbyte(uint8_t *data, int pos)
{
    uint8_t tmp = data[pos >> 1];
    uint8_t h = (tmp >> 4);
    uint8_t l = (tmp & 0x0f);
    return (pos & 1) ? h : l;
}

static int nice_flors_callback(r_device *decoder, bitbuffer_t *bitbuffer)
{
    data_t *data;
    int r;
    int device;
    uint8_t *bytes;
    uint8_t id, holdtime, battery_low, test, humidity = 0;

    bitbuffer_invert(bitbuffer);
    bitbuffer_debug(bitbuffer);

    bytes = bitbuffer->bb;
    id = ffs(halfbyte(bytes, 1));
    holdtime = halfbyte(bytes, 0);

    uint8_t serial[8];
    uint8_t timecode[8];    
    timecode[0] = halfbyte(bytes, 3);
    timecode[1] = halfbyte(bytes, 4);
    timecode[2] = halfbyte(bytes, 5);    
    timecode[3] = halfbyte(bytes, 6);

    serial[0] = halfbyte(bytes, 2);
    serial[1] = halfbyte(bytes, 7);
    serial[2] = halfbyte(bytes, 8);    
    serial[3] = halfbyte(bytes, 9);
    serial[4] = halfbyte(bytes, 10);
    serial[5] = halfbyte(bytes, 11);
    serial[6] = halfbyte(bytes, 12);            

    int i; 
    for (i=0; i< 7; i++) {
        printf("ser[%d]=%x\n", i, serial[i]);
    }

    data = data_make(
            "model",       "",                   DATA_STRING, _X("Nice Flor-S","Nice Flor-S remote"),
            "button",      "Button ID",          DATA_FORMAT, "%02x", DATA_INT, id,
            "hold",        "Hold Time",          DATA_FORMAT, "%02x", DATA_INT, holdtime,
            "serial",      "Serial Number",      DATA_FORMAT, "%u %%", DATA_INT, humidity,
            "counter",     "Encrypted Counter",  DATA_STRING, battery_low ? "LOW" : "OK",
            NULL);
    decoder_output_data(decoder, data);

    return 1;
}

static char *output_fields[] = {
    "model",
    "button",
    "hold",
    "serial",
    "counter",
    NULL
};

r_device nice_flors = {
    .name          = "Nice Flor-S Remote Control",
    .modulation    = OOK_PULSE_PWM,
    .short_width   = 500,    // short pulse is 500 us + 1000 us gap
    .long_width    = 1000,    // long pulse is 1000 us + 500 us gap
    .sync_width    = 1500,    // sync pulse is 1500 us + 1500 us gap
    .gap_limit     = 18500,    // long gap (with short pulse) is ~417 us, sync gap is ~833 us
    .reset_limit   = 18500,   // maximum gap is 18500 us
    .decode_fn     = &nice_flors_callback,
    .disabled      = 0,
    .fields        = output_fields,
};
