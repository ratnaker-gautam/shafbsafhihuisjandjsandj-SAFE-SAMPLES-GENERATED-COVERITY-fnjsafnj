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
    return isalnum((unsigned char)c) || isspace((unsigned char)c) || c == '.' || c == ',';
}

int is_digit_char(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != FINISHED && *ptr != '\0') {
        switch (current_state) {
            case START:
                if (is_digit_char(*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                    printf("Found number: ");
                    putchar(*ptr);
                } else if (is_alpha_char(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                    printf("Found word: ");
                    putchar(*ptr);
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else {
                    ptr++;
                }
                break;
                
            case READING_NUMBER:
                if (is_digit_char(*ptr)) {
                    putchar(*ptr);
                    ptr++;
                } else if (isspace((unsigned char)*ptr) || *ptr == ',' || *ptr == '.') {
                    putchar('\n');
                    current_state = SKIPPING;
                } else {
                    putchar('\n');
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (is_alpha_char(*ptr)) {
                    putchar(*ptr);
                    ptr++;
                } else if (isspace((unsigned char)*ptr) || *ptr == ',' || *ptr == '.') {
                    putchar('\n');
                    current_state = SKIPPING;
                } else {
                    putchar('\n');
                    current_state = START;
                }
                break;
                
            case SKIPPING:
                if (isspace((unsigned char)*ptr) || *ptr == ',' || *ptr == '.') {
                    ptr++;
                } else {
                    current_state = START;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        if (*ptr == '\0') {
            if (current_state == READING_NUMBER || current_state == READING_WORD) {
                putchar('\n');
            }
            current_state = FINISHED;
        }
    }
    
    printf("\nSummary: %d numbers, %d words found\n", number_count, word_count);
}

int main(void) {
    char input[MAX_INPUT + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    process_input(input);
    
    return 0;
}