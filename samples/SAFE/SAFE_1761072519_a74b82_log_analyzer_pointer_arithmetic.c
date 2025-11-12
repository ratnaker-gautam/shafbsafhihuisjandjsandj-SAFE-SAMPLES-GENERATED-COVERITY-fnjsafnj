//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
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

void analyze_logs(struct LogEntry *logs, size_t count) {
    if (logs == NULL || count == 0) return;
    
    size_t error_count = 0;
    size_t warning_count = 0;
    size_t info_count = 0;
    
    struct LogEntry *current = logs;
    struct LogEntry *end = logs + count;
    
    while (current < end) {
        if (strcmp(current->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(current->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(current->level, "INFO") == 0) {
            info_count++;
        }
        current++;
    }
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %zu\n", error_count);
    printf("WARNING entries: %zu\n", warning_count);
    printf("INFO entries: %zu\n", info_count);
    printf("Total entries: %zu\n", count);
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    size_t log_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (log_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (line[0] == '\n') break;
        
        if (parse_log_line(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (log_count > 0) {
        analyze_logs(logs, log_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}