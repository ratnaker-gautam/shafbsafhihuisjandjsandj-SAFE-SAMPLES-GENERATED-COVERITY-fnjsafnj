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
    if (cmd == NULL) return 0;
    if (strlen(cmd) == 0) return 0;
    
    const char *valid_commands[] = {"square", "cube", "double", "negate", "exit"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(cmd, valid_commands[i]) == 0) {
            return 1;
        }
    }
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
    
    long val = 0;
    int sign = 1;
    int i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i++;
    } else if (str[0] == '+') {
        i++;
    }
    
    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
        
        val = val * 10 + (str[i] - '0');
        if (val * sign > INT_MAX || val * sign < INT_MIN) {
            return 0;
        }
    }
    
    *result = (int)(val * sign);
    return 1;
}

void process_command(struct StateMachine *sm) {
    sm->result = sm->number;
    
    if (strcmp(sm->command, "square") == 0) {
        long long temp = (long long)sm->number * sm->number;
        if (temp > INT_MAX || temp < INT_MIN) {
            sm->current_state = ERROR_STATE;
            return;
        }
        sm->result = (int)temp;
    } else if (strcmp(sm->command, "cube") == 0) {
        long long temp = (long long)sm->number * sm->number * sm->number;
        if (temp > INT_MAX || temp < INT_MIN) {
            sm->current_state = ERROR_STATE;
            return;
        }
        sm->result = (int)temp;
    } else if (strcmp(sm->command, "double") == 0) {
        long long temp = (long long)sm->number * 2;
        if (temp > INT_MAX || temp < INT_MIN) {
            sm->current_state = ERROR_STATE;
            return;
        }
        sm->result = (int)temp;
    } else if (strcmp(sm->command, "negate") == 0) {
        if (sm->number == INT_MIN) {
            sm->current_state = ERROR_STATE;
            return;
        }
        sm->result = -sm->number;
    }
    
    sm->current_state = DISPLAY_RESULT;
}

int main(void) {
    struct StateMachine sm;
    char input[64];
    
    state_machine_init(&sm);
    
    printf("Number Processor State Machine\n");
    printf("Commands: square, cube, double, negate, exit\n\n");
    
    while (sm.current_state != EXIT_STATE) {
        switch (sm.current_state) {
            case INIT:
                printf("Enter command: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm.current_state = ERROR_STATE;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (strlen(input) >= sizeof(sm.command)) {
                    sm.current_state = ERROR_STATE;
                    break;
                }
                
                strcpy(sm.command, input);
                sm.current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                if (!is_valid_command(sm.command)) {
                    printf("Invalid command: %s\n", sm.command);
                    sm.current_state = INIT;
                    break;
                }
                
                if (strcmp(sm.command, "exit") == 0) {
                    sm.current_state = EXIT_STATE;
                    break;
                }
                
                printf("Enter number: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm.current_state = ERROR_STATE;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (!is_valid_number(input)) {
                    printf("Invalid number format\n");
                    sm.current_state = INIT;
                    break;
                }
                
                if (!safe_str_to_int(input, &sm.number))