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

QueueHandle_t xQueueBtn;

#define JUMP_BUTTON 15
#define B_BUTTON 14
#define RIGHT_BUTTON 13
#define LEFT_BUTTON 12
#define DOWN_BUTTON 16
#define UP_BUTTON 17
#define VOLUME_PIN 26

#define DEBOUNCE_TIME 100


// void hc06_task(void *p) {
//     uart_init(HC06_UART_ID, HC06_BAUD_RATE);
//     gpio_set_function(HC06_TX_PIN, GPIO_FUNC_UART);
//     gpio_set_function(HC06_RX_PIN, GPIO_FUNC_UART);
//     hc06_init("Chute Boxe", "1234");

//     while (true) {
//         uart_puts(HC06_UART_ID, "OLAAA ");
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

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

typedef struct adc {
    int axis;
    int val;
} adc_t;

void write_package(adc_t data) {
    int val = data.val;
    int msb = val >> 8;
    int lsb = val & 0xFF;

    uart_putc_raw(uart0, data.axis);
    uart_putc_raw(uart0, msb);
    uart_putc_raw(uart0, lsb);
    uart_putc_raw(uart0, -1);
}

void btn_callback(uint gpio, uint32_t events) {
    uint8_t BTN;
    if (gpio == JUMP_BUTTON) {
        if (events == 0x4) {
            BTN = 11;
        }
        else {
            BTN = 10;
        }
    } else if (gpio == B_BUTTON) {
        if (events == 0x4) {
            BTN = 21;
        }
        else {
            BTN = 20;
        }
    } else if (gpio == LEFT_BUTTON) {
        if (events == 0x4) {
            BTN = 31;
        }
        else {
            BTN = 30;
        }
    } else if (gpio == RIGHT_BUTTON) {
        if (events == 0x4) {
            BTN = 41;
        }
        else {
            BTN = 40;
        }
    } else if (gpio == DOWN_BUTTON) {
        if (events == 0x4) {
            BTN = 51;
        }
        else {
            BTN = 50;
        }
    } else if (gpio == UP_BUTTON) {
        if (events == 0x4) {
            BTN = 61;
        }
        else {
            BTN = 60;
        }
    }
    xQueueSendFromISR(xQueueBtn, &BTN, 0);
}

void btn_task(void *p) {
    gpio_init(JUMP_BUTTON);
    gpio_set_dir(JUMP_BUTTON, GPIO_IN);
    gpio_pull_up(JUMP_BUTTON);
    gpio_set_irq_enabled_with_callback(JUMP_BUTTON, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);
    
    gpio_init(B_BUTTON);
    gpio_set_dir(B_BUTTON, GPIO_IN);
    gpio_pull_up(B_BUTTON);
    gpio_set_irq_enabled(B_BUTTON, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    
    gpio_init(LEFT_BUTTON);
    gpio_set_dir(LEFT_BUTTON, GPIO_IN);
    gpio_pull_up(LEFT_BUTTON);
    gpio_set_irq_enabled(LEFT_BUTTON, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

    gpio_init(RIGHT_BUTTON);
    gpio_set_dir(RIGHT_BUTTON, GPIO_IN);
    gpio_pull_up(RIGHT_BUTTON);
    gpio_set_irq_enabled(RIGHT_BUTTON, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
    
    gpio_init(DOWN_BUTTON);
    gpio_set_dir(DOWN_BUTTON, GPIO_IN);
    gpio_pull_up(DOWN_BUTTON);
    gpio_set_irq_enabled(DOWN_BUTTON, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

    uint8_t BTN;
    uint32_t trigger_time;
    adc_t data;

    uint32_t jump_f_btn_last_trigger = 0;
    uint32_t jump_r_btn_last_trigger = 0;
    uint32_t b_f_btn_last_trigger = 0;
    uint32_t b_r_btn_last_trigger = 0;
    uint32_t r_f_btn_last_trigger = 0;
    uint32_t r_r_btn_last_trigger = 0;
    uint32_t l_f_btn_last_trigger = 0;
    uint32_t l_r_btn_last_trigger = 0;
    uint32_t d_f_btn_last_trigger = 0;
    uint32_t d_r_btn_last_trigger = 0;
    uint32_t u_f_btn_last_trigger = 0;
    uint32_t u_r_btn_last_trigger = 0;

    while (1) {
        if (xQueueReceive(xQueueBtn, &BTN, pdMS_TO_TICKS(100)) == pdTRUE) {
            trigger_time = to_ms_since_boot(get_absolute_time());
            if (BTN == 11 && has_debounced(trigger_time, jump_f_btn_last_trigger)) {
                jump_f_btn_last_trigger = trigger_time;
                data.axis = 1;
                data.val = 1;
                write_package(data);
            } else if (BTN == 10 && has_debounced(trigger_time, jump_r_btn_last_trigger)) {
                jump_r_btn_last_trigger = trigger_time;
                data.axis = 1;
                data.val = 0;
                write_package(data);
            } else if (BTN == 21 && has_debounced(trigger_time, b_f_btn_last_trigger)) {
                b_f_btn_last_trigger = trigger_time;
                data.axis = 2;
                data.val = 1;
                write_package(data);
            } else if (BTN == 20 && has_debounced(trigger_time, b_r_btn_last_trigger)) {
                b_r_btn_last_trigger = trigger_time;
                data.axis = 2;
                data.val = 0;
                write_package(data);
            } else if (BTN == 31 && has_debounced(trigger_time, l_f_btn_last_trigger)) {
                l_f_btn_last_trigger = trigger_time;
                data.axis = 3;
                data.val = 1;    return;
                write_package(data);
            } else if (BTN == 30 && has_debounced(trigger_time, l_r_btn_last_trigger)) {
                l_r_btn_last_trigger = trigger_time;
                data.axis = 3;
                data.val = 0;
                write_package(data);
            } else if (BTN == 41 && has_debounced(trigger_time, r_f_btn_last_trigger)) {
                r_f_btn_last_trigger = trigger_time;
                data.axis = 4;
                data.val = 1;
                write_package(data);
            } else if (BTN == 40 && has_debounced(trigger_time, r_r_btn_last_trigger)) {
                r_r_btn_last_trigger = trigger_time;
                data.axis = 4;
                data.val = 0;
                write_package(data);
            } else if (BTN == 51 && has_debounced(trigger_time, d_f_btn_last_trigger)) {
                d_f_btn_last_trigger = trigger_time;
                data.axis = 5;
                data.val = 1;
                write_package(data);
            } else if (BTN == 50 && has_debounced(trigger_time, d_r_btn_last_trigger)) {
                d_r_btn_last_trigger = trigger_time;
                data.axis = 5;
                data.val = 0;
                write_package(data);
            } else if (BTN == 61 && has_debounced(trigger_time, u_f_btn_last_trigger)) {
                u_f_btn_last_trigger = trigger_time;
                data.axis = 6;
                data.val = 1;
                write_package(data);
            } else if (BTN == 60 && has_debounced(trigger_time, u_r_btn_last_trigger)) {
                u_r_btn_last_trigger = trigger_time;
                data.axis = 6;
                data.val = 0;
                write_package(data);
            }
        }
    }
}

void volume_task (void *p) {
    adc_init();
    adc_gpio_init(VOLUME_PIN);
    //uint16_t threshold = 2000;  
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
    uart_init(uart0, 115200);

    xQueueBtn = xQueueCreate(64, sizeof(uint8_t));

    // xTaskCreate(hc06_task, "UART_Task 1", 4096, NULL, 1, NULL);
    xTaskCreate(btn_task, "BTN Task", 4096, NULL, 1, NULL);
    xTaskCreate(volume_task, "Volume Task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}