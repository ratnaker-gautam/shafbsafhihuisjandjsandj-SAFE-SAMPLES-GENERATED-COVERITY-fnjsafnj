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
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count] = token;
        part_count++;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strncpy(entry->timestamp, parts[0], sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, parts[1], sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, parts[2], sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
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
            printf("Invalid log format. Use: timestamp|level|message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < entry_count) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent entries (last 5):\n");
    int start_idx = (entry_count > 5) ? entry_count - 5 : 0;
    
    for (int j = start_idx; j < entry_count; j++) {
        printf("%s | %s | %s\n", entries[j].timestamp, entries[j].level, entries[j].message);
    }
    
    char search_term[64];
    printf("\nEnter search term (or empty to skip): ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        size_t search_len = strlen(search_term);
        if (search_len > 0 && search_term[search_len - 1] == '\n') {
            search_term[search_len - 1] = '\0';
        }
        
        if (strlen(search_term) > 0) {
            printf("Matching entries:\n");
            int found_count = 0;
            
            int k = 0;
            do {
                if (strstr(entries[k].message, search_term) != NULL) {
                    printf("%s | %s | %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
                    found_count++;
                }
                k++;
            } while (k < entry_count);
            
            if (found_count == 0) {
                printf("No entries found containing '%s'\n", search_term);
            }
        }
    }
    
    return 0;
}