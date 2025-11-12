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
    char lower = tolower(c);
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

int is_consonant(char c) {
    char lower = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int process_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= 1000) return 0;
    
    int len = strlen(word);
    if (len >= 64) return 0;
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    strncpy(current->word, word, 63);
    current->word[63] = '\0';
    current->length = len;
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(word[i])) {
            current->vowel_count++;
        } else if (is_consonant(word[i])) {
            current->consonant_count++;
        }
    }
    
    analyzer->total_vowels += current->vowel_count;
    analyzer->total_consonants += current->consonant_count;
    
    if (len > analyzer->longest_word_length) {
        analyzer->longest_word_length = len;
    }
    if (len < analyzer->shortest_word_length && len > 0) {
        analyzer->shortest_word_length = len;
    }
    
    analyzer->word_count++;
    return 1;
}

int extract_words(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return 0;
    
    char buffer[1024];
    if (strlen(text) >= 1023) return 0;
    strncpy(buffer, text, 1023);
    buffer[1023] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}");
    while (token != NULL && analyzer->word_count < 1000) {
        if (strlen(token) > 0) {
            process_word(analyzer, token);
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    
    if (analyzer->word_count > 0) {
        printf("Longest word length: %d\n", analyzer->longest_word_length);
        printf("Shortest word length: %d\n", analyzer->shortest_word_length);
        
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
    if (len > 0 && input_text[len-1] == '\n') {
        input_text[len-1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (!extract_words(&analyzer, input_text)) {
        printf("Error processing text.\n");
        return 1;
    }
    
    if (analyzer.word_count == 0) {
        printf("No valid words found in the text.\n");
        return 1;
    }
    
    print_analysis(&analyzer);
    
    return 0;
}