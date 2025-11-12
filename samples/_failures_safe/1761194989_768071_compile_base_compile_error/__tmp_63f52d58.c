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

int is_palindrome(const char *str, int len) {
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

int resize_words_array(struct TextAnalyzer *analyzer) {
    int new_capacity = analyzer->capacity == 0 ? 8 : analyzer->capacity * 2;
    struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
    if (new_words == NULL) return 0;
    analyzer->words = new_words;
    analyzer->capacity = new_capacity;
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(analyzer->text)) return 0;
    
    strncpy(analyzer->text, input, sizeof(analyzer->text) - 1);
    analyzer->text[sizeof(analyzer->text) - 1] = '\0';
    
    free_analyzer(analyzer);
    
    const char *ptr = analyzer->text;
    while (*ptr != '\0') {
        while (*ptr != '\0' && !isalpha(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        const char *word_start = ptr;
        while (*ptr != '\0' && isalpha(*ptr)) ptr++;
        int word_len = ptr - word_start;
        
        if (word_len >= 64) continue;
        
        if (analyzer->word_count >= analyzer->capacity) {
            if (!resize_words_array(analyzer)) return 0;
        }
        
        struct WordStats *current = &analyzer->words[analyzer->word_count];
        strncpy(current->word, word_start, word_len);
        current->word[word_len] = '\0';
        current->length = word_len;
        
        current->vowel_count = 0;
        current->consonant_count = 0;
        for (int i = 0; i < word_len; i++) {
            if (isalpha(current->word[i])) {
                if (is_vowel(current->word[i])) {
                    current->vowel_count++;
                } else {
                    current->consonant_count++;
                }
            }
        }
        
        current->is_palindrome = is_palindrome(current->word, word_len);
        analyzer->word_count++;
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis results:\n");
    printf("Input text: %s\n", analyzer->text);
    printf("Words found: %d\n", analyzer->word_count);
    printf("\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("Word %d: '%s'\n", i + 1, word->word);
        printf("  Length: %d\n", word->length);
        printf("  Vowels: %d\n", word->vowel_count);
        printf("  Consonants: %d\n", word->consonant_count);
        printf("  Palindrome: %s\n", word->is_palindrome ? "Yes" : "No");
        printf("\n");
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
        printf("Error