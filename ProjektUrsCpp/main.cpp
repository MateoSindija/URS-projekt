#include <avr/io.h>

#include <avr/interrupt.h>

#include <stdlib.h>

#include "UTFT.h"

#include "color.h"

#include "DefaultFonts.h"

#include "ssd1289.h"

/*
* RGB colors
*/

#define BLUE 0, 0, 204
#define YELLOW 246, 255, 0
#define WHITE 255, 255, 255

/*
* Coordinates for option letters
*/

#define A_COORDINATE 10, 100
#define B_COORDINATE 160, 100
#define C_COORDINATE 10, 170
#define D_COORDINATE 160, 170

/*
* Coordinates for displaying options (answers)
*/

#define A_OPTION_COORDINATE 50, 100, 10
#define B_OPTION_COORDINATE 200, 100, 10
#define C_OPTION_COORDINATE 50, 170, 10
#define D_OPTION_COORDINATE 200, 170, 10

/*
* Messages coordinate
*/

#define MILLIONARE 80, 110
#define CORRECT 100, 110
#define INCORRECT 110, 110
#define SECURED 55, 110, 15
#define MESSAGE_16K 110, 140, 15

/*
* Time to answer a question. Increase to increase question expiration time
*/

#define EXPIRATION_TIME 20

/*
* Deboounce time
*/
#define DEBOUNCE 300

/*
* GUI lines
*/

#define TOP_HORIZONTAL_LINE 0, 70, 315, 70
#define MIDDLE_VERTICAL_LINE 150, 70, 150, 210
#define MIDDLE_HORIZONTAL_LINE 0, 150, 315, 150
#define BOTTOM_HORIZONTAL_LINE 0, 210, 315, 210

/*
* Right arrow coordinates
*/
#define RIGHT_ARROW 280, 222

/*
* Global variables
*/
UTFT display; //Display class
uint8_t step = 0; //Current answer streak
uint8_t tSS = 0; //Time
uint8_t tS = EXPIRATION_TIME;

/*
* Question object and constructor
*/
struct objectQuestion {
	char * question;
	char * a;
	char * b;
	char * c;
	char * d;
	uint8_t correctAns;

	objectQuestion(char * question, char * a, char * b, char * c, char * d, uint8_t correctAns): question(question), a(a), b(b), c(c), d(d), correctAns(correctAns) {}
};

/*
* All questions in game
*/

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

/*
* Scores
*/
const char score[10][6] = {
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

/*
* Displays text on whole screen when the subject answers the question
*/

void displayCorrect() {
	display.clrScr();
	display.setFont(BigFont);
	display.fillScr(BLUE);
	display.setBackColor(BLUE);
	if (step == 5) {
		display.printWithMargin("OSIGURALI STE", SECURED);
		display.printWithMargin("16k KN", MESSAGE_16K);
		_delay_ms(2000);
		} else if (step == 10) {
		display.print("MILIJUNAS", MILLIONARE);
		step = 0;
		_delay_ms(10000);
		} else if (step == 0) {
		display.print("NETOCNO", CORRECT);
	} else display.print("TOCNO", INCORRECT);
	display.setFont(SmallFont);
	_delay_ms(1000);
	display.clrScr();
}

/*
* Displays highlighter around the score depending on the question
*/

void drawScoreHighlight() {

	display.setColor(YELLOW);
	if (step >= 6) display.fillRoundRect(5 + 50 * (step % 5), 235, 52 + 50 * (step % 5), 218);
	display.fillRoundRect(5 + 50 * (step % 5), 235, 40 + 50 * (step % 5), 218);
	display.setColor(BLUE);
}

/*
* Displays GUI elements
* Also used for displaying 50/50
*/

void displayGUI(uint8_t jokerFifty = 0) {
	display.drawLine(TOP_HORIZONTAL_LINE); // Top horizontal line
	display.drawLine(MIDDLE_VERTICAL_LINE); // Middle vertical line
	display.drawLine(MIDDLE_HORIZONTAL_LINE); // Middle horizontal line line
	display.drawLine(BOTTOM_HORIZONTAL_LINE); // Bottom horizontal line

	display.setColor(YELLOW);
	display.setFont(BigFont);
	if (jokerFifty) {
		if (1 == questionsEasy[step].correctAns) {
			display.print("A:", A_COORDINATE);
			display.print("D:", D_COORDINATE);
			} else if (2 == questionsEasy[step].correctAns) {
			display.print("C:", C_COORDINATE);
			display.print("D:", D_COORDINATE);
			} else if (3 == questionsEasy[step].correctAns) {
			display.print("B:", B_COORDINATE);
			display.print("A:", A_COORDINATE);
			} else {
			display.print("B:", B_COORDINATE);
			display.print("D:", D_COORDINATE);
		}
		} else {
		display.print("A:", A_COORDINATE);
		display.print("B:", B_COORDINATE);
		display.print("C:", C_COORDINATE);
		display.print("D:", D_COORDINATE);
	}

	display.setColor(WHITE);
	display.setFont(SmallFont);
}

/*
* Displays question and option text
* Also used for displaying 50/50
*/

void displayQuestion(objectQuestion questionObj, uint8_t * jokerFiftyUsed = 0, uint8_t jokerFifty = 0) {
	display.printWithMargin(questionObj.question, 2, 0);
	if (jokerFifty) {
		( * jokerFiftyUsed) = 1;
		if (1 == questionObj.correctAns) {
			display.printWithMargin(questionObj.a, A_OPTION_COORDINATE);
			display.printWithMargin(questionObj.d, D_OPTION_COORDINATE);
			} else if (2 == questionObj.correctAns) {
			display.printWithMargin(questionObj.b, B_OPTION_COORDINATE);
			display.printWithMargin(questionObj.c, C_OPTION_COORDINATE);
			} else if (3 == questionObj.correctAns) {
			display.printWithMargin(questionObj.c, C_OPTION_COORDINATE);
			display.printWithMargin(questionObj.a, A_OPTION_COORDINATE);
			} else {
			display.printWithMargin(questionObj.b, B_OPTION_COORDINATE);
			display.printWithMargin(questionObj.d, D_OPTION_COORDINATE);

		}
		} else {
		display.printWithMargin(questionObj.a, A_OPTION_COORDINATE);
		display.printWithMargin(questionObj.b, B_OPTION_COORDINATE);
		display.printWithMargin(questionObj.c, C_OPTION_COORDINATE);
		display.printWithMargin(questionObj.d, D_OPTION_COORDINATE);
	}
}

/*
* Displays bottom score bar
*/

void displayScore() {

	if (step < 5) {
		for (uint8_t i = 0; i < 5; i++) {
			display.print(score[i], 10 + (i * 50), 221);
			display.print("->", RIGHT_ARROW);
		}
		} else if (step >= 5 && step < 10) {
		for (uint8_t i = 5; i < 10; i++) {
			display.print(score[i], 10 + (i - 5) * 50, 221);
		}
	}
}

/*
* Used for checking if the selected answer was correct,
* Resets time and displays the right title screen
*/

void checkAnswer(uint8_t * ans, objectQuestion questionsObj, uint8_t * jokerFiftyUsed) {

	if (( * ans) == questionsObj.correctAns) {
		step++;
		displayCorrect();
		display.fillScr(BLUE);
		displayGUI();
		displayQuestion(questionsEasy[step]);
		drawScoreHighlight();
		* ans = 0;
		tS = EXPIRATION_TIME;
		} else {
		* jokerFiftyUsed = 0;
		step = 0;
		displayCorrect();
		display.fillScr(BLUE);
		displayGUI();
		displayQuestion(questionsEasy[step]);
		drawScoreHighlight();
		* ans = 0;
		tS = EXPIRATION_TIME;
	};
}

/*
* Timer0 used to limit answer time
*/

ISR(TIMER0_COMP_vect) {
	tSS++;

	if (tSS >= EXPIRATION_TIME) {
		tSS = 0;
		tS--;

		if (tS < 0 ) tS = EXPIRATION_TIME;
	}

}

int main() {
	//SET PORTS
	PORTB = 0xff;
	DDRB = 0x00;

	//VARIABLES
	uint8_t ans = 0;
	uint8_t jokerFiftyUsed = 0;

	//LCD INIT
	display.clrScr();
	display.InitLCD(LANDSCAPE);

	//GUI INIT
	display.setFont(SmallFont);
	display.fillScr(BLUE);
	display.setBackColor(BLUE);
	displayQuestion(questionsEasy[step]);
	drawScoreHighlight();

	//TIMER0 INIT
	TCCR0 = _BV(WGM01) | _BV(CS02) | _BV(CS00);
	OCR0 = 255;
	TIMSK = _BV(OCIE0);
	sei();
	
	//MAIN WHILE
	while (1) {
		displayGUI();
		displayScore();
		
		//WAIT FOR ANSWER OR TIMER EXPIRES
		while (!bit_is_clear(PINB, 0) && !bit_is_clear(PINB, 1) && !bit_is_clear(PINB, 2) && !bit_is_clear(PINB, 3) && !bit_is_clear(PINB, 4)) {
			display.printNumI(tS, 140, 45, 3);
			if (tS <= 0) {
				ans = 5;
				checkAnswer( & ans, questionsEasy[step], & jokerFiftyUsed);
				break;
			}
			displayGUI();
		}
		//CHECK ANSWER
		if (bit_is_clear(PINB, 0)) {
			ans = 1;
			_delay_ms(DEBOUNCE);
			checkAnswer( & ans, questionsEasy[step], & jokerFiftyUsed);
			} else if (bit_is_clear(PINB, 1)) {
			ans = 2;
			_delay_ms(DEBOUNCE);
			checkAnswer( & ans, questionsEasy[step], & jokerFiftyUsed);
			} else if (bit_is_clear(PINB, 2)) {
			ans = 3;
			_delay_ms(DEBOUNCE);
			checkAnswer( & ans, questionsEasy[step], & jokerFiftyUsed);
			} else if (bit_is_clear(PINB, 3)) {
			ans = 4;
			_delay_ms(DEBOUNCE);
			checkAnswer( & ans, questionsEasy[step], & jokerFiftyUsed);
			} else if (bit_is_clear(PINB, 4) && !jokerFiftyUsed) {

			display.clrScr();
			display.fillScr(BLUE);
			drawScoreHighlight();
			displayGUI(1);
			displayQuestion(questionsEasy[step], & jokerFiftyUsed, 1);
		}
	}
}