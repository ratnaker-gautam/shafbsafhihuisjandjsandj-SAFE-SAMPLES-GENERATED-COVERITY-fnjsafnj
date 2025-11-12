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

int main(void) {
    char buffer[1024];
    struct Counter counter = {0, 0};
    enum State current_state = START;
    int pos = 0;
    int ch;

    printf("Enter text (max 1023 chars): ");
    
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
        if (!is_valid_char(buffer[pos])) {
            current_state = ERROR;
            break;
        }
    }

    if (current_state == ERROR) {
        fprintf(stderr, "Invalid character in input\n");
        return 1;
    }

    pos = 0;
    current_state = START;

    while (pos < (int)len && current_state != ERROR && current_state != DONE) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)buffer[pos])) {
                    current_state = READING_NUMBER;
                    counter.numbers++;
                } else if (isalpha((unsigned char)buffer[pos])) {
                    current_state = READING_WORD;
                    counter.words++;
                } else if (isspace((unsigned char)buffer[pos])) {
                    pos++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_NUMBER:
                while (pos < (int)len && isdigit((unsigned char)buffer[pos])) {
                    pos++;
                }
                current_state = START;
                break;

            case READING_WORD:
                while (pos < (int)len && isalpha((unsigned char)buffer[pos])) {
                    pos++;
                }
                current_state = START;
                break;

            case ERROR:
                break;

            case DONE:
                break;
        }
    }

    if (current_state == ERROR) {
        fprintf(stderr, "Processing error\n");
        return 1;
    }

    printf("Numbers: %d\n", counter.numbers);
    printf("Words: %d\n", counter.words);

    return 0;
}