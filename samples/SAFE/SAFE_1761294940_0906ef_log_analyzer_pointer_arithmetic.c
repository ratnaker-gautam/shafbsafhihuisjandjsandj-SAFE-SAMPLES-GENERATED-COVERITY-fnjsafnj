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
        while (*ptr && isspace(*ptr)) ptr++;
        
        char *start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') ptr++;
        
        if (ptr > start) {
            size_t len = ptr - start;
            if (field == 0) {
                if (len >= sizeof(entry->timestamp)) len = sizeof(entry->timestamp) - 1;
                strncpy(entry->timestamp, start, len);
                entry->timestamp[len] = '\0';
            } else if (field == 1) {
                if (len >= sizeof(entry->level)) len = sizeof(entry->level) - 1;
                strncpy(entry->level, start, len);
                entry->level[len] = '\0';
            } else if (field == 2) {
                if (len >= sizeof(entry->message)) len = sizeof(entry->message) - 1;
                strncpy(entry->message, start, len);
                entry->message[len] = '\0';
            }
        }
        
        field++;
        if (*ptr) ptr++;
    }
    
    return field >= 2;
}

int count_log_level(struct LogEntry *entries, int count, const char *level) {
    if (entries == NULL || level == NULL || count <= 0) return 0;
    
    int total = 0;
    struct LogEntry *end = entries + count;
    
    for (struct LogEntry *current = entries; current < end; current++) {
        if (strcmp(current->level, level) == 0) {
            total++;
        }
    }
    
    return total;
}

void print_summary(struct LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char *levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int count_level = count_log_level(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], count_level);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char buffer[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
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
        
        if (parse_log_line(buffer, entries + entry_count)) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count > 0) {
        print_summary(entries, entry_count);
        
        printf("\nRecent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        struct LogEntry *ptr = entries + start;
        struct LogEntry *end = entries + entry_count;
        
        while (ptr < end) {
            printf("%s [%s] %s\n", ptr->timestamp, ptr->level, ptr->message);
            ptr++;
        }
    } else {
        printf("No valid log entries processed.\n");
    }
    
    return 0;
}