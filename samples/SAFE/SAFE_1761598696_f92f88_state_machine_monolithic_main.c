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
    char buffer[1024];
    int buf_index = 0;
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
                    buffer[0] = c;
                    buf_index = 1;
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[0] = c;
                    buf_index = 1;
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    i++;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else {
                    buffer[buf_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    number_count++;
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buf_index < sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    }
                    i++;
                } else {
                    buffer[buf_index] = '\0';
                    word_count++;
                    current_state = START;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isspace(c)) {
                    i++;
                } else {
                    current_state = START;
                }
                break;
                
            case END:
                break;
        }
        
        if (c == '\0') {
            current_state = END;
        }
    }
    
    if (current_state == READING_NUMBER) {
        buffer[buf_index] = '\0';
        int num = atoi(buffer);
        number_sum += num;
        number_count++;
    } else if (current_state == READING_WORD) {
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    printf("Numbers found: %d\n", number_count);
    if (number_count > 0) {
        printf("Sum of numbers: %d\n", number_sum);
    }
    
    return 0;
}