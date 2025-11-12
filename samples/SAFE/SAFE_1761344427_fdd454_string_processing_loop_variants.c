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
    
    printf("Choose operation:\n");
    printf("1. Reverse string\n");
    printf("2. Count vowels\n");
    printf("3. Remove spaces\n");
    printf("4. Convert to uppercase\n");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    switch (choice) {
        case 1: {
            size_t i = 0;
            size_t j = len;
            while (j > 0) {
                j--;
                processed[i] = input[j];
                i++;
            }
            processed[i] = '\0';
            printf("Reversed: %s\n", processed);
            break;
        }
        
        case 2: {
            int vowel_count = 0;
            const char *vowels = "aeiouAEIOU";
            size_t pos = 0;
            do {
                if (strchr(vowels, input[pos]) != NULL) {
                    vowel_count++;
                }
                pos++;
            } while (pos < len);
            printf("Vowel count: %d\n", vowel_count);
            break;
        }
        
        case 3: {
            size_t src = 0;
            size_t dst = 0;
            for (; src < len; src++) {
                if (!isspace((unsigned char)input[src])) {
                    processed[dst] = input[src];
                    dst++;
                }
            }
            processed[dst] = '\0';
            printf("Without spaces: %s\n", processed);
            break;
        }
        
        case 4: {
            size_t idx = 0;
            while (idx < len) {
                processed[idx] = toupper((unsigned char)input[idx]);
                idx++;
            }
            processed[len] = '\0';
            printf("Uppercase: %s\n", processed);
            break;
        }
        
        default:
            fprintf(stderr, "Invalid choice\n");
            return 1;
    }
    
    return 0;
}