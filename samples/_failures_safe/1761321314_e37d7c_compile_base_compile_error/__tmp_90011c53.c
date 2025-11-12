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

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= 64) return 0;
    
    if (analyzer->word_count >= analyzer->capacity) {
        int new_capacity = analyzer->capacity == 0 ? 8 : analyzer->capacity * 2;
        struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
        if (new_words == NULL) return 0;
        analyzer->words = new_words;
        analyzer->capacity = new_capacity;
    }
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->length = len;
    
    ws->vowel_count = 0;
    ws->consonant_count = 0;
    for (int i = 0; i < len; i++) {
        if (is_vowel(ws->word[i])) {
            ws->vowel_count++;
        } else if (is_consonant(ws->word[i])) {
            ws->consonant_count++;
        }
    }
    
    analyzer->word_count++;
    return 1;
}

int parse_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= sizeof(analyzer->text)) return 0;
    
    strncpy(analyzer->text, input, sizeof(analyzer->text) - 1);
    analyzer->text[sizeof(analyzer->text) - 1] = '\0';
    
    free_analyzer(analyzer);
    
    const char *p = input;
    while (*p != '\0') {
        while (*p != '\0' && !isalpha(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && isalpha(*p)) p++;
        int word_len = p - start;
        
        if (!add_word(analyzer, start, word_len)) {
            free_analyzer(analyzer);
            return 0;
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text: %s\n", analyzer->text);
    printf("Word count: %d\n", analyzer->word_count);
    printf("\nWord analysis:\n");
    
    int total_vowels = 0;
    int total_consonants = 0;
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word %d: '%s' (length: %d, vowels: %d, consonants: %d)\n", 
               i + 1, ws->word, ws->length, ws->vowel_count, ws->consonant_count);
        total_vowels += ws->vowel_count;
        total_consonants += ws->consonant_count;
    }
    
    printf("\nTotal vowels: %d\n", total_vowels);
    printf("Total consonants: %d\n", total_consonants);
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!parse_text(&analyzer, input)) {
        printf("Error processing text.\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    print_analysis(&analyzer);
    free_