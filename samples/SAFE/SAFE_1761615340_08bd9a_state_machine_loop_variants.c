//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

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
    int i = 0;
    
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
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (i = 0; i <= input_len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
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
                if (isdigit((unsigned char)c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                } else {
                    if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0) {
                            if (number_sum <= INT_MAX - num) {
                                number_sum += num;
                            }
                        }
                    }
                    
                    if (isalpha((unsigned char)c)) {
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
                if (isalpha((unsigned char)c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                } else {
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    
                    if (isdigit((unsigned char)c)) {
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
                if (isdigit((unsigned char)c)) {
                    buffer_index = 0;
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
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
        if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
            buffer[buffer_index] = '\0';
            int num = atoi(buffer);
            if (num > 0) {
                if (number_sum <= INT_MAX - num) {
                    number_sum += num;
                }
            }
        }
    } else if (current_state == READING_WORD) {
        if (buffer_index > 0) {
            word_count++;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}