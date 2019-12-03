#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "keys.h"

#define LINE 30

struct key_press {
    unsigned long int time_stamp;         // Each keypress logged has a Unix Time Stamp
    int time_elapsed;       // Time since previous keypress logged
    int button_code;
    struct key_press *next;
    struct key_press *prev;
};

typedef struct key_press key;


void print_char(int code, int presses, key *current) {

    switch (code) {
        case 0 :
            presses = presses % 2;
            printf("%c", N7110_KEYPAD_ZERO_ABC_CHARS[presses]);
            break;
        case 1 :
            presses = presses % 14;
            printf("%c", N7110_KEYPAD_ONE_ABC_CHARS[presses]);
            break;
        case 2 :
            presses = presses % 4;
            printf("%c", N7110_KEYPAD_TWO_ABC_CHARS[presses]);
            break;
        case 3 :
            presses = presses % 4;
            printf("%c", N7110_KEYPAD_THREE_ABC_CHARS[presses]);
            break;
        case 4 : 
            presses = presses % 4;
            printf("%c", N7110_KEYPAD_FOUR_ABC_CHARS[presses]);
            break;
        case 5 :
            presses = presses % 4;
            printf("%c", N7110_KEYPAD_FIVE_ABC_CHARS[presses]);
            break;
        case 6 :
            presses = presses % 4;
            printf("%c", N7110_KEYPAD_SIX_ABC_CHARS[presses]);
            break;
        case 7 :
            presses = presses % 5;
            printf("%c", N7110_KEYPAD_SEVEN_ABC_CHARS[presses]);
            break;
        case 8 :
            presses = presses % 4;
            printf("%c", N7110_KEYPAD_EIGHT_ABC_CHARS[presses]);
            break;
        case 9 :
            presses = presses % 5;
            printf("%c", N7110_KEYPAD_NINE_ABC_CHARS[presses]);
            break;
        case 10 :
            presses = presses % 13;
            printf("%c", N7110_KEYPAD_STAR_ABC_CHARS[presses]);
            break;
        case 11 :
            printf("HASH");
            break;
        case 12 :
            presses = presses % 14;
            printf("%c", N7110_KEYPAD_ONE_ABC_CHARS[presses]);
            break;
        case 100 :
            printf("LEFT");
            break;
        case 101 :
            printf("RIGHT");
            break;
        case 102 :
            printf("UP");
            break;
        case 103 :
            printf("DOWN");
            break;
        case 104 :
            printf("ACCEPT");
            break;
        case 105 :
            printf("REJECT");
            break;
        default :
            printf("\n\n***ERROR\t%d\n", code);
            break;
    }
    fflush(stdout);
}

int main(int argc, char *argv[]) {

    key *head = NULL;
    key *current = NULL;

    FILE *in_file  = fopen("/home/dave/Desktop/otwadvent/sms4.csv", "r"); 
    char buf[LINE];          
    char *token;

    int presses = 0;

    if (in_file == NULL) {  
        printf("Error! Could not open file\n");
        exit(-1); 
    }
       
    // First Six lines are setup and can be skipped...
    // Only need timestamp from sixth
    int start = 0;
    while (fgets(buf, LINE, in_file) != NULL && ++start < 6) {}

    while (fgets(buf, LINE, in_file) != NULL) {
        if (!current) { 
            head = (key *)calloc(1, sizeof(key));
            current = head;
        } else { 
            current->next = (key *)calloc(1, sizeof(key));
            current->next->prev = current;
            current = current->next;
        }
        token = strtok(buf, ","); // Known to be 2-field csv
        current->time_stamp = atol(token);
        token = strtok(NULL, ","); 
        current->button_code = atoi(token);

        if (current->prev)
            current->time_elapsed = current->time_stamp - current->prev->time_stamp;
        if (current->time_elapsed > 700) {
//            printf("Code: %3d\tPresses: %3d\tElapsed: %3d\t", current->prev->button_code, presses, current->time_elapsed);
            print_char(current->prev->button_code, presses - 1, current);
  //          printf("\n");
            presses = 1;
        } else {
            presses++;
        }

        if (current->button_code == 100) { // Begin entering phone no.
            current = current->prev;
            free(current->next);
            break;
        }
    }
   
    while (head) {
        current = head;
        head = head->next;
        free(current);
    }
    printf("\n"); 
    exit(0);
}    
