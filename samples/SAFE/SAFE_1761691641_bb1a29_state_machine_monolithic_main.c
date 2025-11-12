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
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != END) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = END;
                } else if (isdigit(c)) {
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
                } else {
                    current_state = SKIPPING_SPACE;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = c;
                    }
                } else {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        int num = atoi(buffer);
                        number_sum += num;
                        number_count++;
                    }
                    
                    if (c == '\0') {
                        current_state = END;
                    } else if (isspace(c)) {
                        current_state = SKIPPING_SPACE;
                    } else if (isalpha(c)) {
                        buffer_index = 0;
                        if (buffer_index < 255) {
                            buffer[buffer_index++] = tolower(c);
                        }
                        current_state = READING_WORD;
                    } else {
                        current_state = SKIPPING_SPACE;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = tolower(c);
                    }
                } else {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    
                    if (c == '\0') {
                        current_state = END;
                    } else if (isdigit(c)) {
                        buffer_index = 0;
                        if (buffer_index < 255) {
                            buffer[buffer_index++] = c;
                        }
                        current_state = READING_NUMBER;
                    } else if (isspace(c)) {
                        current_state = SKIPPING_SPACE;
                    } else {
                        current_state = SKIPPING_SPACE;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = END;
                } else if (isdigit(c)) {
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
                } else if (!isspace(c)) {
                    current_state = SKIPPING_SPACE;
                }
                break;
                
            case END:
                break;
        }
        
        i++;
        if (i >= 1024) {
            current_state = END;
        }
    }
    
    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
    } else {
        printf("No numbers found\n");
    }
    
    return 0;
}