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

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    State current_state = START;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
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
        printf("Empty input\n");
        return 0;
    }
    
    for (char *p = input; *p != '\0'; p++) {
        if (!is_valid_char(*p)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    while (current_state != FINISHED && ptr < input + len) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (!isdigit((unsigned char)*ptr)) {
                    number_count++;
                    if (isalpha((unsigned char)*ptr)) {
                        current_state = READING_WORD;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                ptr++;
                break;
                
            case READING_WORD:
                if (!isalpha((unsigned char)*ptr)) {
                    word_count++;
                    if (isdigit((unsigned char)*ptr)) {
                        current_state = READING_NUMBER;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                ptr++;
                break;
                
            case SKIPPING:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                } else {
                    ptr++;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        if (ptr >= input + len) {
            if (current_state == READING_NUMBER) {
                number_count++;
            } else if (current_state == READING_WORD) {
                word_count++;
            }
            current_state = FINISHED;
        }
    }
    
    printf("Found %d numbers and %d words\n", number_count, word_count);
    
    return 0;
}