/*
 * STM32F103 GPIO Configuration Macros
 * Generates bits for pins in Configuration Register Low and high
 *
 * Copyright (C) 2015 Jason White
 *
 * Permission is granted to use, modify, sell, distribute, and reverse engineer
 * the contents of this file provided a the origin of this file is not
 * misrepresented.
 */

#define GPIO_ICFG_ANALOG	(0b00)
#define GPIO_ICFG_FLOAT		(0b01)
#define GPIO_ICFG_PULLUP	(0b10)

#define GPIO_OCFG_IO		(0b00)
#define GPIO_OCFG_IO_OD		(0b01)
#define GPIO_OCFG_ALT		(0b10)
#define GPIO_OCFG_ALT_OD	(0b11)

/* There are actually 3 output modes, the highest slew rate has been selected
 * as the default output mode
 */
#define GPIO_MODE_INPUT		(0b00)
#define GPIO_MODE_OUTPUT	(0b11)


#define GPIO_CRL(pin, mode, config) (((config<<2)|mode)<<(4*pin))
#define GPIO_CRH(pin, mode, config) (((config<<2)|mode)<<(4*(pin-8)))
