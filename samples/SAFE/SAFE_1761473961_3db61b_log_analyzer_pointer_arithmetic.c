//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    int field = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        char *start = ptr;
        while (*ptr && !isspace(*ptr)) ptr++;
        
        size_t len = ptr - start;
        if (len == 0) break;
        
        switch (field) {
            case 0:
                if (len >= sizeof(entry->timestamp)) len = sizeof(entry->timestamp) - 1;
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
                break;
            case 1:
                if (len >= sizeof(entry->level)) len = sizeof(entry->level) - 1;
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
                break;
            case 2:
                if (len >= sizeof(entry->message)) len = sizeof(entry->message) - 1;
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
                break;
        }
        field++;
    }
    
    return field == 3;
}

void analyze_logs(struct LogEntry *logs, int count) {
    if (logs == NULL || count <= 0) return;
    
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
    printf("Total entries: %d\n", count);
    
    if (count > 0) {
        printf("\nSample entries:\n");
        for (int i = 0; i < 3 && i < count; i++) {
            printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
        }
    }
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message), one per line.\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (len > 1) {
            struct LogEntry *entry = logs + entry_count;
            if (parse_log_line(buffer, entry)) {
                entry_count++;
            } else {
                printf("Invalid log format. Skipping: %s\n", buffer);
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_logs(logs, entry_count);
    
    return 0;
}