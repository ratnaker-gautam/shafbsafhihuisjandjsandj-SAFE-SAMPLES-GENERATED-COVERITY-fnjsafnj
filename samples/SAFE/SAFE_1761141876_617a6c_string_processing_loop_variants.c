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
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
            processed[j++] = tolower((unsigned char)input[i]);
        } else if (isdigit((unsigned char)input[i])) {
            int digit = input[i] - '0';
            for (k = 0; k < digit && j < sizeof(processed) - 1; k++) {
                processed[j++] = '*';
            }
        } else {
            if (j < sizeof(processed) - 1) {
                processed[j++] = input[i];
            }
        }
        
        if (j >= sizeof(processed) - 1) {
            break;
        }
    }
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    int word_count = 0;
    int in_word = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_LEN];
    for (i = len - 1, k = 0; i >= 0; i--, k++) {
        reversed[k] = input[i];
    }
    reversed[k] = '\0';
    printf("Reversed: %s\n", reversed);
    
    int char_freq[256] = {0};
    for (i = 0; i < len; i++) {
        unsigned char c = input[i];
        if (c < 256) {
            char_freq[c]++;
        }
    }
    
    printf("Character frequencies:\n");
    for (i = 0; i < 256; i++) {
        if (char_freq[i] > 0 && isprint(i)) {
            printf("'%c': %d\n", i, char_freq[i]);
        }
    }
    
    return 0;
}