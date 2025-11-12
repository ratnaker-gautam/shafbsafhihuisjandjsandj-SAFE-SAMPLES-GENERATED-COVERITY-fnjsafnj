//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = INIT;
    int pos = 0;
    int ch;
    int valid_input = 1;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    while (buffer[pos] != '\0' && current_state != ERROR && current_state != DONE) {
        ch = buffer[pos];

        switch (current_state) {
            case INIT:
                if (isspace(ch)) {
                    pos++;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (num_count >= 50) {
                        current_state = ERROR;
                        break;
                    }
                    int num = 0;
                    while (isdigit(buffer[pos]) && current_state != ERROR) {
                        int digit = buffer[pos] - '0';
                        if (num > (INT_MAX - digit) / 10) {
                            current_state = ERROR;
                            break;
                        }
                        num = num * 10 + digit;
                        pos++;
                    }
                    if (current_state != ERROR) {
                        numbers[num_count++] = num;
                        current_state = INIT;
                    }
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalpha(ch)) {
                    if (word_count >= 50) {
                        current_state = ERROR;
                        break;
                    }
                    int word_pos = 0;
                    while (isalpha(buffer[pos]) && word_pos < 49) {
                        words[word_count][word_pos++] = buffer[pos++];
                    }
                    words[word_count][word_pos] = '\0';
                    word_count++;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;

            case ERROR:
                valid_input = 0;
                break;

            case DONE:
                break;
        }
    }

    if (current_state == ERROR || !valid_input) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Numbers found: ");
    for (int i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    printf("Words found: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");

    return 0;
}