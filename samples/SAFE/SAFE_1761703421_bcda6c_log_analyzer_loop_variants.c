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
    
    int year, month, day, hour, minute, second;
    char level[10];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%d-%d-%d %d:%d:%d %9s %255[^\n]", 
               &year, &month, &day, &hour, &minute, &second, level, message) == 8) {
        if (year < 2000 || year > 2100) return 0;
        if (month < 1 || month > 12) return 0;
        if (day < 1 || day > 31) return 0;
        if (hour < 0 || hour > 23) return 0;
        if (minute < 0 || minute > 59) return 0;
        if (second < 0 || second > 59) return 0;
        
        snprintf(entry->timestamp, sizeof(entry->timestamp), 
                "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        return 1;
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int info_count = 0, warning_count = 0, error_count = 0;
    int i = 0;
    
    do {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        }
        i++;
    } while (i < entry_count);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    
    for (int j = start; j < entry_count; j++) {
        printf("%s [%s] %s\n", entries[j].timestamp, entries[j].level, entries[j].message);
    }
    
    char search_level[10];
    printf("\nEnter level to search (INFO/WARNING/ERROR): ");
    if (fgets(search_level, sizeof(search_level), stdin) != NULL) {
        search_level[strcspn(search_level, "\n")] = '\0';
        
        printf("Entries with level '%s':\n", search_level);
        int found = 0;
        
        for (int k = 0; k < entry_count; k++) {
            if (strcmp(entries[k].level, search_level) == 0) {
                printf("%s [%s] %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
                found = 1;
            }
        }
        
        if (!found) {
            printf("No entries found with level '%s'\n", search_level);
        }
    }
    
    return 0;
}