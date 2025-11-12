//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
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
    int valid_input = 1;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    for (size_t i = 0; i <= len && current_state != STATE_ERROR && current_state != STATE_DONE; i++) {
        char c = (i < len) ? buffer[i] : ' ';
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    int digit = c - '0';
                    if (output_pos < sizeof(output) - 10) {
                        output_pos += snprintf(output + output_pos, sizeof(output) - output_pos, "Number: %d", digit);
                    }
                    number_sum += digit;
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                    if (output_pos < sizeof(output) - 10) {
                        output_pos += snprintf(output + output_pos, sizeof(output) - output_pos, "Word: %c", c);
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    int digit = c - '0';
                    if (output_pos < sizeof(output) - 2) {
                        output_pos += snprintf(output + output_pos, sizeof(output) - output_pos, "%d", digit);
                    }
                    number_sum += digit;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (output_pos < sizeof(output) - 2) {
                        output_pos += snprintf(output + output_pos, sizeof(output) - output_pos, "\n");
                    }
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (output_pos < sizeof(output) - 2) {
                        output_pos += snprintf(output + output_pos, sizeof(output) - output_pos, "%c", c);
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    if (output_pos < sizeof(output) - 2) {
                        output_pos += snprintf(output + output_pos, sizeof(output) - output_pos, "\n");
                    }
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                valid_input = 0;
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR || !valid_input) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("%s", output);
    printf("\nStatistics:\n");
    printf("Total numbers sum: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}