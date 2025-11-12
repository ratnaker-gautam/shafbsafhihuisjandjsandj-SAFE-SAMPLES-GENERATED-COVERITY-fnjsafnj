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
    enum State current;
    int number;
    char text[64];
    int text_len;
};

void state_machine_init(struct StateMachine *sm) {
    sm->current = INIT;
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
    printf("State Machine Started. Commands: number, text, exit\n");
    sm->current = READ_COMMAND;
}

void process_read_command(struct StateMachine *sm) {
    char input[32];
    printf("Enter command: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "number") == 0) {
        sm->current = PROCESS_NUMBER;
    } else if (strcmp(input, "text") == 0) {
        sm->current = PROCESS_TEXT;
    } else if (strcmp(input, "exit") == 0) {
        sm->current = EXIT;
    } else {
        printf("Invalid command. Use: number, text, exit\n");
        sm->current = READ_COMMAND;
    }
}

void process_number_state(struct StateMachine *sm) {
    char input[32];
    printf("Enter a number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_number(input)) {
        printf("Invalid number. Only digits allowed.\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    long num = atol(input);
    if (num < 0 || num > 1000000) {
        printf("Number out of range (0-1000000).\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    sm->number = (int)num;
    printf("Number stored: %d\n", sm->number);
    sm->current = READ_COMMAND;
}

void process_text_state(struct StateMachine *sm) {
    char input[128];
    printf("Enter text (letters and spaces only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_text(input)) {
        printf("Invalid text. Only letters and spaces allowed.\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    size_t len = strlen(input);
    if (len >= sizeof(sm->text)) {
        printf("Text too long. Maximum %zu characters.\n", sizeof(sm->text) - 1);
        sm->current = READ_COMMAND;
        return;
    }
    
    strncpy(sm->text, input, sizeof(sm->text) - 1);
    sm->text[sizeof(sm->text) - 1] = '\0';
    sm->text_len = (int)len;
    printf("Text stored: %s\n", sm->text);
    sm->current = READ_COMMAND;
}

void process_error_state(struct StateMachine *sm) {
    printf("Error occurred. Resetting state machine.\n");
    state_machine_init(sm);
    sm->current = INIT;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    while (sm.current != EXIT) {
        switch (sm.current) {
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
        
        if (sm.current == ERROR) {
            process_error_state(&sm);
        }
    }
    
    printf("State Machine terminated.\n");
    return 0;
}