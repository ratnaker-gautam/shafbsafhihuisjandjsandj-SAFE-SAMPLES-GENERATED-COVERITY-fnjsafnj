//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_WORD,
    READING_NUMBER,
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
    int word_count = 0;
    int number_count = 0;
    
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
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    while (*ptr != '\0' && current_state != FINISHED) {
        switch (current_state) {
            case START:
                if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case READING_WORD:
                if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else if (!isalpha((unsigned char)*ptr)) {
                    if (isdigit((unsigned char)*ptr)) {
                        current_state = READING_NUMBER;
                        number_count++;
                    } else {
                        current_state = FINISHED;
                    }
                }
                break;
                
            case READING_NUMBER:
                if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else if (!isdigit((unsigned char)*ptr)) {
                    if (isalpha((unsigned char)*ptr)) {
                        current_state = READING_WORD;
                        word_count++;
                    } else {
                        current_state = FINISHED;
                    }
                }
                break;
                
            case SKIPPING:
                if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (!isspace((unsigned char)*ptr)) {
                    current_state = FINISHED;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        if (current_state != FINISHED) {
            ptr++;
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}