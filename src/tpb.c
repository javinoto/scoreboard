#include "queue.h"
#include "tpb.h"
#include <stdlib.h> /* you probably need this */
#include <stdio.h>  /* you definitely need this */

/* add other headers from the CSTDLIB needed */

/* define my boolean type */

#define true 1
#define false 0
typedef int bool;

#define FRAME_NUMBER 10 /* you may need this */

/* Accepted Tokens */
typedef enum {
  TOKEN_LITERAL,      /* Number 0-9 */
  TOKEN_SPARE,        /* / symbol */
  TOKEN_STRIKE,       /* X/x symbol */
  TOKEN_GUTTER,       /* - symbol */
  TOKEN_FAULT        /* F/f symbol */
} Token;

/* Struct for the lexical tokenization */
typedef struct
{
  Token token;
  char character;
  int lookahead;
  int pos;
} Lex;

/* I implement a few functions to help you */

void free_scoreboard(LinkedList * scoreboard)
{
  Node * next = scoreboard->head;
  while (next!=NULL)
  {
    free(next->data);
    next = next->next;
  }

  free_linked_list(scoreboard);
}

/* 
This function is not in the tpb.h, because it doesn't need to be externalised
Given a token of a roll, it return its value. prev_roll is the token of the
previous roll necessary to computer the spare
 */
int get_token_value(Lex* token, Lex * prev_roll)
{
  if (token!= NULL)
  {
    switch (token->token)
    {
      case TOKEN_STRIKE:
        return 10;
      case TOKEN_SPARE:
        if (prev_roll!= NULL)  return 10-get_token_value(prev_roll,NULL); 
        else break;
      case TOKEN_LITERAL:
        return token->character - '0';
      default: /* GUTTER / FAULT*/
        return 0;
    }

  }

  return -1; /* in case on an error */
}

/*
This function performs the tokenization
Also this one is not in the tpb.h as it's not needed to be seen outside

The purpose of this function is that, given a character, it makes the 
corresponding token. The struct Lex l is where the values are stored
because the function returns true in case of success, false otherwise.

*/

bool tokenization(char c, Lex *l)
{

    switch (c)
    {
      case 'x':
      case 'X':
        l->token = TOKEN_STRIKE;
        l->character = c;
        l->lookahead=2;
        break;
      case 'f':
      case 'F':
        l->token = TOKEN_FAULT;
        l->character = c;
        l->lookahead=0;
        break;
      case '-':
        l->token = TOKEN_GUTTER;
        l->character = c;
        l->lookahead=0;
        break;
      case '/':
        l->token = TOKEN_SPARE;
        l->character = c;
        l->lookahead=1; 
        break;
      default:
        if (isdigit(c) && (c!='0'))
        {
          l->token = TOKEN_LITERAL;
          l->character = c;
          l->lookahead=0;
        }
        else return false;
    }

    return true;
}




LinkedList * bowling_score_parser(const char *game_characters, int *err_position)
{
    

    /* initialzation */
    

    /* Lexer */

    

    /* parser */
    

    /* reducer */

    

    /* Garbage collection */

}



void print_scoreboard(LinkedList*scoreboard)
{
 
}


