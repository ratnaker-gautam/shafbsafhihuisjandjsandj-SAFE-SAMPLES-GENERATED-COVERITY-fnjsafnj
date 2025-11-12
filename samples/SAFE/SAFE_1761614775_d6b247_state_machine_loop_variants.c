//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    char output[512] = {0};
    size_t output_pos = 0;
    enum state current_state = START;
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
    
    size_t i = 0;
    while (i < len && current_state != ERROR && current_state != DONE) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    int num = c - '0';
                    if (number_sum <= INT_MAX - num) {
                        number_sum += num;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (c == ' ' || c == '\t') {
                    i++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    int num = c - '0';
                    if (number_sum <= INT_MAX - num) {
                        number_sum += num;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == ' ' || c == '\t') {
                    current_state = START;
                } else if (isalpha((unsigned char)c)) {
                    current_state = ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (c == ' ' || c == '\t') {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ' ';
                    }
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                valid_input = 0;
                break;
                
            case DONE:
                break;
        }
        
        i++;
    }
    
    if (current_state == READING_WORD && output_pos > 0 && output[output_pos - 1] == ' ') {
        output_pos--;
    }
    
    if (output_pos < sizeof(output)) {
        output[output_pos] = '\0';
    }
    
    if (!valid_input || current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Extracted words: %s\n", output);
    printf("Word count: %d\n", word_count);
    printf("Digit sum: %d\n", number_sum);
    
    return 0;
}