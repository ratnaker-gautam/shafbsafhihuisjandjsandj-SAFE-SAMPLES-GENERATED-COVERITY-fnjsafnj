//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    FINISH
} State;

State transition(State current, char c) {
    switch (current) {
        case START:
            if (isdigit(c)) return READING_DIGITS;
            if (isalpha(c)) return READING_LETTERS;
            return ERROR;
        case READING_DIGITS:
            if (isdigit(c)) return READING_DIGITS;
            if (c == '\0') return FINISH;
            return ERROR;
        case READING_LETTERS:
            if (isalpha(c)) return READING_LETTERS;
            if (c == '\0') return FINISH;
            return ERROR;
        case ERROR:
            return ERROR;
        case FINISH:
            return FINISH;
    }
    return ERROR;
}

void process_input(const char *input) {
    State current = START;
    const char *ptr = input;
    
    while (current != FINISH && current != ERROR) {
        current = transition(current, *ptr);
        if (current != FINISH && current != ERROR) {
            ptr++;
        }
    }
    
    if (current == FINISH) {
        printf("Valid input: ");
        const char *start = input;
        while (*start != '\0') {
            putchar(*start);
            start++;
        }
        putchar('\n');
    } else {
        printf("Invalid input\n");
    }
}

int main() {
    char input[MAX_INPUT + 1];
    
    printf("Enter input (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len > MAX_INPUT) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    const char *ptr = input;
    while (*ptr != '\0') {
        if (!isalnum(*ptr)) {
            fprintf(stderr, "Input contains invalid characters\n");
            return 1;
        }
        ptr++;
    }
    
    process_input(input);
    
    return 0;
}