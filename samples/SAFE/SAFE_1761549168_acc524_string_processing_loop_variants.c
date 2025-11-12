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
    
    printf("Enter a string (max %d chars): ", MAX_LENGTH - 1);
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
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    if (len >= MAX_LENGTH) {
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
            int digit = input[i] - '0';
            for (k = 0; k < digit && j < MAX_LENGTH * 2 - 1; k++) {
                processed[j++] = '*';
            }
        } else {
            processed[j++] = input[i];
        }
        i++;
    }
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_LENGTH];
    int start = 0;
    int end = len - 1;
    for (; start < len; start++, end--) {
        if (end >= 0) {
            reversed[start] = input[end];
        } else {
            break;
        }
    }
    reversed[len] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    int idx = 0;
    do {
        if (isspace((unsigned char)input[idx]) || input[idx] == '\0') {
            if (in_word) {
                word_count++;
                in_word = 0;
            }
        } else {
            in_word = 1;
        }
        idx++;
    } while (input[idx - 1] != '\0');
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}