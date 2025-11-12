//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    char buffer[256];
    char output[512] = {0};
    size_t output_pos = 0;
    enum state current_state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case STATE_START:
                if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = 'W';
                    }
                } else if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = 'N';
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = STATE_START;
                } else if (isdigit(c)) {
                    current_state = STATE_ERROR;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = STATE_START;
                } else if (isalpha(c)) {
                    current_state = STATE_ERROR;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            if (output_pos < sizeof(output) - 1) {
                output[output_pos++] = 'E';
            }
        }
    }
    
    if (output_pos < sizeof(output)) {
        output[output_pos] = '\0';
    }
    
    printf("State sequence: %s\n", output);
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    if (current_state == STATE_ERROR) {
        printf("Status: Invalid input format\n");
        return 1;
    } else {
        printf("Status: Valid input format\n");
    }
    
    return 0;
}