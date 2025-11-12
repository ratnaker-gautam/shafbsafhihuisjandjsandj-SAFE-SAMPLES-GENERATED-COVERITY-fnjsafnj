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
    char buffer[256];
    char output[512] = {0};
    size_t output_pos = 0;
    enum state current_state = STATE_START;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_DONE;
                } else if (isdigit((unsigned char)c)) {
                    current_state = STATE_READING_NUMBER;
                    int digit = c - '0';
                    number_sum += digit;
                } else if (isalpha((unsigned char)c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (c != ' ' && c != '\t') {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0') {
                    current_state = STATE_DONE;
                } else if (isdigit((unsigned char)c)) {
                    int digit = c - '0';
                    number_sum += digit;
                } else if (isalpha((unsigned char)c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (c == ' ' || c == '\t') {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0') {
                    current_state = STATE_DONE;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ' ';
                    }
                } else if (isalpha((unsigned char)c)) {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (isdigit((unsigned char)c)) {
                    current_state = STATE_READING_NUMBER;
                    int digit = c - '0';
                    number_sum += digit;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ' ';
                    }
                } else if (c == ' ' || c == '\t') {
                    current_state = STATE_START;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ' ';
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid input character\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input character\n");
            return 1;
        }
    }
    
    if (output_pos > 0 && output[output_pos - 1] == ' ') {
        output[output_pos - 1] = '\0';
    } else if (output_pos < sizeof(output)) {
        output[output_pos] = '\0';
    }
    
    printf("Words found: %s\n", output);
    printf("Word count: %d\n", word_count);
    printf("Sum of digits: %d\n", number_sum);
    
    return 0;
}