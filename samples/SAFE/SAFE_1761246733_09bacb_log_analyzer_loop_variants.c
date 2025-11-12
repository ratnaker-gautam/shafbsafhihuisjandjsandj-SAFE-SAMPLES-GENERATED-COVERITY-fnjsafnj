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
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* token = strtok(temp_line, " ");
    if (!token) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Type 'END' on a new line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    int i = 0;
    
    while (i < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &logs[i])) {
            i++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    log_count = i;
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (int j = 0; j < log_count; j++) {
        if (strcmp(logs[j].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[j].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[j].level, "INFO") == 0) {
            info_count++;
        }
    }
    
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found_errors = 0;
    int k = log_count - 1;
    
    while (k >= 0 && found_errors < 3) {
        if (strcmp(logs[k].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[k].timestamp, logs[k].message);
            found_errors++;
        }
        k--;
    }
    
    if (found_errors == 0) {
        printf("No ERROR entries found.\n");
    }
    
    printf("\nLog distribution by level:\n");
    int total = error_count + warning_count + info_count;
    if (total > 0) {
        int m = 0;
        do {
            const char* level;
            int count;
            
            switch (m) {
                case 0: level = "ERROR"; count = error_count; break;
                case 1: level = "WARNING"; count = warning_count; break;
                case 2: level = "INFO"; count = info_count; break;
                default: break;
            }
            
            if (count > 0) {
                int percentage = (count * 100) / total;
                printf("%s: %d%% (%d entries)\n", level, percentage, count);
            }
            
            m++;
        } while (m < 3);
    }
    
    return 0;
}