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
    char command[32];
};

void state_machine_init(struct StateMachine *sm) {
    sm->current_state = INIT;
    sm->number = 0;
    sm->result = 0;
    memset(sm->command, 0, sizeof(sm->command));
}

int validate_number(const char *input) {
    if (input == NULL || strlen(input) == 0) return 0;
    
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i]) && !(i == 0 && input[i] == '-')) return 0;
    }
    
    long val = strtol(input, NULL, 10);
    if (val < -1000000 || val > 1000000) return 0;
    
    return 1;
}

int process_command(struct StateMachine *sm) {
    if (strcmp(sm->command, "double") == 0) {
        if (sm->number > 1000000 || sm->number < -1000000) return 0;
        sm->result = sm->number * 2;
        return 1;
    } else if (strcmp(sm->command, "square") == 0) {
        if (sm->number > 46340 || sm->number < -46340) return 0;
        sm->result = sm->number * sm->number;
        return 1;
    } else if (strcmp(sm->command, "negate") == 0) {
        if (sm->number == -2147483648) return 0;
        sm->result = -sm->number;
        return 1;
    }
    return 0;
}

void run_state_machine(struct StateMachine *sm) {
    char input[64];
    
    while (sm->current_state != EXIT_STATE) {
        switch (sm->current_state) {
            case INIT:
                printf("State Machine Calculator\n");
                printf("Commands: double, square, negate, exit\n");
                sm->current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                printf("Enter command: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
                
                input[strcspn(input, "\n")] = 0;
                
                if (strcmp(input, "exit") == 0) {
                    sm->current_state = EXIT_STATE;
                    break;
                }
                
                if (strlen(input) >= sizeof(sm->command)) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
                
                strcpy(sm->command, input);
                sm->current_state = PROCESS_NUMBER;
                break;
                
            case PROCESS_NUMBER:
                printf("Enter number: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
                
                input[strcspn(input, "\n")] = 0;
                
                if (!validate_number(input)) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
                
                sm->number = atoi(input);
                
                if (!process_command(sm)) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
                
                sm->current_state = DISPLAY_RESULT;
                break;
                
            case DISPLAY_RESULT:
                printf("Result: %d\n\n", sm->result);
                sm->current_state = READ_COMMAND;
                break;
                
            case ERROR_STATE:
                printf("Error: Invalid input or operation failed\n\n");
                sm->current_state = READ_COMMAND;
                break;
                
            case EXIT_STATE:
                break;
        }
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    run_state_machine(&sm);
    printf("Goodbye!\n");
    return 0;
}