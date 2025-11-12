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
    return isalnum(c) || isspace(c) || c == '.' || c == ',';
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
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if ((buf_ptr - buffer) < MAX_INPUT - 1) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                } else if (isspace(*ptr)) {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = START;
                } else {
                    current_state = SKIPPING;
                    buf_ptr = buffer;
                }
                break;
                
            case READING_WORD:
                if (isalpha(*ptr)) {
                    if ((buf_ptr - buffer) < MAX_INPUT - 1) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                } else if (isspace(*ptr)) {
                    *buf_ptr = '\0';
                    printf("Found word: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = START;
                } else {
                    current_state = SKIPPING;
                    buf_ptr = buffer;
                }
                break;
                
            case SKIPPING:
                if (isspace(*ptr)) {
                    current_state = START;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        ptr++;
    }
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        if ((buf_ptr - buffer) > 0 && (buf_ptr - buffer) < MAX_INPUT) {
            *buf_ptr = '\0';
            if (current_state == READING_NUMBER) {
                printf("Found number: %s\n", buffer);
            } else {
                printf("Found word: %s\n", buffer);
            }
        }
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_input(input);
    
    return 0;
}