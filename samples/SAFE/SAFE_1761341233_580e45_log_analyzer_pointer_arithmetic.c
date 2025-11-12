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
        
        const char *start = ptr;
        while (*ptr && !isspace(*ptr)) ptr++;
        size_t field_len = ptr - start;
        
        if (field == 0) {
            dest = entry->timestamp;
            len = sizeof(entry->timestamp) - 1;
        } else if (field == 1) {
            dest = entry->level;
            len = sizeof(entry->level) - 1;
        } else {
            dest = entry->message;
            len = sizeof(entry->message) - 1;
        }
        
        if (field_len >= len) field_len = len - 1;
        memcpy(dest, start, field_len);
        *(dest + field_len) = '\0';
        
        field++;
    }
    
    return field == 3;
}

void analyze_logs(struct LogEntry *logs, size_t count) {
    if (!logs || count == 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (size_t i = 0; i < count; i++) {
        struct LogEntry *entry = logs + i;
        
        if (strcmp(entry->level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entry->level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entry->level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %zu\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        for (size_t i = 0; i < count; i++) {
            struct LogEntry *entry = logs + i;
            if (strcmp(entry->level, "ERROR") == 0) {
                printf("- %s: %s\n", entry->timestamp, entry->message);
            }
        }
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    size_t log_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (timestamp level message), one per line.\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter empty line to finish.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, logs + log_count)) {
            log_count++;
        } else {
            printf("Invalid log format. Use: timestamp level message\n");
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_logs(logs, log_count);
    
    return 0;
}