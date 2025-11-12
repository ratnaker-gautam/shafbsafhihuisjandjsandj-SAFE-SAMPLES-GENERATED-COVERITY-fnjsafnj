//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

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
    
    int year, month, day, hour, minute, second;
    char level[10];
    char message[MAX_LINE_LEN - 30];
    
    int result = sscanf(temp, "%d-%d-%d %d:%d:%d %9s %255[^\n]", 
                       &year, &month, &day, &hour, &minute, &second, 
                       level, message);
    
    if (result == 8 && year >= 2020 && year <= 2030 &&
        month >= 1 && month <= 12 && day >= 1 && day <= 31 &&
        hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && 
        second >= 0 && second <= 59) {
        
        snprintf(entry->timestamp, sizeof(entry->timestamp), 
                "%04d-%02d-%02d %02d:%02d:%02d", 
                year, month, day, hour, minute, second);
        
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        
        return 1;
    }
    return 0;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found_errors = 0;
    for (int j = count - 1; j >= 0 && found_errors < 3; j--) {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[j].timestamp, entries[j].message);
            found_errors++;
        }
    }
    if (found_errors == 0) {
        printf("No ERROR entries found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    int input_count = 0;
    
    do {
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
        
        if (entry_count < MAX_ENTRIES) {
            if (parse_log_line(line, &entries[entry_count])) {
                entry_count++;
                printf("Added entry %d\n", entry_count);
            } else {
                printf("Invalid log format. Please use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            }
        } else {
            printf("Maximum entries reached (%d)\n", MAX_ENTRIES);
            break;
        }
        
        input_count++;
    } while (input_count < MAX_ENTRIES * 2 && entry_count < MAX_ENTRIES);
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}