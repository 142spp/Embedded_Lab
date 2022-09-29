/* base_var.h */

#define APB2PERIPH_BASE (uint32_t)0x40010000

/* Base Address */
#define RCC_BASE_ADDR       (APB2PERIPH_BASE + 0x20000)
#define GPIOA_BASE_ADDR     (APB2PERIPH_BASE + 0x00800)
#define GPIOB_BASE_ADDR     (APB2PERIPH_BASE + 0x00C00)
#define GPIOC_BASE_ADDR     (APB2PERIPH_BASE + 0x01000)
#define GPIOD_BASE_ADDR     (APB2PERIPH_BASE + 0x01400)
#define GPIOE_BASE_ADDR     (APB2PERIPH_BASE + 0x01800)
#define GPIOF_BASE_ADDR     (APB2PERIPH_BASE + 0x01C00)
#define GPIOG_BASE_ADDR     (APB2PERIPH_BASE + 0x02000)

/* Offsets */
#define RCC_APB2ENR_OFFSET  0x18
#define GPIO_CRL_OFFSET     0x0
#define GPIO_CRH_OFFSET     0x4
#define GPIO_IDR_OFFSET     0x8
#define GPIO_ODR_OFFSET     0xC
#define GPIO_BSRR_OFFSET    0x10
#define GPIO_BRR_OFFSET     0x14
#define GPIO_LCKR_OFFSET    0x18

/* Aditional Address */

#define RCC_APB2ENR_ADDR (RCC_BASE_ADDR + RCC_APB2ENR_OFFSET)

/* GPIO Pin */
#define GPIO_PIN0   0x0001
#define GPIO_PIN1   0x0002
#define GPIO_PIN2   0x0004
#define GPIO_PIN3   0x0008
#define GPIO_PIN4   0x0010
#define GPIO_PIN5   0x0020
#define GPIO_PIN6   0x0040
#define GPIO_PIN7   0x0080

#define GPIO_PIN8   0x0100
#define GPIO_PIN9   0x0200
#define GPIO_PIN10  0x0400
#define GPIO_PIN11  0x0800
#define GPIO_PIN12  0x1000
#define GPIO_PIN13  0x2000
#define GPIO_PIN14  0x4000
#define GPIO_PIN15  0x8000

#define GPIO_PINALL 0xFFFF

/* enum */
enum {RESET = 0, SET = !RESET};

