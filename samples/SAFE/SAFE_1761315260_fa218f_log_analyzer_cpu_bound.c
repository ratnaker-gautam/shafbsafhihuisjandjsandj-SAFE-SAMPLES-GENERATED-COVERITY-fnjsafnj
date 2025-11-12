//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void find_most_frequent_word(const LogEntry* entries, int count, char* result) {
    char words[10000][32];
    int counts[10000] = {0};
    int word_count = 0;
    
    for (int i = 0; i < count; i++) {
        char temp_msg[256];
        strncpy(temp_msg, entries[i].message, 255);
        temp_msg[255] = '\0';
        
        char* word = strtok(temp_msg, " ,.!?;:\t\n");
        while (word != NULL) {
            if (strlen(word) > 2) {
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j], word) == 0) {
                        counts[j]++;
                        found = 1;
                        break;
                    }
                }
                if (!found && word_count < 10000) {
                    strncpy(words[word_count], word, 31);
                    words[word_count][31] = '\0';
                    counts[word_count] = 1;
                    word_count++;
                }
            }
            word = strtok(NULL, " ,.!?;:\t\n");
        }
    }
    
    int max_count = 0;
    int max_index = 0;
    for (int i = 0; i < word_count; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_index = i;
        }
    }
    
    if (word_count > 0) {
        strncpy(result, words[max_index], 31);
        result[31] = '\0';
    } else {
        strcpy(result, "N/A");
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warn_count = count_log_level(entries, entry_count, "WARN");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    
    char frequent_word[32];
    find_most_frequent_word(entries, entry_count, frequent_word);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    printf("Most frequent word: %s\n", frequent_word);
    printf("Analysis time: %.6f seconds\n", cpu_time);
    
    return 0;
}