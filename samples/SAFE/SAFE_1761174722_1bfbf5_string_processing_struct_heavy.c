//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct WordStats {
    char word[64];
    size_t length;
    unsigned int vowel_count;
    unsigned int consonant_count;
};

struct TextAnalyzer {
    struct WordStats *words;
    size_t word_count;
    size_t capacity;
    char *text;
    size_t text_length;
};

void init_analyzer(struct TextAnalyzer *analyzer, const char *input) {
    if (analyzer == NULL || input == NULL) return;
    
    analyzer->text_length = strlen(input);
    if (analyzer->text_length == 0) return;
    
    analyzer->text = malloc(analyzer->text_length + 1);
    if (analyzer->text == NULL) return;
    
    strncpy(analyzer->text, input, analyzer->text_length);
    analyzer->text[analyzer->text_length] = '\0';
    
    analyzer->capacity = 16;
    analyzer->words = malloc(sizeof(struct WordStats) * analyzer->capacity);
    analyzer->word_count = 0;
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    free(analyzer->text);
    free(analyzer->words);
}

int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int is_consonant(char c) {
    c = tolower(c);
    return (c >= 'a' && c <= 'z' && !is_vowel(c));
}

void analyze_word(struct WordStats *stats, const char *word) {
    if (stats == NULL || word == NULL) return;
    
    size_t len = strlen(word);
    if (len >= 64) len = 63;
    
    strncpy(stats->word, word, len);
    stats->word[len] = '\0';
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
}

void process_text(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->text == NULL) return;
    
    char buffer[1024];
    if (strlen(analyzer->text) >= 1024) return;
    
    strncpy(buffer, analyzer->text, 1023);
    buffer[1023] = '\0';
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && analyzer->word_count < 100) {
        if (analyzer->word_count >= analyzer->capacity) {
            size_t new_capacity = analyzer->capacity * 2;
            if (new_capacity > 100) new_capacity = 100;
            struct WordStats *new_words = realloc(analyzer->words, sizeof(struct WordStats) * new_capacity);
            if (new_words == NULL) break;
            analyzer->words = new_words;
            analyzer->capacity = new_capacity;
        }
        
        analyze_word(&analyzer->words[analyzer->word_count], token);
        analyzer->word_count++;
        
        token = strtok(NULL, " \t\n\r");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text analysis results:\n");
    printf("Total words: %zu\n", analyzer->word_count);
    printf("\nWord breakdown:\n");
    
    for (size_t i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *stats = &analyzer->words[i];
        printf("Word: %-15s Length: %2zu Vowels: %2u Consonants: %2u\n",
               stats->word, stats->length, stats->vowel_count, stats->consonant_count);
    }
}

int main(void) {
    char input[1024];
    printf("Enter text to analyze (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer, input);
    
    if (analyzer.text == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    process_text(&analyzer);
    print_analysis(&analyzer);
    
    free_analyzer(&analyzer);
    return 0;
}