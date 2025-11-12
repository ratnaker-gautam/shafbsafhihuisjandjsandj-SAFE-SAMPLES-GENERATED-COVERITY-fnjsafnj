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
    analyzer->words = NULL;
    analyzer->word_count = 0;
    analyzer->capacity = 0;
    analyzer->text[0] = '\0';
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
    }
    analyzer->word_count = 0;
    analyzer->capacity = 0;
}

int is_alpha_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_vowel(char c) {
    char lower = tolower(c);
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

int resize_words_array(struct TextAnalyzer *analyzer, int new_capacity) {
    if (new_capacity <= analyzer->capacity) return 1;
    struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
    if (new_words == NULL) return 0;
    analyzer->words = new_words;
    analyzer->capacity = new_capacity;
    return 1;
}

int analyze_word(struct TextAnalyzer *analyzer, const char *word_start, const char *word_end) {
    if (analyzer->word_count >= analyzer->capacity) {
        int new_cap = analyzer->capacity == 0 ? 8 : analyzer->capacity * 2;
        if (!resize_words_array(analyzer, new_cap)) return 0;
    }
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    size_t word_len = word_end - word_start;
    if (word_len >= sizeof(current->word)) word_len = sizeof(current->word) - 1;
    
    memcpy(current->word, word_start, word_len);
    current->word[word_len] = '\0';
    current->length = (int)word_len;
    current->vowel_count = 0;
    current->consonant_count = 0;
    
    for (int i = 0; i < current->length; i++) {
        if (is_alpha_char(current->word[i])) {
            if (is_vowel(current->word[i])) {
                current->vowel_count++;
            } else {
                current->consonant_count++;
            }
        }
    }
    
    analyzer->word_count++;
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(analyzer->text)) return 0;
    
    strcpy(analyzer->text, input);
    
    const char *ptr = analyzer->text;
    const char *word_start = NULL;
    
    while (*ptr != '\0') {
        if (is_alpha_char(*ptr)) {
            if (word_start == NULL) {
                word_start = ptr;
            }
        } else {
            if (word_start != NULL) {
                if (!analyze_word(analyzer, word_start, ptr)) return 0;
                word_start = NULL;
            }
        }
        ptr++;
    }
    
    if (word_start != NULL) {
        if (!analyze_word(analyzer, word_start, ptr)) return 0;
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis results:\n");
    printf("Input text: %s\n", analyzer->text);
    printf("Total words: %d\n", analyzer->word_count);
    printf("\nWord details:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("Word %d: '%s' (length: %d, vowels: %d, consonants: %d)\n",
               i + 1, word->word, word->length, word->vowel_count, word->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    if (!process_text(&analyzer, input)) {
        printf("Error processing text\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    print_