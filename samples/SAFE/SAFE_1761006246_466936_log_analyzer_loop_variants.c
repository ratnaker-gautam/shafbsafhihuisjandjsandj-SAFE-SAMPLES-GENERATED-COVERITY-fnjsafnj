//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
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
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* token = strtok(temp, " ");
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
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    int i = 0;
    while (i < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
        i++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int j = 0;
    do {
        if (strcmp(entries[j].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[j].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[j].level, "INFO") == 0) {
            info_count++;
        }
        j++;
    } while (j < entry_count);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent entries:\n");
    int start_idx = (entry_count > 5) ? entry_count - 5 : 0;
    
    for (int k = start_idx; k < entry_count; k++) {
        printf("%s %s %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
    }
    
    char search_term[64];
    printf("\nEnter search term: ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        size_t len = strlen(search_term);
        if (len > 0 && search_term[len-1] == '\n') search_term[len-1] = '\0';
        
        if (strlen(search_term) > 0) {
            printf("Matching entries:\n");
            int found = 0;
            
            int m = 0;
            while (m < entry_count) {
                if (strstr(entries[m].message, search_term) != NULL) {
                    printf("%s %s %s\n", entries[m].timestamp, entries[m].level, entries[m].message);
                    found = 1;
                }
                m++;
            }
            
            if (!found) {
                printf("No entries found containing '%s'\n", search_term);
            }
        }
    }
    
    return 0;
}