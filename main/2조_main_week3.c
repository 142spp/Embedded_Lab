#include "stm32f10x.h"

#define RCC_BASE_ADDR 0x40021000
#define RCC_OFFSET 0x18
#define RCC_ADDR (*(volatile unsigned int*)(RCC_BASE_ADDR + RCC_OFFSET))

#define GPIOD_BASE_ADDR 0x40011400
#define GPIOD_CRL *(volatile unsigned int*) (GPIOD_BASE_ADDR + 0x00)
#define GPIOD_CRH *(volatile unsigned int*) (GPIOD_BASE_ADDR + 0x4)
#define GPIOD_BSRR *(volatile unsigned int*) (GPIOD_BASE_ADDR +0x10)
#define GPIOD_BRR *(volatile unsigned int*) (GPIOD_BASE_ADDR + 0x14)

#define GPIOC_BASE_ADDR 0x40011000
#define GPIOC_CRL *(volatile unsigned int*) (GPIOC_BASE_ADDR + 0x00)
#define GPIOC_IDR *(volatile unsigned int*) (GPIOC_BASE_ADDR + 0x08)


int main(void) {

  RCC_ADDR |= 0x30;

  GPIOC_CRL &= ~0x00FFFF00;
  GPIOC_CRL |= 0x00888800;
  
  GPIOD_CRL &= ~0xF00FFF00;
  GPIOD_CRL |= 0x30033300; 


  GPIOC_IDR &= 0x00000000;

  GPIOD_BRR |= 0x9C;

  
  while(1) {
    if(!(GPIOC_IDR&0x4)) GPIOD_BSRR |= 0xC; // 0x1100
    else if(!(GPIOC_IDR&0x8)) GPIOD_BRR |= 0x90;
    else if(!(GPIOC_IDR&0x10)) GPIOD_BRR |= 0xC;
    else if(!(GPIOC_IDR&0x20)) GPIOD_BSRR |= 0x90;
  }
  return 0;
 }