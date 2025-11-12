//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 100
#define MAX_WORD_LEN 50

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
};

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum(c) || c == '_' || c == '-';
}

int extract_words(const char* input, struct WordStats words[], int* word_count) {
    if (!is_valid_input(input)) return 0;
    if (words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t i = 0;
    
    while (i < input_len && *word_count < MAX_WORD_COUNT) {
        while (i < input_len && !is_word_char(input[i])) i++;
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && is_word_char(input[i])) i++;
        size_t end = i;
        
        size_t word_len = end - start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        strncpy(words[*word_count].word, input + start, word_len);
        words[*word_count].word[word_len] = '\0';
        words[*word_count].length = (int)word_len;
        words[*word_count].count = 1;
        (*word_count)++;
    }
    
    return 1;
}

int count_word_frequency(struct WordStats words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) continue;
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count == 0) continue;
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                words[j].count = 0;
            }
        }
    }
    
    int valid_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            words[valid_count] = words[i];
            valid_count++;
        }
    }
    
    return valid_count;
}

void to_lowercase_words(struct WordStats words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    for (int i = 0; i < word_count; i++) {
        for (int j = 0; words[i].word[j] != '\0'; j++) {
            words[i].word[j] = tolower(words[i].word[j]);
        }
    }
}

int compare_word_stats(const void* a, const void* b) {
    const struct WordStats* wa = (const struct WordStats*)a;
    const struct WordStats* wb = (const struct WordStats*)b;
    
    if (wa->count != wb->count) return wb->count - wa->count;
    return strcmp(wa->word, wb->word);
}

void sort_words(struct WordStats words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    qsort(words, word_count, sizeof(struct WordStats), compare_word_stats);
}

void print_word_stats(const struct WordStats words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("%-20s %-10s %-10s\n", "Word", "Count", "Length");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %-10d %-10d\n", words[i].word, words[i].count, words[i].length);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    struct WordStats words[MAX_WORD_COUNT];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input: empty or too long.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error extracting words from input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    to_lowercase_words(words, word_count);
    word_count = count_word_frequency(words, word_count);
    sort_words