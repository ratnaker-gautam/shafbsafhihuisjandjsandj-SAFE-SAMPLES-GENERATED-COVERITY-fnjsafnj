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
    int numbers_found = 0;
    int words_found = 0;
    State current_state = START;
    
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
    
    while (*ptr != '\0' && current_state != FINISHED) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    numbers_found++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    words_found++;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (!isdigit((unsigned char)*ptr)) {
                    if (isalpha((unsigned char)*ptr)) {
                        current_state = READING_WORD;
                        words_found++;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case READING_WORD:
                if (!isalpha((unsigned char)*ptr)) {
                    if (isdigit((unsigned char)*ptr)) {
                        current_state = READING_NUMBER;
                        numbers_found++;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case SKIPPING:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    numbers_found++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    words_found++;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        ptr++;
        
        if (*ptr == '\0') {
            current_state = FINISHED;
        }
    }
    
    printf("Numbers found: %d\n", numbers_found);
    printf("Words found: %d\n", words_found);
    
    return 0;
}