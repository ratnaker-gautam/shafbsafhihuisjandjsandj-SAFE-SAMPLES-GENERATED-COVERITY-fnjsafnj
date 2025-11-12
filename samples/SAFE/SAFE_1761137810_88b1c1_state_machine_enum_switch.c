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
    size_t input_len = strlen(input);
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (i < (int)input_len && current_state != END) {
        char c = input[i];
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    buf_index = 0;
                    if (buf_index < (int)sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    buf_index = 0;
                    if (buf_index < (int)sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = END;
                }
                i++;
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (buf_index < (int)sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else {
                    if (buf_index > 0 && buf_index < (int)sizeof(buffer)) {
                        buffer[buf_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                    }
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (buf_index < (int)sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else {
                    if (buf_index > 0) {
                        word_count++;
                    }
                    current_state = START;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isspace((unsigned char)c)) {
                    i++;
                } else {
                    current_state = START;
                }
                break;
                
            case END:
                break;
        }
    }
    
    if (current_state == READING_NUMBER && buf_index > 0 && buf_index < (int)sizeof(buffer)) {
        buffer[buf_index] = '\0';
        int num = atoi(buffer);
        number_sum += num;
    } else if (current_state == READING_WORD && buf_index > 0) {
        word_count++;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}