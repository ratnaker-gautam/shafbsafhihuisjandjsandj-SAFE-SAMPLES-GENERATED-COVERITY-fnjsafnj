//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LEN 256
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum(c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    char buffer[MAX_LINE_LEN];
    if (strlen(text) >= MAX_LINE_LEN) {
        return 0;
    }
    strcpy(buffer, text);
    
    int word_count = 0;
    char *ptr = buffer;
    
    while (*ptr && word_count < max_words) {
        while (*ptr && !is_valid_char(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_valid_char(*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len >= MAX_WORD_LEN) {
            continue;
        }
        
        char current_word[MAX_WORD_LEN];
        strncpy(current_word, word_start, word_len);
        current_word[word_len] = '\0';
        
        for (char *p = current_word; *p; p++) {
            *p = tolower(*p);
        }
        
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strcpy(words[word_count].word, current_word);
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void sort_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int get_input(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return 0;
    }
    
    printf("Enter text (max %zu characters): ", size - 1);
    
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    char input[MAX_LINE_LEN];
    
    if (!get_input(input, sizeof(input))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    struct WordCount words[MAX_WORDS];
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}