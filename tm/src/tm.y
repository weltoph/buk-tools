%{
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "limits.h"
#include "mapping.h"
#include "tm.h"
#include "tape.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);

%}


%code requires {
#include "limits.h"
#include "mapping.h"
#include "tm.h"
#include "tape.h"
}

%code {
}

%union {
  uint8_t index;
  char character;
  Movement_Direction direction;
  Tapechar_Set charset;
  State_Set stateset;
  Assignment transition;
  Mapping transition_function;
  Turingmachine tm;
}

%token <index> STATE
%token <direction> MOVEMENT
%token <character> TAPECONTENT
%token SETLEFT
%token SETRIGHT
%token TUPELLEFT
%token TUPELRIGHT
%token MAPPING
%token VOIDED
%token DELIMITER
%token SETSEP

%type <tm> spec
%type <stateset> states
%type <stateset> staterec
%type <charset> characters
%type <charset> charrec
%type <transition> trans
%type <transition_function> transitions
%type <transition_function> delta

%%

spec:
    states DELIMITER characters DELIMITER characters DELIMITER TAPECONTENT
    DELIMITER STATE DELIMITER STATE DELIMITER delta {
        const State_Set states = $1;
        const Tapechar_Set input_alphabet = $3;
        const Tapechar_Set tape_alphabet = $5;
        const char blank_symbol = $7;
        const uint8_t start_state = $9;
        const uint8_t end_state = $11;
        const Mapping delta = $13;
        const Tape empty_tape;
        /* TODO: consistency checks */
        Turingmachine tm = {
          .states = states,
          .input_alphabet = input_alphabet,
          .tape_alphabet = tape_alphabet,
          .start_state = start_state,
          .end_state = end_state,
          .delta = delta,
          .blank_symbol = blank_symbol,
          .tape = empty_tape
        };
        $$ = tm;
      }
    ;

states:
      SETLEFT staterec SETRIGHT { $$ = $2; }
      ;
staterec:
        STATE {
            State_Set set;
            const bool success = add_state(&set, $1);
            if(!success) {
              /* TODO: error */
              fprintf(stderr, "state index %u could not be used", $1);
            }
            $$ = set;
          }
        | staterec SETSEP STATE {
            const bool success = add_state(&$1, $3);
            if(!success) {
              /* TODO: error */
              fprintf(stderr, "state index %u could not be used", $1);
            }
            $$ = $1;
          }
        ;

characters:
          SETLEFT charrec SETRIGHT { $$ = $2; }
          ;
charrec:
       TAPECONTENT {
          Tapechar_Set set;
          const bool success = add_tapechar(&set, $1);
          if(!success) {
            /* TODO: error */
            fprintf(stderr, "character %c could not be used", $1);
          }
          $$ = set;
        }
       | charrec SETSEP TAPECONTENT { 
          const bool success = add_tapechar(&$1, $3);
          if(!success) {
            /* TODO: error */
            fprintf(stderr, "character %c could not be used", $1);
          }
          $$ = $1;
        }
       ;
delta:
     SETLEFT transitions SETRIGHT { $$ = $2; }
     ;

trans:
     TUPELLEFT STATE SETSEP TAPECONTENT TUPELRIGHT MAPPING
     TUPELLEFT STATE SETSEP TAPECONTENT SETSEP MOVEMENT TUPELRIGHT {
      /* delta(q, a) = (p, b, X) */
        const uint8_t q            = $2;
        const char a               = $4;
        const uint8_t p            = $8;
        const char b               = $10;
        const Movement_Direction X = $12;
        const Action action = {
          .next_state_index = p,
          .character        = b,
          .direction        = X
        };
        const Assignment assign = {
          .state = q,
          .character = a,
          .result = action
        };
        $$ = assign;
     }
     ;
transitions:
           trans {
              Mapping map;
              const Assignment assign = $1;
              const bool success = add_assignment(&map, assign);
              if(!success) {
                /* TODO: error */
                fprintf(stderr, "could not add action (q%u, %c, %C) for\
                  (q%u, %c)", assign.result.next_state_index,
                    assign.result.character,
                    movement_character(assign.result.direction),
                  assign.state, assign.character);
              }
              $$ = map;
            }
           | transitions SETSEP trans {
                const Assignment assign = $3;
                const Action_Status current_entry_status = check_entry($1, assign.character, assign.state);
                const bool override = current_entry_status == VOIDEDDEF || current_entry_status == DEFINED;
                if(override) {
                  fprintf(stderr, "previous action for (q%u, %c) present",
                    assign.state, assign.character);
                }
                const bool success = add_assignment(&$1, assign);
                if(!success) {
                  /* TODO: error */
                  fprintf(stderr, "could not add action (q%u, %c, %C) for\
                    (q%u, %c)", assign.result.next_state_index,
                      assign.result.character,
                      movement_character(assign.result.direction),
                    assign.state, assign.character);
                }
                $$ = $1;
            }
           ;
%%

int main(int argc, char *argv[])
{
  /* consume executable name */
  argc--; argv++;
  FILE *myfile = fopen(*argv, "r");

  if (!myfile) {
    printf("Couldn't open file %s", *argv);
    return -1;
  }

  /* consume file name */
  argc--; argv++;

  yyin = myfile;

  do {
    yyparse();
  } while (!feof(yyin));
  return 0;
}

void yyerror(const char *s)
{
  printf("An error occurred: %s", s);
}
