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
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    while (fgets(line, MAX_LINE_LEN, stdin) != NULL && entry_count < MAX_ENTRIES) {
        line_count++;
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            continue;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
            
            if (strcmp(entry.level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                total_warnings++;
            }
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total lines processed: %d\n", line_count);
    printf("Valid log entries: %d\n", entry_count);
    printf("Total ERROR entries: %d\n", total_errors);
    printf("Total WARNING entries: %d\n", total_warnings);
    printf("Error rate: %.2f%%\n", entry_count > 0 ? (total_errors * 100.0) / entry_count : 0.0);
    
    char most_common_level[16] = "NONE";
    int level_counts[3] = {0};
    const char* levels[] = {"INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 3; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    int max_count = 0;
    for (int i = 0; i < 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            strcpy(most_common_level, levels[i]);
        }
    }
    
    printf("Most common log level: %s (%d occurrences)\n", most_common_level, max_count);
    
    int recent_errors = 0;
    int recent_limit = entry_count > 100 ? 100 : entry_count;
    for (int i = entry_count - recent_limit; i < entry_count; i++) {
        if (i >= 0 && strcmp(entries[i].level, "ERROR") == 0) {
            recent_errors++;
        }
    }
    
    printf("Recent errors (last %d entries): %d\n", recent_limit, recent_errors);
    
    return 0;
}