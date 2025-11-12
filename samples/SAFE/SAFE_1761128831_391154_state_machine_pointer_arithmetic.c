//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    char *end = input + MAX_INPUT_LEN;
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
    }
    
    for (char *p = input; *p != '\0'; p++) {
        if (!is_valid_char(*p)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    while (ptr < end && *ptr != '\0') {
        switch (current_state) {
            case STATE_START:
                if (isdigit(*ptr)) {
                    current_state = STATE_NUMBER;
                    number_count++;
                } else if (isalpha(*ptr)) {
                    current_state = STATE_WORD;
                    word_count++;
                } else {
                    current_state = STATE_READING;
                }
                break;
                
            case STATE_READING:
                if (isdigit(*ptr)) {
                    current_state = STATE_NUMBER;
                    number_count++;
                } else if (isalpha(*ptr)) {
                    current_state = STATE_WORD;
                    word_count++;
                }
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
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_END) {
            break;
        }
        
        ptr++;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}