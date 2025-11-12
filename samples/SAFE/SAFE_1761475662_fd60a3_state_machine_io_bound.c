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

int is_valid_text(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

int safe_strcpy(char *dest, size_t dest_size, const char *src) {
    if (dest == NULL || src == NULL || dest_size == 0) return 0;
    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return 1;
}

void process_init(struct StateMachine *sm) {
    printf("State Machine Started\n");
    printf("Commands: number, text, exit\n");
    sm->current = READ_COMMAND;
}

void process_read_command(struct StateMachine *sm) {
    char input[32];
    printf("Enter command: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "number") == 0) {
        sm->current = PROCESS_NUMBER;
    } else if (strcmp(input, "text") == 0) {
        sm->current = PROCESS_TEXT;
    } else if (strcmp(input, "exit") == 0) {
        sm->current = EXIT;
    } else {
        printf("Invalid command\n");
        sm->current = READ_COMMAND;
    }
}

void process_number(struct StateMachine *sm) {
    char input[32];
    printf("Enter number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_number(input)) {
        printf("Invalid number\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    long num = atol(input);
    if (num < 0 || num > 1000000) {
        printf("Number out of range\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    sm->number = (int)num;
    printf("Number stored: %d\n", sm->number);
    sm->current = READ_COMMAND;
}

void process_text(struct StateMachine *sm) {
    char input[128];
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        sm->current = ERROR;
        return;
    }
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_text(input)) {
        printf("Invalid text (letters and spaces only)\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    if (!safe_strcpy(sm->text, sizeof(sm->text), input)) {
        printf("Text too long\n");
        sm->current = READ_COMMAND;
        return;
    }
    
    sm->text_len = strlen(sm->text);
    printf("Text stored: %s\n", sm->text);
    sm->current = READ_COMMAND;
}

void process_exit(struct StateMachine *sm) {
    printf("Final number: %d\n", sm->number);
    printf("Final text: %s\n", sm->text);
    printf("Exiting state machine\n");
}

void process_error(struct StateMachine *sm) {
    printf("Error occurred\n");
    sm->current = EXIT;
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    while (sm.current != EXIT) {
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
                break;
            case ERROR:
                process_error(&sm);
                break;
            case EXIT:
                break;
        }
    }
    
    return 0;
}