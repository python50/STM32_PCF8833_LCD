/* Memory Spaces Definitions */
MEMORY
{
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
}

/* higher address of the user mode stack used by startup code*/
_end_stack = 0x20000000 + 20K;

/* There will be a link error if there is not this amount of RAM free at the end. */
_minimum_stack_size = 0x100 ;


/* Define reset_handler as the entry point, required for debugging */
ENTRY(reset_handler)


SECTIONS
{
	/* for Cortex devices, the beginning of the startup code is stored in the .isr_vector section, which goes to FLASH */
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))			/* Startup code */
		. = ALIGN(4);
	} >FLASH
 
	/* the program code is stored in the .text section, which goes to Flash */
	.text :
	{
		. = ALIGN(4);
		
		*(.text)				/* remaining code */
		*(.text.*)				/* remaining code */
		*(.rodata)				/* read-only data (constants) */
		*(.rodata*)
		*(.glue_7)
		*(.glue_7t)

		. = ALIGN(4);
		
   		_end_text = .;
		/* This is used by the startup in order to initialize the .data secion */
   		_start_initalized_data = _end_text;
	} >FLASH
	
 

	/* This is the initialized data section
	The program executes knowing that the data is in the RAM
	but the loader puts the initial values in the FLASH (inidata).
	It is one task of the startup to copy the initial values from FLASH to RAM. */
	.data  : AT ( _start_initalized_data )
	{
		. = ALIGN(4);
		/* This is used by the startup in order to initialize the .data secion */
		_start_data = . ;
		
		*(.data)
		*(.data.*)

		. = ALIGN(4);
		/* This is used by the startup in order to initialize the .data secion */
   		_end_data = . ;
	} >RAM

	/* This is the uninitialized data section */
	.bss :
	{
		. = ALIGN(4);
		/* This is used by the startup in order to initialize the .bss secion */
		_start_bss = .;
		
		*(.bss)
		*(COMMON)
		
		. = ALIGN(4);
		/* This is used by the startup in order to initialize the .bss secion */
   		_end_bss = . ;
	} >RAM
	

	/* This is the user stack section 
	This is just to check that there is enough RAM left for the User mode stack
	It should generate an error if it's full.
	*/
	._usrstack :
	{
		. = ALIGN(4);
		_susrstack = . ;
		
		. = . + _minimum_stack_size ;
		
		. = ALIGN(4);
		_eusrstack = . ;
	} >RAM
}
