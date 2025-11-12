//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    PROCESS_TEXT,
    EXIT,
    ERROR
};

struct StateMachine {
    enum State current_state;
    int number;
    char text[64];
    int text_len;
};

void init_state_machine(struct StateMachine *sm) {
    sm->current_state = INIT;
    sm->number = 0;
    sm->text_len = 0;
    memset(sm->text, 0, sizeof(sm->text));
}

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_valid_text(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

void process_init_state(struct StateMachine *sm) {
    printf("State Machine Started\n");
    printf("Commands: number <value>, text <string>, exit\n");
    sm->current_state = READ_COMMAND;
}

void process_read_command(struct StateMachine *sm) {
    char command[32];
    char argument[64];
    
    printf("> ");
    if (scanf("%31s", command) != 1) {
        sm->current_state = ERROR;
        return;
    }
    
    if (strcmp(command, "number") == 0) {
        if (scanf("%63s", argument) != 1) {
            sm->current_state = ERROR;
            return;
        }
        if (is_valid_number(argument)) {
            sm->number = atoi(argument);
            sm->current_state = PROCESS_NUMBER;
        } else {
            sm->current_state = ERROR;
        }
    } else if (strcmp(command, "text") == 0) {
        if (scanf("%63s", argument) != 1) {
            sm->current_state = ERROR;
            return;
        }
        if (is_valid_text(argument)) {
            strncpy(sm->text, argument, sizeof(sm->text) - 1);
            sm->text[sizeof(sm->text) - 1] = '\0';
            sm->text_len = strlen(sm->text);
            sm->current_state = PROCESS_TEXT;
        } else {
            sm->current_state = ERROR;
        }
    } else if (strcmp(command, "exit") == 0) {
        sm->current_state = EXIT;
    } else {
        sm->current_state = ERROR;
    }
}

void process_number_state(struct StateMachine *sm) {
    printf("Processing number: %d\n", sm->number);
    printf("Square: %d\n", sm->number * sm->number);
    sm->current_state = READ_COMMAND;
}

void process_text_state(struct StateMachine *sm) {
    printf("Processing text: %s\n", sm->text);
    printf("Length: %d\n", sm->text_len);
    sm->current_state = READ_COMMAND;
}

void process_error_state(struct StateMachine *sm) {
    printf("Error: Invalid input\n");
    sm->current_state = READ_COMMAND;
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    while (sm.current_state != EXIT) {
        switch (sm.current_state) {
            case INIT:
                process_init_state(&sm);
                break;
            case READ_COMMAND:
                process_read_command(&sm);
                break;
            case PROCESS_NUMBER:
                process_number_state(&sm);
                break;
            case PROCESS_TEXT:
                process_text_state(&sm);
                break;
            case ERROR:
                process_error_state(&sm);
                break;
            case EXIT:
                break;
        }
    }
    
    printf("State Machine Terminated\n");
    return 0;
}