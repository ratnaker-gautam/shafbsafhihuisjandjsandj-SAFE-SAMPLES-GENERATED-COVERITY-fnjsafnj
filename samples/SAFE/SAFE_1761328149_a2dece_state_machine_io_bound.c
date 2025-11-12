//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READ_COMMAND,
    PROCESS_NUMBER,
    PROCESS_TEXT,
    ERROR,
    EXIT
};

struct StateMachine {
    enum State current_state;
    int number;
    char text[64];
    int text_len;
};

void init_state_machine(struct StateMachine *sm) {
    sm->current_state = START;
    sm->number = 0;
    sm->text_len = 0;
    memset(sm->text, 0, sizeof(sm->text));
}

int is_valid_command(char c) {
    return c == 'n' || c == 't' || c == 'x';
}

int is_valid_number_char(char c) {
    return isdigit(c) || c == '-' || c == '+';
}

int parse_number(const char *str, int *result) {
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    if (val < -1000000 || val > 1000000) {
        return 0;
    }
    *result = (int)val;
    return 1;
}

void process_state_machine(struct StateMachine *sm) {
    char input[128];
    
    while (sm->current_state != EXIT) {
        switch (sm->current_state) {
            case START:
                printf("Enter command (n=number, t=text, x=exit): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current_state = EXIT;
                    break;
                }
                if (strlen(input) > 0 && input[strlen(input)-1] == '\n') {
                    input[strlen(input)-1] = '\0';
                }
                if (strlen(input) != 1 || !is_valid_command(input[0])) {
                    printf("Invalid command. Please use n, t, or x.\n");
                    break;
                }
                if (input[0] == 'x') {
                    sm->current_state = EXIT;
                } else if (input[0] == 'n') {
                    sm->current_state = READ_COMMAND;
                } else if (input[0] == 't') {
                    sm->current_state = PROCESS_TEXT;
                }
                break;
                
            case READ_COMMAND:
                printf("Enter a number (-1000000 to 1000000): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current_state = START;
                    break;
                }
                if (strlen(input) > 0 && input[strlen(input)-1] == '\n') {
                    input[strlen(input)-1] = '\0';
                }
                if (strlen(input) == 0) {
                    printf("Empty input.\n");
                    sm->current_state = START;
                    break;
                }
                for (size_t i = 0; i < strlen(input); i++) {
                    if (!is_valid_number_char(input[i])) {
                        printf("Invalid number format.\n");
                        sm->current_state = START;
                        break;
                    }
                }
                if (sm->current_state != START) {
                    if (parse_number(input, &sm->number)) {
                        sm->current_state = PROCESS_NUMBER;
                    } else {
                        printf("Number out of range or invalid.\n");
                        sm->current_state = START;
                    }
                }
                break;
                
            case PROCESS_NUMBER:
                printf("Processed number: %d\n", sm->number);
                if (sm->number > 0) {
                    printf("Number is positive.\n");
                } else if (sm->number < 0) {
                    printf("Number is negative.\n");
                } else {
                    printf("Number is zero.\n");
                }
                sm->current_state = START;
                break;
                
            case PROCESS_TEXT:
                printf("Enter text (max 63 chars): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current_state = START;
                    break;
                }
                if (strlen(input) > 0 && input[strlen(input)-1] == '\n') {
                    input[strlen(input)-1] = '\0';
                }
                if (strlen(input) >= sizeof(sm->text)) {
                    printf("Text too long.\n");
                    sm->current_state = START;
                    break;
                }
                strncpy(sm->text, input, sizeof(sm->text)-1);
                sm->text[sizeof(sm->text)-1] = '\0';
                sm->text_len = strlen(sm->text);
                printf("Processed text: '%s' (length: %d)\n", sm->text, sm->text_len);
                sm->current_state = START;
                break;
                
            case ERROR:
                printf("An error occurred. Returning to start.\n");
                sm->current_state = START;
                break;
                
            case EXIT:
                break;
        }
    }
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    process_state_machine(&sm);
    printf("Goodbye!\n");
    return 0;
}