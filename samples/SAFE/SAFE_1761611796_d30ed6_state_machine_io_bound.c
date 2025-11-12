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

int is_valid_command(const char *cmd) {
    if (cmd == NULL) return 0;
    if (strlen(cmd) == 0) return 0;
    
    const char *valid_commands[] = {"square", "double", "negate", "exit"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(cmd, valid_commands[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int safe_str_to_int(const char *str, int *value) {
    if (str == NULL || value == NULL) return 0;
    
    char *endptr;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') return 0;
    if (val < -1000000 || val > 1000000) return 0;
    
    *value = (int)val;
    return 1;
}

void process_state_machine(struct StateMachine *sm) {
    char input[64];
    
    while (sm->current_state != EXIT_STATE) {
        switch (sm->current_state) {
            case INIT:
                printf("Enter command (square/double/negate/exit): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    input[strcspn(input, "\n")] = '\0';
                    if (strlen(input) < sizeof(sm->command)) {
                        strcpy(sm->command, input);
                        sm->current_state = READ_COMMAND;
                    } else {
                        sm->current_state = ERROR_STATE;
                    }
                } else {
                    sm->current_state = ERROR_STATE;
                }
                break;
                
            case READ_COMMAND:
                if (is_valid_command(sm->command)) {
                    if (strcmp(sm->command, "exit") == 0) {
                        sm->current_state = EXIT_STATE;
                    } else {
                        printf("Enter a number (-1000000 to 1000000): ");
                        if (fgets(input, sizeof(input), stdin) != NULL) {
                            input[strcspn(input, "\n")] = '\0';
                            if (safe_str_to_int(input, &sm->number)) {
                                sm->current_state = PROCESS_NUMBER;
                            } else {
                                sm->current_state = ERROR_STATE;
                            }
                        } else {
                            sm->current_state = ERROR_STATE;
                        }
                    }
                } else {
                    sm->current_state = ERROR_STATE;
                }
                break;
                
            case PROCESS_NUMBER:
                if (strcmp(sm->command, "square") == 0) {
                    long long temp = (long long)sm->number * sm->number;
                    if (temp > 1000000000LL || temp < -1000000000LL) {
                        sm->current_state = ERROR_STATE;
                    } else {
                        sm->result = (int)temp;
                        sm->current_state = DISPLAY_RESULT;
                    }
                } else if (strcmp(sm->command, "double") == 0) {
                    long long temp = (long long)sm->number * 2;
                    if (temp > 1000000000LL || temp < -1000000000LL) {
                        sm->current_state = ERROR_STATE;
                    } else {
                        sm->result = (int)temp;
                        sm->current_state = DISPLAY_RESULT;
                    }
                } else if (strcmp(sm->command, "negate") == 0) {
                    if (sm->number == -2147483647 - 1) {
                        sm->current_state = ERROR_STATE;
                    } else {
                        sm->result = -sm->number;
                        sm->current_state = DISPLAY_RESULT;
                    }
                } else {
                    sm->current_state = ERROR_STATE;
                }
                break;
                
            case DISPLAY_RESULT:
                printf("Result: %d\n\n", sm->result);
                sm->current_state = INIT;
                break;
                
            case ERROR_STATE:
                printf("Error: Invalid input or operation failed\n\n");
                sm->current_state = INIT;
                break;
                
            case EXIT_STATE:
                break;
        }
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Calculator\n");
    printf("Available commands: square, double, negate, exit\n\n");
    
    process_state_machine(&sm);
    
    printf("Goodbye!\n");
    return 0;
}