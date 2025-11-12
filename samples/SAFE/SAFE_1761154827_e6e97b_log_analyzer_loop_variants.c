//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) < sizeof(entry->timestamp) &&
        strlen(parts[1]) < sizeof(entry->level) &&
        strlen(parts[2]) < sizeof(entry->message)) {
        strcpy(entry->timestamp, parts[0]);
        strcpy(entry->level, parts[1]);
        strcpy(entry->message, parts[2]);
        return 1;
    }
    
    return 0;
}

int count_entries_by_level(struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    int i = 0;
    do {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
        i++;
    } while (i < count);
    return total;
}

void print_summary(struct LogEntry* entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    int j;
    for (j = 0; j < level_count; j++) {
        int cnt = count_entries_by_level(entries, count, levels[j]);
        printf("%s: %d\n", levels[j], cnt);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    print_summary(entries, entry_count);
    
    printf("\nRecent entries:\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    int k = start;
    while (k < entry_count) {
        printf("%s | %s | %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        k++;
    }
    
    return 0;
}