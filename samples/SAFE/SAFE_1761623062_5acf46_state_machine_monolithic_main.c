//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    int i = 0;
    
    printf("Enter text (max 1023 characters): ");
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
    
    while (i <= input_len) {
        char c = (i < input_len) ? input[i] : '\0';
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = END;
                }
                i++;
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    i++;
                } else {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        long num = strtol(buffer, NULL, 10);
                        if (num > 0 && num <= 1000000) {
                            number_sum += (int)num;
                            number_count++;
                        }
                    }
                    
                    if (isalpha(c)) {
                        buffer_index = 0;
                        if (buffer_index < 255) {
                            buffer[buffer_index++] = tolower(c);
                        }
                        current_state = READING_WORD;
                    } else if (isspace(c)) {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                    i++;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    i++;
                } else {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    
                    if (isdigit(c)) {
                        buffer_index = 0;
                        if (buffer_index < 255) {
                            buffer[buffer_index++] = c;
                        }
                        current_state = READING_NUMBER;
                    } else if (isspace(c)) {
                        current_state = SKIPPING_SPACE;
                    } else if (c == '\0') {
                        current_state = END;
                    } else {
                        current_state = START;
                    }
                    i++;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isspace(c)) {
                    i++;
                } else if (isdigit(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                    current_state = READING_NUMBER;
                    i++;
                } else if (isalpha(c)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                    current_state = READING_WORD;
                    i++;
                } else if (c == '\0') {
                    current_state = END;
                    i++;
                } else {
                    current_state = START;
                    i++;
                }
                break;
                
            case END:
                i++;
                break;
        }
    }
    
    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
        printf("Average of numbers: %.2f\n", (double)number_sum / number_count);
    } else {
        printf("No valid numbers found\n");
    }
    
    return 0;
}