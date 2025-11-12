//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    char input[256];
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    enum state current_state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    int i = 0;

    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char c = input[i];

        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isalpha((unsigned char)c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isdigit((unsigned char)c)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (c != ' ' && c != '\t') {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_WORD:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (c == ' ' || c == '\t') {
                    current_state = STATE_START;
                } else if (!isalpha((unsigned char)c)) {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_NUMBER:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (c == ' ' || c == '\t') {
                    current_state = STATE_START;
                } else if (!isdigit((unsigned char)c)) {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_ERROR:
                break;

            case STATE_END:
                break;
        }

        i++;
        if (i > len) {
            current_state = STATE_END;
        }
    }

    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);

    return 0;
}