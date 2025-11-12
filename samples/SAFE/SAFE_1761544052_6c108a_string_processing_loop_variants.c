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
    
    printf("Enter a string to process: ");
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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH - 1) {
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
            for (k = 0; k < digit && j < MAX_LENGTH * 2 - 1; k++) {
                processed[j++] = '*';
            }
        } else {
            processed[j++] = input[i];
        }
        
        if (j >= MAX_LENGTH * 2 - 1) {
            break;
        }
    }
    processed[j] = '\0';
    
    char reversed[MAX_LENGTH * 2];
    int rev_len = strlen(processed);
    for (i = rev_len - 1, k = 0; i >= 0; i--, k++) {
        reversed[k] = processed[i];
    }
    reversed[k] = '\0';
    
    int word_count = 0;
    int in_word = 0;
    for (i = 0; processed[i] != '\0'; i++) {
        if (isalpha((unsigned char)processed[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Word count: %d\n", word_count);
    
    int char_count[26] = {0};
    for (i = 0; processed[i] != '\0'; i++) {
        char c = processed[i];
        if (isalpha((unsigned char)c)) {
            char_count[tolower((unsigned char)c) - 'a']++;
        }
    }
    
    printf("Character frequencies:\n");
    for (i = 0; i < 26; i++) {
        if (char_count[i] > 0) {
            printf("%c: %d\n", 'a' + i, char_count[i]);
        }
    }
    
    return 0;
}