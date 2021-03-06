/*
Long integers.
Write a program capable of operating on big integers (at least 50 decimal digits) with full precision.
At least four basic operations (+, -, *, /) should be implemented.
Details – to be discussed.
*/

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <string>
#include <cassert>

using namespace std;

// AMOUNT OF DIGITS YOU WANT THE PROGRAM TO OPERATE ON (DON'T INCLUDE MINUS HERE, IT'S ALREADY IN)
#define SET_AMOUNT_OF_DIGITS_HERE 6


#define DIGITS_CAPABLE SET_AMOUNT_OF_DIGITS_HERE+1
#define DIGITS_CAPABLE_SECURE DIGITS_CAPABLE -1 // Safety purposes
#define DIGITS DIGITS_CAPABLE+1			// DON'T CHANGE IT (+3 TO INCLUDE MINUS SIGN AND PREVENT PROBLEMS)
#define UNUSED 40						// USED TO MARK UNUSED NUMBERS
#define DEBUG 0							// DEBUGGING MODE -- 1 //			// NORMAL MODE -- 0 //
#define DIRECT_DISPLAY 0				// EMPTY DIGITS REPRESENTATION:     1: -1-1-1-1-1..    0:           ...

#define ADD 1
#define SUBTRACT 2
#define PLUS 1
#define MINUS 0
#define GET_BIGGER 0
#define GET_SMALLER 1

struct BigInt {
	bool hasExceeded = false;
	int value[DIGITS];
	bool sign = true;

private:
	bool invert(bool ex) {
		return !ex;
	}
	bool isEqual(BigInt num1, BigInt num2) {
		if (num1.sign != num2.sign) return false;
		bool equal = true;

		for (int i = DIGITS; i >= 0; i--) {
			if (num1.value[i] != num2.value[i]) return false;
		}

		return true;
	}
	BigInt compare(BigInt num1, BigInt num2, int mode) {
		// MODE: 0 -- returns greater number
		// MODE: 1 -- returns smaller number


		// EQUALITY CHECK
		bool equal = true;
		if (num1.sign != num2.sign) equal = false;
		else if (num1.length() != num2.length()) equal = false;
		else {
			for (int i = 0; i < DIGITS; i++) {
				if (num1.value[i] != num2.value[i]) equal = false;
			}
			if (equal) return num1;
		}

		// NOT EQUAL - NOW CHECK WHICH IS GREATER
		int len1 = num1.length(),
			len2 = num2.length();

		if (len1 == len2 && num1.sign == num2.sign) {
			// SAME LENGTH, SAME SIGNS
			if (num1.value[DIGITS - len1] > num2.value[DIGITS - len2]) {
				if (num1.sign) {
					if (mode == 0) return num1;
					else return num2;
				}
				else {
					if (mode == 0) return num2;
					else return num1;
				}

			}
		}
		else if (len1 == len2 && num1.sign != num2.sign) {
			// SAME LENGTH, DIFFERENT SIGNS
			if (num1.sign) {
				if (mode == 0) return num1;
				else return num2;
			}
			else {
				if (mode == 0) return num2;
				else return num1;
			}
		}
		else if (len1 != len2 && num1.sign == num2.sign) {
			// DIFFERENT LENGTH, SAME SIGNS
			if (len1 > len2) {
				if (num1.sign) {
					if (mode == 0) return num1;
					else return num2;
				}
				else {
					if (mode == 0) return num2;
					else return num1;
				}
			}
			else {
				if (num1.sign) {
					if (mode == 0) return num2;
					else return num1;
				}
				else {
					if (mode == 0) return num1;
					else return num2;
				}
			}
		}
		else {
			// DIFFERENT LENGTH, DIFFERENT SIGNS
			if (num1.sign) {
				if (mode == 0) return num1;
				else return num2;
			}
			else {
				if (mode == 0) return num2;
				else return num1;
			}
		}

	}

public:
	BigInt() {
		hasExceeded = false;
		sign = true;
	}
	bool &operator!() {
		static bool not_sign = invert(sign);
		return not_sign;
	}
	bool operator== (BigInt n2) {
		if (isEqual(*this, n2)) return true;
		else return false;
	}
	bool operator> (BigInt n2) {
		if (isEqual(*this, n2)) return false;
		if (isEqual(*this, compare(*this, n2, GET_BIGGER))) return true;
		else return false;
	}
	bool operator< (BigInt n2) {
		if (isEqual(*this, n2)) return false;
		if (isEqual(*this, compare(*this, n2, GET_SMALLER))) return true;
		else return false;
	}
	void negate() {
		sign = !sign;
	}
	int length() {
		int len = 0;
		for (int i = 0; i < DIGITS; i++) if (value[i] != UNUSED && value[i] != '-') len++;
		return len;
	}
	BigInt abs() { // needs to be tested
		BigInt OUT = *this;
		OUT.sign = true;
		return OUT;
	}
	void set0() {
		for (int i = 0; i < DIGITS - 1; i++) value[i] = UNUSED;
		value[DIGITS - 1] = 0;
	}
	void set1() {
		for (int i = 0; i < DIGITS - 1; i++) value[i] = UNUSED;
		value[DIGITS - 1] = 1;
	}
};

// FUNCTIONS - GUI
void clearScreen();
void clearScreen(int n);
int menu();
void takeNumbers(int mode);
void addition(BigInt num1, BigInt num2);
void subtraction(BigInt num1, BigInt num2);
void multiplication(BigInt num1, BigInt num2);
void division(BigInt num1, BigInt num2);
void exitMessage();

// FUNCTIONS - CALCULATIONS
BigInt add(BigInt num1, BigInt num2, bool s);			// MATHEMATICAL OPERATION:    ADDITION
BigInt subtract(BigInt num1, BigInt num2, bool s);		// MATHEMATICAL OPERATION:    SUBTRACTION
BigInt multiply(BigInt num1, BigInt num2);				// MATHEMATICAL OPERATION:    MULTIPLY
BigInt times10(BigInt num);								// MULTIPLIES BY 10
void times10(BigInt *num);
BigInt divide(BigInt num1, BigInt num2);				// MATHEMATICAL OPERATION:	  DVISION
void divide10(BigInt *num);								// DIVIDES BY 10
BigInt divide10(BigInt num);
BigInt compare(BigInt num1, BigInt num2, int mode);		// RETURNS THE GREATER NUMBER
BigInt chooseAddSub(BigInt num1, BigInt num2, int mode);// CHOOSES BETWEEN ADDITION AND SUBTRACTION
bool isEqual(BigInt num1, BigInt num2);					// RETURNS TRUE IF NUMBERS ARE EQUAL
void initializeNumber(BigInt *num);						// FUNCTION INITIALIZES CERTAIN ARRAYS IN STRUCTURE WITH (UNUSED)
bool setNumber(BigInt *num, string value);				// FUNCTION SETS A NUMBER TO A GIVEN VALUE
void setNumber(BigInt *num, BigInt *neue);
void printNumber(BigInt num);							// FUNCTIONS PRINTS A NUMBER DIGIT-BY-DIGIT
void printModulo();				// PRINTS THE REMINDER OF DIVISION num1/num2

BigInt globalReminder; // I know global variables are not recommended

#define DEBUG_NUMBERS_AMOUNT 20
BigInt numbers[DEBUG_NUMBERS_AMOUNT];

#if !DEBUG
int main()
{
	// OPTION: 1-add, 2-subtract, 3-multiply, 4-divide
	int option = -1;
	while (option != 0) {
		option = menu();
		switch (option) {
		case 1:
			takeNumbers(option);
			break;
		case 2:
			takeNumbers(option);
			break;
		case 3:
			takeNumbers(option);
			break;
		case 4:
			takeNumbers(option);
			break;
		case 0:
			exitMessage();
			exit(0);
		}
	}
	return 0;
}
#endif

// FUNCTIONS RELATED TO THE GUI
void clearScreen() {
	for (int i = 0; i < 100; i++) cout << "\n";
}
void clearScreen(int n) {
	for (int i = 0; i < n; i++) cout << "\n";
}

int menu() {
	clearScreen();

	cout << "\t\t __        ______   __    __   ______         ______  __    __  ________   ______  " << endl;
	cout << "\t\t|  \\      /      \\ |  \\  |  \\ /      \\       |      \\|  \\  |  \\|        \\ /      \\ " << endl;
	cout << "\t\t| $$     |  $$$$$$\\| $$\\ | $$|  $$$$$$\\       \\$$$$$$| $$\\ | $$ \\$$$$$$$$|  $$$$$$\\" << endl;
	cout << "\t\t| $$     | $$  | $$| $$$\\| $$| $$ __\\$$        | $$  | $$$\\| $$   | $$   | $$___\\$$" << endl;
	cout << "\t\t| $$     | $$  | $$| $$$$\\ $$| $$|    \\        | $$  | $$$$\\ $$   | $$    \\$$    \\ " << endl;
	cout << "\t\t| $$     | $$  | $$| $$\\$$ $$| $$ \\$$$$        | $$  | $$\\$$ $$   | $$    _\\$$$$$$\\" << endl;
	cout << "\t\t| $$_____| $$__/ $$| $$ \\$$$$| $$__| $$       _| $$_ | $$ \\$$$$   | $$   |  \\__| $$" << endl;
	cout << "\t\t| $$     \\\\$$    $$| $$  \\$$$ \\$$    $$      |   $$ \\| $$  \\$$$   | $$    \\$$    $$" << endl;
	cout << "\t\t \\$$$$$$$$ \\$$$$$$  \\$$   \\$$  \\$$$$$$        \\$$$$$$ \\$$   \\$$    \\$$     \\$$$$$$ " << endl;
	cout << "\n\t\t\tCP Assignment 2017/2018 \t\t\t\t Tomasz Gorol" << endl;
	cout << "\n\n";
	cout << "\t\t\t  1. Addition" << endl;
	cout << "\t\t\t  2. Subtraction" << endl;
	cout << "\t\t\t  3. Multiplication" << endl;
	cout << "\t\t\t  4. Division" << endl << endl;
	cout << "\t\t\t  0. Exit" << endl;
	clearScreen(7);

	char choice = -1;
	do {
		choice = _getch();
	} while (!(choice >= '0' && choice <= '4'));

	int choiceToInt = choice - '0';
	return choiceToInt;
}

void takeNumbers(int mode) {
AGAIN:
	clearScreen();
	cout << "\t\t\t\t  |''||''| '||\\   ||` '||'''|, '||   ||` |''||''|" << endl;
	cout << "\t\t\t\t     ||     ||\\\\  ||   ||   ||  ||   ||     ||   " << endl;
	cout << "\t\t\t\t     ||     || \\\\ ||   ||...|'  ||   ||     ||   " << endl;
	cout << "\t\t\t\t     ||     ||  \\\\||   ||       ||   ||     ||   " << endl;
	cout << "\t\t\t\t  |..||..| .||   \\||. .||       `|...|'    .||.  " << endl;
	clearScreen(3);
	cout << "\t\t\t\t       Enter two numbers. Max length: [ " << DIGITS_CAPABLE_SECURE << " ]" << endl;
	cout << "\t\t\t\t    Put a minus in front to negate the number." << endl;
	cout << "\t\t\t\t Operations are performed in the respective order." << endl;
	clearScreen(4);

	BigInt in1, in2;
	initializeNumber(&in1); initializeNumber(&in2);
	string str1 = "", str2 = "";


	cout << "\t\tnumber 1: ";
	cin >> str1;
	cout << "\t\tnumber 2: ";
	cin >> str2;

	if (!setNumber(&in1, str1) || !setNumber(&in2, str2)) {
		cout << "\t\t\t\t\t Try once again -- press any key.";
		_getch();
		goto AGAIN;
	}
	else {
		switch (mode) {
		case 1:
			addition(in1, in2);
			break;
		case 2:
			subtraction(in1, in2);
			break;
		case 3:
			multiplication(in1, in2);
			break;
		case 4:
			division(in1, in2);
			break;
		}
	}
}
void addition(BigInt num1, BigInt num2) {
	clearScreen();
	cout << "\t\t\t\t      ,adPPYba,  88       88  88,dPYba,,adPYba,   " << endl;
	cout << "\t\t\t\t      I8[    \"\"  88       88  88P'   \"88\"    \"8a  " << endl;
	cout << "\t\t\t\t       `\"Y8ba,   88       88  88      88      88  " << endl;
	cout << "\t\t\t\t      aa    ]8I  \"8a, ,  a88  88      88      88  " << endl;
	cout << "\t\t\t\t      `\"YbbdP\"\'   `\"YbbdP\'Y8  88      88      88  " << endl;
	clearScreen(8);

	cout << "\t\t   number 1:     "; printNumber(num1); cout << endl;
	cout << "\t\t   number 2:     "; printNumber(num2); cout << endl;
	cout << "\t\t------    +    -----------------------------------------------------------    +    ------" << endl;
	cout << "\t\t                 "; printNumber(chooseAddSub(num1, num2, ADD));
	clearScreen(5);
	_getch();
}
void subtraction(BigInt num1, BigInt num2) {
	clearScreen();
	cout << "\t\t\t          88     ad88     ad88                                       " << endl;
	cout << "\t\t\t          88    d8\"      d8\"                                         " << endl;
	cout << "\t\t\t          88    88       88                                          " << endl;
	cout << "\t\t\t  ,adPPYb,88  MM88MMM  MM88MMM  8b,dPPYba,  8b,dPPYba,    ,adPPYba,  " << endl;
	cout << "\t\t\t a8\"    `Y88    88       88     88P\'   \"Y8  88P\'   `\"8a  a8\"     \"\"  " << endl;
	cout << "\t\t\t 8b       88    88       88     88          88       88  8b          " << endl;
	cout << "\t\t\t \"8a,   ,d88    88       88     88          88       88  \"8a,   ,aa  " << endl;
	cout << "\t\t\t  `\"8bbdP\"Y8    88       88     88          88       88   `\"Ybbd8\"\'  " << endl;
	clearScreen(8);

	cout << "\t\t   number 1:     "; printNumber(num1); cout << endl;
	cout << "\t\t   number 2:     "; printNumber(num2); cout << endl;
	cout << "\t\t------    -    -----------------------------------------------------------    -    ------" << endl;
	cout << "\t\t                 "; printNumber(chooseAddSub(num1, num2, SUBTRACT));
	clearScreen(5);
	_getch();
}
void multiplication(BigInt num1, BigInt num2) {
	clearScreen();
	cout << "\t\t                                                88                                    " << endl;
	cout << "\t\t                                                88                             ,d     " << endl;
	cout << "\t\t                                                88                             88     " << endl;
	cout << "\t\t 8b,dPPYba,   8b,dPPYba,   ,adPPYba,    ,adPPYb,88  88       88   ,adPPYba,  MM88MMM  " << endl;
	cout << "\t\t 88P\'    \"8a  88P\'   \"Y8  a8\"     \"8a  a8\"    `Y88  88       88  a8\"     \"\"    88     " << endl;
	cout << "\t\t 88       d8  88          8b       d8  8b       88  88       88  8b            88     " << endl;
	cout << "\t\t 88b,   ,a8\"  88          \"8a,   ,a8\"  \"8a,   ,d88   \"8a,  ,a88  \"8a,   ,aa    88,    " << endl;
	cout << "\t\t 88`YbbdP\"\'   88           `\"YbbdP\"\'    `\"8bbdP\"Y8   `\"YbbdP\'Y8   `\"Ybbd8\"\'    \"Y888  " << endl;
	cout << "\t\t 88                                                                                   " << endl;
	cout << "\t\t 88                                                                                   " << endl;
	clearScreen(8);

	cout << "\t\t   number 1:     "; printNumber(num1); cout << endl;
	cout << "\t\t   number 2:     "; printNumber(num2); cout << endl;
	cout << "\t\t------    *    -----------------------------------------------------------    *    ------" << endl;
	cout << "\t\t                 "; printNumber(multiply(num1, num2));
	clearScreen(5);
	_getch();
}
void division(BigInt num1, BigInt num2) {
	clearScreen();
	cout << "\t\t                                                 88                                   " << endl;
	cout << "\t\t                                          ,d     \"\"                            ,d     " << endl;
	cout << "\t\t                                          88                                   88     " << endl;
	cout << "\t\t   ,adPPYb,d8  88       88   ,adPPYba,  MM88MMM  88   ,adPPYba,  8b,dPPYba,  MM88MMM  " << endl;
	cout << "\t\t  a8\"    `Y88  88       88  a8\"     \"8a   88     88  a8P_____88  88P\'   `\"8a   88     " << endl;
	cout << "\t\t  8b       88  88       88  8b       d8   88     88  8PP\"\"\"\"\"\"\"  88       88   88     " << endl;
	cout << "\t\t  \"8a    ,d88  \"8a,   ,a88  \"8a,   ,a8\"   88,    88  \"8b,   ,aa  88       88   88,    " << endl;
	cout << "\t\t   `\"YbbdP\'88   `\"YbbdP\'Y8   `\"YbbdP\"\'    \"Y888  88   `\"Ybbd8\"\'  88       88   \"Y888  " << endl;
	cout << "\t\t           88                                                                         " << endl;
	cout << "\t\t           88                                                                         " << endl;
	clearScreen(8);

	cout << "\t\t   number 1:     "; printNumber(num1); cout << endl;
	cout << "\t\t   number 2:     "; printNumber(num2); cout << endl;
	cout << "\t\t------    /    -----------------------------------------------------------    /    ------" << endl;
	cout << "\t\t                 "; printNumber(divide(num1, num2));
	cout << "\t\t                 "; printModulo();
	clearScreen(5);
	_getch();
}
void exitMessage() {
	clearScreen();
	cout << "\t\t\t\t     ____                  ____             " << endl;
	cout << "\t\t\t\t    |  _ \\                |  _ \\            " << endl;
	cout << "\t\t\t\t    | |_) |_   _  ___     | |_) |_   _  ___ " << endl;
	cout << "\t\t\t\t    |  _ <| | | |/ _ \\    |  _ <| | | |/ _ \\" << endl;
	cout << "\t\t\t\t    | |_) | |_| |  __/    | |_) | |_| |  __/" << endl;
	cout << "\t\t\t\t    |____/ \\__, |\\___|    |____/ \\__, |\\___|" << endl;
	cout << "\t\t\t\t            __/ |                 __/ |     " << endl;
	cout << "\t\t\t\t           |___/                 |___/      " << endl;
	clearScreen(11);
	cout << "\t\t\t\t\t";
}

// FUNCTIONS RELATED TO THE CALCULATIONS
void initializeNumber(BigInt *num) {
	for (int i = DIGITS - 1; i >= 0; i--) {
		num->value[i] = UNUSED;
	}
	num->hasExceeded = false;
}
bool setNumber(BigInt *num, string new_value) {
	int newLen = new_value.length();
	for (int g = 0; g < newLen; g++) {
		if (new_value[g] == '-') newLen--;
	}
	if (newLen > DIGITS_CAPABLE_SECURE) { // To prevent problems
		cout << "\n\t\t\t\t       Too many digits.  Number was not set." << endl;
		return false;
	}
	assert(newLen >= 0);
	for (int s = 0; s < new_value.length(); s++) {
		if ((new_value[s] < '0' || new_value[s] > '9') && new_value[s] != '-') {
			cout << "\n\t\t\t\t  Illegal characters found. Number was not set." << endl;
			return false;
		}
	}



	num->sign = true;
	int len = new_value.length(); // Amount of digits to be set
	for (int i = 0; i < len; i++) if (new_value[i] == '-') num->sign = false;

	int j;

	for (j = 0; j <= DIGITS - len - 1; j++) {
		num->value[j] = UNUSED;
	}

	for (int i = j; i < DIGITS; i++) {
		if (new_value[i - (DIGITS - len)] == '-') {
			num->value[i] = UNUSED;
			continue;
		}
		num->value[i] = (int)(new_value[i - (DIGITS - len)] - '0');
	}
	return true;
}
void setNumber(BigInt *num, BigInt *neue) {
	num = neue;
}
void printNumber(BigInt num) {
	if (num.hasExceeded) {
		cout << "EXCEEDED. Change settings to fit that many digits.";
		return;
	}
	for (int i = 0; i < DIGITS; i++) {
#if DIRECT_DISPLAY
		if (num.sign == false && i == DIGITS - num.length()) {
			cout << "-";
		}
		cout << num.value[i];
#else
		if (num.sign == false && i == DIGITS - num.length() - 1) {
			cout << "-";
		}
		else if (num.value[i] == UNUSED) cout << " ";
		else cout << num.value[i];
#endif
	}
}
void printModulo() {
	BigInt init; initializeNumber(&init);
	if (globalReminder == init) cout << "\n\t\t\t  R:     0";
	else cout << "\n\t\t\t  R:     ";  printNumber(globalReminder);
}
bool isEqual(BigInt num1, BigInt num2) {
	if (num1.sign != num2.sign) return false;
	bool equal = true;

	for (int i = DIGITS; i >= 0; i--) {
		if (num1.value[i] != num2.value[i]) return false;
	}

	return true;
}
BigInt add(BigInt num1, BigInt num2, bool s) {
	BigInt OUT; initializeNumber(&OUT);								// STORES THE FINAL RESULT
	OUT.sign = s;
	int carry[DIGITS]; for (int i = 0; i < DIGITS; i++) carry[i] = 0;		// STORES THE CARRY FROM PREVIOUS COLUMN - carry[5] = carry from 6
	int sum[DIGITS]; for (int i = 0; i < DIGITS; i++) sum[i] = 0;			// STORES SUMS OF DIGITS
	bool wasTheLast = false;
	for (int i = DIGITS - 1; i >= 0; i--) {
		if (num1.value[i] == UNUSED && num2.value[i] == UNUSED) {
			if (!wasTheLast && (carry[i] >= 1 && carry[i] <= 10)) {
				OUT.value[i] = carry[i];
				wasTheLast = true;
			}
			break;
		}
		else if (num2.value[i] == UNUSED) {
			sum[i] = num1.value[i] + carry[i];
			if (i != 0) carry[i - 1] = (int)(sum[i] / 10);
			OUT.value[i] = sum[i] % 10;
			continue;
		}
		else if (num1.value[i] == UNUSED) {
			sum[i] = num2.value[i] + carry[i];
			if (i != 0) carry[i - 1] = (int)(sum[i] / 10);
			OUT.value[i] = sum[i] % 10;
			continue;
		}

		sum[i] = num1.value[i] + num2.value[i] + carry[i];
		if (i != 0) carry[i - 1] = (int)(sum[i] / 10);
		OUT.value[i] = sum[i] % 10;
	}
	if (OUT.length() > DIGITS_CAPABLE) OUT.hasExceeded = true;
	return OUT;
}
BigInt subtract(BigInt num1, BigInt num2, bool s) {
	BigInt OUT; initializeNumber(&OUT);
	OUT.sign = s;

	/* Dopisanie 000 od rzędu pierwszej cyfry liczby dłuższej, aż do pierwszej cyfry krótszej, przykład:
	789824
	000013 */
	if (num1.length() > num2.length()) {
		for (int h = DIGITS - num1.length(); h <= DIGITS - num2.length(); h++) {
			num2.value[h] = 0;
		}
	}

	for (int i = DIGITS - 1; i >= 0; i--) {
		if (num1.value[i] == UNUSED && num2.value[i] == UNUSED) break;
		else if (num2.value[i] == UNUSED) {
			OUT.value[i] = num1.value[i];
			continue;
		}

		if (num1.value[i] >= num2.value[i]) {
			OUT.value[i] = num1.value[i] - num2.value[i];
			if (num1.value[i] == num2.value[i] && (num1.value[i - 1] == UNUSED && num2.value[i - 1] == UNUSED)) OUT.value[i] = UNUSED;
		}

		else {
			// Borrowing from the column on the left
			num1.value[i] += 10; num1.value[i - 1]--;

			OUT.value[i] = num1.value[i] - num2.value[i];
			if (num1.value[i - 1] == 0 && num1.value[i - 2] == UNUSED) num1.value[i - 1] = UNUSED;
		}
	}
	if (OUT.length() > DIGITS_CAPABLE) OUT.hasExceeded = true;
	return OUT;
}
BigInt multiply(BigInt num1, BigInt num2) {
	BigInt sumSoFar; setNumber(&sumSoFar, "0");
	if (DIGITS_CAPABLE < num1.length() + num2.length()) {
		sumSoFar.hasExceeded = true;
		return sumSoFar;
	}

	// n1 - longer, n2 - shorter
	BigInt n1, n2;
	if (num2.length() > num1.length()) {
		n1 = num2;
		n2 = num1;
	}
	else {
		n1 = num1;
		n2 = num2;
	}

	for (int i = DIGITS - 1; i >= DIGITS - n2.length(); i--) {
		for (int j = n2.value[i]; j > 0; j--) {
			sumSoFar = add(sumSoFar, n1, PLUS);
		}
		n1 = times10(n1);
	}
	if ((!num1.sign && num2.sign) || (num1.sign && !num2.sign)) sumSoFar.sign = false;
	else sumSoFar.sign = true;

	return sumSoFar;
}
BigInt times10(BigInt num) {
	// Just a sanity check
	// This function will be called only through multiply(...), and multiply(...) checks if multiplying may occur
	if (DIGITS_CAPABLE < num.length() + 1) return num;

	BigInt num_e; initializeNumber(&num_e);
	num_e.sign = num.sign;
	for (int i = DIGITS - 1; i >= DIGITS - num.length(); i--) {
		num_e.value[i - 1] = num.value[i];
	}
	num_e.value[DIGITS - 1] = 0;
	return num_e;
}
void times10(BigInt *num) {
	// Just a sanity check
	// This function will be called only through multiply(...), and multiply(...) checks if multiplying may occur
	if (DIGITS_CAPABLE < num->length() + 1) return;
	BigInt num_e; initializeNumber(&num_e);
	num_e.sign = num->sign;
	for (int i = DIGITS - 1; i >= DIGITS - num->length(); i--) {
		num_e.value[i - 1] = num->value[i];
	}
	num_e.value[DIGITS - 1] = 0;
	*num = num_e;
}
BigInt divide(BigInt n1, BigInt n2) {
	BigInt divSoFar; initializeNumber(&divSoFar);
	BigInt reminder; initializeNumber(&reminder);
	BigInt multiplier; multiplier.set1();
	bool upSign = true;
	if ((!n1.sign && n2.sign) || (n1.sign && !n2.sign)) upSign = false;
	if (n1 == compare(n1, n2, GET_SMALLER)) { //n1 < n2 -- unable to divide, 0 R n2
		reminder = n2;
		n1.set0();
		n1.sign = true;
		setNumber(&globalReminder, "0");
		globalReminder = reminder;
		return n1;
	}

	n1.sign = true;
	n2.sign = true;
	const BigInt diver = n2;
	bool whileBreak1 = true; // false - while won't run
	bool whileBreak2 = true; // false - while won't run
	bool dontTime10 = false;
	bool wasTimed10 = false;
	int timed10 = 0;
	while (whileBreak1) { // while *1*
		if (times10(n2) == compare(n1, times10(n2), GET_SMALLER) && !dontTime10) { // times(10) < n1
			times10(&n2);
			times10(&multiplier);
			wasTimed10 = true;
			timed10++;
		}
		else {														// times(10) >= n1
			while (1) { // while *2*
				if ((n1 == compare(n1, n2, GET_BIGGER) || isEqual(n1, n2))) {
					n1 = subtract(n1, n2, PLUS);
					divSoFar = add(divSoFar, multiplier, PLUS); // TODO: 71 - 70 =  01 POWAŻNY BŁĄD
				}
				else {
					// n1 < n2
					if (wasTimed10 && (divide10(n2) == compare(n1, divide10(n2), GET_SMALLER) && timed10 != 0)) { // div10 < n1
																												  // If dividing by 10 will help
						dontTime10 = true;
						divide10(&n2);
						divide10(&multiplier);
						timed10--;
						continue;
					}
					else {
						// n1 < divider
						reminder = n1;
						multiplier.set1();
						n2 = diver;
						break;
					}

				}
			}
			whileBreak1 = false;
		}
	}

	for (int r = 0; r < DIGITS; r++) {
		// Filling not used digits of reminder with UNUSED
		if (reminder.value[r] >= 1 && reminder.value[r] <= 9) break;
		if (reminder.value[r] == 0) reminder.value[r] = UNUSED;
	}
	divSoFar.sign = upSign;

	setNumber(&globalReminder, "0");

	globalReminder = reminder;
	return divSoFar;
}
void divide10(BigInt *num) {
	BigInt temp; initializeNumber(&temp);
	for (int i = DIGITS - 2; i >= 0; i--) {
		temp.value[i + 1] = num->value[i];
	}
	*num = temp;
}
BigInt divide10(BigInt num) {
	BigInt temp; initializeNumber(&temp);
	for (int i = DIGITS - 2; i >= 0; i--) {
		temp.value[i + 1] = num.value[i];
	}
	return temp;
}
BigInt compare(BigInt num1, BigInt num2, int mode) {
	// MODE: 0 -- returns greater number
	// MODE: 1 -- returns smaller number
	BigInt OUT; initializeNumber(&OUT);

	// EQUALITY CHECK
	bool equal = true;
	if (num1.sign != num2.sign) equal = false;
	else if (num1.length() != num2.length()) equal = false;
	else {
		for (int i = 0; i < DIGITS; i++) {
			if (num1.value[i] != num2.value[i]) equal = false;
			if (!equal) break;
		}
		if (equal) {
			OUT = num1;
			return OUT;
		}
	}

	// NOT EQUAL - NOW CHECK WHICH IS GREATER
	int len1 = num1.length(),
		len2 = num2.length();

	if (len1 == len2 && num1.sign == num2.sign) {
		// SAME LENGTH, SAME SIGNS (but for sure different)
		int len = num1.length();

		for (int p = 0; p < DIGITS - 1; p++) {
			if (num1.value[DIGITS - len + p] > num2.value[DIGITS - len + p]) {
				if (mode == GET_BIGGER) {
					OUT = num1;
					break;
				}
				else {
					OUT = num2;
					break;
				}
			}

			else if (num1.value[DIGITS - len + p] < num2.value[DIGITS - len + p]) {
				if (mode == GET_BIGGER) {
					OUT = num2;
					break;
				}
				else {
					OUT = num1;
					break;
				}
			}
		}
	}
	else if (len1 == len2 && num1.sign != num2.sign) {
		// SAME LENGTH, DIFFERENT SIGNS
		if (num1.sign) {
			if (mode == 0) OUT = num1;
			else OUT = num2;
		}
		else {
			if (mode == 0) OUT = num2;
			else OUT = num1;
		}
	}
	else if (len1 != len2 && num1.sign == num2.sign) {
		// DIFFERENT LENGTH, SAME SIGNS
		if (len1 > len2) {
			if (num1.sign) {
				// DODANIE LICZBY, len1 > len2
				if (mode == 0) OUT = num1;	// GET BIGGER
				else OUT = num2;			// GET SMALLER
			}
			else {
				// UJEMNE LICZBY, len1 > len2
				if (mode == 0) OUT = num2;
				else OUT = num1;
			}
		}
		else {
			// DODANIE LICZBY, len1 < len2
			if (num1.sign) {
				if (mode == 0) OUT = num2;
				else OUT = num1;
			}
			// UJEMNE LICZBY, len1 > len2
			else {
				if (mode == 0) OUT = num1;
				else OUT = num2;
			}
		}
	}
	else {
		// DIFFERENT LENGTH, DIFFERENT SIGNS
		if (num1.sign) {
			if (mode == 0) OUT = num1;
			else OUT = num2;
		}
		else {
			if (mode == 0) OUT = num2;
			else OUT = num1;
		}
	}
	return OUT;
}
BigInt chooseAddSub(BigInt num1, BigInt num2, int mode) {
	bool negate = false;
	BigInt OUT, n1, n2; initializeNumber(&OUT);
	// abs(n1) > abs(n2)
	if (num1.length() > num2.length()) {
		n1 = num1;
		n2 = num2;
	}
	else if (num1.length() < num2.length()) {
		n1 = num2;
		n2 = num1;
		if (mode == 2) negate = true;
	}
	else {
		if (!num1.sign && !num2.sign) {
			n1 = compare(num1, num2, GET_SMALLER);
			n2 = compare(num1, num2, GET_BIGGER);
		}
		else {
			n1 = compare(num2, num1, GET_BIGGER);
			n2 = compare(num2, num1, GET_SMALLER);
			if (isEqual(n1, num2) && mode == 2) negate = true;
		}
	}

	switch (mode) {
	case 1:
		// x1 + x2 =		add(x1, x2)
		// x1 + (-x2) =		subtract(x1, x2)
		// -x1 + x2 =		subtract(x2, x1)
		// -x1 + (-x2) =	- add(x1, x2)

		if (n1.sign && n2.sign) OUT = add(n1, n2, PLUS);
		else if (n1.sign && !n2.sign) OUT = subtract(n1, n2, PLUS);
		else if (!n1.sign && n2.sign) OUT = subtract(n2, n1, PLUS);
		else if (!n1.sign && !n2.sign) OUT = add(n1, n2, MINUS);
		break;

	case 2:
		// SUBTRACTION
		// x1 - x2			subtraction(x1, x2)
		// x1 - (-x2)		add(x1, x2)
		// -x1 - x2 =		- add(x1, x2)
		// -x1 - (-x2) =	- subtraction(x1, x2)
		if (n1.sign && n2.sign) {
			if (isEqual(n1, n2)) {
				setNumber(&OUT, "0");
				return OUT;
			}
			OUT = subtract(n1, n2, PLUS);			// 5555 - 444		= 5555 - 444
		}
		else if (n1.sign && !n2.sign) {
			OUT = add(n1, n2, PLUS);			// 5555 - (-444)	= 5555 + 444
		}
		else if (!n1.sign && n2.sign) {
			OUT = add(n1, n2, MINUS);			// -5555 - 444		= -(5555 + 444)
		}
		else if (!n1.sign && !n2.sign) {
			// W tym miejscu ZAWSZE n1 <= n2
			if (isEqual(n1, n2)) {
				setNumber(&OUT, "0");
				return OUT;
			}
			OUT = subtract(n1, n2, MINUS);
		}
		break;
	}
	if (negate) OUT.negate();
	return OUT;
}


#if DEBUG
int main() {

	for (int i = 0; i < DEBUG_NUMBERS_AMOUNT; i++) {
		initializeNumber(&numbers[i]);
	}

	BigInt a1; setNumber(&a1, "654321");
	BigInt a2; setNumber(&a2, "123456");
	BigInt test1 = compare(a1, a2, GET_SMALLER);

	BigInt a3; setNumber(&a3, "71");
	BigInt a4; setNumber(&a4, "70");
	subtract(a3, a4, PLUS);

	BigInt a5; setNumber(&a5, "345344");
	BigInt a6; setNumber(&a6, "12");
	divide(a5, a6);

	setNumber(&numbers[0], "234187");
	setNumber(&numbers[1], "923");
	setNumber(&numbers[2], "123456");
	setNumber(&numbers[3], "654321");
	setNumber(&numbers[4], "123456");
	setNumber(&numbers[5], "6543");
	//setNumber(&numbers[6], "87265482947625347162345829308153427568923401204927"); // 50 digits
	//setNumber(&numbers[7], "872654829476253471623458293081534275689234012"); // 45 digits
	setNumber(&numbers[6], "4012");
	setNumber(&numbers[7], "872");
	setNumber(&numbers[8], "-2931");
	setNumber(&numbers[9], "-991823");
	setNumber(&numbers[10], "-876482");
	setNumber(&numbers[11], "-876482");
	setNumber(&numbers[12], "84823");
	setNumber(&numbers[13], "84823");
	setNumber(&numbers[14], "-84823");
	setNumber(&numbers[15], "211");
	setNumber(&numbers[16], "7");
	setNumber(&numbers[17], "-211");
	setNumber(&numbers[18], "844");
	setNumber(&numbers[19], "250");
	setNumber(&numbers[20], "9");
	setNumber(&numbers[21], "10");
	setNumber(&numbers[22], "11");
	setNumber(&numbers[23], "800");
	setNumber(&numbers[44], "1"); // Setting the number works properly


	printNumber(numbers[0]);
	cout << endl;
	printNumber(numbers[1]);
	cout << endl << endl;
	printNumber(numbers[2]);
	cout << endl;
	printNumber(numbers[3]);
	cout << endl << endl;
	printNumber(numbers[4]);
	cout << endl;
	printNumber(numbers[5]);
	cout << "\n\nLength of number0: " << numbers[0].length();
	cout << "\nLength of number1: " << numbers[1].length();

	BigInt x1, x2;
	setNumber(&x1, "12");  setNumber(&x2, "15");
	BigInt test = compare(x1, x2, GET_BIGGER);

	cout << "\n\nnumber4 + number5 [VALID: 129999] = ";
	printNumber(chooseAddSub(numbers[4], numbers[5], ADD));
	cout << "\n\nnumber2 + number3 [VALID: 777777] = ";
	printNumber(chooseAddSub(numbers[2], numbers[3], ADD));
	cout << "\n\nnumber0 + number1 [VALID: 235110] = ";
	printNumber(chooseAddSub(numbers[0], numbers[1], ADD));
	cout << "\n\nnumber15 + number18 [VALID: 1055] = ";
	printNumber(chooseAddSub(numbers[15], numbers[18], ADD));
	cout << "\n\nnumber4 - number5 [VALID: 116913] = ";
	printNumber(chooseAddSub(numbers[4], numbers[5], SUBTRACT));
	cout << "\n\nnumber2 - number3 [VALID: -530865] = ";
	printNumber(chooseAddSub(numbers[2], numbers[3], SUBTRACT));
	cout << "\n\nnumber0 - number1 [VALID: 233264] = ";
	printNumber(chooseAddSub(numbers[0], numbers[1], SUBTRACT));
	cout << "\n\nnumber8 - number9 [VALID: 988892] = "; // (-) - (-)
	printNumber(chooseAddSub(numbers[8], numbers[9], SUBTRACT)); //     -2931 - -991823 = 988 892
	cout << "\n\nnumber9 - number8 [VALID: -988892] = "; // (-) - (-)
	printNumber(chooseAddSub(numbers[9], numbers[8], SUBTRACT)); //     -991823 - -2931 = -988 892
	cout << "\n\nnumber4 - number8 [VALID: 126387] = "; // (+) - (-)
	printNumber(chooseAddSub(numbers[4], numbers[8], SUBTRACT)); //     123456 - -2931 = 126 387
	cout << "\n\nnumber23 - number24 [VALID: 799] = "; // (+) - (-)
	printNumber(chooseAddSub(numbers[23], numbers[44], SUBTRACT)); //     800 - 1 = 799
	cout << "\n\nnumber15 * number16 [VALID: 1477] = ";
	printNumber(multiply(numbers[15], numbers[16]));
	cout << "\n\nnumber17 * number18 [VALID: -178084] = ";
	printNumber(multiply(numbers[17], numbers[18]));			 //     844 * (-211) = -178 084
	cout << "\n\nnumber19 * number20 [VALID: 2250] = ";
	printNumber(multiply(numbers[19], numbers[20]));
	cout << "\n\nnumber19 * number21 [VALID: 2500] = ";
	printNumber(multiply(numbers[19], numbers[21]));
	cout << "\n\nnumber19 * number22 [VALID: 2750] = ";
	printNumber(multiply(numbers[19], numbers[22]));
	cout << "\n\ntimes10(number14) [VALID: -848230] = ";
	printNumber(times10(numbers[14]));
	cout << "\n\ntimes10(times10((number14))) [VALID: Exceed (6 digs), -848230(~0)] = ";
	printNumber(times10(times10((numbers[14]))));
	cout << "\n\ntimes10(number44) [VALID: 10] = ";
	printNumber(times10(numbers[44]));
	cout << "\n\ntimes10(times10((number44))) [VALID: 100] = ";
	printNumber(times10(times10((numbers[44]))));
	cout << "\n\nnumber15 / number16 [VALID: 30 R 1] = ";
	printNumber(divide(numbers[15], numbers[16])); printModulo();
	cout << "\n\nnumber17 / number18 [VALID: 0 R 844] = ";
	printNumber(divide(numbers[17], numbers[18])); printModulo();
	cout << "\n\nnumber19 / number20 [VALID: 27 R 7] = ";
	printNumber(divide(numbers[19], numbers[20])); printModulo();
	cout << "\n\nnumber19 / number21 [VALID: 25 R 0] = ";
	printNumber(divide(numbers[19], numbers[21])); printModulo();
	cout << "\n\nnumber19 / number22 [VALID: 22 R 8] = ";
	printNumber(divide(numbers[19], numbers[22])); printModulo();



	cout << "\n\n\nnumber5 + number6 = \n";
	printNumber(numbers[6]);
	cout << endl;
	printNumber(numbers[7]);
	cout << endl;
	cout << "---------------------------------------------------" << endl;
	printNumber(chooseAddSub(numbers[6], numbers[7], ADD));
	cout << endl << endl;

	printNumber(numbers[0]); cout << "  sign: " << numbers[0].sign << "  len: " << numbers[0].length() << endl;
	printNumber(numbers[1]); cout << "  sign: " << numbers[1].sign << "  len: " << numbers[1].length() << endl;
	printNumber(numbers[2]); cout << "  sign: " << numbers[2].sign << "  len: " << numbers[2].length() << endl;
	printNumber(numbers[3]); cout << "  sign: " << numbers[3].sign << "  len: " << numbers[3].length() << endl;
	printNumber(numbers[4]); cout << "  sign: " << numbers[4].sign << "  len: " << numbers[4].length() << endl;
	printNumber(numbers[5]); cout << "  sign: " << numbers[5].sign << "  len: " << numbers[5].length() << endl;
	printNumber(numbers[6]); cout << "  sign: " << numbers[6].sign << "  len: " << numbers[6].length() << endl;
	printNumber(numbers[7]); cout << "  sign: " << numbers[7].sign << "  len: " << numbers[7].length() << endl;
	printNumber(numbers[8]); cout << "  sign: " << numbers[8].sign << "  len: " << numbers[8].length() << endl;
	printNumber(numbers[9]); cout << "  sign: " << numbers[9].sign << "  len: " << numbers[9].length() << endl;

	cout << "Print greater:" << endl;
	// PASSED
	cout << "   num1: "; printNumber(numbers[0]);
	cout << "\n   num2: ";  printNumber(numbers[1]);
	cout << "\nGREATER: "; printNumber(compare(numbers[0], numbers[1], GET_BIGGER));
	cout << "\nSMALLER: "; printNumber(compare(numbers[0], numbers[1], GET_SMALLER)); cout << "\n\n";

	// PASSED
	cout << "   num2: "; printNumber(numbers[2]);
	cout << "\n   num3: ";  printNumber(numbers[3]);
	cout << "\nGREATER: "; printNumber(compare(numbers[3], numbers[2], GET_BIGGER)); cout << "\n\n";
	// printNumber(greater(&numbers[2], &numbers[3])); -- Unhandled exception: Access violation reading location

	// PASSED
	cout << "   num4: "; printNumber(numbers[4]);
	cout << "\n   num5: ";  printNumber(numbers[5]);
	cout << "\nGREATER: "; printNumber(compare(numbers[4], numbers[5], GET_BIGGER)); cout << "\n\n";

	// PASSED
	cout << "   num6: "; printNumber(numbers[6]);
	cout << "\n   num7: "; printNumber(numbers[7]);
	cout << "\nGREATER: "; printNumber(compare(numbers[6], numbers[7], GET_BIGGER));
	cout << "\n    SUM: "; printNumber(chooseAddSub(numbers[6], numbers[7], ADD)); cout << "\n\n";

	// PASSED
	cout << "   num8: "; printNumber(numbers[8]);
	cout << "\n   num9: ";  printNumber(numbers[9]);
	cout << "\nGREATER: "; printNumber(compare(numbers[8], numbers[9], GET_BIGGER));
	cout << "\nSMALLER: "; printNumber(compare(numbers[8], numbers[9], GET_SMALLER)); cout << "\n\n";

	cout << "   num8: "; printNumber(numbers[8]);
	cout << "\n   num9: ";  printNumber(numbers[9]);
	cout << "\nisEqual: " << isEqual(numbers[8], numbers[9]) << endl << endl;

	cout << "   num10: "; printNumber(numbers[10]);
	cout << "\n   num11: ";  printNumber(numbers[11]);
	cout << "\nisEqual: " << isEqual(numbers[10], numbers[11]) << endl << endl;

	cout << "   num12: "; printNumber(numbers[12]);
	cout << "\n   num13: ";  printNumber(numbers[13]);
	cout << "\nisEqual: " << isEqual(numbers[12], numbers[13]) << endl << endl;

	cout << "   num13: "; printNumber(numbers[13]);
	cout << "\n   num14: ";  printNumber(numbers[14]);
	cout << "\nisEqual: " << isEqual(numbers[13], numbers[14]) << endl << endl;

	cout << "   num8: "; printNumber(numbers[8]);
	cout << "\n   num9: ";  printNumber(numbers[9]);
	//cout << "\nGREATER: "; if (numbers[8] > numbers[9]) printNumber(numbers[8]); else printNumber(numbers[9]);
	//cout << "\nSMALLER: "; if (numbers[8] < numbers[9]) printNumber(numbers[8]); else printNumber(numbers[9]);

	cout << "\n\n   num0: "; printNumber(numbers[0]);
	cout << "\n   num1: ";  printNumber(numbers[1]);
	//cout << "\nGREATER: "; if (numbers[0] > numbers[1]) printNumber(numbers[0]); else printNumber(numbers[1]);
	//cout << "\nSMALLER: "; if (numbers[0] < numbers[1]) printNumber(numbers[0]); else printNumber(numbers[1]);

	cout << "\n\n   num8:       "; printNumber(numbers[8]);
	cout << "\n   num8.abs(): "; printNumber(numbers[8].abs());

	cout << "\n\n   num0:       "; printNumber(numbers[0]);
	cout << "\n   num0.abs(): "; printNumber(numbers[0].abs());



	cout << endl << endl;
	return 0;
}
#endif