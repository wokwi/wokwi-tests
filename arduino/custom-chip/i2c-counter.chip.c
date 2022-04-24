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

chip_state_t* EXPORT(chip_init)(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  const pin_t scl = pin_init("SCL", INPUT);
  const pin_t sda = pin_init("SDA", INPUT);
  chip->pin_int = pin_init("INT", INPUT);
  i2c_init(ADDRESS, scl, sda);
  chip->counter = 0;

  // This attribute can be edited by the user. It's defined in wokwi-custom-part.json:
  chip->threshold_attr = attr_init("threshold", 127);

  // The following message will appear in the browser's DevTools console:
  printf("Hello from custom chip!\n");

  return chip;
}

static void counter_updated(chip_state_t *chip) {
  const uint32_t threshold = attr_read(chip->threshold_attr);
  if (chip->counter > threshold) {
    digital_write(chip->pin_int, LOW);
    pin_mode(chip->pin_int, OUTPUT);
  } else {
    pin_mode(chip->pin_int, INPUT);
  }
}

bool EXPORT(chip_i2c_connect)(chip_state_t *chip, uint32_t address, bool connect) {
  return true; /* Ack */
}

uint8_t EXPORT(chip_i2c_read)(chip_state_t *chip) {
  chip->counter++;
  counter_updated(chip);
  return chip->counter;
}

bool EXPORT(chip_i2c_write)(chip_state_t *chip, uint8_t data) {
  chip->counter = data;
  counter_updated(chip);
  return true; // Ack
}

void EXPORT(chip_i2c_disconnect)(chip_state_t *chip) {
  // Do nothing
}
