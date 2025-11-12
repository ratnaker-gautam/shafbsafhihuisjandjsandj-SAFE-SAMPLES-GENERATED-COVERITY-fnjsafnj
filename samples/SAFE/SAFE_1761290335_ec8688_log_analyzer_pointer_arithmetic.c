//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN - 48];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;
    
    char *timestamp_end = strchr(ptr, ' ');
    if (timestamp_end == NULL) return 0;
    size_t timestamp_len = timestamp_end - ptr;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    strncpy(entry->timestamp, ptr, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    ptr = timestamp_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;
    
    char *level_end = strchr(ptr, ' ');
    if (level_end == NULL) return 0;
    size_t level_len = level_end - ptr;
    if (level_len >= sizeof(entry->level)) return 0;
    strncpy(entry->level, ptr, level_len);
    entry->level[level_len] = '\0';
    
    ptr = level_end + 1;
    while (*ptr && isspace(*ptr)) ptr++;
    if (*ptr == '\0') return 0;
    
    size_t msg_len = strlen(ptr);
    if (msg_len >= sizeof(entry->message)) return 0;
    strncpy(entry->message, ptr, msg_len);
    entry->message[msg_len] = '\0';
    
    char *newline = strchr(entry->message, '\n');
    if (newline != NULL) *newline = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry *entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int i = 0; i < count; i++) {
        char *level = entries[i].level;
        if (strcmp(level, "ERROR") == 0) error_count++;
        else if (strcmp(level, "WARNING") == 0) warning_count++;
        else if (strcmp(level, "INFO") == 0) info_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nERROR entries:\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0) {
                printf("%s: %s\n", entries[i].timestamp, entries[i].message);
            }
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (line[0] == '\n' || line[0] == '\0') break;
        
        if (parse_log_line(line, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}