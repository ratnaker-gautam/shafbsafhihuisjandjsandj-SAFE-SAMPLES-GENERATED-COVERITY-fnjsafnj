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
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    parts[0] = temp_line;
    int part_count = 0;
    
    for (char* p = temp_line; *p && part_count < 3; p++) {
        if (*p == '|') {
            *p = '\0';
            parts[part_count++] = p + 1;
        }
    }
    
    if (part_count != 2) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    for (char* p = entry->timestamp; *p; p++) {
        if (!isdigit(*p) && *p != '-' && *p != ':' && *p != ' ' && *p != '.') {
            return 0;
        }
    }
    
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
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log entries (format: timestamp|level|message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
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
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS|LEVEL|message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d (%.1f%%)\n", levels[i], count, 
               entry_count > 0 ? (count * 100.0) / entry_count : 0.0);
    }
    
    printf("\nRecent entries:\n");
    int display_count = entry_count > 5 ? 5 : entry_count;
    for (int i = entry_count - display_count; i < entry_count; i++) {
        printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}