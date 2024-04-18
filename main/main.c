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
#define B_BUTTON 16
#define RIGHT_BUTTON 17
#define LEFT_BUTTON 18


void hc06_task(void *p) {
    uart_init(HC06_UART_ID, HC06_BAUD_RATE);
    gpio_set_function(HC06_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(HC06_RX_PIN, GPIO_FUNC_UART);
    hc06_init("aps2_legal", "1234");

    while (true) {
        uart_puts(HC06_UART_ID, "OLAAA ");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void write_package(uint8_t btn) {
    uart_putc_raw(uart0, 0xFF);
    uart_putc_raw(uart0, btn);
    uart_putc_raw(uart0, 0xFE);
}

void btn_callback(uint gpio, uint32_t events) {
    uint8_t BTN;
    if (gpio == JUMP_BUTTON) {
        if (events & GPIO_IRQ_EDGE_FALL) {
            BTN = 11;
        }
        else if (events & GPIO_IRQ_EDGE_RISE) {
            BTN = 10;
        }
    }
    xQueueSendFromISR(xQueueBtn, &BTN, 0);
}

void btn_task(void *p) {
    gpio_init(JUMP_BUTTON);
    gpio_set_dir(JUMP_BUTTON, GPIO_IN);
    gpio_pull_up(JUMP_BUTTON);
    gpio_set_irq_enabled_with_callback(JUMP_BUTTON, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    uint8_t BTN;
    while (1) {
        if (xQueueReceive(xQueueBtn, &BTN, pdMS_TO_TICKS(10)) == pdTRUE) {
            write_package(BTN);
        }
    }
}
                                                                                                                                                                                                           
int main() {
    stdio_init_all();
    adc_init();
    uart_init(uart0, 115200);

    xQueueBtn = xQueueCreate(32, sizeof(uint8_t));

    xTaskCreate(hc06_task, "UART_Task 1", 4096, NULL, 1, NULL);
    xTaskCreate(btn_task, "Jump Task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;

}