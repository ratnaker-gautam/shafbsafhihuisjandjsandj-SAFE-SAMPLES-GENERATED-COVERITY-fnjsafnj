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
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int analyze_word(const char *word, struct WordInfo *info) {
    if (word == NULL || strlen(word) == 0 || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    
    strncpy(info->word, word, MAX_WORD_LEN - 1);
    info->word[MAX_WORD_LEN - 1] = '\0';
    info->length = strlen(info->word);
    info->vowel_count = 0;
    info->consonant_count = 0;
    
    for (int i = 0; i < info->length; i++) {
        if (is_vowel(info->word[i])) {
            info->vowel_count++;
        } else if (is_consonant(info->word[i])) {
            info->consonant_count++;
        }
    }
    
    return 1;
}

int process_text(const char *text, struct TextAnalyzer *analyzer) {
    if (text == NULL || analyzer == NULL) {
        return 0;
    }
    
    char buffer[512];
    if (strlen(text) >= sizeof(buffer)) {
        return 0;
    }
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        if (analyze_word(token, &analyzer->words[analyzer->word_count])) {
            struct WordInfo *current = &analyzer->words[analyzer->word_count];
            
            analyzer->total_vowels += current->vowel_count;
            analyzer->total_consonants += current->consonant_count;
            
            if (current->length > analyzer->longest_word_length) {
                analyzer->longest_word_length = current->length;
            }
            if (current->length < analyzer->shortest_word_length) {
                analyzer->shortest_word_length = current->length;
            }
            
            analyzer->word_count++;
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    
    printf("\nWord Details:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordInfo *word = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
               word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    char input_text[512];
    
    initialize_analyzer(&analyzer);
    
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
    
    if (!process_text(input_text, &analyzer)) {
        printf("Error processing text.\n");
        return 1;
    }
    
    print_analysis(&analyzer);
    
    return 0;
}