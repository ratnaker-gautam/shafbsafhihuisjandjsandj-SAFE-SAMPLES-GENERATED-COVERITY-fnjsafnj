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
            size_t len = ptr - start;
            if (len >= sizeof(entry->timestamp)) return 0;
            strncpy(entry->timestamp, start, len);
            entry->timestamp[len] = '\0';
        } else if (field == 1) {
            size_t len = ptr - start;
            if (len >= sizeof(entry->level)) return 0;
            strncpy(entry->level, start, len);
            entry->level[len] = '\0';
        } else if (field == 2) {
            size_t len = strlen(start);
            if (len >= sizeof(entry->message)) return 0;
            strncpy(entry->message, start, len);
            entry->message[len] = '\0';
        }
        
        field++;
        if (*ptr) ptr++;
    }
    
    return field == 3;
}

void count_log_levels(struct LogEntry *logs, int count, int *level_counts) {
    if (logs == NULL || level_counts == NULL) return;
    
    for (int i = 0; i < count; i++) {
        char *level = logs[i].level;
        
        if (strcmp(level, "INFO") == 0) {
            level_counts[0]++;
        } else if (strcmp(level, "WARN") == 0) {
            level_counts[1]++;
        } else if (strcmp(level, "ERROR") == 0) {
            level_counts[2]++;
        } else {
            level_counts[3]++;
        }
    }
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int line_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char buffer[MAX_LINE_LENGTH];
    
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
        
        if (parse_log_line(buffer, logs + line_count)) {
            line_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (line_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    int level_counts[4] = {0};
    count_log_levels(logs, line_count, level_counts);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", line_count);
    printf("INFO: %d\n", level_counts[0]);
    printf("WARN: %d\n", level_counts[1]);
    printf("ERROR: %d\n", level_counts[2]);
    printf("OTHER: %d\n", level_counts[3]);
    
    printf("\nRecent log entries:\n");
    int display_count = line_count > 5 ? 5 : line_count;
    for (int i = line_count - display_count; i < line_count; i++) {
        printf("%s [%s] %s\n", logs[i].timestamp, logs[i].level, logs[i].message);
    }
    
    return 0;
}