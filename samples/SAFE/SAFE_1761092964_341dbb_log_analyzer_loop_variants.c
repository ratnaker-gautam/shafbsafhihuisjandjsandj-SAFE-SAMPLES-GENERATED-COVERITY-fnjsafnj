//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token && count < 3) {
        parts[count] = token;
        token = strtok(NULL, "|");
        count++;
    }
    
    if (count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry logs[MAX_ENTRIES];
    int log_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message):\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
    while (log_count < MAX_ENTRIES) {
        if (!fgets(line, MAX_LINE_LEN, stdin)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[log_count] = entry;
            log_count++;
        } else {
            printf("Invalid format. Use: timestamp|level|message\n");
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
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
    
    printf("Total entries: %d\n", log_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found_errors = 0;
    
    for (int j = 0; j < log_count; j++) {
        if (strcmp(logs[j].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[j].timestamp, logs[j].message);
            found_errors++;
            if (found_errors >= 5) break;
        }
    }
    
    if (found_errors == 0) {
        printf("No ERROR entries found.\n");
    }
    
    printf("\nSearch for messages containing: ");
    char search_term[64];
    if (fgets(search_term, 64, stdin)) {
        search_term[strcspn(search_term, "\n")] = '\0';
        
        if (strlen(search_term) > 0) {
            printf("Matching entries:\n");
            int matches = 0;
            
            int k = 0;
            do {
                if (strstr(logs[k].message, search_term)) {
                    printf("- [%s] %s: %s\n", logs[k].level, logs[k].timestamp, logs[k].message);
                    matches++;
                }
                k++;
            } while (k < log_count);
            
            if (matches == 0) {
                printf("No matches found.\n");
            }
        }
    }
    
    return 0;
}