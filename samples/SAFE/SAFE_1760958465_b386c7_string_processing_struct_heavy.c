//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordStats {
    char word[MAX_WORD_LEN];
    size_t length;
    size_t vowel_count;
    size_t consonant_count;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    size_t word_count;
    size_t total_chars;
    size_t total_vowels;
    size_t total_consonants;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
    for (size_t i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].length = 0;
        analyzer->words[i].vowel_count = 0;
        analyzer->words[i].consonant_count = 0;
    }
}

int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int is_consonant(char c) {
    c = tolower(c);
    return (c >= 'a' && c <= 'z' && !is_vowel(c));
}

int process_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return 0;
    }
    
    struct WordStats *stats = &analyzer->words[analyzer->word_count];
    strncpy(stats->word, word, MAX_WORD_LEN - 1);
    stats->word[MAX_WORD_LEN - 1] = '\0';
    stats->length = len;
    
    stats->vowel_count = 0;
    stats->consonant_count = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (is_vowel(word[i])) {
            stats->vowel_count++;
        } else if (is_consonant(word[i])) {
            stats->consonant_count++;
        }
    }
    
    analyzer->total_chars += len;
    analyzer->total_vowels += stats->vowel_count;
    analyzer->total_consonants += stats->consonant_count;
    analyzer->word_count++;
    
    return 1;
}

void tokenize_and_process(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[1024];
    if (strlen(text) >= sizeof(buffer)) {
        return;
    }
    
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        if (strlen(token) > 0) {
            process_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %zu\n", analyzer->word_count);
    printf("Total characters: %zu\n", analyzer->total_chars);
    printf("Total vowels: %zu\n", analyzer->total_vowels);
    printf("Total consonants: %zu\n", analyzer->total_consonants);
    printf("\nWord Details:\n");
    
    for (size_t i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *stats = &analyzer->words[i];
        printf("Word: %-15s Length: %2zu Vowels: %zu Consonants: %zu\n",
               stats->word, stats->length, stats->vowel_count, stats->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text to analyze (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    tokenize_and_process(&analyzer, input);
    print_analysis(&analyzer);
    
    return 0;
}