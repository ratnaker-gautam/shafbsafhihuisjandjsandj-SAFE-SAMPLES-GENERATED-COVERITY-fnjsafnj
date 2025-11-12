//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    int counts[26] = {0};
    
    printf("Enter text to process: ");
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
        printf("No input provided\n");
        return 1;
    }
    
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            char c = tolower((unsigned char)input[i]);
            processed[j++] = c;
            if (c >= 'a' && c <= 'z') {
                counts[c - 'a']++;
            }
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        printf("No alphabetic characters found\n");
        return 1;
    }
    
    printf("Processed text (lowercase letters only): %s\n", processed);
    printf("Character frequencies:\n");
    
    int i = 0;
    while (i < 26) {
        if (counts[i] > 0) {
            printf("%c: %d\n", 'a' + i, counts[i]);
        }
        i++;
    }
    
    char reversed[MAX_INPUT];
    int k = j - 1;
    int l = 0;
    do {
        if (k >= 0) {
            reversed[l++] = processed[k--];
        }
    } while (k >= 0);
    reversed[l] = '\0';
    
    printf("Reversed processed text: %s\n", reversed);
    
    int vowel_count = 0;
    int m = 0;
    char *ptr = processed;
    while (*ptr) {
        switch (*ptr) {
            case 'a': case 'e': case 'i': case 'o': case 'u':
                vowel_count++;
                break;
        }
        ptr++;
        m++;
    }
    
    printf("Vowel count: %d\n", vowel_count);
    printf("Consonant count: %d\n", j - vowel_count);
    
    return 0;
}