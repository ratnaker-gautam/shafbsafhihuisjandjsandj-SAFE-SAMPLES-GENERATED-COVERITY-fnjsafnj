//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_DIGITS,
    FINALIZE,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    char input_buffer[1024];
    int word_count = 0;
    int digit_count = 0;
    int char_count = 0;
    int valid_input = 1;
    size_t input_len = 0;
    size_t pos = 0;
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        current_state = ERROR;
    } else {
        input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        current_state = READ_CHAR;
    }
    
    while (current_state != FINALIZE && current_state != ERROR) {
        switch (current_state) {
            case READ_CHAR:
                if (pos >= input_len) {
                    current_state = FINALIZE;
                } else if (isspace((unsigned char)input_buffer[pos])) {
                    pos++;
                } else if (isalnum((unsigned char)input_buffer[pos])) {
                    current_state = PROCESS_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESS_WORD:
                word_count++;
                while (pos < input_len && isalnum((unsigned char)input_buffer[pos])) {
                    if (isdigit((unsigned char)input_buffer[pos])) {
                        digit_count++;
                    }
                    char_count++;
                    pos++;
                }
                current_state = READ_CHAR;
                break;
                
            case FINALIZE:
                if (valid_input && word_count > 0) {
                    printf("Words: %d\n", word_count);
                    printf("Digits: %d\n", digit_count);
                    printf("Characters: %d\n", char_count);
                } else {
                    printf("Invalid input or no words found\n");
                }
                current_state = ERROR;
                break;
                
            case ERROR:
                printf("Error processing input\n");
                current_state = FINALIZE;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    return current_state == FINALIZE ? EXIT_SUCCESS : EXIT_FAILURE;
}