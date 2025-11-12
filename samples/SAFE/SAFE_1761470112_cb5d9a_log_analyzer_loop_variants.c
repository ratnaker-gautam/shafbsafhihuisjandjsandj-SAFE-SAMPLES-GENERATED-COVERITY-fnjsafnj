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
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message), one per line. Empty line to finish:\n");
    
    int i = 0;
    while (i < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &logs[i])) {
            i++;
        } else {
            printf("Invalid log format. Skipping.\n");
        }
    }
    log_count = i;
    
    if (log_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
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
    int start_idx = (log_count > 5) ? log_count - 5 : 0;
    
    for (int k = start_idx; k < log_count; k++) {
        printf("%s %s %s\n", logs[k].timestamp, logs[k].level, logs[k].message);
    }
    
    char search_level[16];
    printf("\nEnter level to search (ERROR/WARNING/INFO): ");
    if (fgets(search_level, sizeof(search_level), stdin) != NULL) {
        size_t slen = strlen(search_level);
        if (slen > 0 && search_level[slen - 1] == '\n') {
            search_level[slen - 1] = '\0';
        }
        
        printf("Entries with level '%s':\n", search_level);
        int found = 0;
        
        int m = 0;
        while (m < log_count) {
            if (strcmp(logs[m].level, search_level) == 0) {
                printf("%s %s %s\n", logs[m].timestamp, logs[m].level, logs[m].message);
                found = 1;
            }
            m++;
        }
        
        if (!found) {
            printf("No entries found.\n");
        }
    }
    
    return 0;
}