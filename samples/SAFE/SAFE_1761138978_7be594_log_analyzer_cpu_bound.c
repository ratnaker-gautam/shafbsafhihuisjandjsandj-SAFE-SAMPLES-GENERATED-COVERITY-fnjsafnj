//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    if (!timestamp) return 0;
    
    char* level = strtok(NULL, " ");
    if (!level) return 0;
    
    char* message = strtok(NULL, "\n");
    if (!message) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
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

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_counts[4] = {0};
    
    for (int i = 0; i < 4; i++) {
        level_counts[i] = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d (%.1f%%)\n", levels[i], level_counts[i], 
               (float)level_counts[i] / entry_count * 100);
    }
    
    int other_count = entry_count;
    for (int i = 0; i < 4; i++) {
        other_count -= level_counts[i];
    }
    
    if (other_count > 0) {
        printf("OTHER: %d (%.1f%%)\n", other_count, 
               (float)other_count / entry_count * 100);
    }
    
    int error_count = count_log_level(entries, entry_count, "ERROR");
    if (error_count > 0) {
        printf("\nERROR entries found:\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s %s %s\n", entries[i].timestamp, 
                       entries[i].level, entries[i].message);
            }
        }
    }
    
    return 0;
}