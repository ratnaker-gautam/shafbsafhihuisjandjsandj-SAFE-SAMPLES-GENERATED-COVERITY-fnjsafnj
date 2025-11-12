//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBERS,
    READING_WORDS,
    ERROR,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

int is_digit_char(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    char number_buffer[32];
    char word_buffer[32];
    char *num_ptr = number_buffer;
    char *word_ptr = word_buffer;
    
    while (*ptr != '\0' && current_state != ERROR && current_state != FINISHED) {
        if (!is_valid_char(*ptr)) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (is_digit_char(*ptr)) {
                    current_state = READING_NUMBERS;
                    num_ptr = number_buffer;
                    *num_ptr = *ptr;
                    num_ptr++;
                } else if (is_alpha_char(*ptr)) {
                    current_state = READING_WORDS;
                    word_ptr = word_buffer;
                    *word_ptr = *ptr;
                    word_ptr++;
                } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    ptr++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBERS:
                if (is_digit_char(*ptr)) {
                    if ((size_t)(num_ptr - number_buffer) < sizeof(number_buffer) - 1) {
                        *num_ptr = *ptr;
                        num_ptr++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    *num_ptr = '\0';
                    printf("Found number: %s\n", number_buffer);
                    current_state = START;
                } else if (is_alpha_char(*ptr)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORDS:
                if (is_alpha_char(*ptr)) {
                    if ((size_t)(word_ptr - word_buffer) < sizeof(word_buffer) - 1) {
                        *word_ptr = *ptr;
                        word_ptr++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    *word_ptr = '\0';
                    printf("Found word: %s\n", word_buffer);
                    current_state = START;
                } else if (is_digit_char(*ptr)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case FINISHED:
                break;
        }
        
        ptr++;
    }
    
    if (current_state == READING_NUMBERS) {
        *num_ptr = '\0';
        printf("Found number: %s\n", number_buffer);
    } else if (current_state == READING_WORDS) {
        *word_ptr = '\0';
        printf("Found word: %s\n", word_buffer);
    } else if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
    }
}

int main() {
    char input[MAX_INPUT + 1];
    
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
    
    if (len >= MAX_INPUT) {
        printf("Warning: Input may have been truncated\n");
    }
    
    process_input(input);
    
    return 0;
}