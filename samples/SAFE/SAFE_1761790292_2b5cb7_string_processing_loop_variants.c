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
    
    len = (int)strlen(input);
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
    i = 0;
    while (i < len) {
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
            k = 0;
            do {
                processed[j++] = tolower((unsigned char)input[i]);
                k++;
            } while (k < 2);
        } else if (isdigit((unsigned char)input[i])) {
            int digit = input[i] - '0';
            for (k = 0; k < digit && j < sizeof(processed) - 1; k++) {
                processed[j++] = '#';
            }
        } else {
            processed[j++] = input[i];
        }
        
        if (j >= (int)sizeof(processed) - 1) {
            break;
        }
        i++;
    }
    processed[j] = '\0';
    
    char reversed[MAX_LEN * 2];
    int rev_len = (int)strlen(processed);
    for (i = rev_len - 1, k = 0; i >= 0; i--, k++) {
        if (k < (int)sizeof(reversed) - 1) {
            reversed[k] = processed[i];
        } else {
            break;
        }
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
    
    return 0;
}