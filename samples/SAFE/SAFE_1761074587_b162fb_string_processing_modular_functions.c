//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

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
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char* text, struct WordCount words[], int* word_count) {
    if (text == NULL || words == NULL || word_count == NULL) return 0;
    
    int count = 0;
    size_t len = strlen(text);
    size_t i = 0;
    
    while (i < len && count < MAX_WORDS) {
        while (i < len && !is_word_char(text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(text[i]) && (i - start) < MAX_WORD_LEN - 1) i++;
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[count].word, text + start, word_len);
            words[count].word[word_len] = '\0';
            words[count].count = 1;
            count++;
        }
    }
    
    *word_count = count;
    return 1;
}

int find_word_index(const struct WordCount words[], int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(struct WordCount words[], int* word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < *word_count; i++) {
        int found_index = find_word_index(words, unique_count, words[i].word);
        if (found_index == -1) {
            if (unique_count < MAX_WORDS) {
                if (i != unique_count) {
                    strncpy(words[unique_count].word, words[i].word, MAX_WORD_LEN - 1);
                    words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[unique_count].count = 1;
                }
                unique_count++;
            }
        } else {
            if (words[found_index].count < 1000) {
                words[found_index].count++;
            }
        }
    }
    
    *word_count = unique_count;
    return 1;
}

int compare_word_counts(const void* a, const void* b) {
    const struct WordCount* wa = (const struct WordCount*)a;
    const struct WordCount* wb = (const struct WordCount*)b;
    return wb->count - wa->count;
}

void print_word_stats(const struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("Total unique words: %d\n", count);
    printf("Top 10 most frequent words:\n");
    
    int print_count = (count > 10) ? 10 : count;
    for (int i = 0; i < print_count; i++) {
        printf("%2d. %-20s : %d\n", i + 1, words[i].word, words[i].count);
    }
}

int process_text_analysis(const char* input_text) {
    if (!is_valid_input(input_text)) return 0;
    
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    if (!extract_words(input_text, words, &word_count)) return 0;
    if (word_count == 0) return 1;
    
    if (!count_unique_words(words, &word_count)) return 0;
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_counts);
    print_word_stats(words, word_count);
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!process_text_analysis(input_buffer)) {
        printf("Error processing text.\n");
        return 1;
    }
    
    return 0;
}