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
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &logs[log_count])) {
            log_count++;
        } else {
            printf("Invalid log format. Skipping: %s\n", line);
        }
    }
    
    if (log_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < log_count) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent log entries:\n");
    int start_idx = (log_count > 5) ? log_count - 5 : 0;
    
    for (int j = start_idx; j < log_count; j++) {
        printf("%s %s %s\n", logs[j].timestamp, logs[j].level, logs[j].message);
    }
    
    printf("\nSearch for entries containing text: ");
    char search_term[128];
    if (fgets(search_term, sizeof(search_term), stdin)) {
        search_term[strcspn(search_term, "\n")] = '\0';
        
        if (strlen(search_term) > 0) {
            printf("Matching entries:\n");
            int found_count = 0;
            
            int k = 0;
            do {
                if (strstr(logs[k].message, search_term) != NULL) {
                    printf("%s %s %s\n", logs[k].timestamp, logs[k].level, logs[k].message);
                    found_count++;
                }
                k++;
            } while (k < log_count);
            
            if (found_count == 0) {
                printf("No entries found containing '%s'\n", search_term);
            }
        }
    }
    
    return 0;
}