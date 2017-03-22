	/* front.c - a lexical analyzer system for simple
	                                arithmetic expressions */
	                   #include <stdio.h>
	                   #include <ctype.h>
	/* Global declarations */ /* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
int place_count=0;
FILE *in_fp, *fopen();

void term();
void expr();
void factor();
void error();

	/* Function declarations */ void addChar();
void getChar();
void getNonBlank();
int lex();
	/* Character classes */
	#define LETTER 0
	#define DIGIT 1
	#define UNKNOWN 99
	/* Token codes */
	#define INT_LIT 10
	#define IDENT 11
	#define ASSIGN_OP 20
	#define ADD_OP 21
	#define SUB_OP 22
	#define MULT_OP 23
	#define DIV_OP 24
	#define LEFT_PAREN 25
	#define RIGHT_PAREN 26
	/******************************************************/
	/* main driver */
int main(int argc, char *argv[]) {
	/* Open the input data file and process its contents */ 


   if( argc == 2 ) {
      printf("The argument supplied is %s\n", argv[1]);
      if ((in_fp = fopen(argv[1], "r")) == NULL)
		printf("ERROR - cannot open front.in \n"); else {
			getChar(); do {
				lex();
				expr();
			
			} 
			while (nextToken != EOF);
		} 
	}
   
   else if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
   }
   else {
      printf("One argument expected.\n");

   }
}
	
	


	/*****************************************************/ /* lookup - a function to lookup operators and parentheses
	and return the token */ int lookup(char ch) {
	switch (ch) { case '(':
	addChar();
	nextToken = LEFT_PAREN; break;
	case ')':
	addChar();
	nextToken = RIGHT_PAREN; break;
	case '+':
	addChar(); nextToken = ADD_OP; break;
	case '-':
	addChar(); nextToken = SUB_OP; break;
	case '*':
	addChar();
	nextToken = MULT_OP; break;
	case '/':
	addChar(); nextToken = DIV_OP; break;
	default: addChar(); nextToken = EOF; break;
}
return nextToken; }

	/*****************************************************/ /* addChar - a function to add nextChar to lexeme */ void addChar() {
if (lexLen <= 98) { lexeme[lexLen++] = nextChar; lexeme[lexLen] = 0;
}
else
	printf("Error - lexeme is too long \n");
}
	/*****************************************************/
	/* getChar - a function to get the next character of
	input and determine its character class */ 
void getChar() {
	place_count++;
if ((nextChar = getc(in_fp)) != EOF) { if (isalpha(nextChar))
	charClass = LETTER;
	else if (isdigit(nextChar))
		charClass = DIGIT;
	else charClass = UNKNOWN;
}
else
	charClass = EOF;
}
	/*****************************************************/
	/* getNonBlank - a function to call getChar until it
	returns a non-whitespace character */ void getNonBlank() {
while (isspace(nextChar)) 
	getChar();
	} /*****************************************************/
	/* lex - a simple lexical analyzer for arithmetic
	expressions */ 
int lex() {
lexLen = 0; getNonBlank();
switch (charClass) {
		/* Parse identifiers */ case LETTER:
	addChar();
	getChar();
	while (charClass == LETTER || charClass == DIGIT) {
		addChar();
		getChar(); }
		nextToken = IDENT; break;
	/* Parse integer literals */ case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar(); }
			nextToken = INT_LIT; break;
	/* Parentheses and operators */ case UNKNOWN:
			lookup(nextChar); getChar();
			break;
	/* EOF */
			case EOF:
			nextToken = EOF;
			lexeme[0] = 'E';
			lexeme[1] = 'O';
			lexeme[2] = 'F';
			lexeme[3] = 0;
			break;
	} /* End of switch */
			printf("Next token is: %d, Next lexeme is %s\n",
				nextToken, lexeme); return nextToken;

	}  /* End of function lex */


	/* expr
	                      Parses strings in the language generated by the rule:
	                      <expr> -> <term> {(+ | -) <term>}
	                      */
void expr() 
{ 
	printf("Enter <expr>\n");
           /* Parse the first term */
	term();
           /* As long as the next token is + or -, get
              the next token and parse the next term */
	while (nextToken == ADD_OP || nextToken == SUB_OP) { 
		lex();
		term(); 
	}
		printf("Exit <expr>\n");
}  /* End of function expr */


/* term
   Parses strings in the language generated by the rule:
   <term> -> <factor> {(* | /) <factor>)
   */
void term() 
{ printf("Enter <term>\n");
/* Parse the first factor */
factor();
/* As long as the next token is * or /, get the
next token and parse the next factor */
while (nextToken == MULT_OP || nextToken == DIV_OP) { lex();
	factor(); }
	printf("Exit <term>\n");
}  /* End of function term */

/* factor
   Parses strings in the language generated by the rule:
   <factor> -> id | int_constant | ( <expr )
   */
void factor() {
printf("Enter <factor>\n");
/* Determine which RHS */
if (nextToken == IDENT || nextToken == INT_LIT)
	/* Get the next token */
lex();
                   /* If the RHS is ( <expr>), call lex to pass over the
                      left parenthesis, call expr, and check for the right
                      parenthesis */
else {
if (nextToken == LEFT_PAREN) {
lex();
expr();
if (nextToken == RIGHT_PAREN)
lex();
else					
							
                           error();
                       }  /* End of if (nextToken == ... */
                   /* It was not an id, an integer literal, or a left
                      parenthesis */
else
						
                         error();
                     }  /* End of else */
                     printf("Exit <factor>\n");;
                   }  /* End of function factor */


void error(){
	printf("Lexeme with error: %s ******* \nError detected at %d character ********\n",lexeme, place_count-1);
}

