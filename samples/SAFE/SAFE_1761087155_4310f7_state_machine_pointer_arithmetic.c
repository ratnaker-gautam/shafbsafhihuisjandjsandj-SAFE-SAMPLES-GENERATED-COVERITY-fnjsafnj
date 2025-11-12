//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

int main(void) {
    char input[MAX_INPUT + 1];
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
    if (len > 0 && *(ptr + len - 1) == '\n') {
        *(ptr + len - 1) = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(*(ptr + i))) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    while (current_state != FINISHED && ptr < input + len) {
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (!isdigit(*ptr)) {
                    if (isalpha(*ptr)) {
                        current_state = READING_WORD;
                        word_count++;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case READING_WORD:
                if (!isalpha(*ptr)) {
                    if (isdigit(*ptr)) {
                        current_state = READING_NUMBER;
                        number_count++;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case SKIPPING:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        ptr++;
        
        if (ptr >= input + len) {
            current_state = FINISHED;
        }
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}