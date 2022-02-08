#include <avr/io.h>
#include <stdlib.h>
#include "UTFT.h"
#include "color.h"
#include "DefaultFonts.h"
#include "ssd1289.h"
#include <avr/pgmspace.h>
#define BLUE 0, 0, 204
#define YELLOW 246, 255, 0
#define WHITE 255, 255, 255

UTFT display;

struct objectQuestion {
	char * question;
	char * a;
	char * b;
	char * c;
	char * d;
	uint8_t correctAns;

	objectQuestion(char * question, char * a, char * b, char * c, char * d, uint8_t correctAns): question(question), a(a), b(b), c(c), d(d), correctAns(correctAns) {}

};
	 const objectQuestion questionsEasy[12] PROGMEM= {
		 objectQuestion("Najveci planet suncevog sustava je?", "Saturn", "Pluton", "Venera", "Jupiter", 4),
		 objectQuestion("Sto nije elementarna nepogoda", "Zemljotres", "Pozar", "Glad", "Poplava", 3),
		 objectQuestion("Ako u splitu pitate koliko je sati i dobijete odgovor 'Po Bota'?", "12:00 sati", "12:30 sati", "13:30 sati", "13:00 sati", 2),
		 objectQuestion("Sto je celesta?", "Glazbeni  instrument", "Juzno voce", "Orientalni kolac", "Vrsta     pjesme", 1),
		 objectQuestion("Na kojoj hrvatskoj novcanici se nalazi slika Marka Marulica?", "500", "200", "100", "1000", 1),
		 objectQuestion("Najveci planet suncevog sustava je?", "Saturn", "Pluton", "Venera", "Jupiter", 4),
		 objectQuestion("Sto nije elementarna nepogoda", "Zemljotres", "Pozar", "Glad", "Poplava", 3),
		 objectQuestion("Ako u splitu pitate koliko je sati i dobijete odgovor 'Po Bota'?", "12:00 sati", "12:30 sati", "13:30 sati", "13:00 sati", 2),
		 objectQuestion("Sto je celesta?", "Glazbeni  instrument", "Juzno voce", "Orientalni kolac", "Vrsta     pjesme", 1),
		 objectQuestion("Na kojoj hrvatskoj novcanici se nalazi slika Marka Marulica?", "500", "200", "100", "1000", 1),
		 objectQuestion("h", "Prvi dan  proljeca", "Prvi dan zime", "Prvi dan  ljeta", "Prvi dan  jeseni", 3),
		 objectQuestion("Which of the following is cotle?", "Klavir", "Saksofon", "Gitaru", "Harmonika", 2),
	 };
	 
void checkAnswer(uint8_t *ans, objectQuestion questionsObj, uint8_t *step) {
	if(*ans == 0) return;
	if ((*ans) == questionsObj.correctAns) {
		(*step)++;
	} else (*step) = 0;
}

void drawScoreHighlight(uint8_t *step) {
	display.setColor(YELLOW);
	display.fillRoundRect(5 + 50 * (*step % 5), 235, 37 + 50 * (*step % 5), 218);
	display.setColor(BLUE);
}

void displayGUI()
{
			display.drawLine(0, 70, 315, 70); //gornja horizontalna linija
			display.drawLine(150, 70, 150, 210); //srednja okomita linija
			display.drawLine(0, 150, 315, 150); //srednja horizontalna linija
			display.drawLine(0, 210, 315, 210); //donja horrizontalna linija

			display.setColor(YELLOW);
			display.setFont(BigFont);
			display.print("A:", 10, 100);
			display.print("B:", 160, 100);
			display.print("C:", 10, 170);
			display.print("D:", 160, 170);
			
			
			display.setColor(WHITE);
			display.setFont(SmallFont);
}

void displayQuestion(objectQuestion questionObj)
{
		display.printWithMargin(questionObj.question, 2, 0);
		display.printWithMargin(questionObj.a, 50, 100, 10);
		display.printWithMargin(questionObj.b, 200, 100, 10);
		display.printWithMargin(questionObj.c, 50, 170, 10);
		display.printWithMargin(questionObj.d, 200, 170, 10);
}

void displayScore(char score[][6], uint8_t *step)
{
	
	if (*step < 5) {
		for (uint8_t i = 0; i < 5; i++) {
			display.print(score[i], 10 + i * 50, 221);
			display.print("->", 280, 225);
		}
		} else if(*step >= 5 && *step < 10) {
		for (uint8_t i = 5; i < 10; i++) {
			display.print(score[i], 10 + (i - 5) * 50, 221);
			display.print("->", 280, 225);
		}
		} else {
		for (uint8_t i = 10; i < 15; i++) {
			display.print(score[i], 10 + (i - 10) * 50, 221);
			display.print("->", 280, 225);
		}
	}
}

int main() {
	
	uint8_t step = 0;
	
	char score[13][6] PROGMEM= {
		"100",
		"300",
		"500",
		"1k",
		"4k",
		"16k",
		"32k",
		"64k",
		"125k",
		"250k",
		"500k",
		"1000k"
	}; 

	//SET PORTS
	PORTB = 0x0f;
	DDRB = 0xf0;
	


	//LCD INIT
	display.InitLCD(LANDSCAPE);
	display.setFont(SmallFont);
	display.clrScr();
	display.fillScr(BLUE);
	display.setBackColor(BLUE);
	drawScoreHighlight(&step);

	uint8_t ans = 0;

	while (1) 
	{
		displayGUI();
		displayScore(score, &step);
		
		if (bit_is_clear(PINB, 0)) {
			ans = 1;
			drawScoreHighlight(&step);
			} else if (bit_is_clear(PINB, 1)) {
			ans = 2;
			drawScoreHighlight(&step);
			} else if (bit_is_clear(PINB, 2)) {
			ans = 3;
			drawScoreHighlight(&step);
			} else if (bit_is_clear(PINB, 3)) {
			ans = 4;
			drawScoreHighlight(&step);
		}
		
		// DISPLAY QUESTION AND CHECK ANSWER
		displayQuestion(questionsEasy[step]);
		checkAnswer(&ans, questionsEasy[step], &step);
		
		ans = 0;
	}
}