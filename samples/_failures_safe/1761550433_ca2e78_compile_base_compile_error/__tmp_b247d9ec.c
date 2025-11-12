//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct WordStats {
    char word[64];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordStats *words;
    int word_count;
    int capacity;
    char *text;
    int total_chars;
    int total_words;
};

void init_analyzer(struct TextAnalyzer *analyzer, int initial_capacity) {
    if (analyzer == NULL || initial_capacity <= 0) return;
    analyzer->words = malloc(initial_capacity * sizeof(struct WordStats));
    if (analyzer->words == NULL) return;
    analyzer->capacity = initial_capacity;
    analyzer->word_count = 0;
    analyzer->text = NULL;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
    }
    if (analyzer->text != NULL) {
        free(analyzer->text);
        analyzer->text = NULL;
    }
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

void analyze_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (analyzer == NULL || word == NULL || len <= 0) return;
    
    if (analyzer->word_count >= analyzer->capacity) {
        int new_capacity = analyzer->capacity * 2;
        if (new_capacity <= analyzer->capacity) return;
        struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
        if (new_words == NULL) return;
        analyzer->words = new_words;
        analyzer->capacity = new_capacity;
    }
    
    struct WordStats *stats = &analyzer->words[analyzer->word_count];
    int copy_len = len < 63 ? len : 63;
    strncpy(stats->word, word, copy_len);
    stats->word[copy_len] = '\0';
    stats->length = len;
    
    stats->vowel_count = 0;
    stats->consonant_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(word[i])) {
            stats->vowel_count++;
        } else if (is_consonant(word[i])) {
            stats->consonant_count++;
        }
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (analyzer == NULL || input == NULL) return;
    
    int len = strlen(input);
    if (len <= 0) return;
    
    analyzer->text = malloc(len + 1);
    if (analyzer->text == NULL) return;
    strncpy(analyzer->text, input, len);
    analyzer->text[len] = '\0';
    analyzer->total_chars = len;
    
    char buffer[256];
    int buf_pos = 0;
    
    for (int i = 0; i <= len; i++) {
        char c = input[i];
        if (isalnum(c) || c == '\'' || c == '-') {
            if (buf_pos < 255) {
                buffer[buf_pos++] = c;
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                analyze_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
                buf_pos = 0;
            }
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *stats = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               stats->word, stats->length, stats->vowel_count, stats->consonant_count);
    }
    
    if (analyzer->word_count > 0) {
        int total_vowels = 0;
        int total_consonants = 0;
        int total_letters = 0;
        
        for (int i = 0; i < analyzer->word_count; i++) {
            total_vowels += analyzer->words[i].vowel_count;
            total_consonants += analyzer->words[i].consonant_count;
            total_letters += analyzer->words[i].length;
        }
        
        printf("\nSummary:\n");
        printf("Total vowels: %d\n", total_vowels);
        printf("Total consonants: %d\n", total_consonants);
        printf("Average word length: %.2f\n", (float