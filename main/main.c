/*
 * LED blink with FreeRTOS
 */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include <string.h>

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/adc.h"
#include <stdlib.h>

#include "hc06.h"

QueueHandle_t xQueueBtn, xQueueBT;

#define JUMP_BUTTON 15
#define B_BUTTON 14
#define RIGHT_BUTTON 17
#define LEFT_BUTTON 12
#define DOWN_BUTTON 16
#define UP_BUTTON 13
#define VOLUME_PIN 26

#define DEBOUNCE_TIME 200

typedef struct adc {
    int axis;
    int val;
} adc_t;

void write_package(adc_t data) {
    int val = data.val;
    int msb = val >> 8;
    int lsb = val & 0xFF;

    uart_putc_raw(uart1, data.axis);
    uart_putc_raw(uart1, msb);
    uart_putc_raw(uart1, lsb);
    uart_putc_raw(uart1, -1);
}

void hc06_task(void *p) {
    uart_init(HC06_UART_ID, HC06_BAUD_RATE);
    gpio_set_function(HC06_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(HC06_RX_PIN, GPIO_FUNC_UART);
    hc06_init("chutebox", "1234");

    uint8_t BTN;
    adc_t data2;

    while (true) {
        if (xQueueReceive(xQueueBT, &BTN, pdMS_TO_TICKS(100)) == pdTRUE) {
            if (BTN == 11) {
                data2.axis = 1;
                data2.val = 0;
            } else if (BTN == 21) {
                data2.axis = 2;
                data2.val = 0;
            } else if (BTN == 31) {
                data2.axis = 3;
                data2.val = 0;
            } else if (BTN == 41) {
                data2.axis = 4;
                data2.val = 0;
            } else if (BTN == 51) {
                data2.axis = 5;
                data2.val = 0;
            } else if (BTN == 61) {
                data2.axis = 6;
                data2.val = 0;
            }
            write_package(data2);  
        }
    }
}

bool has_debounced(uint32_t current_trigger, uint32_t last_trigger) {
    return current_trigger - last_trigger > DEBOUNCE_TIME;
}

int adc_normalizer(uint32_t result) {
    if (result < 400) {
        return 0;
    } else if (result < 800) {
        return 1;
    } else if (result < 1200) {
        return 2;
    } else if (result < 1600) {
        return 3;
    } else if (result < 2000) {
        return 4;
    } else if (result < 2400) {
        return 5;
    } else if (result < 2800) {
        return 6;
    } else if (result < 3200) {
        return 7;
    } else if (result < 3600) {
        return 8;
    } else {
        return 9;
    }
}

void btn_callback(uint gpio, uint32_t events) {
    uint8_t BTN;
    if (gpio == JUMP_BUTTON) {
        if (events == 0x4) {
            BTN = 11;
        }
    } else if (gpio == B_BUTTON) {
        if (events == 0x4) {
            BTN = 21;
        }
    } else if (gpio == LEFT_BUTTON) {
        if (events == 0x4) {
            BTN = 31;
        }
    } else if (gpio == RIGHT_BUTTON) {
        if (events == 0x4) {
            BTN = 41;
        }
    } else if (gpio == DOWN_BUTTON) {
        if (events == 0x4) {
            BTN = 51;
        }
    } else if (gpio == UP_BUTTON) {
        if (events == 0x4) {
            BTN = 61;
        }
    }
    xQueueSendFromISR(xQueueBtn, &BTN, 0);
}

void btn_task(void *p) {
    gpio_init(JUMP_BUTTON);
    gpio_set_dir(JUMP_BUTTON, GPIO_IN);
    gpio_pull_up(JUMP_BUTTON);
    gpio_set_irq_enabled_with_callback(JUMP_BUTTON, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    
    gpio_init(B_BUTTON);
    gpio_set_dir(B_BUTTON, GPIO_IN);
    gpio_pull_up(B_BUTTON);
    gpio_set_irq_enabled(B_BUTTON, GPIO_IRQ_EDGE_FALL, true);
    
    gpio_init(LEFT_BUTTON);
    gpio_set_dir(LEFT_BUTTON, GPIO_IN);
    gpio_pull_up(LEFT_BUTTON);
    gpio_set_irq_enabled(LEFT_BUTTON, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(RIGHT_BUTTON);
    gpio_set_dir(RIGHT_BUTTON, GPIO_IN);
    gpio_pull_up(RIGHT_BUTTON);
    gpio_set_irq_enabled(RIGHT_BUTTON, GPIO_IRQ_EDGE_FALL, true);
    
    gpio_init(DOWN_BUTTON);
    gpio_set_dir(DOWN_BUTTON, GPIO_IN);
    gpio_pull_up(DOWN_BUTTON);
    gpio_set_irq_enabled(DOWN_BUTTON, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(UP_BUTTON);
    gpio_set_dir(UP_BUTTON, GPIO_IN);
    gpio_pull_up(UP_BUTTON);
    gpio_set_irq_enabled(UP_BUTTON, GPIO_IRQ_EDGE_FALL, true);

    uint8_t BTN;
    uint32_t trigger_time;
    adc_t data;

    uint32_t jump_f_btn_last_trigger = 0;
    uint32_t b_f_btn_last_trigger = 0;
    uint32_t r_f_btn_last_trigger = 0;
    uint32_t l_f_btn_last_trigger = 0;
    uint32_t d_f_btn_last_trigger = 0;
    uint32_t u_f_btn_last_trigger = 0;

    while (1) {
        if (xQueueReceive(xQueueBtn, &BTN, pdMS_TO_TICKS(100)) == pdTRUE) {
            trigger_time = to_ms_since_boot(get_absolute_time());
            if (BTN == 11 && has_debounced(trigger_time, jump_f_btn_last_trigger)) {
                jump_f_btn_last_trigger = trigger_time;
                xQueueSend(xQueueBT, &BTN, 0);
            } else if (BTN == 21 && has_debounced(trigger_time, b_f_btn_last_trigger)) {
                b_f_btn_last_trigger = trigger_time;
                xQueueSend(xQueueBT, &BTN, 0);
            } else if (BTN == 31 && has_debounced(trigger_time, l_f_btn_last_trigger)) {
                l_f_btn_last_trigger = trigger_time;
                xQueueSend(xQueueBT, &BTN, 0);
            } else if (BTN == 41 && has_debounced(trigger_time, r_f_btn_last_trigger)) {
                r_f_btn_last_trigger = trigger_time;
                xQueueSend(xQueueBT, &BTN, 0);
            } else if (BTN == 51 && has_debounced(trigger_time, d_f_btn_last_trigger)) {
                d_f_btn_last_trigger = trigger_time;
                xQueueSend(xQueueBT, &BTN, 0);
            } else if (BTN == 61 && has_debounced(trigger_time, u_f_btn_last_trigger)) {
                u_f_btn_last_trigger = trigger_time;
                xQueueSend(xQueueBT, &BTN, 0);
            }
        }
    }
}

void volume_task (void *p) {
    adc_init();
    adc_gpio_init(VOLUME_PIN);
    
    uint16_t previous_result = 0;
    adc_t data;

    while (1) {
        adc_select_input(0);
        uint16_t result = adc_read();
        result = adc_normalizer(result);
        if (result != previous_result) {
            data.axis = 7;
            data.val = result;
            write_package(data);
            previous_result = result;
        }
        vTaskDelay(100);
    }
} 


int main() {
    stdio_init_all();

    xQueueBtn = xQueueCreate(64, sizeof(uint8_t));
    xQueueBT = xQueueCreate(64, sizeof(uint8_t));

    //printf("Ola amigo!");

    xTaskCreate(hc06_task, "UART_Task 1", 4096, NULL, 1, NULL);
    xTaskCreate(btn_task, "BTN Task", 4096, NULL, 1, NULL);
    xTaskCreate(volume_task, "Volume Task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}