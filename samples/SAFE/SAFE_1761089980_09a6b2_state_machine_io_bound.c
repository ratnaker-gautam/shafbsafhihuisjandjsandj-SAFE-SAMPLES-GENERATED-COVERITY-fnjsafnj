//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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

void init_state_machine(struct StateMachine *sm) {
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

int safe_atoi(const char *str, int *result) {
    if (str == NULL || result == NULL) return 0;
    
    long val = 0;
    int sign = 1;
    int i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
        val = val * 10 + (str[i] - '0');
        if (sign == 1) {
            if (val > INT_MAX) return 0;
        } else {
            if (-val < INT_MIN) return 0;
        }
    }
    
    *result = (int)(val * sign);
    return 1;
}

void process_state_machine(struct StateMachine *sm) {
    char input[128];
    
    while (sm->current != EXIT) {
        switch (sm->current) {
            case INIT:
                printf("State machine initialized. Available commands: number, text, exit\n");
                sm->current = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                printf("Enter command: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current = ERROR;
                    break;
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
                }
                break;
                
            case PROCESS_NUMBER:
                printf("Enter a number: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current = ERROR;
                    break;
                }
                
                input[strcspn(input, "\n")] = '\0';
                
                if (is_valid_number(input)) {
                    if (safe_atoi(input, &sm->number)) {
                        printf("Number processed: %d\n", sm->number);
                    } else {
                        printf("Number out of range\n");
                    }
                } else {
                    printf("Invalid number format\n");
                }
                sm->current = READ_COMMAND;
                break;
                
            case PROCESS_TEXT:
                printf("Enter text (max 63 chars): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current = ERROR;
                    break;
                }
                
                input[strcspn(input, "\n")] = '\0';
                
                size_t len = strlen(input);
                if (len > 0 && len < sizeof(sm->text)) {
                    strncpy(sm->text, input, sizeof(sm->text) - 1);
                    sm->text[sizeof(sm->text) - 1] = '\0';
                    sm->text_len = (int)len;
                    printf("Text processed: '%s' (length: %d)\n", sm->text, sm->text_len);
                } else if (len >= sizeof(sm->text)) {
                    printf("Text too long. Maximum %zu characters allowed.\n", sizeof(sm->text) - 1);
                } else {
                    printf("No text entered\n");
                }
                sm->current = READ_COMMAND;
                break;
                
            case ERROR:
                printf("An error occurred. Resetting state machine.\n");
                init_state_machine(sm);
                break;
                
            default:
                sm->current = ERROR;
                break;
        }
        
        if (sm->current == ERROR) {
            printf("Fatal error occurred. Exiting.\n");
            break;
        }
    }
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    process_state_machine(&sm);
    printf("State machine terminated.\n");
    return 0;
}