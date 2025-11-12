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
        while (*ptr && !isspace(*ptr)) ptr++;
        
        size_t len = ptr - start;
        if (len == 0) break;
        
        switch (field) {
            case 0:
                if (len >= sizeof(entry->timestamp)) return 0;
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
                break;
            case 1:
                if (len >= sizeof(entry->level)) return 0;
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
                break;
            case 2:
                if (len >= sizeof(entry->message)) return 0;
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
                break;
        }
        field++;
    }
    
    return field == 3;
}

void analyze_log_levels(struct LogEntry *logs, int count) {
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
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    printf("TOTAL: %d\n", count);
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
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
        
        if (len == 0 || len >= MAX_LINE_LENGTH) {
            continue;
        }
        
        struct LogEntry *entry = logs + line_count;
        if (parse_log_line(buffer, entry)) {
            line_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", buffer);
        }
    }
    
    if (line_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_log_levels(logs, line_count);
    
    printf("\nRecent ERROR entries:\n");
    struct LogEntry *current = logs + line_count - 1;
    int errors_shown = 0;
    
    while (current >= logs && errors_shown < 5) {
        if (strcmp(current->level, "ERROR") == 0) {
            printf("%s %s %s\n", current->timestamp, current->level, current->message);
            errors_shown++;
        }
        current--;
    }
    
    if (errors_shown == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}