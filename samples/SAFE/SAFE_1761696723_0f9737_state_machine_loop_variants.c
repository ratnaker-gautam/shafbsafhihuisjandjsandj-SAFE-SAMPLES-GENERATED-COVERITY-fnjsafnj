//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

int main(void) {
    char input[256];
    char buffer[256];
    int buffer_index = 0;
    enum State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int char_index = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    for (char_index = 0; char_index <= input_len; char_index++) {
        char c = input[char_index];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
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
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                } else {
                    buffer[buffer_index] = '\0';
                    int number = atoi(buffer);
                    number_sum += number;
                    
                    if (isalpha(c)) {
                        buffer_index = 0;
                        if (buffer_index < sizeof(buffer) - 1) {
                            buffer[buffer_index++] = c;
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
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                } else {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    
                    if (isdigit(c)) {
                        buffer_index = 0;
                        if (buffer_index < sizeof(buffer) - 1) {
                            buffer[buffer_index++] = c;
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
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
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
    
    if (current_state == READING_NUMBER) {
        buffer[buffer_index] = '\0';
        int number = atoi(buffer);
        number_sum += number;
    } else if (current_state == READING_WORD) {
        word_count++;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}