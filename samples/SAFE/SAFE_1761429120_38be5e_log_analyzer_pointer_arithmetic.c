//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    const char *ptr = line;
    size_t len = strlen(line);
    if (len < 20 || len >= MAX_LINE_LEN) return 0;
    
    int field = 0;
    char *dest = NULL;
    size_t dest_len = 0;
    
    while (*ptr && field < 3) {
        while (*ptr && isspace(*ptr)) ptr++;
        
        if (field == 0) {
            dest = entry->timestamp;
            dest_len = sizeof(entry->timestamp) - 1;
        } else if (field == 1) {
            dest = entry->level;
            dest_len = sizeof(entry->level) - 1;
        } else {
            dest = entry->message;
            dest_len = sizeof(entry->message) - 1;
        }
        
        size_t i = 0;
        while (*ptr && !isspace(*ptr) && i < dest_len) {
            *dest++ = *ptr++;
            i++;
        }
        *dest = '\0';
        
        field++;
    }
    
    return field == 3;
}

int count_log_level(const struct LogEntry *entries, int count, const char *level) {
    if (!entries || count <= 0 || !level) return 0;
    
    int total = 0;
    const struct LogEntry *end = entries + count;
    const struct LogEntry *current = entries;
    
    while (current < end) {
        if (strcmp(current->level, level) == 0) {
            total++;
        }
        current++;
    }
    
    return total;
}

void print_summary(const struct LogEntry *entries, int count) {
    if (!entries || count <= 0) return;
    
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
    
    char line[MAX_LINE_LEN];
    
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
        
        if (len > 0 && parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        print_summary(entries, entry_count);
        
        printf("\nRecent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        struct LogEntry *current = entries + start;
        struct LogEntry *end = entries + entry_count;
        
        while (current < end) {
            printf("%s [%s] %s\n", current->timestamp, current->level, current->message);
            current++;
        }
    } else {
        printf("No valid log entries found.\n");
    }
    
    return 0;
}