#include <avr/io.h>
#include "UTFT.h"
#include "color.h"
#include "had.c"
#include "DefaultFonts.h"
#include "ssd1289.h"

int main()
{
	UTFT display;
	
	display.InitLCD(LANDSCAPE);
	display.setFont(BigFont);
	display.clrScr();
	//status_led_init();
	
	
	while(1)
	{
		
		display.print("Hello world!", 10, 10, 2);
		
		
	}
	
}