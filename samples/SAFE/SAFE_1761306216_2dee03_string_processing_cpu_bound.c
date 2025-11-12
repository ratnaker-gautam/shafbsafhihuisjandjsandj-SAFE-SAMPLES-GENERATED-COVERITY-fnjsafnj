//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

struct WordCount {
    char word[64];
    int count;
};

int is_word_char(char c) {
    return isalnum(c) || c == '_';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void sort_words(struct WordCount words[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
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
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    while (pos < len && word_count < MAX_WORDS) {
        while (pos < len && !is_word_char(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        int start = pos;
        while (pos < len && is_word_char(input[pos])) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len >= 63) {
            word_len = 62;
        }
        
        char current_word[64];
        strncpy(current_word, &input[start], word_len);
        current_word[word_len] = '\0';
        to_lower_case(current_word);
        
        int existing_index = find_word_index(words, word_count, current_word);
        if (existing_index != -1) {
            words[existing_index].count++;
        } else {
            if (word_count < MAX_WORDS) {
                strcpy(words[word_count].word, current_word);
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("=======================\n");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}