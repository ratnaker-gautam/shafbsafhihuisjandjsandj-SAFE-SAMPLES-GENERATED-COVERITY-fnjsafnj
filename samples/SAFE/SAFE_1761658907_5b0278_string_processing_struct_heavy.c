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
    if (capacity <= 0) {
        capacity = 100;
    }
    analyzer->words = malloc(capacity * sizeof(struct WordStats));
    if (analyzer->words == NULL) {
        exit(EXIT_FAILURE);
    }
    analyzer->max_words = capacity;
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
}

void cleanup_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
    }
}

int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int is_consonant(char c) {
    c = tolower(c);
    return (c >= 'a' && c <= 'z' && !is_vowel(c));
}

int check_palindrome(const char *str, int len) {
    if (len <= 0) {
        return 0;
    }
    for (int i = 0; i < len / 2; i++) {
        if (tolower(str[i]) != tolower(str[len - 1 - i])) {
            return 0;
        }
    }
    return 1;
}

void process_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (analyzer->word_count >= analyzer->max_words) {
        return;
    }
    if (len <= 0 || len >= 64) {
        return;
    }
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    strncpy(current->word, word, len);
    current->word[len] = '\0';
    current->length = len;
    current->vowel_count = 0;
    current->consonant_count = 0;
    current->is_palindrome = check_palindrome(word, len);
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(word[i])) {
            current->vowel_count++;
            analyzer->total_vowels++;
        } else if (is_consonant(word[i])) {
            current->consonant_count++;
            analyzer->total_consonants++;
        }
    }
    
    analyzer->total_chars += len;
    analyzer->word_count++;
}

void analyze_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    int text_len = strlen(text);
    if (text_len <= 0) {
        return;
    }
    
    char buffer[64];
    int buf_index = 0;
    
    for (int i = 0; i <= text_len; i++) {
        if (i < text_len && isalpha(text[i])) {
            if (buf_index < 63) {
                buffer[buf_index++] = text[i];
            }
        } else {
            if (buf_index > 0) {
                process_word(analyzer, buffer, buf_index);
                buf_index = 0;
            }
        }
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

int main(void) {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer, 50);
    
    char input_text[1024];
    printf("Enter text to analyze: ");
    
    if (fgets(input_text, sizeof(input_text), stdin) != NULL) {
        size_t len = strlen(input_text);
        if (len > 0 && input_text[len - 1] == '\n') {
            input_text[len - 1] = '\0';
        }
        
        analyze_text(&analyzer, input_text);
        print_analysis(&analyzer);
    }
    
    cleanup_analyzer(&analyzer);
    return 0;
}