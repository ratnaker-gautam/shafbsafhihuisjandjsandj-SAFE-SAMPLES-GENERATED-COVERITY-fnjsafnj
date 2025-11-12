//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 1000
#define MAX_LINE_LEN 256

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
        
        char *start = ptr;
        while (*ptr && !isspace(*ptr) && *ptr != ']') ptr++;
        
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
        if (*ptr) ptr++;
    }
    
    return field == 3;
}

int count_log_level(struct LogEntry *logs, int count, const char *level) {
    if (logs == NULL || level == NULL) return 0;
    
    int total = 0;
    struct LogEntry *end = logs + count;
    
    for (struct LogEntry *current = logs; current < end; current++) {
        if (strcmp(current->level, level) == 0) {
            total++;
        }
    }
    
    return total;
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int line_count = 0;
    
    printf("Enter log entries (format: [timestamp] [level] [message]):\n");
    printf("Enter empty line to finish.\n");
    
    char buffer[MAX_LINE_LEN];
    
    while (line_count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') {
            break;
        }
        
        if (parse_log_line(buffer, logs + line_count)) {
            line_count++;
        } else {
            printf("Invalid log format. Skipping.\n");
        }
    }
    
    if (line_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", line_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_log_level(logs, line_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\nRecent entries:\n");
    int show_count = line_count > 5 ? 5 : line_count;
    struct LogEntry *start = logs + (line_count - show_count);
    struct LogEntry *end = logs + line_count;
    
    for (struct LogEntry *current = start; current < end; current++) {
        printf("[%s] [%s] %s\n", current->timestamp, current->level, current->message);
    }
    
    return 0;
}