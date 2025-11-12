//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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
    char *end = ptr + strlen(line);
    
    if (end - ptr < 20) return 0;
    
    strncpy(entry->timestamp, ptr, 19);
    entry->timestamp[19] = '\0';
    ptr += 20;
    
    if (end - ptr < 10) return 0;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ' && ptr - level_start < 9) {
        ptr++;
    }
    strncpy(entry->level, level_start, ptr - level_start);
    entry->level[ptr - level_start] = '\0';
    
    if (ptr < end && *ptr == ' ') ptr++;
    
    if (end - ptr <= 0) return 0;
    
    strncpy(entry->message, ptr, MAX_LINE_LEN - 30);
    entry->message[MAX_LINE_LEN - 31] = '\0';
    
    return 1;
}

int count_log_level(struct LogEntry *logs, int count, const char *level) {
    int total = 0;
    struct LogEntry *end = logs + count;
    
    for (struct LogEntry *current = logs; current < end; current++) {
        if (strcmp(current->level, level) == 0) {
            total++;
        }
    }
    return total;
}

void print_log_summary(struct LogEntry *logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int cnt = count_log_level(logs, count, levels[i]);
        printf("%s: %d\n", levels[i], cnt);
    }
    
    printf("\nFirst 5 entries:\n");
    struct LogEntry *end = (count > 5) ? logs + 5 : logs + count;
    for (struct LogEntry *current = logs; current < end; current++) {
        printf("%s [%s] %s\n", current->timestamp, current->level, current->message);
    }
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int line_count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    char buffer[MAX_LINE_LEN];
    while (line_count < MAX_LINES) {
        if (fgets(buffer, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') break;
        
        if (parse_log_line(buffer, logs + line_count)) {
            line_count++;
        } else {
            printf("Invalid log format: %s\n", buffer);
        }
    }
    
    if (line_count > 0) {
        print_log_summary(logs, line_count);
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}