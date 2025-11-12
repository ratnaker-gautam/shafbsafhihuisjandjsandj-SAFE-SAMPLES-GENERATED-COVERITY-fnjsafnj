//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
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
        val = val * 10 + (str[i] - '0');
        if (val > 1000000) return -1;
    }
    return (int)val;
}

void process_state_machine(struct StateMachine *sm, const char *input) {
    switch (sm->current_state) {
        case INIT:
            printf("Enter command (number/text/quit): ");
            sm->current_state = READ_COMMAND;
            break;
            
        case READ_COMMAND:
            if (strcmp(input, "number") == 0) {
                printf("Enter a number (1-1000000): ");
                sm->current_state = PROCESS_NUMBER;
            } else if (strcmp(input, "text") == 0) {
                printf("Enter text (letters and spaces only): ");
                sm->current_state = PROCESS_TEXT;
            } else if (strcmp(input, "quit") == 0) {
                sm->current_state = EXIT;
            } else {
                printf("Invalid command. ");
                sm->current_state = INIT;
            }
            break;
            
        case PROCESS_NUMBER:
            if (is_valid_number(input)) {
                int num = parse_number(input);
                if (num >= 1 && num <= 1000000) {
                    sm->number = num;
                    printf("Number stored: %d\n", num);
                    sm->current_state = INIT;
                } else {
                    printf("Number out of range. ");
                    sm->current_state = READ_COMMAND;
                }
            } else {
                printf("Invalid number format. ");
                sm->current_state = READ_COMMAND;
            }
            break;
            
        case PROCESS_TEXT:
            if (is_valid_text(input) && strlen(input) < sizeof(sm->text)) {
                strncpy(sm->text, input, sizeof(sm->text) - 1);
                sm->text[sizeof(sm->text) - 1] = '\0';
                sm->text_len = strlen(sm->text);
                printf("Text stored: %s\n", sm->text);
                sm->current_state = INIT;
            } else {
                printf("Invalid text format or too long. ");
                sm->current_state = READ_COMMAND;
            }
            break;
            
        case EXIT:
            printf("Goodbye!\n");
            break;
            
        case ERROR:
            printf("Error occurred. Resetting.\n");
            sm->current_state = INIT;
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    char input[128];
    
    init_state_machine(&sm);
    
    while (sm.current_state != EXIT) {
        if (sm.current_state != INIT && sm.current_state != ERROR) {
            if (fgets(input, sizeof(input), stdin) != NULL) {
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                }
                if (len == 1 && input[0] == '\n') {
                    continue;
                }
                process_state_machine(&sm, input);
            } else {
                if (feof(stdin)) {
                    break;
                }
                sm.current_state = ERROR;
            }
        } else {
            process_state_machine(&sm, "");
        }
    }
    
    return 0;
}