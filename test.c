#include <stdint.h>

/* Base address for UART0 */
#define UART0_BASE_ADDR ...
/* Data register address for UART */
#define UART0_DR ...
/* Interrupt Mask set/clear register address for UART0 */
#define UART0_IMSC ...

/* Base address for GIC (Generic Interrupt Controller) and some registers */
#define GIC1_BASE_ADDR 0x10040000
#define GIC2_BASE_ADDR 0x10050000
#define GICC_CTRL 				(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x0000)))
#define GICC_PMR 				(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x0004)))
#define GICD_CTRL 				(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x1000)))
#define GICD_ISENABLER0 		(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x1100)))
#define GICD_ISENABLER1 		(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x1104)))
#define GICD_IPTR				(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x182c)))

/* UART interrupt (this is irq interrupt) */
void __attribute__((interrupt)) ...()
{
	/* To be completed */
	...
}

/* all other handlers are infinite loops */
void __attribute__((interrupt)) undef_handler(void)
{
	for (;;)
		;
}
void __attribute__((interrupt)) swi_handler(void)
{
	for (;;)
		;
}
void __attribute__((interrupt)) prefetch_abort_handler(void)
{
	for (;;)
		;
}
void __attribute__((interrupt)) data_abort_handler(void)
{
	for (;;)
		;
}
void __attribute__((interrupt)) fiq_handler(void)
{
	for (;;)
		;
}
/*
 * Copy interrupt vectors to address 0
 */
void copy_vectors(void)
{
	extern uint32_t vectors_start;
	extern uint32_t vectors_end;

	uint32_t *vectors_src = ...;	/* See vector.S to get symbols names*/
	uint32_t *vectors_dst = ...;

	while (vectors_src < &vectors_end)
		... = ...;
}
/*
 * Turn off interrupts in the ARM processor
 */
void disable_interrupts(void)
{
	int status = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}
/*
 * Turn on interrupts in the ARM processor
 */
void enable_interrupts(void)
{
	int status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}
/*
 * Configure interrupt for UART0
 */
void config_interrupt_UART (void)
{
	GICD_ISENABLER1 = 0xffffffff;
	GICD_IPTR = 0x1;
}

/*
 * Configure the Generic Interrupt Controller (GIC)
 */
void config_GIC(void)
{
	config_interrupt_UART();    // configure the UART0 interrupt (44)
	GICC_PMR = 0xFF;			// Set Interrupt Priority Mask Register (ICCPMR). Enable all priorities
	GICC_CTRL = 1;				// Set the enable in the CPU Interface Control Register (ICCICR)
	GICD_CTRL = 1;				// Set the enable in the Distributor Control Register (ICDDCR)
}
/*
 * Unmask the receive interrupt
 */
void config_UART(void)
{
	UART0_IMSC = ...;
}

void main(void)
{
	int seed = 57;

	disable_interrupts();   	// disable interrupts in the processor
	config_GIC();              	// configure the general interrupt controller
	config_UART();             	// configure UART to generate interrupts
	enable_interrupts();    	// enable interrupts in the processor

	while (1) {
		seed = seed * 0x343fd + 0x269ec3;
		if ( ((seed >> 0x4) & 0x7ffffff) == 2 )
			UART0_DR = '.';
	}
}
