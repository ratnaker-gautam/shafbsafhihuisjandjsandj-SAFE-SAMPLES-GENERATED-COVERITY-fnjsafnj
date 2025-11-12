//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    int i, j, count;
    
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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    count = 1;
    
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            input[i] = tolower((unsigned char)input[i]);
        }
    }
    
    i = 0;
    while (i < len) {
        char current = input[i];
        int char_count = 0;
        
        int k = i;
        while (k < len && input[k] == current) {
            char_count++;
            k++;
        }
        
        if (char_count > 2) {
            int written = snprintf(processed + j, sizeof(processed) - j, "%d%c", char_count, current);
            if (written < 0 || written >= (int)(sizeof(processed) - j)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            j += written;
        } else {
            int l;
            for (l = 0; l < char_count; l++) {
                if (j >= (int)sizeof(processed) - 1) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                processed[j++] = current;
            }
        }
        
        i += char_count;
    }
    
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    
    char *ptr = input;
    do {
        if (isalpha((unsigned char)*ptr)) {
            switch (*ptr) {
                case 'a': case 'e': case 'i': case 'o': case 'u':
                    vowel_count++;
                    break;
                default:
                    consonant_count++;
                    break;
            }
        }
        ptr++;
    } while (*ptr != '\0');
    
    printf("Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);
    
    return 0;
}