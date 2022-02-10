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
	 const objectQuestion questionsEasy[10] = {
		 objectQuestion("Najveci planet suncevog sustava je", "Saturn", "Pluton", "Venera", "Jupiter", 4),
		 objectQuestion("Sto nije elementarna nepogoda", "Zemljotres", "Pozar", "Glad", "Poplava", 3),
		 objectQuestion("Koliko je sati ako je 'Po Bota'", "12:00", "12:30", "13:30", "13:00 ", 2),
		 objectQuestion("Koji je trenutni HTML standard", "HTML5", "HTML5.5", "HTML4", "HTML3.3", 1),
		 objectQuestion("Na kojoj hrvatskoj novcanici se nalazi slika Marka Marulica", "500", "200", "100", "1000", 1),
		 objectQuestion("U kojoj je godini The Godfather prvi put objavljen", "1982", "1972", "1977", "1997", 2),
		 objectQuestion("Kada je zadnji put hajduk bio prvak HNLa", "2000", "2005", "1999", "2007", 2),
		 objectQuestion("Koliko igraca ima u olimpijskom timu zacurling", "3", "10", "11", "4", 4),
		 objectQuestion("Kako je srednje ime Sir Alexa Fergusona", "Mile", "Torbar", "Lord", "Brill ", 2),
		 objectQuestion("Beirut je glavni grad", "Lebanon", "Bahrain", "Kuwait", "Sirija", 1),
	 };
	 
void displayWin() {
	display.clrScr();
	display.setFont(BigFont);
	display.fillScr(BLUE);
	display.setBackColor(BLUE);
	display.print("MILIJUNAS", 80, 110);
	display.setFont(SmallFont);
	_delay_ms(3000);
	display.clrScr();
} 

void displayCorrect() {
	display.clrScr();
	display.setFont(BigFont);
	display.fillScr(BLUE);
	display.setBackColor(BLUE);
	display.print("TOCNO", 110, 110);
	display.setFont(SmallFont);
	_delay_ms(1000);
	display.clrScr();
}

void displayIncorrect() {
	display.clrScr();
	display.setFont(BigFont);
	display.fillScr(BLUE);
	display.setBackColor(BLUE);
	display.print("NETOCNO", 100, 110);
	display.setFont(SmallFont);
	_delay_ms(1000);
	display.clrScr();
}

void drawScoreHighlight(uint8_t *step) {

	display.setColor(YELLOW);
	display.fillRoundRect(5 + 50 * ((*step) % 5), 235, 37 + 50 * ((*step) % 5), 218);
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
	
	if ((*step) < 5) {
		for (uint8_t i = 0; i < 5; i++) {
			display.print(score[i], 10 + (i * 50), 221);
			display.print("->", 280, 225);
		}
		} else if(*step >= 5 && *step < 10) {
		for (uint8_t i =5; i < 10; i++) {
			display.print(score[i], 10 + (i - 5) * 50, 221);
			display.print("->", 280, 225);
		}
	}
}

void checkAnswer(uint8_t *ans, objectQuestion questionsObj, uint8_t *step, char score[][6]) {
	if((*step) >= 10) {
		displayWin();
		*ans = 4;
	}
	if((*ans) == 0) return;
	if ((*ans) == questionsObj.correctAns) {
			(*step)++;
			if((*step) < 10) displayCorrect();
			display.fillScr(BLUE);
			displayGUI();
			displayQuestion(questionsEasy[*step]);
			drawScoreHighlight(step);
		} 
		else {
			if((*step) < 10 ) displayIncorrect();
			(*step) = 0;
			display.fillScr(BLUE);
			displayGUI();
			displayQuestion(questionsEasy[*step]);
			drawScoreHighlight(step);
	};
}

int main() {
	
	uint8_t step = 0;
	
	char score[10][6]  = {
		"100",
		"500",
		"2k",
		"4k",
		"16k",
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
	display.clrScr();
	display.InitLCD(LANDSCAPE);
	display.setFont(SmallFont);
	display.fillScr(BLUE);
	display.setBackColor(BLUE);
	//INIT GUI
	displayGUI();
	displayQuestion(questionsEasy[step]);
	drawScoreHighlight(&step);
	displayScore(score, &step);
	
	uint8_t ans = 0;

	while (1) 
	{

		displayGUI();
		displayScore(score, &step);
		
		if (bit_is_clear(PINB, 0)) {
			ans = 1;
			_delay_ms(300);
			} else if (bit_is_clear(PINB, 1)) {
			ans = 2;
			_delay_ms(300);
			} else if (bit_is_clear(PINB, 2)) {
			ans = 3;
			_delay_ms(300);
			} else if (bit_is_clear(PINB, 3)) {
			ans = 4;
			_delay_ms(300);
		}
		
		// DISPLAY QUESTION AND CHECK ANSWER
		
		checkAnswer(&ans, questionsEasy[step], &step, score);
		
		ans = 0;
	}
}