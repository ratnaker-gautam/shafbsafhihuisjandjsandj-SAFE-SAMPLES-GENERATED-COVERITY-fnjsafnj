//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    char *end = ptr + strlen(line);
    
    if (end - ptr < 25) return 0;
    
    char *timestamp_end = ptr + 19;
    if (*timestamp_end != ' ') return 0;
    
    memcpy(entry->timestamp, ptr, 19);
    entry->timestamp[19] = '\0';
    
    ptr = timestamp_end + 1;
    
    char *level_end = ptr;
    while (level_end < end && *level_end != ' ' && level_end - ptr < 9) {
        level_end++;
    }
    
    if (level_end >= end || level_end - ptr == 0) return 0;
    
    size_t level_len = level_end - ptr;
    memcpy(entry->level, ptr, level_len);
    entry->level[level_len] = '\0';
    
    ptr = level_end + 1;
    if (ptr >= end) return 0;
    
    size_t msg_len = end - ptr;
    if (msg_len >= MAX_LINE_LENGTH - 30) {
        msg_len = MAX_LINE_LENGTH - 31;
    }
    
    memcpy(entry->message, ptr, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry *logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    struct LogEntry *current = logs;
    struct LogEntry *end = logs + count;
    
    while (current < end) {
        char *level = current->level;
        
        if (strcmp(level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(level, "INFO") == 0) {
            info_count++;
        }
        
        current++;
    }
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", count);
    
    if (count > 0) {
        printf("Error rate: %.2f%%\n", (error_count * 100.0) / count);
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (len < 25) {
            printf("Invalid log line (too short), skipping...\n");
            continue;
        }
        
        if (parse_log_line(line, logs + entry_count)) {
            entry_count++;
        } else {
            printf("Failed to parse log line, skipping...\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(logs, entry_count);
    
    printf("\nLast 5 log entries:\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    struct LogEntry *display = logs + start;
    struct LogEntry *display_end = logs + entry_count;
    
    while (display < display_end) {
        printf("%s %s %s\n", display->timestamp, display->level, display->message);
        display++;
    }
    
    return 0;
}