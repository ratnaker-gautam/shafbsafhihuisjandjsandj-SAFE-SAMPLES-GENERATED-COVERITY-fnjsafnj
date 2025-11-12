//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

    buffer[strcspn(buffer, "\n")] = '\0';
    int len = strlen(buffer);

    while (pos < len && current_state != ERROR && current_state != DONE) {
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
                    pos++;
                } else if (isspace(ch)) {
                    if (num_count < 50) {
                        char num_str[20];
                        int start = pos;
                        while (start > 0 && isdigit(buffer[start - 1])) start--;
                        int num_len = pos - start;
                        if (num_len < 20) {
                            strncpy(num_str, &buffer[start], num_len);
                            num_str[num_len] = '\0';
                            numbers[num_count] = atoi(num_str);
                            num_count++;
                        }
                    }
                    current_state = INIT;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalpha(ch)) {
                    pos++;
                } else if (isspace(ch)) {
                    if (word_count < 50) {
                        int start = pos;
                        while (start > 0 && isalpha(buffer[start - 1])) start--;
                        int word_len = pos - start;
                        if (word_len < 50) {
                            strncpy(words[word_count], &buffer[start], word_len);
                            words[word_count][word_len] = '\0';
                            word_count++;
                        }
                    }
                    current_state = INIT;
                    pos++;
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

    if (current_state == READING_NUMBER && num_count < 50) {
        char num_str[20];
        int start = pos;
        while (start > 0 && isdigit(buffer[start - 1])) start--;
        int num_len = pos - start;
        if (num_len < 20) {
            strncpy(num_str, &buffer[start], num_len);
            num_str[num_len] = '\0';
            numbers[num_count] = atoi(num_str);
            num_count++;
        }
    } else if (current_state == READING_WORD && word_count < 50) {
        int start = pos;
        while (start > 0 && isalpha(buffer[start - 1])) start--;
        int word_len = pos - start;
        if (word_len < 50) {
            strncpy(words[word_count], &buffer[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }

    if (!valid_input || current_state == ERROR) {
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