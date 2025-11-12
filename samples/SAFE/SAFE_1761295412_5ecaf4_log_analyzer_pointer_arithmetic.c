//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    const char *ptr = line;
    int field = 0;
    char *dest = NULL;
    size_t len = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        const char *start = ptr;
        while (*ptr && !isspace(*ptr)) ptr++;
        size_t field_len = ptr - start;
        
        switch (field) {
            case 0:
                dest = entry->timestamp;
                len = sizeof(entry->timestamp) - 1;
                break;
            case 1:
                dest = entry->level;
                len = sizeof(entry->level) - 1;
                break;
            case 2:
                dest = entry->message;
                len = sizeof(entry->message) - 1;
                break;
        }
        
        if (field_len >= len) field_len = len - 1;
        if (dest && len > 0) {
            memcpy(dest, start, field_len);
            *(dest + field_len) = '\0';
        }
        
        field++;
    }
    
    return field >= 2;
}

void analyze_logs(struct LogEntry *logs, size_t count) {
    if (!logs || count == 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    struct LogEntry *current = logs;
    struct LogEntry *end = logs + count;
    
    while (current < end) {
        char *level = current->level;
        
        if (*level == 'E' && strncmp(level, "ERROR", 5) == 0) {
            error_count++;
        } else if (*level == 'W' && strncmp(level, "WARN", 4) == 0) {
            warning_count++;
        } else if (*level == 'I' && strncmp(level, "INFO", 4) == 0) {
            info_count++;
        }
        
        current++;
    }
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries processed: %zu\n", count);
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    size_t log_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    while (log_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, logs + log_count)) {
            log_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (log_count > 0) {
        analyze_logs(logs, log_count);
        
        printf("\nRecent ERROR entries:\n");
        struct LogEntry *current = logs + log_count - 1;
        struct LogEntry *start = logs;
        int found = 0;
        
        while (current >= start && found < 3) {
            if (strncmp(current->level, "ERROR", 5) == 0) {
                printf("%s %s %s\n", current->timestamp, current->level, current->message);
                found++;
            }
            current--;
        }
        
        if (found == 0) {
            printf("No ERROR entries found.\n");
        }
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}