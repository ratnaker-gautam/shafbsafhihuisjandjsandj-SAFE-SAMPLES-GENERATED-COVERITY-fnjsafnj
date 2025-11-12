//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

int main(void) {
    char input[1024];
    char buffer[256];
    int buffer_index = 0;
    enum State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int number_count = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
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
                } else if (isspace(c)) {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0' || c == '\n') {
                    current_state = END;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < sizeof(buffer) - 1) {
                        buffer[buffer_index++] = c;
                    }
                } else {
                    if (buffer_index > 0 && buffer_index < sizeof(buffer)) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        number_count++;
                    }
                    
                    if (isalpha(c)) {
                        buffer_index = 0;
                        if (buffer_index < sizeof(buffer) - 1) {
                            buffer[buffer_index++] = c;
                        }
                        current_state = READING_WORD;
                    } else if (isspace(c)) {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0' || c == '\n') {
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
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    
                    if (isdigit(c)) {
                        buffer_index = 0;
                        if (buffer_index < sizeof(buffer) - 1) {
                            buffer[buffer_index++] = c;
                        }
                        current_state = READING_NUMBER;
                    } else if (isspace(c)) {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0' || c == '\n') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (!isspace(c)) {
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
                    } else if (c == '\0' || c == '\n') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                }
                break;
                
            case END:
                break;
        }
        
        if (c == '\0' || c == '\n') {
            break;
        }
    }
    
    if (current_state == READING_NUMBER && buffer_index > 0) {
        if (buffer_index < sizeof(buffer)) {
            buffer[buffer_index] = '\0';
            int num = atoi(buffer);
            number_sum += num;
            number_count++;
        }
    } else if (current_state == READING_WORD && buffer_index > 0) {
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
    } else {
        printf("No numbers to sum\n");
    }
    
    return 0;
}