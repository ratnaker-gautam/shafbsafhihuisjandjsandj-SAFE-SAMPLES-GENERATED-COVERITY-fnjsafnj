//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    int i, j, k;
    int word_count = 0;
    int in_word = 0;

    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("Empty input string\n");
        return 0;
    }

    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    j = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            processed[j++] = toupper((unsigned char)input[i]);
        } else if (isspace((unsigned char)input[i])) {
            in_word = 0;
            processed[j++] = ' ';
        } else {
            processed[j++] = input[i];
        }
    }
    processed[j] = '\0';

    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Word count: %d\n", word_count);

    char reversed[MAX_LENGTH];
    k = 0;
    i = len - 1;
    while (i >= 0) {
        reversed[k++] = input[i];
        i--;
    }
    reversed[k] = '\0';

    printf("Reversed: %s\n", reversed);

    int char_count[26] = {0};
    for (i = 0; i < len; i++) {
        char c = tolower((unsigned char)input[i]);
        if (c >= 'a' && c <= 'z') {
            char_count[c - 'a']++;
        }
    }

    printf("Character frequencies:\n");
    for (i = 0; i < 26; i++) {
        if (char_count[i] > 0) {
            printf("%c: %d\n", 'a' + i, char_count[i]);
        }
    }

    char words[50][MAX_LENGTH];
    int word_index = 0;
    int char_index = 0;
    in_word = 0;

    for (i = 0; i <= len; i++) {
        if (i < len && !isspace((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                char_index = 0;
            }
            if (char_index < MAX_LENGTH - 1) {
                words[word_index][char_index++] = input[i];
            }
        } else {
            if (in_word) {
                words[word_index][char_index] = '\0';
                word_index++;
                in_word = 0;
            }
        }
    }

    printf("Individual words:\n");
    for (i = 0; i < word_index; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }

    return 0;
}