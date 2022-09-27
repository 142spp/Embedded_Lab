#include "stm32f10x.h"

#define RCC_BASE_ADDR 0x40021000
#define RCC_OFFSET 0x18
#define RCC_ADDR (*(volatile unsigned int*)(RCC_BASE_ADDR + RCC_OFFSET))

#define GPIOC_BASE_ADDR 0x40011000
#define GPIOC_CRL *(volatile unsigned int*) (GPIOC_BASE_ADDR + 0x00)
#define GPIOC_IDR *(volatile unsigned int*) (GPIOC_BASE_ADDR + 0x08)

#define GPIOD_BASE_ADDR 0x40011400
#define GPIOD_CRL *(volatile unsigned int*) (GPIOD_BASE_ADDR + 0x00)
#define GPIOD_CRH *(volatile unsigned int*) (GPIOD_BASE_ADDR + 0x4)
#define GPIOD_IDR *(volatile unsigned int*) (GPIOD_BASE_ADDR + 0x08)
#define GPIOD_BSRR *(volatile unsigned int*) (GPIOD_BASE_ADDR +0x10)
#define GPIOD_BRR *(volatile unsigned int*) (GPIOD_BASE_ADDR + 0x14)

#define GPIOE_BASE_ADDR 0x40011800
#define GPIOE_CRL *(volatile unsigned int*) (GPIOE_BASE_ADDR + 0x00)
#define GPIOE_BSRR *(volatile unsigned int*) (GPIOE_BASE_ADDR +0x10)
#define GPIOE_BRR *(volatile unsigned int*) (GPIOE_BASE_ADDR + 0x14)

void delay(){
  int i;
  for(i = 0; i< 1000000; i++){}
}

int main(void) {
 
  RCC_ADDR |= 0x70; // set clock 0111 0000
  
  GPIOC_CRL &= ~0x00FFFF00; // joy stick
  GPIOC_CRL |= 0x00888800;
  
  GPIOD_CRH &= ~0x000F0000; // switch
  GPIOD_CRH |= 0x00080000; 
 
  GPIOE_CRL &= ~0x00000F0F; // relay 
  GPIOE_CRL |= 0x00000303;
  
  while(1) {
    if(!(GPIOC_IDR&0x04)){  
      GPIOE_BSRR |= 0x01; 
      GPIOE_BRR |= 0x04;
    }
    else if(!(GPIOC_IDR&0x20)) {
      GPIOE_BSRR |= 0x04;
      GPIOE_BRR |= 0x01;
    } 
    if(!(GPIOD_IDR&0x0800)) GPIOE_BRR |= 0x05;
    delay();
  }
  return 0;
 }