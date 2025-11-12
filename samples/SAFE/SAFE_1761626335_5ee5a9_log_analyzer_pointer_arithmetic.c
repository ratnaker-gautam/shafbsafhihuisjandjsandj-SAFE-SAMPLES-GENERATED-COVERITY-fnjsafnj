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
        char *start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') {
            ptr++;
        }
        
        size_t len = ptr - start;
        if (len == 0) break;
        
        switch (field) {
            case 0:
                if (len >= sizeof(entry->timestamp)) return 0;
                memcpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
                break;
            case 1:
                if (len >= sizeof(entry->level)) return 0;
                memcpy(entry->level, start, len);
                entry->level[len] = '\0';
                break;
            case 2:
                if (len >= sizeof(entry->message)) return 0;
                memcpy(entry->message, start, len);
                entry->message[len] = '\0';
                break;
        }
        
        field++;
        while (*ptr == ' ' || *ptr == '\t') ptr++;
    }
    
    return field == 3;
}

int count_log_level(struct LogEntry *logs, int count, const char *level) {
    if (logs == NULL || level == NULL || count <= 0) return 0;
    
    int total = 0;
    struct LogEntry *end = logs + count;
    struct LogEntry *current = logs;
    
    while (current < end) {
        if (strcmp(current->level, level) == 0) {
            total++;
        }
        current++;
    }
    
    return total;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (strncmp(buffer, "END", 3) == 0 && (buffer[3] == '\n' || buffer[3] == '\0')) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len <= 1) continue;
        
        struct LogEntry entry;
        if (parse_log_line(buffer, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", buffer);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_log_level(logs, log_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\nRecent entries:\n");
    int display_count = (log_count > 5) ? 5 : log_count;
    struct LogEntry *start = logs + (log_count - display_count);
    struct LogEntry *end = logs + log_count;
    
    while (start < end) {
        printf("%s [%s] %s\n", start->timestamp, start->level, start->message);
        start++;
    }
    
    return 0;
}