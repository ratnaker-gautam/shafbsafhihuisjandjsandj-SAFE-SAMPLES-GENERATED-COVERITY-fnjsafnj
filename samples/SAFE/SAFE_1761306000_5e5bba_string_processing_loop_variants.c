//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }

    int alnum_count = j;
    processed[j] = '\0';

    printf("Alphanumeric characters: %s\n", processed);
    printf("Count: %d\n", alnum_count);

    char reversed[MAX_LEN];
    for (i = len - 1, k = 0; i >= 0; i--, k++) {
        if (k < MAX_LEN - 1) {
            reversed[k] = input[i];
        } else {
            break;
        }
    }
    reversed[k] = '\0';

    printf("Reversed string: %s\n", reversed);

    int word_count = 0;
    int in_word = 0;
    for (i = 0; i < len; i++) {
        if (isspace((unsigned char)input[i])) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
    }

    printf("Word count: %d\n", word_count);

    char vowels[] = "aeiouAEIOU";
    int vowel_count = 0;
    int consonant_count = 0;

    for (i = 0; i < len; i++) {
        char c = input[i];
        if (isalpha((unsigned char)c)) {
            int is_vowel = 0;
            for (k = 0; vowels[k] != '\0'; k++) {
                if (c == vowels[k]) {
                    is_vowel = 1;
                    break;
                }
            }
            if (is_vowel) {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
    }

    printf("Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);

    char no_duplicates[MAX_LEN];
    int dup_index = 0;
    int seen[256] = {0};

    for (i = 0; i < len && dup_index < MAX_LEN - 1; i++) {
        unsigned char c = (unsigned char)input[i];
        if (!seen[c]) {
            no_duplicates[dup_index++] = c;
            seen[c] = 1;
        }
    }
    no_duplicates[dup_index] = '\0';

    printf("Without duplicates: %s\n", no_duplicates);

    return 0;
}