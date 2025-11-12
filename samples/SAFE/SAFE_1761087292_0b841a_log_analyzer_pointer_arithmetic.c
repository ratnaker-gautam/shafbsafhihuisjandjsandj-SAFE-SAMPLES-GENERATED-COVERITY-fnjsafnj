//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    const char *ptr = line;
    size_t len = strlen(line);
    if (len < 30 || len >= MAX_LINE_LENGTH) return 0;
    
    if (*ptr != '[') return 0;
    ptr++;
    
    char *timestamp_end = strchr(ptr, ']');
    if (!timestamp_end || (timestamp_end - ptr) >= 19) return 0;
    
    size_t timestamp_len = timestamp_end - ptr;
    strncpy(entry->timestamp, ptr, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    char *level_end = strchr(ptr, ' ');
    if (!level_end || (level_end - ptr) >= 9) return 0;
    
    size_t level_len = level_end - ptr;
    strncpy(entry->level, ptr, level_len);
    entry->level[level_len] = '\0';
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    
    size_t msg_len = strlen(ptr);
    if (msg_len == 0 || msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, ptr, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

void analyze_log_levels(struct LogEntry *entries, size_t count) {
    if (!entries || count == 0) return;
    
    int info_count = 0, warn_count = 0, error_count = 0, debug_count = 0;
    
    for (size_t i = 0; i < count; i++) {
        struct LogEntry *entry = entries + i;
        
        if (strcmp(entry->level, "INFO") == 0) info_count++;
        else if (strcmp(entry->level, "WARN") == 0) warn_count++;
        else if (strcmp(entry->level, "ERROR") == 0) error_count++;
        else if (strcmp(entry->level, "DEBUG") == 0) debug_count++;
    }
    
    printf("Log Level Analysis:\n");
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("DEBUG: %d\n", debug_count);
    printf("Total entries: %zu\n", count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    size_t entry_count = 0;
    
    printf("Enter log entries (format: [timestamp] LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_log_levels(entries, entry_count);
    
    printf("\nRecent entries (last 5):\n");
    size_t start = (entry_count > 5) ? entry_count - 5 : 0;
    for (size_t i = start; i < entry_count; i++) {
        struct LogEntry *entry = entries + i;
        printf("[%s] %s %s\n", entry->timestamp, entry->level, entry->message);
    }
    
    return 0;
}