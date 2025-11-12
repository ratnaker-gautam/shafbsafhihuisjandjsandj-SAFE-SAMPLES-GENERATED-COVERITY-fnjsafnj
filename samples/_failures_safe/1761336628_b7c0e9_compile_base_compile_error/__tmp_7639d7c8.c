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

int check_palindrome(const char *str, int len) {
    if (len <= 1) return 1;
    int left = 0;
    int right = len - 1;
    while (left < right) {
        if (tolower(str[left]) != tolower(str[right])) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
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
    
    ws->is_palindrome = check_palindrome(ws->word, len);
    analyzer->word_count++;
    return 1;
}

int parse_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(analyzer->text)) return 0;
    
    strncpy(analyzer->text, input, sizeof(analyzer->text) - 1);
    analyzer->text[sizeof(analyzer->text) - 1] = '\0';
    
    int in_word = 0;
    int word_start = 0;
    
    for (int i = 0; analyzer->text[i] != '\0'; i++) {
        if (isalpha(analyzer->text[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
            }
        } else {
            if (in_word) {
                int word_len = i - word_start;
                if (!add_word(analyzer, &analyzer->text[word_start], word_len)) {
                    return 0;
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word) {
        int word_len = strlen(analyzer->text) - word_start;
        if (!add_word(analyzer, &analyzer->text[word_start], word_len)) {
            return 0;
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text: %s\n", analyzer->text);
    printf("Word count: %d\n", analyzer->word_count);
    printf("\nWord analysis:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d  Palindrome: %s\n",
               ws->word, ws->length, ws->vowel_count, ws->consonant_count,
               ws->is_palindrome ? "Yes" : "No");
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