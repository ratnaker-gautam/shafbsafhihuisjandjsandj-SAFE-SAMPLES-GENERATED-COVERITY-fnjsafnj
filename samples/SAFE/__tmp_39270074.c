//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[64];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char buffer[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(buffer, line);
    
    char* timestamp = strtok(buffer, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp)) return 0;
    if (strlen(level) >= sizeof(entry->level)) return 0;
    if (strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int is_valid_level(const char* level) {
    if (level == NULL) return 0;
    return (strcmp(level, "INFO") == 0 || 
            strcmp(level, "WARN") == 0 || 
            strcmp(level, "ERROR") == 0);
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    int info_count = 0, warn_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARN") == 0) warn_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO:  %d entries\n", info_count);
    printf("WARN:  %d entries\n", warn_count);
    printf("ERROR: %d entries\n", error_count);
}

void find_most_frequent_message(struct LogEntry* entries, int count) {
    if (count <= 0) return;
    
    char messages[MAX_ENTRIES][256];
    int frequencies[MAX_ENTRIES] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(entries[i].message, messages[j]) == 0) {
                frequencies[j]++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_ENTRIES) {
            if (strlen(entries[i].message) < sizeof(messages[0])) {
                strcpy(messages[unique_count], entries[i].message);
                frequencies[unique_count] = 1;
                unique_count++;
            }
        }
    }
    
    int max_freq = 0;
    int max_index = 0;
    for (int i = 0; i < unique_count; i++) {
        if (frequencies[i] > max_freq) {
            max_freq = frequencies[i];
            max_index = i;
        }
    }
    
    if (max_freq > 0) {
        printf("Most frequent message: \"%s\" (appears %d times)\n", 
               messages[max_index], max_freq);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARN, ERROR\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
            } else {
                printf("Invalid log level. Supported: INFO, WARN, ERROR\n");
            }
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    
    analyze_log_levels(entries, entry_count);
    find_most_frequent_message(