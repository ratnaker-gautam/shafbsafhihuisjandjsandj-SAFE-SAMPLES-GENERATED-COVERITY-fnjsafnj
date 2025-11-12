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
    if (input == NULL || len == 0 || len >= MAX_INPUT) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 1;
        if (!isprint((unsigned char)input[i])) return 0;
    }
    return 1;
}

void process_input(const char *input) {
    if (input == NULL) return;
    
    State current_state = START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != FINISHED && *ptr != '\0') {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *ptr;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *ptr;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    if ((size_t)(buf_ptr - buffer) < MAX_INPUT - 1) {
                        *(buf_ptr++) = *ptr;
                    }
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)*ptr)) {
                    if ((size_t)(buf_ptr - buffer) < MAX_INPUT - 1) {
                        *(buf_ptr++) = *ptr;
                    }
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found word: %s\n", buffer);
                    current_state = START;
                }
                break;
                
            case SKIPPING:
                if (isdigit((unsigned char)*ptr) || isalpha((unsigned char)*ptr)) {
                    current_state = START;
                } else {
                    ptr++;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        if (*ptr == '\0') {
            if (current_state == READING_NUMBER || current_state == READING_WORD) {
                *buf_ptr = '\0';
                if (current_state == READING_NUMBER) {
                    printf("Found number: %s\n", buffer);
                } else {
                    printf("Found word: %s\n", buffer);
                }
            }
            current_state = FINISHED;
        }
    }
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input, len)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    process_input(input);
    
    return 0;
}