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
    i = 0;
    while (i < len) {
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
            processed[j++] = tolower((unsigned char)input[i]);
        } else if (isdigit((unsigned char)input[i])) {
            for (k = 0; k < 3; k++) {
                if (j < sizeof(processed) - 1) {
                    processed[j++] = input[i];
                }
            }
        } else {
            if (j < sizeof(processed) - 1) {
                processed[j++] = '*';
            }
        }
        i++;
    }
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_LENGTH];
    int left = 0;
    int right = len - 1;
    for (; left <= right; left++, right--) {
        char temp = input[left];
        reversed[left] = input[right];
        reversed[right] = temp;
    }
    reversed[len] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    i = 0;
    do {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        i++;
    } while (i < len);
    
    printf("Word count: %d\n", word_count);
    
    char search_char;
    printf("Enter a character to search for: ");
    if (scanf(" %c", &search_char) != 1) {
        fprintf(stderr, "Error reading character\n");
        return 1;
    }
    
    int char_count = 0;
    for (i = 0; i < len; i++) {
        if (input[i] == search_char) {
            char_count++;
        }
    }
    
    printf("Character '%c' appears %d times\n", search_char, char_count);
    
    return 0;
}