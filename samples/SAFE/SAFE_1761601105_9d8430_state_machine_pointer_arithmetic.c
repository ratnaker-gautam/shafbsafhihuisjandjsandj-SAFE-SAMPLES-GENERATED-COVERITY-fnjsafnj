//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_input(char *input) {
    State current_state = START;
    char *ptr = input;
    char *end = input + strlen(input);
    
    while (current_state != FINISHED && ptr < end) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    printf("Found number: ");
                    putchar(*ptr);
                    ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    printf("Found word: ");
                    putchar(*ptr);
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                    ptr++;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    putchar(*ptr);
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    putchar('\n');
                    current_state = SKIPPING;
                    ptr++;
                } else {
                    putchar('\n');
                    current_state = FINISHED;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)*ptr)) {
                    putchar(*ptr);
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    putchar('\n');
                    current_state = SKIPPING;
                    ptr++;
                } else {
                    putchar('\n');
                    current_state = FINISHED;
                }
                break;
                
            case SKIPPING:
                if (isspace((unsigned char)*ptr)) {
                    ptr++;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    printf("Found number: ");
                    putchar(*ptr);
                    ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    printf("Found word: ");
                    putchar(*ptr);
                    ptr++;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case FINISHED:
                break;
        }
    }
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        putchar('\n');
    }
}

int main() {
    char input[MAX_INPUT + 1] = {0};
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    for (char *p = input; *p != '\0'; p++) {
        if (!is_valid_char(*p)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    process_input(input);
    
    return 0;
}