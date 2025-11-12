//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_words;
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_words = 0;
    proc->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].count = 0;
        proc->words[i].first_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
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
    if (proc == NULL || input == NULL) return 0;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) return 0;
    
    char *ptr = buffer;
    int pos = 0;
    
    while (*ptr != '\0' && pos < MAX_TEXT_LEN - 1) {
        if (!is_valid_char(*ptr)) {
            ptr++;
            continue;
        }
        
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
            if (pos > 0 && proc->processed_text[pos-1] != ' ') {
                proc->processed_text[pos++] = ' ';
            }
            ptr++;
        }
        
        char word[MAX_WORD_LEN];
        int word_len = 0;
        
        while (isalnum((unsigned char)*ptr) && word_len < MAX_WORD_LEN - 1) {
            word[word_len++] = tolower((unsigned char)*ptr);
            ptr++;
        }
        
        if (word_len > 0) {
            word[word_len] = '\0';
            
            if (pos + word_len < MAX_TEXT_LEN - 1) {
                if (pos > 0 && proc->processed_text[pos-1] != ' ') {
                    proc->processed_text[pos++] = ' ';
                }
                memcpy(&proc->processed_text[pos], word, word_len);
                pos += word_len;
            }
            
            int found = 0;
            for (int i = 0; i < proc->word_count; i++) {
                if (strcmp(proc->words[i].word, word) == 0) {
                    proc->words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && proc->word_count < MAX_WORDS) {
                safe_strncpy(proc->words[proc->word_count].word, word, MAX_WORD_LEN);
                proc->words[proc->word_count].count = 1;
                proc->words[proc->word_count].first_position = proc->total_words;
                proc->word_count++;
            }
            
            proc->total_words++;
        }
    }
    
    proc->processed_text[pos] = '\0';
    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    if (proc == NULL) return;
    
    printf("Processed text: %s\n", proc->processed_text);
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->word_count);
    printf("\nWord statistics:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("'%s': count=%d, first_position=%d\n", 
               proc->words[i].word, proc->words[i].count, proc->words[i].first_position);
    }
}

int main() {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to process (max %d characters): ", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len-1] == '\n') {
        input_text[len-1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!process_text(&processor, input_text)) {
        printf("Error processing text\n");
        return 1;
    }
    
    print_stats(&processor);
    
    return 0;
}