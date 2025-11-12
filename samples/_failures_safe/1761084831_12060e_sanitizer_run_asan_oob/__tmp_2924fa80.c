//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
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

int count_level_occurrences(struct LogEntry *entries, int count, const char *level) {
    int occurrences = 0;
    struct LogEntry *end = entries + count;
    
    for (struct LogEntry *e = entries; e < end; e++) {
        if (strcmp(e->level, level) == 0) {
            occurrences++;
        }
    }
    
    return occurrences;
}

void print_summary(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count = count_level_occurrences(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], count);
    }
    
    printf("\nFirst 5 entries:\n");
    int display_count = count < 5 ? count : 5;
    struct LogEntry *end = entries + display_count;
    
    for (struct LogEntry *e = entries; e < end; e++) {
        printf("%s [%s] %s\n", e->timestamp, e->level, e->message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry *current = entries + entry_count;
        if (parse_log_line(line, current)) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (entry_count > 0) {
        print_summary(entries, entry_count);
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}