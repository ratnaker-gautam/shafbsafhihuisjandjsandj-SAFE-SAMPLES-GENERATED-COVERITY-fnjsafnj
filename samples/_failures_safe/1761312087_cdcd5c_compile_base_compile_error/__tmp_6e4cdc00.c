//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordInfo {
    char word[MAX_WORD_LEN];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_vowels;
    int total_consonants;
    int longest_word_length;
    int shortest_word_length;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
}

int is_vowel(char c) {
    char lower = tolower(c);
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

int is_consonant(char c) {
    char lower = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int process_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return 0;
    }
    
    struct WordInfo *info = &analyzer->words[analyzer->word_count];
    
    if (strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    strncpy(info->word, word, MAX_WORD_LEN - 1);
    info->word[MAX_WORD_LEN - 1] = '\0';
    
    info->length = (int)len;
    info->vowel_count = 0;
    info->consonant_count = 0;
    
    for (int i = 0; i < info->length; i++) {
        if (is_vowel(info->word[i])) {
            info->vowel_count++;
        } else if (is_consonant(info->word[i])) {
            info->consonant_count++;
        }
    }
    
    analyzer->total_vowels += info->vowel_count;
    analyzer->total_consonants += info->consonant_count;
    
    if (info->length > analyzer->longest_word_length) {
        analyzer->longest_word_length = info->length;
    }
    
    if (info->length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = info->length;
    }
    
    analyzer->word_count++;
    return 1;
}

void tokenize_and_process(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[512];
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
    if (analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("Average word length: %.2f\n", 
           (float)(analyzer->total_vowels + analyzer->total_consonants) / analyzer->word_count);
    
    printf("\nWord Details:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordInfo *info = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               info->word, info->length, info->vowel_count, info->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    char input_text[512];
    printf("Enter text to analyze (max 511 characters): ");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
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