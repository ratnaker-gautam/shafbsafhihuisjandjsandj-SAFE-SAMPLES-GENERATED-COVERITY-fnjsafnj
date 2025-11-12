//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN - 48];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len < 20 || len >= MAX_LINE_LEN) return 0;
    
    int scanned = sscanf(line, "%31s %15s", entry->timestamp, entry->level);
    if (scanned != 2) return 0;
    
    const char* msg_start = strchr(line, ' ');
    if (msg_start == NULL) return 0;
    msg_start = strchr(msg_start + 1, ' ');
    if (msg_start == NULL) return 0;
    msg_start++;
    
    size_t msg_len = strlen(msg_start);
    if (msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_level_occurrences(struct LogEntry* entries, int count, const char* level) {
    int occurrences = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            occurrences++;
        }
    }
    return occurrences;
}

void print_summary(struct LogEntry* entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARNING", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < level_count; i++) {
        int occ = count_level_occurrences(entries, count, levels[i]);
        printf("%s: %d\n", levels[i], occ);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (one per line). Format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_summary(entries, entry_count);
    
    printf("\nRecent entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}