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
        while (*ptr && !isspace(*ptr) && *ptr != ']') ptr++;
        
        if (field == 0) {
            if (*start == '[') start++;
            size_t len = ptr - start;
            if (len >= sizeof(entry->timestamp)) len = sizeof(entry->timestamp) - 1;
            strncpy(entry->timestamp, start, len);
            entry->timestamp[len] = '\0';
        } else if (field == 1) {
            size_t len = ptr - start;
            if (len >= sizeof(entry->level)) len = sizeof(entry->level) - 1;
            strncpy(entry->level, start, len);
            entry->level[len] = '\0';
        } else if (field == 2) {
            size_t len = strlen(start);
            if (len >= sizeof(entry->message)) len = sizeof(entry->message) - 1;
            strncpy(entry->message, start, len);
            entry->message[len] = '\0';
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
        int displayed = 0;
        while (current < end && displayed < 3) {
            if (strcmp(current->level, "ERROR") == 0) {
                printf("- %s: %s\n", current->timestamp, current->message);
                displayed++;
            }
            current++;
        }
    }
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int line_count = 0;
    
    printf("Enter log entries (format: [timestamp] level message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char buffer[MAX_LINE_LENGTH];
    
    while (line_count < MAX_LINES) {
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
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        if (parse_log_line(buffer, &logs[line_count])) {
            line_count++;
        } else {
            printf("Invalid log format. Please use: [timestamp] level message\n");
        }
    }
    
    if (line_count > 0) {
        analyze_logs(logs, line_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}