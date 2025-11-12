//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

int main(void) {
    char input[1024];
    char buffer[256];
    int buf_index = 0;
    enum state current_state = START;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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
            case START:
                if (isdigit(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = READING_WORD;
                } else if (c == ' ' || c == '\t') {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = END;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                } else {
                    buffer[buf_index] = '\0';
                    if (buf_index > 0) {
                        long num = strtol(buffer, NULL, 10);
                        if (num > 0 && num <= 1000000) {
                            number_sum += (int)num;
                        }
                    }
                    buf_index = 0;
                    
                    if (isalpha(c)) {
                        if (buf_index < sizeof(buffer) - 1) {
                            buffer[buf_index++] = c;
                        }
                        current_state = READING_WORD;
                    } else if (c == ' ' || c == '\t') {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                } else {
                    buffer[buf_index] = '\0';
                    if (buf_index > 0) {
                        word_count++;
                    }
                    buf_index = 0;
                    
                    if (isdigit(c)) {
                        if (buf_index < sizeof(buffer) - 1) {
                            buffer[buf_index++] = c;
                        }
                        current_state = READING_NUMBER;
                    } else if (c == ' ' || c == '\t') {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (isdigit(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    current_state = READING_WORD;
                } else if (c == '\0') {
                    current_state = END;
                } else if (c != ' ' && c != '\t') {
                    current_state = START;
                }
                break;
                
            case END:
                break;
        }
        
        if (current_state == END) {
            break;
        }
    }
    
    printf("Sum of valid numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}