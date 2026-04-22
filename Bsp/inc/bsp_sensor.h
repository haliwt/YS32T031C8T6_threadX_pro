#ifndef __BSP_SENSOR_H
#define __BSP_SENSOR_H
#include "main.h"


#include "ys32t031.h"
#include "tx_api.h"
#include <stdint.h>

/* 根据你的实际接线修改 */
#define DHT11_GPIO_PORT      GPIOA
#define DHT11_GPIO_PIN       GPIO_Pin_4
#define DHT11_GPIO_CLK       RCC_AHB2Periph_GPIOA

#define DHT11_DATA_PIN              GPIO_Pin_4
#define DHT11_DATA_GPIO_PORT        GPIOA


/* 对外接口 */
void   DHT11_Init(void);
uint8_t DHT11_ReadData(uint8_t *humi, uint8_t *temp);

uint8_t dht11_read_temp_humidity_value(void);





#endif 

