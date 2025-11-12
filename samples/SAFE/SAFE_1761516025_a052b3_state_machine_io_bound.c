//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    char command[32];
};

void state_machine_init(struct StateMachine *sm) {
    sm->current_state = INIT;
    sm->number = 0;
    sm->result = 0;
    memset(sm->command, 0, sizeof(sm->command));
}

int is_valid_command(const char *cmd) {
    if (cmd == NULL || strlen(cmd) == 0) return 0;
    
    if (strcmp(cmd, "square") == 0) return 1;
    if (strcmp(cmd, "double") == 0) return 1;
    if (strcmp(cmd, "negate") == 0) return 1;
    if (strcmp(cmd, "exit") == 0) return 1;
    
    return 0;
}

int safe_multiply(int a, int b) {
    if (a > 0 && b > 0) {
        if (a > INT_MAX / b) return -1;
    } else if (a < 0 && b < 0) {
        if (a < INT_MAX / b) return -1;
    } else if (a > 0 && b < 0) {
        if (b < INT_MIN / a) return -1;
    } else if (a < 0 && b > 0) {
        if (a < INT_MIN / b) return -1;
    }
    return a * b;
}

int safe_add(int a, int b) {
    if (b > 0 && a > INT_MAX - b) return -1;
    if (b < 0 && a < INT_MIN - b) return -1;
    return a + b;
}

void process_transition(struct StateMachine *sm) {
    switch (sm->current_state) {
        case INIT:
            printf("Enter command (square/double/negate/exit): ");
            sm->current_state = READ_COMMAND;
            break;
            
        case READ_COMMAND:
            if (scanf("%31s", sm->command) != 1) {
                sm->current_state = ERROR_STATE;
                break;
            }
            
            if (!is_valid_command(sm->command)) {
                sm->current_state = ERROR_STATE;
                break;
            }
            
            if (strcmp(sm->command, "exit") == 0) {
                sm->current_state = EXIT_STATE;
            } else {
                sm->current_state = PROCESS_NUMBER;
            }
            break;
            
        case PROCESS_NUMBER:
            printf("Enter a number: ");
            if (scanf("%d", &sm->number) != 1) {
                sm->current_state = ERROR_STATE;
                break;
            }
            
            if (strcmp(sm->command, "square") == 0) {
                sm->result = safe_multiply(sm->number, sm->number);
                if (sm->result == -1) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
            } else if (strcmp(sm->command, "double") == 0) {
                sm->result = safe_add(sm->number, sm->number);
                if (sm->result == -1) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
            } else if (strcmp(sm->command, "negate") == 0) {
                sm->result = -sm->number;
            }
            
            sm->current_state = DISPLAY_RESULT;
            break;
            
        case DISPLAY_RESULT:
            printf("Result: %d\n\n", sm->result);
            sm->current_state = INIT;
            break;
            
        case ERROR_STATE:
            printf("Error: Invalid input or overflow detected\n\n");
            while (getchar() != '\n');
            sm->current_state = INIT;
            break;
            
        case EXIT_STATE:
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Calculator\n");
    printf("=======================\n\n");
    
    while (sm.current_state != EXIT_STATE) {
        process_transition(&sm);
    }
    
    printf("Goodbye!\n");
    return 0;
}