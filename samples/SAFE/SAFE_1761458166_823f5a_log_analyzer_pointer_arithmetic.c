//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH - 48];
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
        
        switch (field) {
            case 0: dest = entry->timestamp; len = 31; break;
            case 1: dest = entry->level; len = 15; break;
            case 2: dest = entry->message; len = MAX_LINE_LENGTH - 49; break;
        }
        
        size_t i = 0;
        while (*ptr && !isspace(*ptr) && i < len) {
            *dest++ = *ptr++;
            i++;
        }
        *dest = '\0';
        field++;
    }
    
    return field == 3;
}

void analyze_logs(struct LogEntry *logs, size_t count) {
    if (!logs || count == 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
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
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %zu\n", count);
    
    if (count > 0) {
        printf("First timestamp: %s\n", logs->timestamp);
        printf("Last timestamp: %s\n", (logs + count - 1)->timestamp);
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    size_t log_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES && fgets(buffer, sizeof(buffer), stdin)) {
        if (strncmp(buffer, "END", 3) == 0 && (buffer[3] == '\n' || buffer[3] == '\0')) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len <= 1) continue;
        
        if (parse_log_line(buffer, logs + log_count)) {
            log_count++;
        } else {
            printf("Invalid log format. Expected: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(logs, log_count);
    
    return 0;
}