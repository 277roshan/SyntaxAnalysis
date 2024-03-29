	/* front.c - a lexical analyzer system for simple
	                                arithmetic expressions */
	                   #include <stdio.h>
	                   #include <ctype.h>
						#include <string.h>

	/* Global declarations */ /* Variables */
int charClass;
char lexeme [100];
char previous_lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
char lexeme_place_count [100000]; // array to store lexeme
int place_count = 0; // which character you are in
int place_count_temp = 0; //where in array you should store
FILE *in_fp, *fopen();
int errorcheck = 0;

int term();
int expr();
int factor();
void error();

	/* Function declarations */ 
void addChar();
void getChar();
void getNonBlank();
int getUpToNextLine();
int lex();
	/* Character classes */
	#define LETTER 0
	#define DIGIT 1
	#define ENDLINE 2
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
			getChar();
			// for no input provided case directly EOF case
			if (charClass == EOF){
				printf("No input provided at all\n");
				return 1;
			}
			do {
				errorcheck = 0;
				lex();
				// handle for error cases when you get -10 error signal
				if (expr() == -10){
					if (getUpToNextLine() == -10){
						return 1;
					}
					place_count_temp = 0;
					memset(lexeme_place_count,0,99999);
					memset(lexeme,0,99);
					memset(previous_lexeme,0,99);
				}
				// Handle for the end line case			
				if (nextToken == ENDLINE){
					memset(lexeme_place_count,0,99999);
					memset(lexeme,0,99);
					memset(previous_lexeme,0,99);
					getChar();
				}
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
	and return the token */ 

int lookup(char ch) {
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
	case '\n':
	addChar(); nextToken = ENDLINE; break;
	default: addChar(); nextToken = EOF; break;
}
return nextToken; }

	/*****************************************************/ /* addChar - a function to add nextChar to lexeme */ 
void addChar() {
if (lexLen <= 98) { 
	lexeme[lexLen++] = nextChar; 
	lexeme[lexLen] = 0;
}
else
	printf("Error - lexeme is too long \n");
}

	/*****************************************************/
	/* getChar - a function to get the next character of
	input and determine its character class */ 
void getChar() {
	place_count++;
	place_count_temp++;


nextChar = getc(in_fp);

if (nextChar =='\n'){
	charClass = ENDLINE;
}
			
else if (nextChar!= EOF) { 
	if (isalpha(nextChar))
		charClass = LETTER;
	else if (isdigit(nextChar))
		charClass = DIGIT;
	else charClass = UNKNOWN;

	lexeme_place_count[place_count_temp] = nextChar;

}
else
	charClass = EOF;
}
	/*****************************************************/
	/* getNonBlank - a function to call getChar until it
	returns a non-whitespace character */ 
void getNonBlank() {
	if (nextChar =='\n'){
		charClass = ENDLINE;
		return;}
while (isspace(nextChar)) 
		getChar();
	} /*****************************************************/
	/* lex - a simple lexical analyzer for arithmetic
	expressions */ 

// when you get error get up to the next line
int getUpToNextLine() {
	place_count++;
	while (1){
		// if \n found then break
		if (nextChar == '\n'){
			break;
		}
		
		getChar();
		if (charClass == EOF){
			return -10;
			break;
		}
	nextToken = ENDLINE;
	}
	return 1;
	} 
	

int lex() {
int g = 0;
for (int g; g<lexLen;g++){
	previous_lexeme[g] = lexeme[g];
}
lexLen = 0; 
getNonBlank();
switch (charClass) {
		/* Parse identifiers */ case LETTER:
	addChar();
	getChar();
	while (charClass == LETTER || charClass == DIGIT) {
		addChar();
		getChar(); }
		nextToken = IDENT; break;
	/* Parse integer literals */ 
		case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar(); }
			nextToken = INT_LIT; break;
	/* Parentheses and operators */ case UNKNOWN:
			lookup(nextChar); 
			getChar();
			break;
	/* EOF */
			case ENDLINE:

			place_count_temp = 0;
			nextToken = ENDLINE;
			lexeme[0] = 'E';
			lexeme[1] = 'O';
			lexeme[2] = 'F';
			lexeme[3] = 0;
			break;
			case EOF:
			g=0;
			place_count_temp = 0;
			nextToken = EOF;
			lexeme[0] = 'E';
			lexeme[1] = 'O';
			lexeme[2] = 'F';
			lexeme[3] = 0;
			break;
	} /* End of switch */

	printf("Next token is: %d, Next lexeme is %s\n",
		nextToken, lexeme); 
	return nextToken;




}  /* End of function lex */


	/* expr
	                      Parses strings in the language generated by the rule:
	                      <expr> -> <term> {(+ | -) <term>}
	                      */
int expr() 
{ 
	printf("Enter <expr>\n");
           /* Parse the first term */
	if (term() == -10){
		return -10;
	}
           /* As long as the next token is + or -, get
              the next token and parse the next term */
	while (nextToken == ADD_OP || nextToken == SUB_OP) { 
		lex();
		if (term() == -10){
		return -10;
	}
	}
		printf("Exit <expr>\n");
		return 1;
}  /* End of function expr */


/* term
   Parses strings in the language generated by the rule:
   <term> -> <factor> {(* | /) <factor>)
   */
int term() { 
	printf("Enter <term>\n");
/* Parse the first factor */
 if (factor() == -10){
 	return -10;
 }
 
/* As long as the next token is * or /, get the
next token and parse the next factor */
while (nextToken == MULT_OP || nextToken == DIV_OP) { 
	lex();
	if (factor() == -10){
 	return -10;
 }
}
	printf("Exit <term>\n");
	return 1;
}  /* End of function term */

/* factor
   Parses strings in the language generated by the rule:
   <factor> -> id | int_constant | ( <expr )
   */
int factor() {
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

if (expr() == -10){
	return -10;
}

if (nextToken == RIGHT_PAREN)
lex();
else					
							
                           error();
                       	return -10;
                       
                       }  /* End of if (nextToken == ... */
                   /* It was not an id, an integer literal, or a left
                      parenthesis */
else
						
                         error();
                     return -10;
           

                     }  /* End of else */
                     printf("Exit <factor>\n");
                    return 1;
                     
                   }  /* End of function factor */

// When error occurs print the message and terminate the program
void error(){
	errorcheck = 1;
	// print the expression up to the point
	for(int u=0; u<place_count; u++){
		printf("%c", lexeme_place_count[u]);
	}
	printf("\n");
	printf("Error at: ");

	// handle for EOF and ENDLINE CASES
	if(nextToken == EOF || nextToken == ENDLINE)
	{
		for (int y = place_count; 1; y--){
			if (lexeme_place_count[y] == ' '){
				break;
			}

			printf("%c",lexeme_place_count[y]);
		}
		printf("\n");
	}
	else{
		// print lexeme for normal cases
		printf("%s\n",lexeme);
	}
}

