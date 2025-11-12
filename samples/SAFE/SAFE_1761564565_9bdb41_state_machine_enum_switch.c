//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    END
};

int main(void) {
    char buffer[256];
    char output[512] = "";
    size_t out_pos = 0;
    enum state current_state = START;
    int number_sum = 0;
    int number_value = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = END;
                } else if (isdigit((unsigned char)c)) {
                    number_value = c - '0';
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (out_pos < sizeof(output) - 1) {
                        output[out_pos++] = c;
                    }
                    current_state = READING_WORD;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    int digit = c - '0';
                    if (number_value <= (INT_MAX - digit) / 10) {
                        number_value = number_value * 10 + digit;
                    }
                } else {
                    number_sum += number_value;
                    number_value = 0;
                    
                    if (isalpha((unsigned char)c)) {
                        if (out_pos < sizeof(output) - 1) {
                            output[out_pos++] = c;
                        }
                        current_state = READING_WORD;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (out_pos < sizeof(output) - 1) {
                        output[out_pos++] = c;
                    }
                } else {
                    if (out_pos > 0 && out_pos < sizeof(output) - 1) {
                        output[out_pos++] = ' ';
                    }
                    word_count++;
                    
                    if (isdigit((unsigned char)c)) {
                        number_value = c - '0';
                        current_state = READING_NUMBER;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case SKIPPING:
                if (isdigit((unsigned char)c)) {
                    number_value = c - '0';
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (out_pos < sizeof(output) - 1) {
                        output[out_pos++] = c;
                    }
                    current_state = READING_WORD;
                } else if (c == '\0') {
                    current_state = END;
                }
                break;
                
            case END:
                break;
        }
    }
    
    if (out_pos > 0 && output[out_pos-1] == ' ') {
        output[out_pos-1] = '\0';
    } else if (out_pos < sizeof(output)) {
        output[out_pos] = '\0';
    }
    
    printf("Extracted words: %s\n", output);
    printf("Word count: %d\n", word_count);
    printf("Number sum: %d\n", number_sum);
    
    return 0;
}