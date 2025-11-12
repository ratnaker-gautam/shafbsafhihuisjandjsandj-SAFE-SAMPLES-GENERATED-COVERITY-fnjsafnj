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
    SKIPPING_SPACE,
    END,
    ERROR
};

int main(void) {
    char buffer[256];
    char output[512] = {0};
    size_t output_pos = 0;
    enum state current_state = START;
    int number_value = 0;
    char current_word[64];
    size_t word_pos = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
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
                } else if (isdigit(c)) {
                    number_value = c - '0';
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    if (word_pos < sizeof(current_word) - 1) {
                        current_word[word_pos++] = c;
                        current_state = READING_WORD;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    int digit = c - '0';
                    if (number_value <= (INT_MAX - digit) / 10) {
                        number_value = number_value * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == '\0' || isspace(c)) {
                    if (output_pos < sizeof(output) - 16) {
                        int written = snprintf(output + output_pos, 
                                             sizeof(output) - output_pos,
                                             "[NUM:%d] ", number_value);
                        if (written > 0) {
                            output_pos += written;
                        }
                    }
                    number_value = 0;
                    current_state = (c == '\0') ? END : SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (word_pos < sizeof(current_word) - 1) {
                        current_word[word_pos++] = c;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == '\0' || isspace(c)) {
                    current_word[word_pos] = '\0';
                    if (output_pos < sizeof(output) - word_pos - 4) {
                        int written = snprintf(output + output_pos,
                                             sizeof(output) - output_pos,
                                             "[WORD:%s] ", current_word);
                        if (written > 0) {
                            output_pos += written;
                        }
                    }
                    word_pos = 0;
                    current_state = (c == '\0') ? END : SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = END;
                } else if (!isspace(c)) {
                    i--;
                    current_state = START;
                }
                break;
                
            case END:
                break;
                
            case ERROR:
                fprintf(stderr, "Parse error at position %zu\n", i);
                return 1;
        }
        
        if (current_state == ERROR) {
            fprintf(stderr, "Parse error at position %zu\n", i);
            return 1;
        }
    }
    
    if (output_pos > 0 && output_pos < sizeof(output) && output[output_pos-1] == ' ') {
        output[output_pos-1] = '\0';
    }
    
    printf("Parsed: %s\n", output);
    return 0;
}