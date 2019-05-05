/**
 * file nice_flors.c
 * Andy 'Necromant' Andrianov
 * brief 
 * version 0.1
 * date 2019-04-04
 *
 * copyright Copyright (c) 2019
 * Actual coding: 
 * Logic 0: 4;12;4;12
 * Logic 1: 12,4,12,4
 * Logic f: 4,12,12,4
 * Sync: 4,128
 * 
 */
#include "decoder.h"




static int nice_flors_callback(r_device *decoder, bitbuffer_t *bitbuffer)
{
    data_t *data;
    int r;
    int device;
    uint8_t *bytes;
    uint8_t id, holdtime, battery_low, test, humidity = 0;

    bitbuffer_invert(bitbuffer);
    bitbuffer_debug(bitbuffer);

    #if 0
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
    #endif
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

r_device beamish = {
    .name          = "Beamish remote",
    .modulation    = OOK_PULSE_PWM,
    .short_width   = 64,    
    .long_width    = 192,    // long pulse is 1000 us + 500 us gap
    .sync_width    = 0,    // sync pulse is 1500 us + 1500 us gap
    .gap_limit     = 0,   
    .reset_limit   = 50000,   // maximum gap is 18500 us
    .decode_fn     = &nice_flors_callback,
    .disabled      = 1,
    .fields        = output_fields,
};
