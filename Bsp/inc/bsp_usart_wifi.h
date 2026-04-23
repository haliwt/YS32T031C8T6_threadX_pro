#ifndef __BSP_USART_WIFI_H
#define __BSP_USART_WIFI_H
#include "main.h"





void usart2_rx_callback_invoke(uint8_t data);

void Subscribe_Rx_Interrupt_Handler(void);

void Wifi_Rx_InputInfo_Handler(void);

void send_usart2_data(const uint8_t* pdata,uint8_t lenght);

void wifi_communication_tnecent_handler(void);

//void Parse_Json_Statement(void);
void Reconnection_Wifi_Order(void);


void Parse_Tencent_Data(void) ;
void wifi_check_id_handler(void);


#endif 


