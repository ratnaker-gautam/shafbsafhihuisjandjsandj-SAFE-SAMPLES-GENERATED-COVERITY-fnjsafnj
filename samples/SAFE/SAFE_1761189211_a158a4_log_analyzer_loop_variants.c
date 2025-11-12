//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char *token = strtok(temp_line, " ");
    if (!token) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry *entries, int count, const char *level) {
    if (!entries || !level || count <= 0) return 0;
    
    int level_count = 0;
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
        i++;
    }
    return level_count;
}

void print_log_summary(const struct LogEntry *entries, int count) {
    if (!entries || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char *levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    
    int j;
    for (j = 0; j < num_levels; j++) {
        int cnt = count_log_level(entries, count, levels[j]);
        printf("%s: %d\n", levels[j], cnt);
    }
    
    printf("\nRecent entries:\n");
    int start_idx = (count > 5) ? count - 5 : 0;
    int k = start_idx;
    do {
        if (k >= 0 && k < count) {
            printf("%s %s %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        }
        k++;
    } while (k < count && k < start_idx + 5);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char *sample_logs[] = {
        "2024-01-15T10:30:00 INFO System started successfully",
        "2024-01-15T10:31:15 DEBUG Initializing modules",
        "2024-01-15T10:32:45 WARNING High memory usage detected",
        "2024-01-15T10:33:20 INFO User login: john_doe",
        "2024-01-15T10:34:10 ERROR Database connection failed",
        "2024-01-15T10:35:00 INFO Retrying connection",
        "2024-01-15T10:35:30 DEBUG Connection parameters: host=db1",
        "2024-01-15T10:36:15 INFO Connection established",
        "2024-01-15T10:37:00 WARNING Cache size approaching limit",
        "2024-01-15T10:38:45 INFO Processing completed"
    };
    
    int num_samples = sizeof(sample_logs) / sizeof(sample_logs[0]);
    int i;
    
    for (i = 0; i < num_samples; i++) {
        if (entry_count >= MAX_ENTRIES) break;
        
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_log_summary(entries, entry_count);
    
    printf("\nSearching for ERROR entries:\n");
    int found_errors = 0;
    int idx;
    for (idx = 0; idx < entry_count; idx++) {
        if (strcmp(entries[idx].level, "ERROR") == 0) {
            printf("Found: %s %s\n", entries[idx].timestamp, entries[idx].message);
            found_errors = 1;
        }
    }
    
    if (!found_errors) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}