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

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    for (int i = 0; i < 50; i++) {
        numbers[i] = 0;
        words[i][0] = '\0';
    }

    while (current_state != STATE_DONE && current_state != STATE_ERROR && pos < (int)len) {
        ch = buffer[pos];

        switch (current_state) {
            case STATE_START:
                if (isspace(ch)) {
                    pos++;
                } else if (isdigit(ch)) {
                    if (num_count < 50) {
                        numbers[num_count] = ch - '0';
                    }
                    current_state = STATE_READING_NUMBER;
                    pos++;
                } else if (isalpha(ch)) {
                    if (word_count < 50) {
                        words[word_count][0] = ch;
                        words[word_count][1] = '\0';
                    }
                    current_state = STATE_READING_WORD;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_NUMBER:
                if (num_count >= 50) {
                    current_state = STATE_ERROR;
                    break;
                }
                if (isdigit(ch)) {
                    if (numbers[num_count] > (INT_MAX - (ch - '0')) / 10) {
                        current_state = STATE_ERROR;
                        break;
                    }
                    numbers[num_count] = numbers[num_count] * 10 + (ch - '0');
                    pos++;
                } else if (isspace(ch)) {
                    num_count++;
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_WORD:
                if (word_count >= 50) {
                    current_state = STATE_ERROR;
                    break;
                }
                if (isalpha(ch)) {
                    int word_len = strlen(words[word_count]);
                    if (word_len >= 49) {
                        current_state = STATE_ERROR;
                        break;
                    }
                    words[word_count][word_len] = ch;
                    words[word_count][word_len + 1] = '\0';
                    pos++;
                } else if (isspace(ch)) {
                    word_count++;
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_ERROR:
                valid_input = 0;
                current_state = STATE_DONE;
                break;

            case STATE_DONE:
                break;
        }
    }

    if (current_state == STATE_READING_NUMBER && num_count < 50) {
        num_count++;
    } else if (current_state == STATE_READING_WORD && word_count < 50) {
        word_count++;
    }

    if (!valid_input || current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", num_count);
    for (int i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    printf("Words found: %d\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");

    return 0;
}