//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0);
}

void count_log_levels(struct LogEntry* entries, int count, int* info_count, int* warn_count, int* error_count, int* debug_count) {
    *info_count = *warn_count = *error_count = *debug_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) (*info_count)++;
        else if (strcmp(entries[i].level, "WARN") == 0) (*warn_count)++;
        else if (strcmp(entries[i].level, "ERROR") == 0) (*error_count)++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) (*debug_count)++;
    }
}

void find_most_frequent_message(struct LogEntry* entries, int count, char* most_freq_msg, int* max_count) {
    strcpy(most_freq_msg, "");
    *max_count = 0;
    
    for (int i = 0; i < count; i++) {
        int current_count = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(entries[i].message, entries[j].message) == 0) {
                current_count++;
            }
        }
        if (current_count > *max_count) {
            *max_count = current_count;
            strcpy(most_freq_msg, entries[i].message);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer\n");
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter 'END' on a new line to finish input\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
                printf("Added: %s %s %s\n", entry.timestamp, entry.level, entry.message);
            } else {
                printf("Invalid log level. Skipping: %s\n", line);
            }
        } else {
            printf("Invalid format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int info_count, warn_count, error_count, debug_count;
    count_log_levels(entries, entry_count, &info_count, &warn_count, &error_count, &debug_count);
    
    printf("INFO: %d (%.1f%%)\n", info_count, (info_count * 100.0) / entry_count);
    printf("WARN: %d (%.1f%%)\n", warn_count, (warn_count * 100.0) / entry_count);
    printf("ERROR: %d (%.1f%%)\n", error_count, (error_count * 100.0) / entry_count);
    printf("DEBUG: %d (%.1f%%)\n", debug_count, (debug_count * 100.0) / entry_count);
    
    char most_freq_msg[256];
    int max_count;
    find_most_frequent_message(entries, entry_count, most