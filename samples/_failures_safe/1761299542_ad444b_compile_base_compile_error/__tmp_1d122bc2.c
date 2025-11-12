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

int is_valid_command(char c) {
    return c == 'n' || c == 't' || c == 'e' || c == 'q';
}

int is_valid_number_input(const char *input) {
    if (input == NULL || strlen(input) == 0) return 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (i == 0 && input[i] == '-') continue;
        if (!isdigit(input[i])) return 0;
    }
    return 1;
}

int is_valid_text_input(const char *input) {
    if (input == NULL || strlen(input) == 0) return 0;
    if (strlen(input) > 63) return 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int parse_number(const char *input) {
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') return 0;
    if (val < -1000000 || val > 1000000) return 0;
    return (int)val;
}

void process_init(struct StateMachine *sm) {
    printf("State Machine Demo\n");
    printf("Commands: n (number), t (text), e (execute), q (quit)\n");
    sm->current = READ_COMMAND;
}

void process_read_command(struct StateMachine *sm) {
    char input[16];
    printf("Enter command: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) != 1 || !is_valid_command(input[0])) {
        printf("Invalid command\n");
        return;
    }
    
    switch (input[0]) {
        case 'n':
            sm->current = PROCESS_NUMBER;
            break;
        case 't':
            sm->current = PROCESS_TEXT;
            break;
        case 'e':
            sm->current = EXIT;
            break;
        case 'q':
            sm->current = EXIT;
            break;
        default:
            sm->current = ERROR;
            break;
    }
}

void process_number(struct StateMachine *sm) {
    char input[32];
    printf("Enter number (-1000000 to 1000000): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_number_input(input)) {
        printf("Invalid number\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    int num = parse_number(input);
    if (num == 0 && strcmp(input, "0") != 0) {
        printf("Number out of range\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    sm->number = num;
    printf("Number stored: %d\n", sm->number);
    sm->current = READ_COMMAND;
}

void process_text(struct StateMachine *sm) {
    char input[64];
    printf("Enter text (max 63 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_text_input(input)) {
        printf("Invalid text\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    strncpy(sm->text, input, sizeof(sm->text) - 1);
    sm->text[sizeof(sm->text) - 1] = '\0';
    sm->text_len = strlen(sm->text);
    printf("Text stored: %s\n", sm->text);
    sm->current = READ_COMMAND;
}

void process_execute(struct StateMachine *sm) {
    printf("Executing with number=%d, text='%s'\n", sm->number, sm->text);
    printf("Processing...\n");
    
    int result = sm->number * sm->text_len;
    printf("Result (number * text_length): %d\n", result);
    
    sm->current = READ_COMMAND;
}

int main(void) {
    struct StateMachine sm;
    init_machine(&sm);
    
    while (1) {
        switch (sm.current) {
            case INIT:
                process_init(&sm);
                break;
            case READ_COMMAND:
                process_read_command(&sm);
                break;
            case PROCESS_NUMBER:
                process_number(&sm);
                break;
            case PROCESS_TEXT:
                process_text(&sm);