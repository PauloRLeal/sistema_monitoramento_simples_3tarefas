#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "semphr.h"

SemaphoreHandle_t xMutex;
QueueHandle_t dataQueue;
int buttonState = 0; // Variável compartilhada do estado do botão (não pressionado: 0, pressionado: 1)

// define a pinagem de saída do led_red
const uint led_pin_red = 13;

// define a pinagem de saída do botão B
#define BTN_B_PIN 6

void setup(void)
{
    stdio_init_all();

    // Inicializando o led_red
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);

    // Inicializando o botão A
    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN); // Configura o botão B em pull_up
}

void vLedTask(void *pvParamters)
{
    int receivedData;
    uint *led = (uint *)pvParamters;
    for (;;)
    {
        // Verifica a presença de elementos na Queue
        if (xQueueReceive(dataQueue, &receivedData, portMAX_DELAY) == pdTRUE)
        {
            // Atualiza o estado do led
            gpio_put(*led, receivedData);

            // Imprime o estado atual do led
            printf("Led Task: estado = %i\n", receivedData);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void vReadButtonTask(void *pvParamters)
{
    for (;;)
    {
        // Protege a região crítica
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            // Atualizando a variável de estado do botão (buttonState) ao pressionar o botão B
            if (gpio_get(BTN_B_PIN) == 0)
            {
                buttonState = 1;
            }
            else
            {
                buttonState = 0;
            }

            // Imprime o estado atual do botão B
            printf("Button Task: estado = %i\n", buttonState);

            xSemaphoreGive(xMutex);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void vProcessingButtonTask(void *pvParamters)
{
    static uint afterStatus = 0;
    for (;;)
    {
        // Protege a região crítica
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            // Verifica se o estado do botão alterou, para evitar atualizações de estado do led desnecessárias
            if (((buttonState == 1) && (afterStatus == 0)) || ((buttonState == 0) && (afterStatus == 1)))
            {
                // Atualiza a variável que guarda o estado do botão B
                afterStatus = buttonState;

                // Acrescenta o valor na Queue
                xQueueSend(dataQueue, &afterStatus, portMAX_DELAY);

                // Imprime o estado de atualização do led
                printf("Processing Button Task: estado = %i\n", afterStatus);
            }
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

int main()
{
    setup();

    xMutex = xSemaphoreCreateMutex();
    dataQueue = xQueueCreate(5, sizeof(float)); // Fila com capacidade de 5 itens

    if ((xMutex != NULL) && (dataQueue != NULL))
    {
        // Crias as Task
        xTaskCreate(vReadButtonTask, "Read Button Task", configMINIMAL_STACK_SIZE * 4, NULL, 1, NULL);
        xTaskCreate(vProcessingButtonTask, "Processing Button Task", configMINIMAL_STACK_SIZE * 4, NULL, 1, NULL);
        xTaskCreate(vLedTask, "Led Task", configMINIMAL_STACK_SIZE * 4, (void *)&led_pin_red, 1, NULL);
        vTaskStartScheduler();
    }

    for (;;)
        ;
    return 0;
}
