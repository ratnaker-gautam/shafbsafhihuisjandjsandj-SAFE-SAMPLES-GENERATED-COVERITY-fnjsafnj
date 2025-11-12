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
    return isalpha(c) || c == ' ' || c == '\0';
}

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower(str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int count_consonants(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower(str[i]);
        if (isalpha(c) && c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
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
    memcpy(ws->word, word, len);
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

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!validate_text(text)) {
        return 0;
    }
    
    strncpy(analyzer->text, text, sizeof(analyzer->text) - 1);
    analyzer->text[sizeof(analyzer->text) - 1] = '\0';
    
    free_analyzer(analyzer);
    
    char buffer[64];
    int buf_index = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            if (buf_index < (int)sizeof(buffer) - 1) {
                buffer[buf_index++] = text[i];
            }
        } else if (buf_index > 0) {
            buffer[buf_index] = '\0';
            if (!add_word(analyzer, buffer)) {
                return 0;
            }
            buf_index = 0;
        }
    }
    
    if (buf_index > 0) {
        buffer[buf_index] = '\0';
        if (!add_word(analyzer, buffer)) {
            return 0;
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text: %s\n", analyzer->text);
    printf("Word count: %d\n", analyzer->word_count);
    printf("Word analysis:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("  %s: length=%d, vowels=%d, consonants=%d\n", 
               ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text