//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = STATE_START;
    int pos = 0;
    int ch;
    int valid_input = 1;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    while (current_state != STATE_DONE && current_state != STATE_ERROR && buffer[pos] != '\0') {
        ch = buffer[pos];

        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    if (num_count < 50) {
                        numbers[num_count] = ch - '0';
                        current_state = STATE_READING_NUMBER;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isalpha(ch)) {
                    if (word_count < 50) {
                        words[word_count][0] = ch;
                        words[word_count][1] = '\0';
                        current_state = STATE_READING_WORD;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (numbers[num_count] <= (INT_MAX - (ch - '0')) / 10) {
                        numbers[num_count] = numbers[num_count] * 10 + (ch - '0');
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    num_count++;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                pos++;
                break;

            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    size_t len = strlen(words[word_count]);
                    if (len < 49) {
                        words[word_count][len] = ch;
                        words[word_count][len + 1] = '\0';
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    word_count++;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                pos++;
                break;

            default:
                current_state = STATE_ERROR;
                break;
        }
    }

    if (current_state == STATE_READING_NUMBER) {
        num_count++;
    } else if (current_state == STATE_READING_WORD) {
        word_count++;
    }

    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        valid_input = 0;
    }

    if (valid_input) {
        printf("Numbers found: ");
        for (int i = 0; i < num_count; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\nWords found: ");
        for (int i = 0; i < word_count; i++) {
            printf("%s ", words[i]);
        }
        printf("\n");
    }

    return current_state == STATE_ERROR ? 1 : 0;
}