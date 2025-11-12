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
    
    int j = 0;
    do {
        if (strcmp(logs[j].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[j].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[j].level, "INFO") == 0) {
            info_count++;
        }
        j++;
    } while (j < log_count);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent log entries:\n");
    int k;
    for (k = (log_count > 5 ? log_count - 5 : 0); k < log_count; k++) {
        printf("%s %s %s\n", logs[k].timestamp, logs[k].level, logs[k].message);
    }
    
    char search_term[64];
    printf("\nEnter search term (or press Enter to skip): ");
    if (fgets(search_term, sizeof(search_term), stdin)) {
        search_term[strcspn(search_term, "\n")] = '\0';
        
        if (strlen(search_term) > 0) {
            printf("Matching entries:\n");
            int found = 0;
            int m;
            for (m = 0; m < log_count; m++) {
                if (strstr(logs[m].message, search_term)) {
                    printf("%s %s %s\n", logs[m].timestamp, logs[m].level, logs[m].message);
                    found = 1;
                }
            }
            if (!found) {
                printf("No entries matching '%s'\n", search_term);
            }
        }
    }
    
    return 0;
}