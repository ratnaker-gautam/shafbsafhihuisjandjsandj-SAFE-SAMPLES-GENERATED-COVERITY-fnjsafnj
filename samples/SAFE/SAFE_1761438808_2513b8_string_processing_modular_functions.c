//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char* text, struct WordCount* words, int* word_count) {
    if (text == NULL || words == NULL || word_count == NULL) return 0;
    
    int count = 0;
    size_t len = strlen(text);
    size_t i = 0;
    
    while (i < len && count < MAX_WORDS) {
        while (i < len && !is_word_char(text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(text[i])) i++;
        size_t word_len = i - start;
        
        if (word_len >= MAX_WORD_LEN) continue;
        
        char word[MAX_WORD_LEN];
        for (size_t j = 0; j < word_len; j++) {
            word[j] = tolower((unsigned char)text[start + j]);
        }
        word[word_len] = '\0';
        
        int found = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(words[j].word, word) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (count < MAX_WORDS) {
                strcpy(words[count].word, word);
                words[count].count = 1;
                count++;
            }
        }
    }
    
    *word_count = count;
    return 1;
}

int compare_word_count(const void* a, const void* b) {
    const struct WordCount* wa = (const struct WordCount*)a;
    const struct WordCount* wb = (const struct WordCount*)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void print_word_frequencies(const struct WordCount* words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies (sorted by count):\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}