//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
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

    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    enum state current_state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    char buffer[64];
    int buf_index = 0;

    for (size_t i = 0; i <= len; i++) {
        char c = input[i];

        switch (current_state) {
            case STATE_START:
                if (isalpha((unsigned char)c)) {
                    buffer[buf_index++] = c;
                    current_state = STATE_READING_WORD;
                } else if (isdigit((unsigned char)c)) {
                    buffer[buf_index++] = c;
                    current_state = STATE_READING_NUMBER;
                } else if (c == '\0') {
                    current_state = STATE_END;
                } else if (!isspace((unsigned char)c)) {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (buf_index < (int)sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    buffer[buf_index] = '\0';
                    word_count++;
                    buf_index = 0;
                    
                    if (isdigit((unsigned char)c)) {
                        buffer[buf_index++] = c;
                        current_state = STATE_READING_NUMBER;
                    } else if (c == '\0') {
                        current_state = STATE_END;
                    } else if (!isspace((unsigned char)c)) {
                        current_state = STATE_ERROR;
                    } else {
                        current_state = STATE_START;
                    }
                }
                break;

            case STATE_READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (buf_index < (int)sizeof(buffer) - 1) {
                        buffer[buf_index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    buffer[buf_index] = '\0';
                    number_count++;
                    buf_index = 0;
                    
                    if (isalpha((unsigned char)c)) {
                        buffer[buf_index++] = c;
                        current_state = STATE_READING_WORD;
                    } else if (c == '\0') {
                        current_state = STATE_END;
                    } else if (!isspace((unsigned char)c)) {
                        current_state = STATE_ERROR;
                    } else {
                        current_state = STATE_START;
                    }
                }
                break;

            case STATE_END:
                break;

            case STATE_ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
        }

        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
    }

    if (current_state != STATE_END) {
        fprintf(stderr, "Unexpected end of input\n");
        return 1;
    }

    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}