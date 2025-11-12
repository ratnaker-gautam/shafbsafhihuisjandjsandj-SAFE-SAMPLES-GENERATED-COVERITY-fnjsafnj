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
    int buffer_pos = 0;
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
    
    if (input_len >= sizeof(input)) {
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_DONE;
                } else if (isdigit((unsigned char)c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    current_state = STATE_READING_WORD;
                } else if (!isspace((unsigned char)c)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    if (c == '\0') {
                        current_state = STATE_DONE;
                    } else {
                        current_state = STATE_START;
                    }
                } else if (isdigit((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || isspace((unsigned char)c)) {
                    if (buffer_pos > 0 && buffer_pos < sizeof(buffer)) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    if (c == '\0') {
                        current_state = STATE_DONE;
                    } else {
                        current_state = STATE_START;
                    }
                } else if (isalnum((unsigned char)c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Error: Invalid input format\n");
            return 1;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}