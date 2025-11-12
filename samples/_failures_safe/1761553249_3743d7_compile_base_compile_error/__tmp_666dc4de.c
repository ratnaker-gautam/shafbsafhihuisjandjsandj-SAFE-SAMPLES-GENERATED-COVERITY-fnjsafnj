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
    int is_palindrome;
};

struct TextAnalyzer {
    struct WordStats *words;
    int word_count;
    int max_words;
    int total_chars;
    int total_vowels;
    int total_consonants;
};

void initialize_analyzer(struct TextAnalyzer *analyzer, int capacity) {
    if (capacity <= 0) capacity = 10;
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
    analyzer->words = NULL;
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int check_palindrome(const char *str, int len) {
    if (len <= 1) return 1;
    for (int i = 0; i < len / 2; i++) {
        if (tolower(str[i]) != tolower(str[len - 1 - i])) {
            return 0;
        }
    }
    return 1;
}

void analyze_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= analyzer->max_words) {
        int new_capacity = analyzer->max_words * 2;
        struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
        if (new_words == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            return;
        }
        analyzer->words = new_words;
        analyzer->max_words = new_capacity;
    }
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    int word_len = 0;
    while (word[word_len] != '\0' && word_len < 63) {
        current->word[word_len] = word[word_len];
        word_len++;
    }
    current->word[word_len] = '\0';
    
    current->length = word_len;
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; i < word_len; i++) {
        if (is_vowel(current->word[i])) {
            current->vowel_count++;
            analyzer->total_vowels++;
        } else if (is_consonant(current->word[i])) {
            current->consonant_count++;
            analyzer->total_consonants++;
        }
    }
    
    analyzer->total_chars += word_len;
    current->is_palindrome = check_palindrome(current->word, word_len);
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[1024];
    int buf_index = 0;
    
    for (int i = 0; text[i] != '\0' && i < 1023; i++) {
        if (isalnum(text[i])) {
            buffer[buf_index++] = text[i];
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                analyze_word(analyzer, buffer);
                buf_index = 0;
            }
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        analyze_word(analyzer, buffer);
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
        const struct WordStats *word = &analyzer->words[i];
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d Palindrome: %s\n",
               word->word, word->length, word->vowel_count, word->consonant_count,
               word->is_palindrome ? "Yes" : "No");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer, 20);
    
    char input[2048];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        cleanup_analyzer(&analyzer);
        return 1;