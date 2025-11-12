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
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_chars;
    int total_words;
};

struct AnalysisResult {
    struct TextProcessor processor;
    char most_frequent_word[MAX_WORD_LEN];
    int unique_word_count;
    double avg_word_length;
};

void initialize_processor(struct TextProcessor *proc) {
    if (proc == NULL) return;
    proc->word_count = 0;
    proc->total_chars = 0;
    proc->total_words = 0;
    proc->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].count = 0;
        proc->words[i].first_position = -1;
        proc->words[i].last_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.' || c == ',' || c == '!' || c == '?';
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

int process_text(struct TextProcessor *proc, const char *input) {
    if (input == NULL || proc == NULL) return 0;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) return 0;
    
    int in_word = 0;
    int word_start = 0;
    int text_pos = 0;
    
    for (int i = 0; buffer[i] != '\0' && text_pos < MAX_TEXT_LEN - 1; i++) {
        if (!is_valid_char(buffer[i])) continue;
        
        if (isalnum((unsigned char)buffer[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
            proc->processed_text[text_pos++] = tolower((unsigned char)buffer[i]);
        } else {
            if (in_word) {
                proc->processed_text[text_pos++] = ' ';
                in_word = 0;
                
                char current_word[MAX_WORD_LEN];
                int word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
                
                strncpy(current_word, &buffer[word_start], word_len);
                current_word[word_len] = '\0';
                for (int j = 0; j < word_len; j++) {
                    current_word[j] = tolower((unsigned char)current_word[j]);
                }
                
                int found = -1;
                for (int j = 0; j < proc->word_count; j++) {
                    if (strcmp(proc->words[j].word, current_word) == 0) {
                        found = j;
                        break;
                    }
                }
                
                if (found == -1 && proc->word_count < MAX_WORDS) {
                    found = proc->word_count;
                    safe_strncpy(proc->words[found].word, current_word, MAX_WORD_LEN);
                    proc->words[found].first_position = proc->total_words;
                    proc->words[found].count = 0;
                    proc->word_count++;
                }
                
                if (found != -1) {
                    proc->words[found].count++;
                    proc->words[found].last_position = proc->total_words;
                    proc->total_words++;
                }
            }
        }
    }
    
    proc->processed_text[text_pos] = '\0';
    proc->total_chars = text_pos;
    return 1;
}

void analyze_text(struct AnalysisResult *result, const struct TextProcessor *proc) {
    if (result == NULL || proc == NULL) return;
    
    result->processor = *proc;
    result->unique_word_count = proc->word_count;
    
    int max_count = 0;
    int total_length = 0;
    
    for (int i = 0; i < proc->word_count; i++) {
        total_length += strlen(proc->words[i].word) * proc->words[i].count;
        
        if (proc->words[i].count > max_count) {
            max_count = proc->words[i].count;
            safe_strncpy(result->most_frequent_word, proc->words[i].word, MAX_WORD_LEN);
        }
    }
    
    if (proc->total_words > 0) {
        result->avg_word_length = (double)total_length / proc->total_words;
    } else {
        result->avg_word_length = 0.0;
    }
}

void print_analysis(const struct AnalysisResult *result) {
    if