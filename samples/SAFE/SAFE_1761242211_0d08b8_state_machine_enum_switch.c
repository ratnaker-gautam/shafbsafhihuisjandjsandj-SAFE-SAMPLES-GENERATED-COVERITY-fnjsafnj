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
        current_state = STATE_ERROR;
    }
    
    for (size_t i = 0; i <= input_len && current_state != STATE_ERROR; i++) {
        char c = (i < input_len) ? input[i] : ' ';
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_pos = 0;
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = tolower(c);
                    }
                    current_state = STATE_READING_WORD;
                } else if (c != ' ' && c != '\t' && c != '\0') {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    current_state = (c == '\0') ? STATE_DONE : STATE_START;
                } else if (isalpha(c)) {
                    current_state = STATE_ERROR;
                } else {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                }
                break;
                
            case STATE_READING_WORD:
                if (isalnum(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = tolower(c);
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    current_state = (c == '\0') ? STATE_DONE : STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}