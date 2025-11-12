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
    char *end = ptr + strlen(line);
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *timestamp_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    
    size_t timestamp_len = ptr - timestamp_start;
    if (timestamp_len >= sizeof(entry->timestamp)) return 0;
    memcpy(entry->timestamp, timestamp_start, timestamp_len);
    entry->timestamp[timestamp_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *level_start = ptr;
    while (ptr < end && *ptr != ' ') ptr++;
    if (ptr >= end) return 0;
    
    size_t level_len = ptr - level_start;
    if (level_len >= sizeof(entry->level)) return 0;
    memcpy(entry->level, level_start, level_len);
    entry->level[level_len] = '\0';
    
    while (ptr < end && isspace(*ptr)) ptr++;
    if (ptr >= end) return 0;
    
    char *message_start = ptr;
    char *message_end = end - 1;
    while (message_end > message_start && isspace(*message_end)) message_end--;
    
    size_t message_len = message_end - message_start + 1;
    if (message_len >= sizeof(entry->message)) return 0;
    memcpy(entry->message, message_start, message_len);
    entry->message[message_len] = '\0';
    
    return 1;
}

int count_log_level(struct LogEntry *logs, int count, const char *level) {
    if (logs == NULL || level == NULL) return 0;
    
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

void print_log_summary(struct LogEntry *logs, int count) {
    if (logs == NULL || count <= 0) return;
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count_level = count_log_level(logs, count, levels[i]);
        printf("%s: %d\n", levels[i], count_level);
    }
    
    printf("\nRecent entries:\n");
    int to_show = count > 5 ? 5 : count;
    struct LogEntry *start = logs + (count - to_show);
    struct LogEntry *end = logs + count;
    
    while (start < end) {
        printf("[%s] %s: %s\n", start->timestamp, start->level, start->message);
        start++;
    }
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int log_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Enter empty line to finish.\n");
    
    while (log_count < MAX_LINES) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) break;
        
        if (parse_log_line(buffer, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", buffer);
        }
    }
    
    if (log_count > 0) {
        print_log_summary(logs, log_count);
    } else {
        printf("No valid log entries processed.\n");
    }
    
    return 0;
}