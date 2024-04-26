#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdbool.h>

/* Button definitions */
#define JUMP_BUTTON 15
#define B_BUTTON 14
#define RIGHT_BUTTON 17
#define LEFT_BUTTON 12
#define DOWN_BUTTON 16
#define UP_BUTTON 13

/* ADC pin for volume control */
#define VOLUME_PIN 26

/* Debounce time for button press */
#define DEBOUNCE_TIME 200

/* Type definition for ADC values */
typedef struct adc {
    int axis;
    int val;
} adc_t;

/* Function Prototypes */
void write_package(adc_t data);
void hc06_task(void *p);
bool has_debounced(uint32_t current_trigger, uint32_t last_trigger);
int adc_normalizer(uint32_t result);
void btn_callback(uint32_t gpio, uint32_t events);
void btn_task(void *p);
void volume_task(void *p);

#endif /* MAIN_H */
