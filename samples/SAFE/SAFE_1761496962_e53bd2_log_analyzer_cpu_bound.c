//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
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
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int compare_entries(const void* a, const void* b) {
    const LogEntry* entry_a = (const LogEntry*)a;
    const LogEntry* entry_b = (const LogEntry*)b;
    return strcmp(entry_a->timestamp, entry_b->timestamp);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            total_errors += entries[entry_count].error_count;
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    qsort(entries, entry_count, sizeof(LogEntry), compare_entries);
    
    char unique_levels[10][16];
    int level_counts[10] = {0};
    int unique_level_count = 0;
    
    for (int i = 0; i < entry_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_level_count; j++) {
            if (strcmp(entries[i].level, unique_levels[j]) == 0) {
                level_counts[j]++;
                found = 1;
                break;
            }
        }
        if (!found && unique_level_count < 10) {
            strncpy(unique_levels[unique_level_count], entries[i].level, 15);
            unique_levels[unique_level_count][15] = '\0';
            level_counts[unique_level_count] = 1;
            unique_level_count++;
        }
    }
    
    int longest_message_length = 0;
    char longest_message[256] = "";
    
    for (int i = 0; i < entry_count; i++) {
        int msg_len = strlen(entries[i].message);
        if (msg_len > longest_message_length) {
            longest_message_length = msg_len;
            strncpy(longest_message, entries[i].message, 255);
            longest_message[255] = '\0';
        }
    }
    
    clock_t end_time = clock();
    double analysis_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Unique log levels: %d\n", unique_level_count);
    
    for (int i = 0; i < unique_level_count; i++) {
        printf("  %s: %d entries\n", unique_levels[i], level_counts[i]);
    }
    
    printf("Longest message (%d chars): %s\n", longest_message_length, longest_message);
    printf("Analysis time: %.6f seconds\n", analysis_time);
    
    return 0;
}