//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    enum State current_state;
    int number;
    char text[64];
    int text_len;
};

void init_state_machine(struct StateMachine *sm) {
    sm->current_state = INIT;
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

int parse_number(const char *str) {
    long val = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (val > 214748364) return -1;
        val = val * 10 + (str[i] - '0');
        if (val > 2147483647) return -1;
    }
    return (int)val;
}

void process_state_machine(struct StateMachine *sm) {
    char input[128];
    
    while (sm->current_state != EXIT) {
        switch (sm->current_state) {
            case INIT:
                printf("State Machine Started\n");
                printf("Commands: number, text, exit\n");
                sm->current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                printf("Enter command: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (strcmp(input, "number") == 0) {
                    sm->current_state = PROCESS_NUMBER;
                } else if (strcmp(input, "text") == 0) {
                    sm->current_state = PROCESS_TEXT;
                } else if (strcmp(input, "exit") == 0) {
                    sm->current_state = EXIT;
                } else {
                    printf("Invalid command\n");
                }
                break;
                
            case PROCESS_NUMBER:
                printf("Enter a number (0-2147483647): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (!is_valid_number(input)) {
                    printf("Invalid number format\n");
                    sm->current_state = READ_COMMAND;
                    break;
                }
                
                int num = parse_number(input);
                if (num == -1) {
                    printf("Number too large\n");
                    sm->current_state = READ_COMMAND;
                    break;
                }
                
                sm->number = num;
                printf("Number stored: %d\n", num);
                sm->current_state = READ_COMMAND;
                break;
                
            case PROCESS_TEXT:
                printf("Enter text (letters and spaces only, max 63 chars): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    sm->current_state = ERROR;
                    break;
                }
                input[strcspn(input, "\n")] = '\0';
                
                if (!is_valid_text(input)) {
                    printf("Invalid text format\n");
                    sm->current_state = READ_COMMAND;
                    break;
                }
                
                size_t len = strlen(input);
                if (len >= sizeof(sm->text)) {
                    printf("Text too long\n");
                    sm->current_state = READ_COMMAND;
                    break;
                }
                
                strncpy(sm->text, input, sizeof(sm->text) - 1);
                sm->text[sizeof(sm->text) - 1] = '\0';
                sm->text_len = (int)len;
                printf("Text stored: %s\n", sm->text);
                sm->current_state = READ_COMMAND;
                break;
                
            case ERROR:
                printf("An error occurred\n");
                sm->current_state = EXIT;
                break;
                
            default:
                sm->current_state = ERROR;
                break;
        }
    }
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    process_state_machine(&sm);
    printf("State Machine Terminated\n");
    return 0;
}