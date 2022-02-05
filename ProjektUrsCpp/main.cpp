#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "UTFT.h"
#include "color.h"
#include "had.c"
#include "DefaultFonts.h"
#include "ssd1289.h"


//#include "URTouch.h"
//#include "URTouchCD.h"
UTFT display;

int main()
{
	
	display.InitLCD(LANDSCAPE);
	display.setFont(SmallFont);
	display.clrScr();
	display.fillScr(0,0,204);
	
	
	//URTouch touch;
	//touch.InitTouch(LANDSCAPE);
	////status_led_init();
	//touch.read();
	//uint16_t x = touch.getX();
	//uint16_t y =  touch.getY();
	

	
	while(1)
	{//36 10
		
		display.printWithMargin("Ovo sranje je sranjew5663e bas ono fdlblaf32la tutu kgdfgdfiko tata tor peoa 234243324234324 t4ghr  ssslskkftkt ,mumumuu  ##", 2, 0);
		display.drawLine(0,100,315,100); //gornja horizontalna linija
		display.drawLine(150,100,150,240); //srednja okomita linija
		display.drawLine(0,170,315,170); //srednja horizontalna linija
		
		display.printWithMargin("Arrrwerewerrwerwerweerwwrerwe",10,130,15);
		display.print("B",160,130,0);
	}
	
}