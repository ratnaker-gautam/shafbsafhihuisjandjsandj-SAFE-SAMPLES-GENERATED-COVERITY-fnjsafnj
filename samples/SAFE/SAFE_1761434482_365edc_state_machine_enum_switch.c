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
    FINISHED,
    ERROR
};

int main(void) {
    char input[256];
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    enum state current_state = START;
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    char current_char;
    
    while (current_state != FINISHED && current_state != ERROR) {
        if (i >= len) {
            current_state = FINISHED;
            break;
        }
        
        current_char = input[i];
        
        switch (current_state) {
            case START:
                if (isdigit(current_char)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(current_char)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace(current_char)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(current_char)) {
                    current_state = READING_NUMBER;
                } else if (isspace(current_char)) {
                    current_state = SKIPPING_SPACE;
                } else if (i == len) {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(current_char)) {
                    current_state = READING_WORD;
                } else if (isspace(current_char)) {
                    current_state = SKIPPING_SPACE;
                } else if (i == len) {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isspace(current_char)) {
                    current_state = SKIPPING_SPACE;
                } else if (isdigit(current_char)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(current_char)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (i == len) {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid character at position %d: '%c'\n", i, current_char);
                break;
        }
        
        i++;
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Input contains invalid format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}