//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_NUMBER,
    STATE_WORD,
    STATE_END
} State;

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *ptr = input;
    State current_state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
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
    
    char *end = ptr + len;
    
    while (ptr < end) {
        if (!is_valid_char(*ptr)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isalpha(*ptr)) {
                    current_state = STATE_WORD;
                    word_count++;
                } else if (isdigit(*ptr)) {
                    current_state = STATE_NUMBER;
                    number_count++;
                } else {
                    current_state = STATE_READING;
                }
                ptr++;
                break;
                
            case STATE_READING:
                if (isalpha(*ptr)) {
                    current_state = STATE_WORD;
                    word_count++;
                } else if (isdigit(*ptr)) {
                    current_state = STATE_NUMBER;
                    number_count++;
                }
                ptr++;
                break;
                
            case STATE_WORD:
                if (!isalpha(*ptr)) {
                    if (isdigit(*ptr)) {
                        current_state = STATE_NUMBER;
                        number_count++;
                    } else {
                        current_state = STATE_READING;
                    }
                }
                ptr++;
                break;
                
            case STATE_NUMBER:
                if (!isdigit(*ptr)) {
                    if (isalpha(*ptr)) {
                        current_state = STATE_WORD;
                        word_count++;
                    } else {
                        current_state = STATE_READING;
                    }
                }
                ptr++;
                break;
                
            case STATE_END:
                break;
        }
    }
    
    current_state = STATE_END;
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}