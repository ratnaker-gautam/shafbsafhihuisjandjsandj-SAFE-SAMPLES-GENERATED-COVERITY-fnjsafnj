//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

struct Counter {
    int numbers;
    int words;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_input(const char* input, struct Counter* counter) {
    enum State current_state = START;
    size_t i = 0;
    char c;
    
    counter->numbers = 0;
    counter->words = 0;
    
    while (1) {
        c = input[i];
        
        if (c == '\0') {
            if (current_state == READING_NUMBER) {
                counter->numbers++;
            } else if (current_state == READING_WORD) {
                counter->words++;
            }
            current_state = DONE;
            break;
        }
        
        if (!is_valid_char(c)) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                }
                break;
                
            case READING_NUMBER:
                if (isspace((unsigned char)c)) {
                    counter->numbers++;
                    current_state = START;
                } else if (!isdigit((unsigned char)c)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isspace((unsigned char)c)) {
                    counter->words++;
                    current_state = START;
                } else if (!isalpha((unsigned char)c)) {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
        
        if (current_state == ERROR) {
            break;
        }
        
        i++;
        if (i >= 1024) {
            current_state = ERROR;
            break;
        }
    }
}

int main(void) {
    char input[1025];
    struct Counter counter;
    
    printf("Enter text (max 1024 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_input(input, &counter);
    
    if (counter.numbers < 0 || counter.words < 0) {
        fprintf(stderr, "Processing error\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", counter.numbers);
    printf("Words found: %d\n", counter.words);
    
    return 0;
}