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
    
    return field == 3;
}

int count_log_level(struct LogEntry *logs, int count, const char *level) {
    if (logs == NULL || level == NULL) return 0;
    
    int total = 0;
    struct LogEntry *current = logs;
    struct LogEntry *end = logs + count;
    
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
    printf("INFO entries: %d\n", count_log_level(logs, count, "INFO"));
    printf("WARN entries: %d\n", count_log_level(logs, count, "WARN"));
    printf("ERROR entries: %d\n", count_log_level(logs, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(logs, count, "DEBUG"));
    
    printf("\nRecent entries:\n");
    int to_show = count > 5 ? 5 : count;
    struct LogEntry *current = logs + (count - to_show);
    struct LogEntry *end = logs + count;
    
    while (current < end) {
        printf("[%s] %s: %s\n", current->timestamp, current->level, current->message);
        current++;
    }
}

int main(void) {
    struct LogEntry logs[MAX_LINES];
    int line_count = 0;
    
    printf("Enter log entries (format: [timestamp] level message):\n");
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
        
        if (parse_log_line(buffer, &logs[line_count])) {
            line_count++;
        } else {
            printf("Invalid log format. Expected: [timestamp] level message\n");
        }
    }
    
    if (line_count > 0) {
        print_log_summary(logs, line_count);
    } else {
        printf("No valid log entries provided.\n");
    }
    
    return 0;
}