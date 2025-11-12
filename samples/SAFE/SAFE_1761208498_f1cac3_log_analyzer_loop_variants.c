//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    int year, month, day, hour, min, sec;
    char level[10];
    
    int parsed = sscanf(temp, "%d-%d-%d %d:%d:%d %9s %255[^\n]", 
                       &year, &month, &day, &hour, &min, &sec, 
                       level, entry->message);
    
    if (parsed == 8) {
        if (year < 2000 || year > 2100) return 0;
        if (month < 1 || month > 12) return 0;
        if (day < 1 || day > 31) return 0;
        if (hour < 0 || hour > 23) return 0;
        if (min < 0 || min > 59) return 0;
        if (sec < 0 || sec > 59) return 0;
        
        snprintf(entry->timestamp, sizeof(entry->timestamp), 
                "%04d-%02d-%02d %02d:%02d:%02d", 
                year, month, day, hour, min, sec);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        return 1;
    }
    return 0;
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int info_count = 0, warn_count = 0, error_count = 0;
    
    int i = 0;
    while (i < entry_count) {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
        i++;
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (entry_count > 0) {
        printf("\nRecent entries:\n");
        int start = (entry_count > 5) ? entry_count - 5 : 0;
        
        for (int j = start; j < entry_count; j++) {
            printf("%s [%s] %s\n", 
                   entries[j].timestamp, 
                   entries[j].level, 
                   entries[j].message);
        }
        
        printf("\nError entries:\n");
        int k = 0;
        do {
            if (strcmp(entries[k].level, "ERROR") == 0) {
                printf("%s: %s\n", entries[k].timestamp, entries[k].message);
            }
            k++;
        } while (k < entry_count);
    }
    
    return 0;
}