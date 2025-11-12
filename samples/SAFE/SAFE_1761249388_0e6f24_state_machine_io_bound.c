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

void handle_init(struct StateMachine *sm) {
    printf("State Machine Started\n");
    printf("Commands: number <value>, text <string>, exit\n");
    sm->current_state = READ_COMMAND;
}

void handle_read_command(struct StateMachine *sm) {
    char input[128];
    printf("> ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current_state = EXIT;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strncmp(input, "number ", 7) == 0) {
        const char *num_str = input + 7;
        if (is_valid_number(num_str)) {
            int num = parse_number(num_str);
            if (num >= 0) {
                sm->number = num;
                sm->current_state = PROCESS_NUMBER;
            } else {
                sm->current_state = ERROR;
            }
        } else {
            sm->current_state = ERROR;
        }
    } else if (strncmp(input, "text ", 5) == 0) {
        const char *text_str = input + 5;
        if (is_valid_text(text_str) && strlen(text_str) < sizeof(sm->text)) {
            strncpy(sm->text, text_str, sizeof(sm->text) - 1);
            sm->text[sizeof(sm->text) - 1] = '\0';
            sm->text_len = strlen(sm->text);
            sm->current_state = PROCESS_TEXT;
        } else {
            sm->current_state = ERROR;
        }
    } else if (strcmp(input, "exit") == 0) {
        sm->current_state = EXIT;
    } else {
        sm->current_state = ERROR;
    }
}

void handle_process_number(struct StateMachine *sm) {
    printf("Processing number: %d\n", sm->number);
    printf("Number squared: %d\n", sm->number * sm->number);
    sm->current_state = READ_COMMAND;
}

void handle_process_text(struct StateMachine *sm) {
    printf("Processing text: %s\n", sm->text);
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
    sm->current_state = READ_COMMAND;
}

void handle_error(struct StateMachine *sm) {
    printf("Error: Invalid command or input\n");
    sm->current_state = READ_COMMAND;
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    while (sm.current_state != EXIT) {
        switch (sm.current_state) {
            case INIT:
                handle_init(&sm);
                break;
            case READ_COMMAND:
                handle_read_command(&sm);
                break;
            case PROCESS_NUMBER:
                handle_process_number(&sm);
                break;
            case PROCESS_TEXT:
                handle_process_text(&sm);
                break;
            case ERROR:
                handle_error(&sm);
                break;
            case EXIT:
                break;
        }
    }
    
    printf("State Machine Terminated\n");
    return 0;
}