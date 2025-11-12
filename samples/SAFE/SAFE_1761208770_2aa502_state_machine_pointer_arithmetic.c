//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUM,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != FINISHED && *ptr != '\0') {
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUM;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *(ptr++);
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *(ptr++);
                } else {
                    current_state = SKIPPING;
                    ptr++;
                }
                break;
                
            case READING_NUM:
                if (isdigit(*ptr)) {
                    *(buf_ptr++) = *(ptr++);
                } else {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha(*ptr)) {
                    *(buf_ptr++) = *(ptr++);
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
                
            default:
                current_state = FINISHED;
                break;
        }
        
        if (buf_ptr - buffer >= MAX_INPUT - 1) {
            *buffer = '\0';
            printf("Buffer overflow prevented\n");
            current_state = START;
            buf_ptr = buffer;
        }
    }
    
    if (current_state == READING_NUM || current_state == READING_WORD) {
        *buf_ptr = '\0';
        if (current_state == READING_NUM) {
            printf("Found number: %s\n", buffer);
        } else {
            printf("Found word: %s\n", buffer);
        }
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}