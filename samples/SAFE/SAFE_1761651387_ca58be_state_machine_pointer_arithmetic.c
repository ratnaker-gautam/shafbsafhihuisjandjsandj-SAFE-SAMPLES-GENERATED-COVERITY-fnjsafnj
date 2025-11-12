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

int is_valid_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') {
            return i == len;
        }
    }
    return 1;
}

void process_token(const char *start, const char *end, int token_type) {
    if (start == NULL || end == NULL || start > end) {
        return;
    }
    
    size_t len = end - start;
    if (len == 0 || len >= MAX_INPUT) {
        return;
    }
    
    char buffer[MAX_INPUT];
    if (len >= sizeof(buffer)) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (start + i >= end) {
            break;
        }
        buffer[i] = *(start + i);
    }
    buffer[len] = '\0';
    
    if (token_type == 1) {
        printf("Number: %s\n", buffer);
    } else if (token_type == 2) {
        printf("Word: %s\n", buffer);
    }
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter text (max %d chars): ", MAX_INPUT - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[--input_len] = '\0';
    }
    
    if (!is_valid_input(input, input_len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    State current_state = START;
    const char *token_start = NULL;
    const char *current_pos = input;
    const char *end_pos = input + input_len;
    
    while (current_state != FINISHED && current_pos < end_pos) {
        char c = *current_pos;
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    token_start = current_pos;
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    token_start = current_pos;
                    current_state = READING_WORD;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (!isdigit(c)) {
                    process_token(token_start, current_pos, 1);
                    current_state = START;
                    continue;
                }
                break;
                
            case READING_WORD:
                if (!isalpha(c)) {
                    process_token(token_start, current_pos, 2);
                    current_state = START;
                    continue;
                }
                break;
                
            case SKIPPING:
                if (isdigit(c)) {
                    token_start = current_pos;
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    token_start = current_pos;
                    current_state = READING_WORD;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        current_pos++;
        
        if (current_pos >= end_pos) {
            if (current_state == READING_NUMBER) {
                process_token(token_start, current_pos, 1);
            } else if (current_state == READING_WORD) {
                process_token(token_start, current_pos, 2);
            }
            current_state = FINISHED;
        }
    }
    
    return 0;
}