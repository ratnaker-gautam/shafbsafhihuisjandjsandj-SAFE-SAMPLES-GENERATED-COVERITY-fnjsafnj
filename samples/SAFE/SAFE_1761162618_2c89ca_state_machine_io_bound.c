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
    
    if (strcmp(cmd, "double") == 0) return 1;
    if (strcmp(cmd, "square") == 0) return 1;
    if (strcmp(cmd, "negate") == 0) return 1;
    if (strcmp(cmd, "exit") == 0) return 1;
    
    return 0;
}

int is_valid_number(const char *str) {
    if (str == NULL || strlen(str) == 0) return 0;
    
    int i = 0;
    if (str[0] == '-' || str[0] == '+') i++;
    
    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    
    return 1;
}

int safe_str_to_int(const char *str, int *result) {
    if (str == NULL || result == NULL) return 0;
    
    long val;
    char *endptr;
    val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (val > INT_MAX || val < INT_MIN) return 0;
    
    *result = (int)val;
    return 1;
}

void process_transition(struct StateMachine *sm) {
    switch (sm->current_state) {
        case INIT:
            printf("Enter command (double/square/negate/exit): ");
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
                printf("Enter a number: ");
                sm->current_state = PROCESS_NUMBER;
            }
            break;
            
        case PROCESS_NUMBER: {
            char num_str[32];
            if (scanf("%31s", num_str) != 1) {
                sm->current_state = ERROR_STATE;
                break;
            }
            
            if (!is_valid_number(num_str) || !safe_str_to_int(num_str, &sm->number)) {
                sm->current_state = ERROR_STATE;
                break;
            }
            
            if (strcmp(sm->command, "double") == 0) {
                if (sm->number > INT_MAX / 2 || sm->number < INT_MIN / 2) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
                sm->result = sm->number * 2;
            } else if (strcmp(sm->command, "square") == 0) {
                if (sm->number > 46340 || sm->number < -46340) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
                sm->result = sm->number * sm->number;
            } else if (strcmp(sm->command, "negate") == 0) {
                if (sm->number == INT_MIN) {
                    sm->current_state = ERROR_STATE;
                    break;
                }
                sm->result = -sm->number;
            }
            
            sm->current_state = DISPLAY_RESULT;
            break;
        }
            
        case DISPLAY_RESULT:
            printf("Result: %d\n\n", sm->result);
            sm->current_state = INIT;
            break;
            
        case ERROR_STATE:
            printf("Error: Invalid input\n\n");
            sm->current_state = INIT;
            break;
            
        case EXIT_STATE:
            printf("Goodbye!\n");
            break;
            
        default:
            sm->current_state = ERROR_STATE;
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("Number Processing State Machine\n");
    printf("===============================\n\n");
    
    while (sm.current_state != EXIT_STATE) {
        process_transition(&sm);
    }
    
    return 0;
}