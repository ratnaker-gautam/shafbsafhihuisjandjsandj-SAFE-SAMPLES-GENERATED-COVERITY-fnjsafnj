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

void init_machine(struct StateMachine *sm) {
    sm->current = INIT;
    sm->number = 0;
    sm->text_len = 0;
    memset(sm->text, 0, sizeof(sm->text));
}

int is_valid_number(const char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_valid_text(const char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

int parse_number(const char *str) {
    int result = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (result > 1000000) return -1;
        result = result * 10 + (str[i] - '0');
    }
    return result;
}

void handle_init(struct StateMachine *sm) {
    printf("State machine initialized. Enter commands:\n");
    printf("number <value> - process a number\n");
    printf("text <string> - process text\n");
    printf("exit - terminate program\n");
    sm->current = READ_COMMAND;
}

void handle_read_command(struct StateMachine *sm) {
    char input[128];
    char command[32];
    char argument[96];
    
    printf("> ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = EXIT;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int parsed = sscanf(input, "%31s %95[^\n]", command, argument);
    
    if (parsed < 1) {
        printf("Invalid command format\n");
        return;
    }
    
    if (strcmp(command, "number") == 0) {
        if (parsed < 2) {
            printf("Missing number argument\n");
            return;
        }
        if (!is_valid_number(argument)) {
            printf("Invalid number format\n");
            return;
        }
        sm->number = parse_number(argument);
        if (sm->number < 0) {
            printf("Number too large\n");
            return;
        }
        sm->current = PROCESS_NUMBER;
    } else if (strcmp(command, "text") == 0) {
        if (parsed < 2) {
            printf("Missing text argument\n");
            return;
        }
        if (!is_valid_text(argument)) {
            printf("Invalid text format (letters and spaces only)\n");
            return;
        }
        if (strlen(argument) >= sizeof(sm->text)) {
            printf("Text too long\n");
            return;
        }
        strncpy(sm->text, argument, sizeof(sm->text) - 1);
        sm->text[sizeof(sm->text) - 1] = '\0';
        sm->text_len = strlen(sm->text);
        sm->current = PROCESS_TEXT;
    } else if (strcmp(command, "exit") == 0) {
        sm->current = EXIT;
    } else {
        printf("Unknown command: %s\n", command);
    }
}

void handle_process_number(struct StateMachine *sm) {
    printf("Processing number: %d\n", sm->number);
    printf("Square: %d\n", sm->number * sm->number);
    printf("Double: %d\n", sm->number * 2);
    sm->current = READ_COMMAND;
}

void handle_process_text(struct StateMachine *sm) {
    printf("Processing text: %s\n", sm->text);
    printf("Length: %d\n", sm->text_len);
    
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
    init_machine(&sm);
    
    while (sm.current != EXIT) {
        switch (sm.current) {
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
                printf("Error state reached\n");
                sm.current = READ_COMMAND;
                break;
            case EXIT:
                break;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}