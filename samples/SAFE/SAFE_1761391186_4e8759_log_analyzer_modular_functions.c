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
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp_start = strchr(temp_line, '[');
    if (timestamp_start == NULL) return 0;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (timestamp_end == NULL) return 0;
    
    size_t timestamp_len = timestamp_end - timestamp_start - 1;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, timestamp_start + 1, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    char* level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char* level_end = strchr(level_start, ':');
    if (level_end == NULL) return 0;
    
    size_t level_len = level_end - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    char* message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    size_t message_len = strlen(message_start);
    if (message_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_statistics(const struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int level_counts[4] = {0};
    
    for (int i = 0; i < 4; i++) {
        level_counts[i] = count_log_level(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    int error_count = count_log_level(entries, count, "ERROR");
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        int printed = 0;
        for (int i = count - 1; i >= 0 && printed < 3; i--) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("- %s: %s\n", entries[i].timestamp, entries[i].message);
                printed++;
            }
        }
    }
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    printf("Enter log data (one entry per line, format: [timestamp] LEVEL: message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (entry_count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    return entry_count;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    int entry_count = read_log_file(entries, MAX_ENTRIES);
    
    if (entry_count > 0) {
        print_statistics(entries, entry_count);
    } else {
        printf("No valid log entries were processed.\n");
    }
    
    return 0;
}