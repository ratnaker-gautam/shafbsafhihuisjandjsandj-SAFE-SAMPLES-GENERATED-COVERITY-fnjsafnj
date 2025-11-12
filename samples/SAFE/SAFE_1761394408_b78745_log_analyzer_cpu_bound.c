//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int error_count;
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, level, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message, 255);
    entry->message[255] = '\0';
    
    entry->error_count = (strcmp(level, "ERROR") == 0) ? 1 : 0;
    
    return 1;
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int total_errors = 0;
    int total_warnings = 0;
    
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (one per line, format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, MAX_LINE_LEN, stdin)) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "ERROR") == 0) {
                total_errors++;
            } else if (strcmp(entries[entry_count].level, "WARNING") == 0) {
                total_warnings++;
            }
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("Total errors: %d\n", total_errors);
    printf("Total warnings: %d\n", total_warnings);
    printf("Error rate: %.2f%%\n", (total_errors * 100.0) / entry_count);
    
    int max_error_day = 0;
    char error_day[11] = "";
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            char day[11];
            strncpy(day, entries[i].timestamp, 10);
            day[10] = '\0';
            
            int day_errors = 0;
            for (int j = 0; j < entry_count; j++) {
                if (entries[j].error_count > 0 && 
                    strncmp(entries[j].timestamp, day, 10) == 0) {
                    day_errors++;
                }
            }
            
            if (day_errors > max_error_day) {
                max_error_day = day_errors;
                strncpy(error_day, day, 10);
                error_day[10] = '\0';
            }
        }
    }
    
    if (max_error_day > 0) {
        printf("Day with most errors: %s (%d errors)\n", error_day, max_error_day);
    }
    
    int consecutive_errors = 0;
    int max_consecutive = 0;
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].error_count > 0) {
            consecutive_errors++;
            if (consecutive_errors > max_consecutive) {
                max_consecutive = consecutive_errors;
            }
        } else {
            consecutive_errors = 0;
        }
    }
    
    printf("Longest error sequence: %d consecutive errors\n", max_consecutive);
    
    return 0;
}