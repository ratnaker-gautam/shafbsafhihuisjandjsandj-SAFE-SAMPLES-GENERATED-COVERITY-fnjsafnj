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
    struct WordStats *words;
    int word_count;
    int max_words;
    int total_chars;
    int total_vowels;
    int total_consonants;
};

void init_analyzer(struct TextAnalyzer *analyzer, int capacity) {
    if (capacity <= 0) capacity = 100;
    analyzer->words = malloc(capacity * sizeof(struct WordStats));
    if (analyzer->words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    analyzer->max_words = capacity;
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
}

void cleanup_analyzer(struct TextAnalyzer *analyzer) {
    free(analyzer->words);
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

void analyze_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= analyzer->max_words) return;
    if (word == NULL || strlen(word) == 0) return;
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    size_t len = strlen(word);
    if (len >= sizeof(current->word)) len = sizeof(current->word) - 1;
    
    strncpy(current->word, word, len);
    current->word[len] = '\0';
    current->length = (int)len;
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; i < current->length; i++) {
        if (is_vowel(current->word[i])) {
            current->vowel_count++;
            analyzer->total_vowels++;
        } else if (is_consonant(current->word[i])) {
            current->consonant_count++;
            analyzer->total_consonants++;
        }
    }
    
    analyzer->total_chars += current->length;
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[1024];
    if (strlen(text) >= sizeof(buffer)) return;
    strcpy(buffer, text);
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && analyzer->word_count < analyzer->max_words) {
        analyze_word(analyzer, token);
        token = strtok(NULL, " \t\n\r");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("\nWord details:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %2d Consonants: %2d\n",
               word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer, 50);
    
    char input[1024];
    printf("Enter text to analyze (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        cleanup_analyzer(&analyzer);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        cleanup_analyzer(&analyzer);
        return 1;
    }
    
    process_text(&analyzer, input);
    print_analysis(&analyzer);
    
    cleanup_analyzer(&analyzer);
    return 0;
}