//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
                    buffer[buf_index++] = c;
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[buf_index++] = c;
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    i++;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    if (buf_index > 0 && buf_index < sizeof(buffer)) {
                        buffer[buf_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                    }
                    current_state = END;
                } else if (isdigit(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else {
                    if (buf_index > 0 && buf_index < sizeof(buffer)) {
                        buffer[buf_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                    }
                    buf_index = 0;
                    current_state = SKIPPING_SPACE;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    if (buf_index > 0) {
                        word_count++;
                    }
                    current_state = END;
                } else if (isalpha(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else {
                    if (buf_index > 0) {
                        word_count++;
                    }
                    buf_index = 0;
                    current_state = SKIPPING_SPACE;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = END;
                } else if (isspace(c)) {
                    i++;
                } else {
                    current_state = START;
                }
                break;
                
            case END:
                break;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}