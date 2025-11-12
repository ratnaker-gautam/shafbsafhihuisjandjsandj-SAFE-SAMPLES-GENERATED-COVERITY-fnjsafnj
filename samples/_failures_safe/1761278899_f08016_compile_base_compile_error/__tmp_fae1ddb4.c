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

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ';
}

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int count_consonants(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (isalpha((unsigned char)c) && c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            count++;
        }
    }
    return count;
}

int resize_words_array(struct TextAnalyzer *analyzer) {
    int new_capacity = analyzer->capacity == 0 ? 8 : analyzer->capacity * 2;
    struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
    if (new_words == NULL) {
        return 0;
    }
    analyzer->words = new_words;
    analyzer->capacity = new_capacity;
    return 1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= analyzer->capacity) {
        if (!resize_words_array(analyzer)) {
            return 0;
        }
    }
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    size_t len = strlen(word);
    if (len >= sizeof(ws->word)) {
        len = sizeof(ws->word) - 1;
    }
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->length = (int)len;
    ws->vowel_count = count_vowels(ws->word);
    ws->consonant_count = count_consonants(ws->word);
    analyzer->word_count++;
    return 1;
}

int validate_text(const char *text) {
    if (text == NULL) {
        return 0;
    }
    size_t len = strlen(text);
    if (len == 0 || len >= 1024) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(text[i])) {
            return 0;
        }
    }
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *input_text) {
    if (!validate_text(input_text)) {
        return 0;
    }
    strncpy(analyzer->text, input_text, sizeof(analyzer->text) - 1);
    analyzer->text[sizeof(analyzer->text) - 1] = '\0';
    
    char text_copy[1024];
    strcpy(text_copy, analyzer->text);
    
    char *token = strtok(text_copy, " ");
    while (token != NULL) {
        if (strlen(token) > 0) {
            if (!add_word(analyzer, token)) {
                return 0;
            }
        }
        token = strtok(NULL, " ");
    }
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis results:\n");
    printf("Input text: %s\n", analyzer->text);
    printf("Total words: %d\n", analyzer->word_count);
    printf("\nWord details:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word %d: '%s' (length: %d, vowels: %d, consonants: %d)\n", 
               i + 1, ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text to analyze (letters and spaces only, max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    size