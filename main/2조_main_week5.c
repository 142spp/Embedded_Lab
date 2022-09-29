#include "stm32f10x.h"
#include "stm32f10x_usart.h"

void SysInit(void) {
    /* Set HSION bit */
    /* Internal Clock Enable */
    RCC->CR |= (uint32_t)0x00000001; //HSION

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= (uint32_t)0xF0FF0000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    RCC->CFGR &= (uint32_t)0xFF80FFFF;

    /* Reset PLL2ON and PLL3ON bits */
    RCC->CR &= (uint32_t)0xEBFFFFFF;

    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = 0x00FF0000;

    /* Reset CFGR2 register */
    RCC->CFGR2 = 0x00000000;
}

void SetSysClock(void) {
    volatile uint32_t StartUpCounter = 0, HSEStatus = 0;
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    /* Enable HSE */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);
    /* Wait till HSE is ready and if Time out is reached exit */
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
        HSEStatus = (uint32_t)0x01;
    }
    else {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01) {
        /* Enable Prefetch Buffer */
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        /* Flash 0 wait state */
        FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;

//@TODO - 1 Set the clock ///////////////////////////////////////////////////////////////
        // -*- Reference Manual 102p
        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        /* PCLK2 = HCLK / 2, use PPRE2 */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV2;
        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

        /* Reset PLLs */
        RCC->CFGR &= ~(uint32_t)(RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC |
                    RCC_CFGR_PLLMULL);
        RCC->CFGR2 &= ~(uint32_t)(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
                    RCC_CFGR2_PREDIV1 | RCC_CFGR2_PREDIV1SRC);
        /* Configure PLLs ------------------------------------------------------*/
        // -*- default Clock : 25MHz 
        // 1. 28 MHz
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLSRC_PREDIV1 | 
                    RCC_CFGR_PLLMULL7); 
                    // -*- /1 /1 *7
        RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
                    RCC_CFGR2_PREDIV1SRC_PLL2| RCC_CFGR2_PREDIV1_DIV10); 
                    // -*- /5 *8 /10 = 28MHz
        // 2. 52 MHz
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLSRC_PREDIV1 |
                    RCC_CFGR_PLLMULL4)  
                    // -*- /1 /1 *4
        RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL13 |
                    RCC_CFGR2_PREDIV1SRC_PLL2| RCC_CFGR2_PREDIV1_DIV5); 
                    // -*- /5 *13 /5 = 52MHz
       
//@End of TODO - 1 ///////////////////////////////////////////////////////////////

        /* Enable PLL2 */
        RCC->CR |= RCC_CR_PLL2ON;
        /* Wait till PLL2 is ready */
        while ((RCC->CR & RCC_CR_PLL2RDY) == 0)
        {
        }
        /* Enable PLL */
        RCC->CR |= RCC_CR_PLLON;
        /* Wait till PLL is ready */
        while ((RCC->CR & RCC_CR_PLLRDY) == 0)
        {
        }
        /* Select PLL as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
        {
        }
        /* Select System Clock as output of MCO */

//@TODO - 2 Set the MCO port for system clock output
        // -*- Reference Manual 101p
        RCC->CFGR &= ~(uint32_t)RCC_CFGR_MCO; 
        // -*- reset MCO
        RCC->CFGR |= (uint32_t)RCC_CFGR_MCO_SYSCLK; 
        // -*- set MCO Clock to System Clock = 28MHz
//@End of TODO - 2

    }
    else {
        /* If HSE fails to start-up, the application will have wrong clock
        configuration. User can add here some code to deal with this error */
    }
}

void RCC_Enable(void) {
//@TODO - 3 RCC Setting
    // -*- Reference Manual 112p
    /*---------------------------- RCC Configuration -----------------------------*/
    /* GPIO RCC Enable  */
    /* UART Tx, Rx, MCO port */ 
    RCC->APB2ENR |= (uint32_t)(RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPDEN);
    // -*- Port A : USART & Port D : Switch Enable
    /* USART RCC Enable */      
    RCC->APB2ENR |= (uint32_t)RCC_APB2ENR_USART1EN;
    // -*- USART1 Enable
}

void PortConfiguration(void) {
//@TODO - 4 GPIO Configuration
    // -*- Reference Manual 172p
    /* Reset(Clear) Port A CRH - MCO, USART1 TX,RX */
    GPIOA->CRH &= ~(
	    (GPIO_CRH_CNF8 | GPIO_CRH_MODE8) |
	    (GPIO_CRH_CNF9 | GPIO_CRH_MODE9) |
	    (GPIO_CRH_CNF10 | GPIO_CRH_MODE10)
	);
    /* MCO Pin Configuration */
    GPIOA->CRH |= (uint32_t)(GPIO_CRH_CNF8_1|GPIO_CRH_MODE8);
                // -*- MCO : Set Push-Pull, Max Hz = 50MHz
    /* USART Pin Configuration */
    GPIOA->CRH |= (uint32_t)(GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9|
                // -*- USART1_TX : Set Push-Pull, Max Hz = 50MHz
                GPIO_CRH_CNF10_1);
    GPIOA->CRH &= ~(uint32_t)(GPIO_CRH_MODE10);
                // -*- USART1_RX : Set Input, (Pull-up, Pull-down)
    /* Reset(Clear) Port D CRH - User S1 Button */
    GPIOD->CRH &= ~(uint32_t)(GPIO_CRH_MODE | GPIO_CRH_MODE); // 
    /* User S1 Button Configuration */
    GPIOD->CRH |= (uint32_t)(GPIO_CRH_CNF11_1);
    GPIOD->CRH &= ~(uint32_t)(GPIO_CRH_MODE11);
                // -*- Switch : Set Input, (Pull-up, Pull-down)
}

void UartInit(void) {
    /*---------------------------- USART CR1 Configuration -----------------------*/
    /* Clear M, PCE, PS, TE and RE bits */
    USART1->CR1 &= ~(uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE);
    /* Configure the USART Word Length, Parity and mode ----------------------- */
//@TODO - 6: WordLength : 8bit
    // -*- ReferenceManual 814p 
    /* Set the M bits according to USART_WordLength value */
    USART1->CR1 |= (uint32_t)(USART_CR1_M); // -*- 9bit
    // USART1->CR1 &= ~(uint32_t)(USART_CR1_M); // -*- 8bit
//@TODO - 7: Parity : None
    /* Set PCE and PS bits according to USART_Parity value */
    USART1->CR1 |= (uint32_t)(USART_CR1_PCE | USART_CR1_PS);
//@TODO - 8: Enable Tx and Rx
    /* Set TE and RE bits according to USART_Mode value */
    USART1->CR1 |= (uint32_t)(USART_CR1_TE | USART_CR1_RE);

    /*---------------------------- USART CR2 Configuration -----------------------*/
    /* Clear STOP[13:12] bits */
    USART1->CR2 &= ~(uint32_t)(USART_CR2_STOP);
    /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit ------------*/
    USART1->CR2 &= ~(uint32_t)(USART_CR2_CPHA | USART_CR2_CPOL | USART_CR2_CLKEN);
//@TODO - 9: Stop bit : 1bit
    // -*- Reference Manual 816p
    /* Set STOP[13:12] bits according to USART_StopBits value */
    USART1->CR2 &= ~(uint32_t)(USART_CR2_STOP); 
    // -*- 00 : 1bit, 01 : 0.5bit, 10 : 2bit, 11 : 1.5bit
    /*---------------------------- USART CR3 Configuration -----------------------*/
    /* Clear CTSE and RTSE bits */
    USART1->CR3 &= ~(uint32_t)(USART_CR3_CTSE | USART_CR3_RTSE);
    /* Configure the USART HFC -------------------------------------------------*/

//@TODO - 10: CTS, RTS : disable
    // -*- Reference Manual 817p
    /* Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
    USART1->CR3 &= ~(uint32_t)(USART_CR3_CTSE | USART_CR3_RTSE); 
    // -*- 0 : Disable, 1 : Enable

    /*---------------------------- USART BRR Configuration -----------------------*/
    /* Configure the USART Baud Rate -------------------------------------------*/
    /* Determine the integer part */
    /* Determine the fractional part */
//@TODO - 11: Calculate & configure BRR
    USART1->BRR |= 0X1E4;
    // -*- Mantissa : 0x1E = 30, Fraction : 0x4 -> 4/16 = 0.25
    // -*- => 0d30.25
    /*---------------------------- USART Enable ----------------------------------*/
    /* USART Enable Configuration */
//@TODO - 12: Enable UART (UE)
    USART1->CR1 |= USART_CR1_UE ;
    // -*- 0 : Disable, 1: Enable
}
void delay(void){
    int i = 0;
    for(i=0;i<1000000;i++);
}

void SendData(uint16_t data) {
    /* Transmit Data */
	USART1->DR = data;

	/* Wait till TC is set */
	while ((USART1->SR & USART_SR_TC) == 0);
}

int main() {
	int i;
	char msg[] = "Hello Team02\r\n"; // -*- length = 14;
	
    SysInit();
    SetSysClock();
    RCC_Enable();
    PortConfiguration();
    UartInit();
    
    while (1) {
      if (!(GPIOD->IDR & GPIO_IDR_IDR12)) { // -*- when swith is pushed
        for (i=0;i<14; i++) {   // -*- repeat for msg.length
          SendData(msg[i]);     // -*- Serial Communication
        }
      }
      delay();
    }
    


}// end main