//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    char input[256];
    char buffer[256];
    size_t buf_pos = 0;
    enum state current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_DONE;
                } else if (isdigit((unsigned char)c)) {
                    if (buf_pos < sizeof(buffer) - 1) {
                        buffer[buf_pos++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (buf_pos < sizeof(buffer) - 1) {
                        buffer[buf_pos++] = c;
                    }
                    current_state = STATE_READING_WORD;
                } else if (!isspace((unsigned char)c)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buf_pos > 0) {
                        buffer[buf_pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                        buf_pos = 0;
                    }
                    current_state = (c == '\0') ? STATE_DONE : STATE_START;
                } else if (isdigit((unsigned char)c)) {
                    if (buf_pos < sizeof(buffer) - 1) {
                        buffer[buf_pos++] = c;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buf_pos > 0) {
                        buffer[buf_pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                        buf_pos = 0;
                    }
                    current_state = (c == '\0') ? STATE_DONE : STATE_START;
                } else if (isalpha((unsigned char)c)) {
                    if (buf_pos < sizeof(buffer) - 1) {
                        buffer[buf_pos++] = c;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid character at position %zu\n", i);
                buf_pos = 0;
                if (c == '\0') {
                    current_state = STATE_DONE;
                } else if (isspace((unsigned char)c)) {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_DONE) {
            break;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}