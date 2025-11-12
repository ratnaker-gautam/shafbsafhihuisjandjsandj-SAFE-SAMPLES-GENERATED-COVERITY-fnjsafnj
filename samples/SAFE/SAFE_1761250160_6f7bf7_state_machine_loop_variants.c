//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    END
};

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    enum state current_state = START;
    int pos = 0;
    int i;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    for (i = 0; buffer[i] != '\0'; i++) {
        switch (current_state) {
            case START:
                if (isdigit(buffer[i])) {
                    current_state = READING_NUMBER;
                    numbers[num_count] = buffer[i] - '0';
                    pos = 1;
                } else if (isalpha(buffer[i])) {
                    current_state = READING_WORD;
                    words[word_count][0] = buffer[i];
                    pos = 1;
                } else {
                    current_state = SKIPPING;
                }
                break;

            case READING_NUMBER:
                if (isdigit(buffer[i])) {
                    if (num_count < 50 && pos < 10) {
                        numbers[num_count] = numbers[num_count] * 10 + (buffer[i] - '0');
                        pos++;
                    }
                } else if (isalpha(buffer[i])) {
                    current_state = READING_WORD;
                    if (word_count < 50) {
                        words[word_count][0] = buffer[i];
                        pos = 1;
                    }
                } else {
                    current_state = SKIPPING;
                    if (num_count < 50) {
                        num_count++;
                    }
                }
                break;

            case READING_WORD:
                if (isalpha(buffer[i])) {
                    if (word_count < 50 && pos < 49) {
                        words[word_count][pos] = buffer[i];
                        pos++;
                    }
                } else if (isdigit(buffer[i])) {
                    current_state = READING_NUMBER;
                    if (num_count < 50) {
                        numbers[num_count] = buffer[i] - '0';
                        pos = 1;
                    }
                } else {
                    current_state = SKIPPING;
                    if (word_count < 50) {
                        words[word_count][pos] = '\0';
                        word_count++;
                    }
                }
                break;

            case SKIPPING:
                if (isdigit(buffer[i])) {
                    current_state = READING_NUMBER;
                    if (num_count < 50) {
                        numbers[num_count] = buffer[i] - '0';
                        pos = 1;
                    }
                } else if (isalpha(buffer[i])) {
                    current_state = READING_WORD;
                    if (word_count < 50) {
                        words[word_count][0] = buffer[i];
                        pos = 1;
                    }
                }
                break;

            case END:
                break;
        }
    }

    if (current_state == READING_NUMBER && num_count < 50) {
        num_count++;
    } else if (current_state == READING_WORD && word_count < 50) {
        words[word_count][pos] = '\0';
        word_count++;
    }

    printf("Numbers found: ");
    for (i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    printf("Words found: ");
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");

    return 0;
}