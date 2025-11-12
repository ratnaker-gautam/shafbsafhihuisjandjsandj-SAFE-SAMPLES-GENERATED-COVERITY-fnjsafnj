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

    if (len >= MAX_LEN - 1) {
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
    for (k = 0, i = alnum_count - 1; i >= 0; i--, k++) {
        if (k < MAX_LEN - 1) {
            reversed[k] = processed[i];
        } else {
            break;
        }
    }
    reversed[k] = '\0';

    printf("Reversed: %s\n", reversed);

    char vowels_only[MAX_LEN];
    int vowel_count = 0;
    char c;
    
    i = 0;
    while (i < len) {
        c = tolower((unsigned char)input[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            if (vowel_count < MAX_LEN - 1) {
                vowels_only[vowel_count++] = input[i];
            }
        }
        i++;
    }
    vowels_only[vowel_count] = '\0';

    printf("Vowels: %s\n", vowels_only);
    printf("Vowel count: %d\n", vowel_count);

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

    char no_duplicates[MAX_LEN];
    int dup_count = 0;
    int found;
    
    for (i = 0; i < len; i++) {
        found = 0;
        for (k = 0; k < dup_count; k++) {
            if (no_duplicates[k] == input[i]) {
                found = 1;
                break;
            }
        }
        if (!found && dup_count < MAX_LEN - 1) {
            no_duplicates[dup_count++] = input[i];
        }
    }
    no_duplicates[dup_count] = '\0';

    printf("Unique characters: %s\n", no_duplicates);
    printf("Unique count: %d\n", dup_count);

    return 0;
}