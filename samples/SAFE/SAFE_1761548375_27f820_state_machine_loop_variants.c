//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

int main(void) {
    char buffer[256];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = START;
    int i = 0;
    int ch;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    while (1) {
        switch (current_state) {
            case START:
                ch = buffer[i];
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUM;
                    num_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace(ch)) {
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_NUM:
                ch = buffer[i];
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    i++;
                } else if (isspace(ch)) {
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                ch = buffer[i];
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isalpha(ch)) {
                    i++;
                } else if (isspace(ch)) {
                    current_state = START;
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;

            case ERROR:
                fprintf(stderr, "Invalid character at position %d\n", i);
                return 1;

            case DONE:
                printf("Numbers: %d\nWords: %d\n", num_count, word_count);
                return 0;
        }

        if (i >= 255) {
            fprintf(stderr, "Input too long\n");
            return 1;
        }
    }

    return 0;
}