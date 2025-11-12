//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
    struct WordStats words[1000];
    int word_count;
    int total_chars;
    int total_vowels;
    int total_consonants;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
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
    if (analyzer->word_count >= 1000) return 0;
    
    size_t len = strlen(word);
    if (len >= 64) return 0;
    
    struct WordStats *stats = &analyzer->words[analyzer->word_count];
    strncpy(stats->word, word, 63);
    stats->word[63] = '\0';
    stats->length = (int)len;
    stats->vowel_count = 0;
    stats->consonant_count = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (is_vowel(word[i])) {
            stats->vowel_count++;
            analyzer->total_vowels++;
        } else if (is_consonant(word[i])) {
            stats->consonant_count++;
            analyzer->total_consonants++;
        }
    }
    
    analyzer->total_chars += (int)len;
    analyzer->word_count++;
    return 1;
}

void analyze_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[1024];
    strncpy(buffer, text, 1023);
    buffer[1023] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL) {
        if (strlen(token) > 0) {
            process_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("\nWord Details:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *stats = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               stats->word, stats->length, stats->vowel_count, stats->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    char input_text[2048];
    printf("Enter text to analyze (max 2047 characters):\n");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    analyze_text(&analyzer, input_text);
    print_analysis(&analyzer);
    
    return 0;
}