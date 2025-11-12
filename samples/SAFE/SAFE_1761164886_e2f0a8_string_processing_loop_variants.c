//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    int choice;
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
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
    
    printf("Choose processing method:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Reverse string\n");
    printf("3. Remove vowels\n");
    printf("4. Count words\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    if (choice < 1 || choice > 4) {
        fprintf(stderr, "Choice must be between 1 and 4\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            size_t i = 0;
            while (input[i] != '\0') {
                processed[i] = toupper((unsigned char)input[i]);
                i++;
            }
            processed[i] = '\0';
            printf("Uppercase: %s\n", processed);
            break;
        }
        
        case 2: {
            size_t i = len;
            size_t j = 0;
            do {
                if (i > 0) {
                    processed[j] = input[i - 1];
                    j++;
                    i--;
                }
            } while (i > 0);
            processed[j] = '\0';
            printf("Reversed: %s\n", processed);
            break;
        }
        
        case 3: {
            size_t i = 0;
            size_t j = 0;
            for (; input[i] != '\0'; i++) {
                char c = tolower((unsigned char)input[i]);
                if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
                    processed[j] = input[i];
                    j++;
                }
            }
            processed[j] = '\0';
            printf("Without vowels: %s\n", processed);
            break;
        }
        
        case 4: {
            int word_count = 0;
            int in_word = 0;
            size_t i = 0;
            
            while (input[i] != '\0') {
                if (isspace((unsigned char)input[i])) {
                    if (in_word) {
                        word_count++;
                        in_word = 0;
                    }
                } else {
                    in_word = 1;
                }
                i++;
            }
            
            if (in_word) {
                word_count++;
            }
            
            printf("Word count: %d\n", word_count);
            break;
        }
    }
    
    return 0;
}