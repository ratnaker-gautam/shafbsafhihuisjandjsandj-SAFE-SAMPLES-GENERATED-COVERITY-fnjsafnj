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
    int accumulator;
    int number_count;
    int numbers[10];
};

void init_state_machine(struct StateMachine *sm) {
    sm->current_state = INIT;
    sm->accumulator = 0;
    sm->number_count = 0;
    memset(sm->numbers, 0, sizeof(sm->numbers));
}

int is_valid_number(const char *input) {
    if (input == NULL || strlen(input) == 0) {
        return 0;
    }
    
    for (size_t i = 0; i < strlen(input); i++) {
        if (i == 0 && input[i] == '-') {
            continue;
        }
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    
    return 1;
}

int parse_number(const char *input) {
    long value = strtol(input, NULL, 10);
    if (value < -1000 || value > 1000) {
        return 0;
    }
    return (int)value;
}

void handle_init_state(struct StateMachine *sm) {
    printf("State Machine Calculator\n");
    printf("Commands: add <number>, show, reset, exit\n");
    sm->current_state = READ_COMMAND;
}

void handle_read_command(struct StateMachine *sm) {
    char input[256];
    printf("> ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = ERROR_STATE;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "exit") == 0) {
        sm->current_state = EXIT_STATE;
    } else if (strcmp(input, "show") == 0) {
        sm->current_state = DISPLAY_RESULT;
    } else if (strcmp(input, "reset") == 0) {
        init_state_machine(sm);
        sm->current_state = INIT;
    } else if (strncmp(input, "add ", 4) == 0) {
        char *number_str = input + 4;
        if (is_valid_number(number_str)) {
            int number = parse_number(number_str);
            if (number != 0 || strcmp(number_str, "0") == 0) {
                if (sm->number_count < 10) {
                    sm->numbers[sm->number_count] = number;
                    sm->number_count++;
                    sm->accumulator += number;
                    sm->current_state = PROCESS_NUMBER;
                } else {
                    printf("Error: Maximum numbers reached\n");
                    sm->current_state = READ_COMMAND;
                }
            } else {
                printf("Error: Invalid number range\n");
                sm->current_state = READ_COMMAND;
            }
        } else {
            printf("Error: Invalid number format\n");
            sm->current_state = READ_COMMAND;
        }
    } else {
        printf("Error: Unknown command\n");
        sm->current_state = READ_COMMAND;
    }
}

void handle_process_number(struct StateMachine *sm) {
    printf("Number added successfully\n");
    sm->current_state = READ_COMMAND;
}

void handle_display_result(struct StateMachine *sm) {
    printf("Current numbers: ");
    for (int i = 0; i < sm->number_count; i++) {
        printf("%d", sm->numbers[i]);
        if (i < sm->number_count - 1) {
            printf(", ");
        }
    }
    printf("\n");
    printf("Accumulated sum: %d\n", sm->accumulator);
    sm->current_state = READ_COMMAND;
}

void handle_error_state(struct StateMachine *sm) {
    printf("Error: Input error occurred\n");
    sm->current_state = READ_COMMAND;
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    while (sm.current_state != EXIT_STATE) {
        switch (sm.current_state) {
            case INIT:
                handle_init_state(&sm);
                break;
            case READ_COMMAND:
                handle_read_command(&sm);
                break;
            case PROCESS_NUMBER:
                handle_process_number(&sm);
                break;
            case DISPLAY_RESULT:
                handle_display_result(&sm);
                break;
            case ERROR_STATE:
                handle_error_state(&sm);
                break;
            case EXIT_STATE:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}