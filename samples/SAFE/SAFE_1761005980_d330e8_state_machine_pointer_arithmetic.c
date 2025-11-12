//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_DIGITS,
    READING_LETTERS,
    ERROR,
    FINISHED
} State;

State transition(State current, char c) {
    switch (current) {
        case START:
            if (isdigit(c)) return READING_DIGITS;
            if (isalpha(c)) return READING_LETTERS;
            return ERROR;
        case READING_DIGITS:
            if (isdigit(c)) return READING_DIGITS;
            if (c == '\0') return FINISHED;
            return ERROR;
        case READING_LETTERS:
            if (isalpha(c)) return READING_LETTERS;
            if (c == '\0') return FINISHED;
            return ERROR;
        case ERROR:
            return ERROR;
        case FINISHED:
            return FINISHED;
    }
    return ERROR;
}

void process_string(const char *input) {
    State current = START;
    const char *ptr = input;
    
    while (current != FINISHED && current != ERROR) {
        current = transition(current, *ptr);
        if (current == ERROR) {
            printf("Invalid input format\n");
            return;
        }
        ptr++;
    }
    
    if (current == FINISHED) {
        printf("Valid input: ");
        ptr = input;
        while (*ptr != '\0') {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    
    printf("Enter a string (digits only or letters only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\n') {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_string(input);
    
    return 0;
}