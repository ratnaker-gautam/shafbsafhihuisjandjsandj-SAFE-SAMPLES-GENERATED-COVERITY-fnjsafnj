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
    char text[1024];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->words = malloc(sizeof(struct WordStats) * 50);
    if (analyzer->words == NULL) {
        exit(1);
    }
    analyzer->word_count = 0;
    analyzer->capacity = 50;
    analyzer->text[0] = '\0';
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
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

void analyze_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= analyzer->capacity) {
        int new_capacity = analyzer->capacity * 2;
        struct WordStats *new_words = realloc(analyzer->words, sizeof(struct WordStats) * new_capacity);
        if (new_words == NULL) {
            return;
        }
        analyzer->words = new_words;
        analyzer->capacity = new_capacity;
    }
    
    struct WordStats *stats = &analyzer->words[analyzer->word_count];
    int len = 0;
    int vowels = 0;
    int consonants = 0;
    
    while (word[len] != '\0' && len < 63) {
        if (is_vowel(word[len])) {
            vowels++;
        } else if (is_consonant(word[len])) {
            consonants++;
        }
        len++;
    }
    
    strncpy(stats->word, word, 63);
    stats->word[63] = '\0';
    stats->length = len;
    stats->vowel_count = vowels;
    stats->consonant_count = consonants;
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer) {
    char buffer[1024];
    strncpy(buffer, analyzer->text, 1023);
    buffer[1023] = '\0';
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL) {
        if (strlen(token) > 0) {
            analyze_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *stats = &analyzer->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n",
               stats->word, stats->length, stats->vowel_count, stats->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    printf("Enter text to analyze (max 1023 characters): ");
    if (fgets(analyzer.text, sizeof(analyzer.text), stdin) == NULL) {
        printf("Error reading input\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    size_t len = strlen(analyzer.text);
    if (len > 0 && analyzer.text[len-1] == '\n') {
        analyzer.text[len-1] = '\0';
    }
    
    if (strlen(analyzer.text) == 0) {
        printf("No text entered\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    process_text(&analyzer);
    print_analysis(&analyzer);
    
    free_analyzer(&analyzer);
    return 0;
}