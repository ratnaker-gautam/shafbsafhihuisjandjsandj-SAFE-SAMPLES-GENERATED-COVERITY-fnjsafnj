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
    struct WordStats words[1000];
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
    analyzer->shortest_word_length = 1000;
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
    if (analyzer->word_count >= 1000) return;
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    
    size_t len = strlen(word);
    if (len >= 64) len = 63;
    
    strncpy(current->word, word, len);
    current->word[len] = '\0';
    current->length = (int)len;
    
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; i < current->length; i++) {
        if (is_vowel(current->word[i])) {
            current->vowel_count++;
        } else if (is_consonant(current->word[i])) {
            current->consonant_count++;
        }
    }
    
    analyzer->total_vowels += current->vowel_count;
    analyzer->total_consonants += current->consonant_count;
    
    if (current->length > analyzer->longest_word_length) {
        analyzer->longest_word_length = current->length;
    }
    
    if (current->length < analyzer->shortest_word_length && current->length > 0) {
        analyzer->shortest_word_length = current->length;
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[1024];
    if (strlen(text) >= 1024) return;
    
    strcpy(buffer, text);
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL && analyzer->word_count < 1000) {
        if (strlen(token) > 0) {
            analyze_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    
    if (analyzer->word_count > 0) {
        printf("Longest word length: %d\n", analyzer->longest_word_length);
        printf("Shortest word length: %d\n", analyzer->shortest_word_length);
        printf("Average word length: %.2f\n", 
               (float)(analyzer->total_vowels + analyzer->total_consonants) / analyzer->word_count);
        
        printf("\nWord details (first 10 words):\n");
        int limit = analyzer->word_count < 10 ? analyzer->word_count : 10;
        for (int i = 0; i < limit; i++) {
            printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d\n",
                   analyzer->words[i].word,
                   analyzer->words[i].length,
                   analyzer->words[i].vowel_count,
                   analyzer->words[i].consonant_count);
        }
    }
}

int main() {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    char input_text[1024];
    printf("Enter text to analyze (max 1023 characters):\n");
    
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
    
    process_text(&analyzer, input_text);
    print_analysis(&analyzer);
    
    return 0;
}