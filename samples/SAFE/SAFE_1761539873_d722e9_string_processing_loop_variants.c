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
            processed[j++] = '*';
        }
        i++;
    }
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_LENGTH];
    int left = 0;
    int right = len - 1;
    for (; left < len; left++, right--) {
        if (right >= 0) {
            reversed[left] = input[right];
        } else {
            reversed[left] = '\0';
            break;
        }
    }
    reversed[len] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    int idx = 0;
    do {
        if (idx < len && isalpha((unsigned char)input[idx])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        idx++;
    } while (idx <= len);
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}