//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordStats {
    char word[MAX_WORD_LEN];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_vowels;
    int total_consonants;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
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

int analyze_word(const char *word, struct WordStats *stats) {
    if (word == NULL || stats == NULL) return 0;
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    strncpy(stats->word, word, MAX_WORD_LEN - 1);
    stats->word[MAX_WORD_LEN - 1] = '\0';
    stats->length = (int)len;
    stats->vowel_count = 0;
    stats->consonant_count = 0;
    
    for (int i = 0; i < stats->length; i++) {
        if (is_vowel(stats->word[i])) {
            stats->vowel_count++;
        } else if (is_consonant(stats->word[i])) {
            stats->consonant_count++;
        }
    }
    
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (analyzer == NULL || text == NULL) return 0;
    
    init_analyzer(analyzer);
    
    char buffer[512];
    if (strlen(text) >= sizeof(buffer)) return 0;
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        if (analyze_word(token, &analyzer->words[analyzer->word_count])) {
            analyzer->total_chars += analyzer->words[analyzer->word_count].length;
            analyzer->total_vowels += analyzer->words[analyzer->word_count].vowel_count;
            analyzer->total_consonants += analyzer->words[analyzer->word_count].consonant_count;
            analyzer->word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL) return;
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("\nWord Details:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("Word: %-15s Length: %2d Vowels: %2d Consonants: %2d\n",
               analyzer->words[i].word,
               analyzer->words[i].length,
               analyzer->words[i].vowel_count,
               analyzer->words[i].consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    char input[512];
    
    printf("Enter text to analyze (max 511 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_text(&analyzer, input)) {
        printf("Error processing text.\n");
        return 1;
    }
    
    print_analysis(&analyzer);
    
    return 0;
}