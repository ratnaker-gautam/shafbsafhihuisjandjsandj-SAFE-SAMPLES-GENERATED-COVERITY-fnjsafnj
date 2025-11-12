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
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
        size_t field_len = ptr - start;
        
        if (field_len == 0) break;
        
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
        
        if (field_len > len) field_len = len;
        memcpy(dest, start, field_len);
        *(dest + field_len) = '\0';
        
        field++;
    }
    
    return field == 3;
}

void analyze_logs(struct LogEntry *logs, int count) {
    if (!logs || count <= 0) return;
    
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
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message). Empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, logs + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(logs, entry_count);
        
        printf("\nRecent ERROR entries:\n");
        struct LogEntry *current = logs;
        struct LogEntry *end = logs + entry_count;
        int found_errors = 0;
        
        while (current < end) {
            if (strcmp(current->level, "ERROR") == 0) {
                printf("%s %s %s\n", current->timestamp, current->level, current->message);
                found_errors++;
                if (found_errors >= 5) break;
            }
            current++;
        }
        
        if (found_errors == 0) {
            printf("No ERROR entries found.\n");
        }
    } else {
        printf("No valid log entries provided.\n");
    }
    
    return 0;
}