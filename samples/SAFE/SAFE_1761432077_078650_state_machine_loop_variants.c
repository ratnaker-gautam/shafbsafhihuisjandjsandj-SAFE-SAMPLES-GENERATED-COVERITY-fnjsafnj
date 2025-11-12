//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

struct Counter {
    int numbers;
    int words;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

int main(void) {
    char buffer[256];
    struct Counter counter = {0, 0};
    enum State current_state = START;
    int pos = 0;
    int ch;

    printf("Enter text (max 255 chars): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }

    for (pos = 0; pos < (int)len; pos++) {
        char c = buffer[pos];
        
        if (!is_valid_char(c)) {
            current_state = ERROR;
            break;
        }

        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUM;
                    counter.numbers++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    counter.words++;
                } else if (isspace((unsigned char)c)) {
                    current_state = START;
                }
                break;
                
            case READING_NUM:
                if (isspace((unsigned char)c)) {
                    current_state = START;
                } else if (!isdigit((unsigned char)c)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isspace((unsigned char)c)) {
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
    }

    if (current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Numbers: %d\n", counter.numbers);
    printf("Words: %d\n", counter.words);

    return 0;
}