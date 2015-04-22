# gpio specific usage consideration for stm32fxxx port

## Constraints
STM32FXXX series of microcontroller have 16 lines for external interrupt handling which line is dedicated to a single pin.
Not all gpio pins can be used as external interrupt source because gpio pins and exti lines are connected in following manner: Pxy -> EXTIy, where x is port designation, and can be A, B, C... (depend of concrete microcontroller), and y is pin designation and goes from 0 to 15, maximal number of exti line is 16. 

No all exti lines have its own interrupt vector, some exti lines share the same interrupt vector, user must put 
`ngpio_isr()` function in depending HAL exti interrupt handler which can be positioned in `stm32xxx_it.c` file.

    

	 