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
    enum State current;
    int number;
    char text[64];
    int text_len;
};

void state_machine_init(struct StateMachine *sm) {
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

int is_valid_text(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

void process_init_state(struct StateMachine *sm) {
    printf("State machine initialized. Enter commands:\n");
    printf("  number <value> - process a number\n");
    printf("  text <value>   - process text\n");
    printf("  exit           - exit program\n");
    sm->current = READ_COMMAND;
}

void process_read_command(struct StateMachine *sm) {
    char input[128];
    printf("> ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "exit") == 0) {
        sm->current = EXIT;
    } else if (strncmp(input, "number ", 7) == 0) {
        const char *num_str = input + 7;
        if (is_valid_number(num_str)) {
            sm->number = atoi(num_str);
            sm->current = PROCESS_NUMBER;
        } else {
            printf("Error: Invalid number format\n");
        }
    } else if (strncmp(input, "text ", 5) == 0) {
        const char *text_str = input + 5;
        if (is_valid_text(text_str)) {
            size_t len = strlen(text_str);
            if (len < sizeof(sm->text)) {
                strncpy(sm->text, text_str, sizeof(sm->text) - 1);
                sm->text[sizeof(sm->text) - 1] = '\0';
                sm->text_len = (int)len;
                sm->current = PROCESS_TEXT;
            } else {
                printf("Error: Text too long\n");
            }
        } else {
            printf("Error: Invalid text format (letters and spaces only)\n");
        }
    } else {
        printf("Error: Unknown command\n");
    }
}

void process_number_state(struct StateMachine *sm) {
    printf("Processing number: %d\n", sm->number);
    
    if (sm->number > 0) {
        printf("Number is positive\n");
    } else if (sm->number == 0) {
        printf("Number is zero\n");
    } else {
        printf("Number is negative\n");
    }
    
    int square = sm->number * sm->number;
    printf("Square: %d\n", square);
    
    sm->current = READ_COMMAND;
}

void process_text_state(struct StateMachine *sm) {
    printf("Processing text: '%s'\n", sm->text);
    printf("Text length: %d\n", sm->text_len);
    
    int word_count = 0;
    int in_word = 0;
    
    for (int i = 0; i < sm->text_len; i++) {
        if (isalpha(sm->text[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    sm->current = READ_COMMAND;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    while (sm.current != EXIT && sm.current != ERROR) {
        switch (sm.current) {
            case INIT:
                process_init_state(&sm);
                break;
            case READ_COMMAND:
                process_read_command(&sm);
                break;
            case PROCESS_NUMBER:
                process_number_state(&sm);
                break;
            case PROCESS_TEXT:
                process_text_state(&sm);
                break;
            case EXIT:
                break;
            case ERROR:
                printf("State machine error occurred\n");
                break;
        }
        
        if (sm.current == ERROR) {
            break;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}