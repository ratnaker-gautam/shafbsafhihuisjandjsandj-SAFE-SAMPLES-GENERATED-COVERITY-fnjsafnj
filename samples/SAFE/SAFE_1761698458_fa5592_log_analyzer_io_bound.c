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

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char buffer[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log lines (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
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
        
        if (parse_log_line(buffer, &entries[entry_count])) {
            entry_count++;
            printf("Entry %d recorded\n", entry_count);
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_counts[4] = {0};
    
    for (int i = 0; i < 4; i++) {
        level_counts[i] = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    printf("\nRecent entries:\n");
    int show_count = (entry_count < 5) ? entry_count : 5;
    for (int i = entry_count - show_count; i < entry_count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}