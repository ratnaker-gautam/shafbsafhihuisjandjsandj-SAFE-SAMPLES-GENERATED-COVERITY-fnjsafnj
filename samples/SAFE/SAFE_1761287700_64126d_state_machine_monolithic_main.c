//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = START;
    int pos = 0;
    int ch;
    int valid_input = 1;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    int len = strlen(buffer);

    while (pos < len && current_state != ERROR && current_state != DONE) {
        ch = buffer[pos];

        switch (current_state) {
            case START:
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
                if (num_count >= 50) {
                    current_state = ERROR;
                    break;
                }
                numbers[num_count] = 0;
                while (pos < len && isdigit(ch)) {
                    if (numbers[num_count] > (INT_MAX - (ch - '0')) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    numbers[num_count] = numbers[num_count] * 10 + (ch - '0');
                    pos++;
                    if (pos < len) ch = buffer[pos];
                }
                if (current_state != ERROR) {
                    num_count++;
                    current_state = START;
                }
                break;

            case READING_WORD:
                if (word_count >= 50) {
                    current_state = ERROR;
                    break;
                }
                int word_pos = 0;
                while (pos < len && isalpha(ch)) {
                    if (word_pos >= 49) {
                        current_state = ERROR;
                        break;
                    }
                    words[word_count][word_pos++] = ch;
                    pos++;
                    if (pos < len) ch = buffer[pos];
                }
                if (current_state != ERROR) {
                    words[word_count][word_pos] = '\0';
                    word_count++;
                    current_state = START;
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