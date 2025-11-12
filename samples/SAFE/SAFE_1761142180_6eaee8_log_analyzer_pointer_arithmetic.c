//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char *ptr = line;
    int field = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        
        char *start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
        
        if (ptr > start) {
            size_t len = ptr - start;
            if (field == 0 && len < sizeof(entry->timestamp)) {
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
            } else if (field == 1 && len < sizeof(entry->level)) {
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
            } else if (field == 2) {
                size_t msg_len = len;
                if (msg_len >= sizeof(entry->message)) {
                    msg_len = sizeof(entry->message) - 1;
                }
                strncpy(entry->message, start, msg_len);
                entry->message[msg_len] = '\0';
            }
        }
        
        field++;
        if (*ptr) ptr++;
    }
    
    return field >= 2;
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
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    if (error_count > 0) {
        printf("\nRecent ERROR entries:\n");
        current = logs;
        int shown = 0;
        while (current < end && shown < 3) {
            if (strcmp(current->level, "ERROR") == 0) {
                printf("- %s: %s\n", current->timestamp, current->message);
                shown++;
            }
            current++;
        }
    }
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int line_count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (line_count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        if (parse_log_line(buffer, &logs[line_count])) {
            line_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (line_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_logs(logs, line_count);
    
    return 0;
}