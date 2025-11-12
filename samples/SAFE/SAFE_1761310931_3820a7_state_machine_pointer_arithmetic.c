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
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n';
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != FINISHED && *ptr != '\0') {
        if (!is_valid_char(*ptr)) {
            current_state = FINISHED;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    buf_ptr = buffer;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    buf_ptr = buffer;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;
                
            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if ((buf_ptr - buffer) < MAX_INPUT - 1) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha(*ptr)) {
                    if ((buf_ptr - buffer) < MAX_INPUT - 1) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found word: %s\n", buffer);
                    current_state = START;
                }
                break;
                
            case SKIPPING:
                if (isdigit(*ptr) || isalpha(*ptr)) {
                    current_state = START;
                } else {
                    ptr++;
                }
                break;
                
            case FINISHED:
                break;
        }
    }
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        *buf_ptr = '\0';
        if (current_state == READING_NUMBER) {
            printf("Found number: %s\n", buffer);
        } else {
            printf("Found word: %s\n", buffer);
        }
    }
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
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    process_input(input);
    
    return 0;
}