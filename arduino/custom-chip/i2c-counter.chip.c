// Wokwi Custom Chip - For information and examples see:
// https://link.wokwi.com/custom-chips-alpha
//
// SPDX-License-Identifier: MIT
// Copyright (C) 2022 Uri Shaked / wokwi.com

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

const int ADDRESS = 0x22;

DEFINE_PIN(VCC);
DEFINE_PIN(GND);
DEFINE_PIN(SCL);
DEFINE_PIN(SDA);
DEFINE_PIN(INT);

typedef struct {
  pin_t pin_int;
  uint8_t counter;
  uint32_t threshold_attr;
} chip_state_t;

static bool on_i2c_connect(void *chip, uint32_t address, bool connect);
static uint8_t on_i2c_read(void *chip);
static bool on_i2c_write(void *chip, uint8_t data);
static void on_i2c_disconnect(void *chip);

void* chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  chip->pin_int = pin_init("INT", INPUT);
  chip->counter = 0;

  const i2c_dev_t i2c_dev = {
    .address = ADDRESS,
    .scl = pin_init("SCL", INPUT),
    .sda = pin_init("SDA", INPUT),
    .connect = on_i2c_connect,
    .read = on_i2c_read,
    .write = on_i2c_write,
    .disconnect = on_i2c_disconnect, // Optional
  };
  i2c_init(chip, &i2c_dev);

  // This attribute can be edited by the user. It's defined in wokwi-custom-part.json:
  chip->threshold_attr = attr_init("threshold", 127);

  // The following message will appear in the browser's DevTools console:
  printf("Hello from custom chip!\n");

  // Returning the chip instance is going away soon - but is still needed for some APIs (SPI, timers, pin_watch)
  return chip;
}

static void counter_updated(chip_state_t *chip) {
  const uint32_t threshold = attr_read(chip->threshold_attr);
  if (chip->counter > threshold) {
    pin_write(chip->pin_int, LOW);
    pin_mode(chip->pin_int, OUTPUT);
  } else {
    pin_mode(chip->pin_int, INPUT);
  }
}

bool on_i2c_connect(void *user_ctx, uint32_t address, bool connect) {
  return true; /* Ack */
}

uint8_t on_i2c_read(void *user_ctx) {
  chip_state_t *chip = user_ctx;
  chip->counter++;
  counter_updated(chip);
  return chip->counter;
}

bool on_i2c_write(void *user_ctx, uint8_t data) {
  chip_state_t *chip = user_ctx;
  chip->counter = data;
  counter_updated(chip);
  return true; // Ack
}

void on_i2c_disconnect(void *user_ctx) {
  // Do nothing
}
