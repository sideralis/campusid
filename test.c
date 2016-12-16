#include <stdint.h>

#define UART0_BASE_ADDR 0x10009000
#define UART0_DR (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x000)))
#define UART0_IMSC (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x038)))
#define UART0_MIS (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x040)))
#define UART0_ICR (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x044)))

#define UART1_BASE_ADDR ...
#define UART1_DR ...
#define UART1_IMSC ...
#define UART1_MIS ...
#define UART1_ICR ...

#define GIC1_BASE_ADDR 0x10040000
#define GIC2_BASE_ADDR 0x10050000
#define GICC_CTRL 				(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x0000)))
#define GICC_PMR 				(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x0004)))
#define GICD_CTRL 				(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x1000)))
#define GICD_ISENABLER0 		(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x1100)))
#define GICD_ISENABLER1 		(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x1104)))
#define GICD_IPTR				(*((volatile uint32_t *)(GIC1_BASE_ADDR + 0x182c)))

void __attribute__((interrupt)) irq_handler() {

	if (UART0_MIS ...) {
		/* IRQ is coming from UART0 */
		...;					/* Write data on UART0 TX */
		UART0_ICR = ...;		/* Acknowledge RX interrupt from UART0 */
	}

	if (UART1_MIS ...) {
		/* IRQ is coming from UART1 */
		...;					/* Write data on UART1 TX */
		UART1_ICR = ...;		/* Acknowledge RX interrupt from UART1 */
	}

}

/* all other handlers are infinite loops */
void __attribute__((interrupt)) undef_handler(void) {
	for (;;)
		;
}
void __attribute__((interrupt)) swi_handler(void) {
	for (;;)
		;
}
void __attribute__((interrupt)) prefetch_abort_handler(void) {
	for (;;)
		;
}
void __attribute__((interrupt)) data_abort_handler(void) {
	for (;;)
		;
}
void __attribute__((interrupt)) fiq_handler(void) {
	for (;;)
		;
}

void copy_vectors(void) {
	extern uint32_t vectors_start;
	extern uint32_t vectors_end;
	uint32_t *vectors_src = &vectors_start;
	uint32_t *vectors_dst = (uint32_t *) 0;

	while (vectors_src < &vectors_end)
		*vectors_dst++ = *vectors_src++;
}
/*
 * Turn off interrupts in the ARM processor
 */
void disable_interrupts(void) {
	int status = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}
/*
 * Turn on interrupts in the ARM processor
 */
void enable_interrupts(void) {
	int status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

void config_interrupt_UART(void) {
	GICD_ISENABLER1 = 0xffffffff;
	GICD_IPTR = 0x1;
}

/*
 * Configure the Generic Interrupt Controller (GIC)
 */
void config_GIC(void) {
	config_interrupt_UART();    // configure the UART0 interrupt (44)
	GICC_PMR = 0xFF; // Set Interrupt Priority Mask Register (ICCPMR). Enable all priorities
	GICC_CTRL = 1; // Set the enable in the CPU Interface Control Register (ICCICR)
	GICD_CTRL = 1; // Set the enable in the Distributor Control Register (ICDDCR)
}
void config_UART(void) {
	UART0_IMSC = 1 << 4;		// Enable RX for UART0
	...;						// Enable RX for UART1
}

void main(void) {

	disable_interrupts();   	// disable interrupts in the processor
	config_GIC();              	// configure the general interrupt controller
	config_UART();             	// configure UART to generate interrupts
	enable_interrupts();    	// enable interrupts in the processor

	while (1) {
	}

}
