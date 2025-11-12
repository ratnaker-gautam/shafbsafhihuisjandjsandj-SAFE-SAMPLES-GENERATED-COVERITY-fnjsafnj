//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    DISPLAY_RESULT,
    ERROR_STATE,
    EXIT_STATE
};

struct StateMachine {
    enum State current_state;
    int number;
    int result;
    char buffer[64];
};

void state_machine_init(struct StateMachine *sm) {
    sm->current_state = INIT;
    sm->number = 0;
    sm->result = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int is_valid_number(const char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int safe_atoi(const char *str, int *value) {
    if (str == NULL || value == NULL) return 0;
    long temp = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
        temp = temp * 10 + (str[i] - '0');
        if (temp > 1000000) return 0;
    }
    *value = (int)temp;
    return 1;
}

void process_state(struct StateMachine *sm) {
    switch (sm->current_state) {
        case INIT:
            printf("Welcome to Number Processor\n");
            printf("Enter 'process <number>' or 'quit': ");
            sm->current_state = READ_COMMAND;
            break;
            
        case READ_COMMAND:
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
                sm->current_state = ERROR_STATE;
                break;
            }
            sm->buffer[strcspn(sm->buffer, "\n")] = '\0';
            
            if (strcmp(sm->buffer, "quit") == 0) {
                sm->current_state = EXIT_STATE;
            } else if (strncmp(sm->buffer, "process ", 8) == 0) {
                char *num_str = sm->buffer + 8;
                if (is_valid_number(num_str) && safe_atoi(num_str, &sm->number)) {
                    sm->current_state = PROCESS_NUMBER;
                } else {
                    sm->current_state = ERROR_STATE;
                }
            } else {
                sm->current_state = ERROR_STATE;
            }
            break;
            
        case PROCESS_NUMBER:
            sm->result = sm->number * 2;
            if (sm->result < 0) {
                sm->current_state = ERROR_STATE;
            } else {
                sm->current_state = DISPLAY_RESULT;
            }
            break;
            
        case DISPLAY_RESULT:
            printf("Result: %d\n", sm->result);
            printf("Enter 'process <number>' or 'quit': ");
            sm->current_state = READ_COMMAND;
            break;
            
        case ERROR_STATE:
            printf("Error: Invalid input\n");
            printf("Enter 'process <number>' or 'quit': ");
            sm->current_state = READ_COMMAND;
            break;
            
        case EXIT_STATE:
            printf("Goodbye!\n");
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    while (sm.current_state != EXIT_STATE) {
        process_state(&sm);
    }
    
    return 0;
}