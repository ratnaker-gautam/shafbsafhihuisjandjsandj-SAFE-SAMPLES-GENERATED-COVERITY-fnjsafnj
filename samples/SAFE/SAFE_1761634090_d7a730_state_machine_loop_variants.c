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
    int i = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != END) {
        switch (current_state) {
            case START:
                if (i >= (int)strlen(input)) {
                    current_state = END;
                } else if (isdigit((unsigned char)input[i])) {
                    current_state = READING_NUMBER;
                    buffer_index = 0;
                } else if (isalpha((unsigned char)input[i])) {
                    current_state = READING_WORD;
                    buffer_index = 0;
                } else if (isspace((unsigned char)input[i])) {
                    current_state = SKIPPING_SPACE;
                } else {
                    i++;
                }
                break;
                
            case READING_NUMBER:
                while (i < (int)strlen(input) && isdigit((unsigned char)input[i])) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = input[i++];
                    } else {
                        i++;
                    }
                }
                buffer[buffer_index] = '\0';
                if (buffer_index > 0) {
                    long num = strtol(buffer, NULL, 10);
                    if (num > 0 && num <= 1000000) {
                        number_sum += (int)num;
                    }
                }
                current_state = START;
                break;
                
            case READING_WORD:
                while (i < (int)strlen(input) && isalpha((unsigned char)input[i])) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = input[i++];
                    } else {
                        i++;
                    }
                }
                buffer[buffer_index] = '\0';
                if (buffer_index > 0) {
                    word_count++;
                }
                current_state = START;
                break;
                
            case SKIPPING_SPACE:
                while (i < (int)strlen(input) && isspace((unsigned char)input[i])) {
                    i++;
                }
                current_state = START;
                break;
                
            case END:
                break;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}