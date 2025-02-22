#include "queue.h"
#include "tpb.h"
#include <stdlib.h> /* you probably need this */
#include <stdio.h>  /* you definitely need this */

/* add other headers from the CSTDLIB needed */
#include <string.h>
#include <ctype.h>

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

/* Implement a few functions to help you */


/* --------------------------------------------------------------
   NUEVO: Aquí colocas las 3 funciones helper 
   validate_frame, validate_last_frame, get_next_rolls_value
   -------------------------------------------------------------- */

/* Función para validar un frame 1..9 */
bool validate_frame(LinkedList *fr, int *err_position)
{
    Node *n;
    int count = 0;
    bool has_strike = false;

    n = fr->head;
    while (n != NULL) {
        Lex *lx = (Lex *)n->data;
        count++;
        /* 1er tiro strike => ok, 2do tiro strike => no */
        if (count == 1 && lx->token == TOKEN_STRIKE) {
            has_strike = true;
        }
        else if (lx->token == TOKEN_SPARE && count == 1) {
            /* Spare no puede ser la 1ra tirada */
            if (err_position) {
                *err_position = lx->pos;
            }
            return false;
        }
        else if (lx->token == TOKEN_STRIKE && count == 2) {
            /* Strike en 2da tirada no permitido en frames 1..9 */
            if (err_position) {
                *err_position = lx->pos;
            }
            return false;
        }
        else if (lx->token == TOKEN_SPARE && has_strike) {
            /* Mismo frame, strike + spare => no */
            if (err_position) {
                *err_position = lx->pos;
            }
            return false;
        }
        n = n->next;
    }
    /* Máximo 2 tiradas en frames 1..9 */
    if (count > 2) {
        if (err_position) {
            Node *last = fr->tail;
            Lex *lx = (Lex *)last->data;
            *err_position = lx->pos;
        }
        return false;
    }
    return true;
}

/* Función para validar el frame 10 */
bool validate_last_frame(LinkedList *fr, int *err_position)
{
    Node *n;
    int count = 0;

    n = fr->head;
    while (n != NULL) {
        Lex *lx = (Lex *)n->data;
        count++;

        if (count > 3) {
            if (err_position) {
                *err_position = lx->pos;
            }
            return false;
        }
        if (count == 2 && lx->token == TOKEN_STRIKE) {
            /* 2da tirada strike => solo si 1ra también strike */
            Node *firstRoll = fr->head;
            Lex *firstLex = (Lex *)firstRoll->data;
            if (firstLex->token != TOKEN_STRIKE) {
                if (err_position) {
                    *err_position = lx->pos;
                }
                return false;
            }
        }
        if (count == 3) {
            /* 3ra tirada => check la 1ra y 2da */
            Lex *rolls[3];
            int i = 0;
            Node *tmp = fr->head;
            while (tmp != NULL && i < 3) {
                rolls[i] = (Lex *)tmp->data;
                tmp = tmp->next;
                i++;
            }
            if (i == 3) {
                bool firstStrike  = (rolls[0]->token == TOKEN_STRIKE);
                bool secondStrike = (rolls[1]->token == TOKEN_STRIKE);
                bool secondSpare  = (rolls[1]->token == TOKEN_SPARE);

                /* 3er lanzamiento => permitido si 1er strike o 2do es strike/spare */
                if (!firstStrike && !secondSpare) {
                    if (!secondStrike) {
                        if (err_position) {
                            *err_position = rolls[2]->pos;
                        }
                        return false;
                    }
                }
            }
        }
        n = n->next;
    }
    return true;
}



/* Función para obtener la suma de las próximas 'n' tiradas 
   después del frame 'frame_index' => para bonificaciones. */
int get_next_rolls_value(LinkedList *frame_list[], int frame_index, int n)
{
    int i, f;
    Node *cur;
    Lex *roll_lex;
    Lex *rolls_array[24];
    int rolls_count = 0;
    int last_of_frame_index;
    int sum = 0;
    int collected = 0;
    int arr_idx;
    int previous_roll_val = -1; 
   
    memset(rolls_array, 0, sizeof(rolls_array));

    for (f = 0; f < FRAME_NUMBER; f++) {
        cur = frame_list[f]->head;
        while (cur != NULL && rolls_count < 24) {
            roll_lex = (Lex *)cur->data;
            rolls_array[rolls_count++] = roll_lex;
            cur = cur->next;
        }
    }

    cur = frame_list[frame_index]->head;
    if (!cur) {
        return 0; 
    }

    while (cur->next != NULL) {
        cur = cur->next;
    }

    last_of_frame_index = -1;
    for (i = 0; i < rolls_count; i++) {
        if (rolls_array[i] == (Lex *)cur->data) {
            last_of_frame_index = i;
            break;
        }
    }

    if (last_of_frame_index == -1) {
        return 0; 
    }

  
    arr_idx = last_of_frame_index + 1;
    while (arr_idx < rolls_count && collected < n) {
        roll_lex = rolls_array[arr_idx];

        if (roll_lex->token == TOKEN_STRIKE) {
            sum += 10;
            previous_roll_val = 10;
        }
        else if (roll_lex->token == TOKEN_SPARE) {
            if (previous_roll_val >= 0) {
                sum += (10 - previous_roll_val);
            }
            previous_roll_val = (10 - previous_roll_val);
        }
        else if (roll_lex->token == TOKEN_LITERAL) {
            sum += (roll_lex->character - '0');
            previous_roll_val = (roll_lex->character - '0');
        }
        else { 
            sum += 0;
            previous_roll_val = 0;
        }

        collected++;
        arr_idx++;
    }

    return sum;
}




/* ---------------------------------------------------------------------
   FIn:Funciones auxiliares 
   --------------------------------------------------------------------- */



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


/* -------------------------------------------------------- */
/*  Sección Principal: bowling_score_parser                  */
/* -------------------------------------------------------- */

/*
 * bowling_score_parser:
 *   - parsea la cadena de caracteres y devuelve una LinkedList de Frames (10 frames),
 *     o NULL en caso de error.
 *   - Utiliza `err_position` para indicar la posición 0-based del error.
 *
 * Sigue la estructura:
 *   1. Initialization
 *   2. Lexer
 *   3. Parser
 *   4. Reducer
 *   5. Epilogue
 */



LinkedList * bowling_score_parser(const char *game_characters, int *err_position)
{
    /* -------------------- SECTION 1: INITIALISATION -------------------- */
    Queue *tokens;
    LinkedList *frame_list[FRAME_NUMBER];      /* Declara frame_list como un arreglo de punteros */
    LinkedList *scoreboard;
    size_t length;
    int i;               /* Para bucles en C90 */
    int k;  /* Declarar fuera del bucle */
    int current_frame;
    int rolls_in_frame;
    int cumulative_score;
    
    /* Verificar puntero nulo en la cadena de entrada */
    if (game_characters == NULL) {
        if (err_position != NULL) {
            *err_position = 0;
        }
        return NULL;
    }

    /* Inicializar la cola de tokens */
    tokens = initialise_queue();
    if (tokens == NULL) {
        if (err_position != NULL) {
            *err_position = 0;
        }
        return NULL;
    }

    /* Inicializar el array de frame_list */
    
    for (i = 0; i < FRAME_NUMBER; i++) {
    frame_list[i] = initialise_linked_list(); /* Asigna el puntero directamente */
    if (frame_list[i] == NULL) {              /* Verifica si la inicialización falló */
        if (err_position != NULL) {
            *err_position = 0;
        }
        /* Liberar las listas ya inicializadas */
        for (k = 0; k < i; k++) {             /* Usar la variable declarada */
            free_linked_list(frame_list[k]);
        }
        free_queue(tokens);
        return NULL;
        }
    }

    /* Inicializar el scoreboard final (lista enlazada de frames) */
    scoreboard = initialise_linked_list();
    if (scoreboard == NULL) {
        if (err_position != NULL) {
            *err_position = 0;
        }
        free_queue(tokens);
        return NULL;
    }

    /* -------------------- SECTION 2: LEXER -------------------- */
    length = strlen(game_characters);  /* requiere <string.h> */
    for (i = 0; i < (int)length; i++) {
        Lex lexchar;
        char c = game_characters[i];
        
        /* Tokenizar el carácter */
        if (!tokenization(c, &lexchar)) {
            /* Error de token inválido */
            if (err_position != NULL) {
                *err_position = i;     /* posición 0-based del error */
            }
            free_queue(tokens);
            free_scoreboard(scoreboard);
            return NULL;
        }
        /* Asignar la posición del token y meterlo a la cola */
        lexchar.pos = i;
        push_queue(tokens, &lexchar, sizeof(Lex));
    }

    /* -------------------- SECTION 3: PARSER -------------------- */
    current_frame = 0;
    rolls_in_frame = 0;

    /* Mientras queden tokens y no hayamos llenado los 10 frames */
    while (current_frame < FRAME_NUMBER && tokens->head != NULL) {
        Lex *tk;
        tk = pop_queue(tokens);  /* extraer siguiente token */
        if (tk == NULL) {
            break;  /* sin tokens => salir */
        }

        /* Agregar el token al frame actual */
        append_linked_list(frame_list[current_frame], tk);

        /* Lógica para frames 1..9 */
        if (current_frame < 9) {
            /* Strike => frame cerrado inmediatamente */
            if (tk->token == TOKEN_STRIKE) {
                rolls_in_frame = 0;
                current_frame++;
            } else {
                /* No es strike => puede ser spare / gutter / literal / fault */
                rolls_in_frame++;
                if (rolls_in_frame == 2) {
                    /* Completó 2 rolls => cerrar frame */
                    rolls_in_frame = 0;
                    current_frame++;
                }
            }
        }
        else {
            /* Si estamos en frame #10 => permitir hasta 3 tiradas */
            rolls_in_frame++;
            if (rolls_in_frame == 3) {
                /* Alcanzamos 3 lanzamientos => cerramos el frame 10 */
                current_frame++;  
                break;
            } else {
                /* Podríamos cerrar antes si los 2 primeros rolls son open 
                (sin strike/spare). Recorremos el frame actual */
                Node *n;
                int roll_count = 0;
                int strike_count = 0;
                int spare_count = 0;

                n = frame_list[current_frame]->head;
                while (n != NULL) {
                    Lex *roll_lex = (Lex *)n->data;
                    if (roll_lex->token == TOKEN_STRIKE) {
                        strike_count++;
                    }
                    if (roll_lex->token == TOKEN_SPARE) {
                        spare_count++;
                    }
                    roll_count++;
                    n = n->next;
                }

                /* Si ya llevamos 2 tiradas, pero ni strike ni spare => frame cerrado */
                if (roll_count == 2) {
                    if (strike_count == 0 && spare_count == 0) {
                        current_frame++;  
                        break;
                    }
                }
            }
        }
    }

    /* -------------------- VERIFICACIÓN DE TOKENS SOBRANTES -------------------- */

    /* -- Verificar si completaste 10 frames y aún quedan tokens (sobran tiradas) -- */
    if (current_frame == FRAME_NUMBER && tokens->head != NULL) {
        Lex *leftover_lex = (Lex *)tokens->head->data;  /* primer token sobrante */
        if (err_position != NULL) {
            *err_position = leftover_lex->pos;  /* índice 0-based del char sobrante */
        }
        /* Limpieza */
        for (k = 0; k < FRAME_NUMBER; k++) {
            free_linked_list(frame_list[k]);
        }
        free_scoreboard(scoreboard);
        free_queue(tokens);
        return NULL;
    }




    /* -------------------- SECTION 4: REDUCER -------------------- */
    cumulative_score = 0;
    for (i = 0; i < FRAME_NUMBER; i++) {
        Frame *newframe;
        Node *nd;
        int roll_count;
        bool is_strike;
        bool is_spare;
        int frame_score;
        Lex *rolls_in_this_frame[4];

        /* Si no hay tokens en este frame => asumimos juego incompleto y salimos */
        if (frame_list[i]->head == NULL) {
            break;
        }

        /* Reservar un Frame para la scoreboard */
        newframe = (Frame *)malloc(sizeof(Frame));
        if (newframe == NULL) {
            if (err_position != NULL) {
                *err_position = 0; /* no podemos indicar la posición exacta */
            }
            /* Limpieza: liberar todo */
            {
                int k;
                for (k = 0; k < FRAME_NUMBER; k++) {
                    free_linked_list(frame_list[k]);
                }
            }
            free_scoreboard(scoreboard);
            free_queue(tokens);
            return NULL;
        }
        newframe->score = 0;
        newframe->n_rolls = 0;
        /* Para C90, memset requiere <string.h> */
        memset(newframe->rolls, 0, sizeof(newframe->rolls));

        /* Validar la estructura del frame */
        if (i < 9) {
            /* frames 1..9 => validate_frame */
            if (!validate_frame(frame_list[i], err_position)) {
                /* error => limpiar y salir */
                free(newframe);
                {
                    int k;
                    for (k = 0; k < FRAME_NUMBER; k++) {
                        free_linked_list(frame_list[k]);
                    }
                }
                free_scoreboard(scoreboard);
                free_queue(tokens);
                return NULL;
            }
        } else {
            /* frame 10 => validate_last_frame */
            if (!validate_last_frame(frame_list[i], err_position)) {
                free(newframe);
                {
                    int k;
                    for (k = 0; k < FRAME_NUMBER; k++) {
                        free_linked_list(frame_list[k]);
                    }
                }
                free_scoreboard(scoreboard);
                free_queue(tokens);
                return NULL;
            }
        }

        /* Recolectar los tokens (Lex *) del frame */
        nd = frame_list[i]->head;
        roll_count = 0;
        while (nd != NULL && roll_count < 4) {
            rolls_in_this_frame[roll_count] = (Lex *)nd->data;
            roll_count++;
            nd = nd->next;
        }

        /* Calcular la puntuación base del frame */
        is_strike = false;
        is_spare  = false;
        frame_score = 0;

        if (roll_count == 1 && rolls_in_this_frame[0]->token == TOKEN_STRIKE) {
            /* Strike */
            is_strike = true;
            frame_score = 10;
        }
        else if (roll_count >= 2) {
            /* Podría ser spare o open */
            if (rolls_in_this_frame[1]->token == TOKEN_SPARE) {
                is_spare = true;
                frame_score = 10;
            } else {
                /* open frame => sumar roll 1 + roll 2 */
                int val1;
                int val2;
                val1 = get_token_value(rolls_in_this_frame[0], NULL);
                val2 = get_token_value(rolls_in_this_frame[1], rolls_in_this_frame[0]);
                frame_score = val1 + val2;
            }
        }

        /* Bonus: strikes/spares en frames 1..9 */
        if (i < 9) {
            if (is_strike) {
                int extra;
                extra = get_next_rolls_value(frame_list, i, 2);
                frame_score += extra;
            }
            else if (is_spare) {
                int extra;
                extra = get_next_rolls_value(frame_list, i, 1);
                frame_score += extra;
            }
        }
        else {
            /* Frame 10 => simplemente sumar todo lo que haya */
            int r;
            frame_score = 0;
            for (r = 0; r < roll_count; r++) {
                int val;
                Lex *prevLex = NULL;
                if (r > 0) {
                    prevLex = rolls_in_this_frame[r - 1];
                }
                val = get_token_value(rolls_in_this_frame[r], prevLex);
                frame_score += val;
            }
        }

        /* Copiar los símbolos al Frame->rolls (máx 3) */
        {
            int r2;
            for (r2 = 0; r2 < roll_count && r2 < 3; r2++) {
                newframe->rolls[r2] = rolls_in_this_frame[r2]->character;
            }
            newframe->n_rolls = (roll_count < 3) ? roll_count : 3;
        }

        /* Actualizar score acumulado */
        cumulative_score += frame_score;
        newframe->score = cumulative_score;

        /* Agregar el nuevo Frame a la lista scoreboard */
        append_linked_list(scoreboard, newframe);
    }

    /* -------------------- SECTION 5: EPILOGUE -------------------- */
    /* Liberar la memoria reservada en frame_list (Lex*) */
    for (i = 0; i < FRAME_NUMBER; i++) {
        Node *nd2;
        nd2 = frame_list[i]->head;
        while (nd2 != NULL) {
            /* Cada nodo->data era un Lex* devuelto por pop_queue (malloc) */
            free(nd2->data);
            nd2 = nd2->next;
        }
        free_linked_list(frame_list[i]);
    }

    /* Liberar la cola */
    free_queue(tokens);

    /* Devolver el scoreboard si todo fue bien */
    return scoreboard;
}


/* -------------------------------------------------------- */
/*  Fin Sección Principal: bowling_score_parser                  */
/* -------------------------------------------------------- */



/* Function to print the scoreboard */
void print_scoreboard(LinkedList *scoreboard) {
    Node *current;
    Frame *frame;
    int frame_count;
    int scores[10];
    char rolls[10][6]; 
    int i;

    /* Initialize buffers */
    for (i = 0; i < 10; i++) {
        memset(rolls[i], ' ', sizeof(rolls[i]));
        rolls[i][0] = '\0'; 
    }
    memset(scores, 0, sizeof(scores));

    frame_count = 0;

    /* Traverse the scoreboard */
    current = scoreboard->head;
    while (current != NULL && frame_count < 10) {
        frame = (Frame *)current->data;
        if (frame != NULL) {
            /* Store rolls */
            if (frame_count < 9) { /* Frames 1 to 9 */
                for (i = 0; i < frame->n_rolls && i < 2; i++) {
                    /* Ensure characters are uppercase if necessary */
                    char roll_char = frame->rolls[i];
                    if (roll_char == 'x') {
                        roll_char = 'X';
                    }
                    rolls[frame_count][i * 2] = roll_char;
                    if (i < frame->n_rolls - 1) {
                        rolls[frame_count][i * 2 + 1] = ' '; /* Add space between rolls */
                    }
                }
                /* Properly terminate the string without additional space */
                rolls[frame_count][frame->n_rolls * 2 - 1] = '\0';
            } else { /* Frame 10 */
                for (i = 0; i < frame->n_rolls && i < 3; i++) {
                    /* Ensure characters are uppercase if necessary */
                    char roll_char = frame->rolls[i];
                    if (roll_char == 'x') {
                        roll_char = 'X';
                    }
                    rolls[frame_count][i * 2] = roll_char;
                    if (i < frame->n_rolls - 1 && i < 2) {
                        rolls[frame_count][i * 2 + 1] = ' '; /* Add space between rolls */
                    }
                }
                rolls[frame_count][5] = '\0'; /* Properly terminate the string for the last frame */
            }
            /* Store the cumulative score */
            scores[frame_count] = frame->score;
        }
        frame_count++;
        current = current->next;
    }

    /* Construct the table */
    /* Top border line */
    printf("+-----------------------------------------+\n");

    /* Rolls line */
    printf("|");
    for (i = 0; i < 10; i++) {
        if (i < 9) {
            printf("%-3s|", rolls[i]); /* Frames 1 to 9 with 3 characters */
        } else {
            printf("%-5s|", rolls[i]); /* Last frame with 5 characters */
        }
    }
    printf("\n");

    /* Scores line */
    printf("|");
    for (i = 0; i < 10; i++) {
        if (i < 9) {
            if (scores[i] == 0 && strlen(rolls[i]) == 0) {
                /* Empty cells for incomplete games */
                printf("   |");
            } else {
                printf("%3d|", scores[i]);
            }
        } else {
            /* Last cell always empty according to the desired format */
            printf("     |");
        }
    }
    printf("\n");

    /* Bottom border line */
    printf("+-----------------------------------------+\n");
}
