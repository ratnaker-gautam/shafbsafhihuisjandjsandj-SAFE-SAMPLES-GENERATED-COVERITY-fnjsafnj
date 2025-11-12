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
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Remove vowels\n");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    if (choice < 1 || choice > 4) {
        fprintf(stderr, "Invalid choice: %d\n", choice);
        return 1;
    }
    
    size_t i, j;
    
    switch (choice) {
        case 1:
            for (i = 0; i < len; i++) {
                processed[i] = toupper((unsigned char)input[i]);
            }
            processed[len] = '\0';
            break;
            
        case 2:
            for (i = 0; i < len; i++) {
                processed[i] = tolower((unsigned char)input[i]);
            }
            processed[len] = '\0';
            break;
            
        case 3:
            j = 0;
            for (i = len; i > 0; i--) {
                processed[j++] = input[i - 1];
            }
            processed[len] = '\0';
            break;
            
        case 4:
            j = 0;
            for (i = 0; i < len; i++) {
                char c = tolower((unsigned char)input[i]);
                if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
                    processed[j++] = input[i];
                }
            }
            processed[j] = '\0';
            break;
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    return 0;
}