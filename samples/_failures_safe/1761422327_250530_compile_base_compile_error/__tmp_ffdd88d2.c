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
    if (str == NULL || *str == '\0') return 0;
    
    int has_digits = 0;
    int i = 0;
    
    if (str[i] == '-' || str[i] == '+') i++;
    
    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
        has_digits = 1;
    }
    
    return has_digits;
}

int safe_str_to_int(const char *str, int *result) {
    if (str == NULL || result == NULL) return 0;
    
    char *endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (val > INT_MAX || val < INT_MIN) return 0;
    
    *result = (int)val;
    return 1;
}

void process_command(struct StateMachine *sm) {
    switch (sm->current_state) {
        case INIT:
            printf("Enter a command (square, cube, double, negate, exit): ");
            sm->current_state = READ_COMMAND;
            break;
            
        case READ_COMMAND:
            if (fgets(sm->command, sizeof(sm->command), stdin) != NULL) {
                size_t len = strlen(sm->command);
                if (len > 0 && sm->command[len - 1] == '\n') {
                    sm->command[len - 1] = '\0';
                }
                
                if (is_valid_command(sm->command)) {
                    if (strcmp(sm->command, "exit") == 0) {
                        sm->current_state = EXIT_STATE;
                    } else {
                        printf("Enter a number: ");
                        sm->current_state = PROCESS_NUMBER;
                    }
                } else {
                    printf("Invalid command. ");
                    sm->current_state = INIT;
                }
            } else {
                sm->current_state = ERROR_STATE;
            }
            break;
            
        case PROCESS_NUMBER:
            {
                char input[64];
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    
                    if (is_valid_number(input) && safe_str_to_int(input, &sm->number)) {
                        if (strcmp(sm->command, "square") == 0) {
                            long long temp = (long long)sm->number * sm->number;
                            if (temp > INT_MAX || temp < INT_MIN) {
                                sm->current_state = ERROR_STATE;
                            } else {
                                sm->result = (int)temp;
                                sm->current_state = DISPLAY_RESULT;
                            }
                        } else if (strcmp(sm->command, "cube") == 0) {
                            long long temp = (long long)sm->number * sm->number * sm->number;
                            if (temp > INT_MAX || temp < INT_MIN) {
                                sm->current_state = ERROR_STATE;
                            } else {
                                sm->result = (int)temp;
                                sm->current_state = DISPLAY_RESULT;
                            }
                        } else if (strcmp(sm->command, "double") == 0) {
                            long long temp = (long long)sm->number * 2;
                            if (temp > INT_MAX || temp < INT_MIN) {
                                sm->current_state = ERROR_STATE;
                            } else {
                                sm->result = (int)temp;
                                sm->current_state = DISPLAY_RESULT;
                            }
                        } else if (strcmp(sm->command, "negate") == 0) {
                            if (sm->number == INT_MIN) {
                                sm->current_state = ERROR_STATE;
                            } else {
                                sm->result = -sm->number;
                                sm->current_state = DISPLAY_RESULT;
                            }
                        }
                    } else {
                        printf("Invalid number. ");
                        sm->current_state = INIT;
                    }
                } else {
                    sm->current_state = ERROR_STATE;
                }
            }
            break;
            
        case DISPLAY_RESULT:
            printf("Result: %d\n\n", sm->result);
            sm->current_state = INIT;