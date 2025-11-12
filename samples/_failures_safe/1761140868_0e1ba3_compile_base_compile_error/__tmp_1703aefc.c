//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_TEXT_LEN 10000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    double frequency;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    char text[MAX_TEXT_LEN];
    int text_length;
};

struct AnalysisResult {
    struct TextAnalyzer analyzer;
    int unique_words;
    char most_frequent_word[MAX_WORD_LEN];
    int max_count;
    double avg_word_length;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->text_length = 0;
    memset(analyzer->text, 0, sizeof(analyzer->text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, sizeof(analyzer->words[i].word));
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].frequency = 0.0;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int read_input_text(struct TextAnalyzer *analyzer) {
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(analyzer->text, MAX_TEXT_LEN, stdin) == NULL) {
        return 0;
    }
    
    analyzer->text_length = strlen(analyzer->text);
    if (analyzer->text_length > 0 && analyzer->text[analyzer->text_length - 1] == '\n') {
        analyzer->text[analyzer->text_length - 1] = '\0';
        analyzer->text_length--;
    }
    
    for (int i = 0; i < analyzer->text_length; i++) {
        if (!is_valid_char(analyzer->text[i])) {
            return 0;
        }
    }
    
    return analyzer->text_length > 0;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(struct TextAnalyzer *analyzer) {
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, analyzer->text, sizeof(buffer))) {
        return 0;
    }
    
    to_lowercase(buffer);
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && analyzer->word_count < MAX_WORDS) {
        int token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            int found = 0;
            for (int i = 0; i < analyzer->word_count; i++) {
                if (strcmp(analyzer->words[i].word, token) == 0) {
                    analyzer->words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (!safe_strncpy(analyzer->words[analyzer->word_count].word, token, MAX_WORD_LEN)) {
                    return 0;
                }
                analyzer->words[analyzer->word_count].count = 1;
                analyzer->words[analyzer->word_count].length = token_len;
                analyzer->word_count++;
            }
            analyzer->total_words++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->total_words > 0) {
            analyzer->words[i].frequency = (double)analyzer->words[i].count / analyzer->total_words;
        }
    }
    
    return 1;
}

void analyze_text(struct TextAnalyzer *analyzer, struct AnalysisResult *result) {
    result->analyzer = *analyzer;
    result->unique_words = analyzer->word_count;
    result->max_count = 0;
    result->avg_word_length = 0.0;
    
    memset(result->most_frequent_word, 0, sizeof(result->most_frequent_word));
    
    int total_length = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        total_length += analyzer->words[i].length * analyzer->words[i].count;
        
        if (analyzer->words[i].count > result->max_count) {
            result->max_count = analyzer->words[i].count;
            safe_strncpy(result->most_frequent_word, analyzer->words[i].word, MAX_WORD_LEN);
        }
    }