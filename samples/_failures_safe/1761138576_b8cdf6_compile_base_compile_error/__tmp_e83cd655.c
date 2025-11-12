//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
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
    
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp_start = strchr(temp_line, '[');
    if (timestamp_start == NULL) return 0;
    
    char* timestamp_end = strchr(timestamp_start, ']');
    if (timestamp_end == NULL) return 0;
    
    *timestamp_end = '\0';
    if (strlen(timestamp_start + 1) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, timestamp_start + 1);
    
    char* level_start = timestamp_end + 1;
    while (*level_start == ' ') level_start++;
    
    char* level_end = strchr(level_start, ' ');
    if (level_end == NULL) return 0;
    
    *level_end = '\0';
    if (strlen(level_start) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, level_start);
    
    char* message_start = level_end + 1;
    while (*message_start == ' ') message_start++;
    
    if (strlen(message_start) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message_start);
    
    return 1;
}

int count_log_levels(struct LogEntry* entries, int count, const char* level) {
    if (entries == NULL || level == NULL) return 0;
    
    int level_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
    }
    return level_count;
}

void print_log_summary(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_levels(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_log_levels(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_log_levels(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_levels(entries, count, "DEBUG"));
    
    printf("\nRecent entries:\n");
    int display_count = (count < 5) ? count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    if (entries == NULL || max_entries <= 0) return 0;
    
    printf("Enter log entries (one per line, format: [timestamp] LEVEL message):\n");
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

int validate_log_data(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return 0;
    
    for (int i = 0; i < count; i++) {
        if (strlen(entries[i].timestamp) == 0) return 0;
        if (strlen(entries[i].level) == 0) return 0;
        if (strlen(entries[i].message) == 0) return 0;
        
        const char* valid_levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
        int valid_level = 0;
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, valid_levels[j]) == 0) {
                valid_level = 1;
                break;
            }
        }
        if (!valid_level) return 0;
    }
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    int entry_count = read_log_file(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    if (!validate_log_data(entries, entry_count)) {