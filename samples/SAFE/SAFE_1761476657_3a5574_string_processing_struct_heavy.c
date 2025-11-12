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

void init_analyzer(struct TextAnalyzer *analyzer, int max_words) {
    if (max_words <= 0) max_words = 100;
    analyzer->words = malloc(max_words * sizeof(struct WordStats));
    if (analyzer->words == NULL) exit(1);
    analyzer->max_words = max_words;
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

int check_palindrome(const char *str, int len) {
    if (len <= 0) return 0;
    for (int i = 0; i < len / 2; i++) {
        if (tolower(str[i]) != tolower(str[len - i - 1])) {
            return 0;
        }
    }
    return 1;
}

void analyze_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= analyzer->max_words) return;
    
    int len = 0;
    while (word[len] != '\0' && len < 63) len++;
    if (len == 0) return;
    
    struct WordStats *stats = &analyzer->words[analyzer->word_count];
    
    strncpy(stats->word, word, 63);
    stats->word[63] = '\0';
    stats->length = len;
    stats->vowel_count = 0;
    stats->consonant_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(word[i])) {
            stats->vowel_count++;
            analyzer->total_vowels++;
        } else if (is_consonant(word[i])) {
            stats->consonant_count++;
            analyzer->total_consonants++;
        }
    }
    
    stats->is_palindrome = check_palindrome(word, len);
    analyzer->total_chars += len;
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    char buffer[1024];
    strncpy(buffer, text, 1023);
    buffer[1023] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    while (token != NULL && analyzer->word_count < analyzer->max_words) {
        analyze_word(analyzer, token);
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
        printf("Word: %-15s Length: %2d Vowels: %d Consonants: %d Palindrome: %s\n",
               stats->word, stats->length, stats->vowel_count, 
               stats->consonant_count, stats->is_palindrome ? "Yes" : "No");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer, 50);
    
    char input[1024];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        cleanup_analyzer(&analyzer);
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
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