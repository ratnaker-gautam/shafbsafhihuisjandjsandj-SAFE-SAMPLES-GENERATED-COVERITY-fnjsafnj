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
    int capacity;
    char text_buffer[1024];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->words = NULL;
    analyzer->word_count = 0;
    analyzer->capacity = 0;
    analyzer->text_buffer[0] = '\0';
}

void cleanup_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
    }
    analyzer->word_count = 0;
    analyzer->capacity = 0;
}

int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int is_consonant(char c) {
    c = tolower(c);
    return (c >= 'a' && c <= 'z' && !is_vowel(c));
}

int resize_word_array(struct TextAnalyzer *analyzer) {
    int new_capacity = analyzer->capacity == 0 ? 16 : analyzer->capacity * 2;
    struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
    if (new_words == NULL) {
        return 0;
    }
    analyzer->words = new_words;
    analyzer->capacity = new_capacity;
    return 1;
}

int process_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= 64) {
        return 0;
    }
    
    if (analyzer->word_count >= analyzer->capacity) {
        if (!resize_word_array(analyzer)) {
            return 0;
        }
    }
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    strncpy(current->word, word, len);
    current->word[len] = '\0';
    current->length = len;
    
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(current->word[i])) {
            current->vowel_count++;
        } else if (is_consonant(current->word[i])) {
            current->consonant_count++;
        }
    }
    
    analyzer->word_count++;
    return 1;
}

int analyze_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL || strlen(input) >= 1024) {
        return 0;
    }
    
    strncpy(analyzer->text_buffer, input, 1023);
    analyzer->text_buffer[1023] = '\0';
    
    const char *ptr = analyzer->text_buffer;
    int word_start = -1;
    
    for (int i = 0; analyzer->text_buffer[i] != '\0'; i++) {
        if (isalpha(analyzer->text_buffer[i])) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (!process_word(analyzer, &analyzer->text_buffer[word_start], word_len)) {
                    return 0;
                }
                word_start = -1;
            }
        }
    }
    
    if (word_start != -1) {
        int word_len = strlen(analyzer->text_buffer) - word_start;
        if (!process_word(analyzer, &analyzer->text_buffer[word_start], word_len)) {
            return 0;
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("Word %d: '%s'\n", i + 1, word->word);
        printf("  Length: %d\n", word->length);
        printf("  Vowels: %d\n", word->vowel_count);
        printf("  Consonants: %d\n", word->consonant_count);
        printf("\n");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text to analyze (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        cleanup_analyzer(&analyzer);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        cleanup_analyzer(&analyzer);
        return 1;
    }
    
    if