//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char *ptr = line;
    int field = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (!*ptr) break;
        
        char *start = ptr;
        while (*ptr && !isspace(*ptr)) ptr++;
        
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
    }
    
    return field == 3;
}

int count_log_level(struct LogEntry *entries, int count, const char *level) {
    if (!entries || count <= 0 || !level) return 0;
    
    int total = 0;
    struct LogEntry *end = entries + count;
    
    for (struct LogEntry *ptr = entries; ptr < end; ptr++) {
        if (strcmp(ptr->level, level) == 0) {
            total++;
        }
    }
    return total;
}

void print_summary(struct LogEntry *entries, int count) {
    if (!entries || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_log_level(entries, count, "INFO"));
    printf("WARN entries: %d\n", count_log_level(entries, count, "WARN"));
    printf("ERROR entries: %d\n", count_log_level(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_log_level(entries, count, "DEBUG"));
    
    printf("\nFirst 5 entries:\n");
    int display_count = count > 5 ? 5 : count;
    for (int i = 0; i < display_count; i++) {
        printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (len > 1) {
            if (parse_log_line(line, &entries[entry_count])) {
                entry_count++;
            } else {
                printf("Invalid log format. Skipping: %s\n", line);
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    print_summary(entries, entry_count);
    
    return 0;
}