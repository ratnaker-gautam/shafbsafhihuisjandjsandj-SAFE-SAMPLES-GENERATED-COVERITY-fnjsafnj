//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    int i, j, k;
    int len;

    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("Empty string provided\n");
        return 0;
    }

    if (len >= MAX_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    j = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
            processed[j++] = tolower((unsigned char)input[i]);
        } else if (isdigit((unsigned char)input[i])) {
            int digit = input[i] - '0';
            for (k = 0; k < digit && j < sizeof(processed) - 1; k++) {
                processed[j++] = '*';
            }
        } else {
            processed[j++] = input[i];
        }

        if (j >= sizeof(processed) - 1) {
            break;
        }
    }
    processed[j] = '\0';

    char reversed[MAX_LEN];
    int rev_idx = 0;
    i = len - 1;
    while (i >= 0) {
        reversed[rev_idx++] = input[i--];
    }
    reversed[rev_idx] = '\0';

    int word_count = 0;
    int in_word = 0;
    for (i = 0; i < len; i++) {
        if (isspace((unsigned char)input[i])) {
            in_word = 0;
        } else {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        }
    }

    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Word count: %d\n", word_count);

    int char_counts[26] = {0};
    for (i = 0; i < len; i++) {
        char c = tolower((unsigned char)input[i]);
        if (c >= 'a' && c <= 'z') {
            char_counts[c - 'a']++;
        }
    }

    printf("Character frequencies:\n");
    for (i = 0; i < 26; i++) {
        if (char_counts[i] > 0) {
            printf("%c: %d\n", 'a' + i, char_counts[i]);
        }
    }

    return 0;
}