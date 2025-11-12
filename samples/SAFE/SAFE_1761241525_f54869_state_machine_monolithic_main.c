//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    char buffer[256];
    char current_token[256];
    int token_length = 0;
    enum State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    int ch;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    while (buffer[i] != '\0' && current_state != STATE_ERROR && current_state != STATE_DONE) {
        ch = buffer[i];

        switch (current_state) {
            case STATE_START:
                if (isspace(ch)) {
                    i++;
                } else if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    token_length = 0;
                    if (token_length < 255) {
                        current_token[token_length++] = ch;
                    }
                    i++;
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    token_length = 0;
                    if (token_length < 255) {
                        current_token[token_length++] = ch;
                    }
                    i++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (token_length < 255) {
                        current_token[token_length++] = ch;
                    }
                    i++;
                } else if (isspace(ch) || ch == '\0') {
                    current_token[token_length] = '\0';
                    number_count++;
                    printf("Found number: %s\n", current_token);
                    current_state = STATE_START;
                    i++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    if (token_length < 255) {
                        current_token[token_length++] = ch;
                    }
                    i++;
                } else if (isspace(ch) || ch == '\0') {
                    current_token[token_length] = '\0';
                    word_count++;
                    printf("Found word: %s\n", current_token);
                    current_state = STATE_START;
                    i++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;

            case STATE_DONE:
                break;
        }
    }

    if (current_state == STATE_READING_NUMBER) {
        current_token[token_length] = '\0';
        number_count++;
        printf("Found number: %s\n", current_token);
    } else if (current_state == STATE_READING_WORD) {
        current_token[token_length] = '\0';
        word_count++;
        printf("Found word: %s\n", current_token);
    } else if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}