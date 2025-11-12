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
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (timestamp level message), one per line. Enter 'END' to finish:\n");
    
    int i = 0;
    while (i < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
        i++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
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
    
    printf("\nRecent log entries (last 5):\n");
    int start_idx = (entry_count > 5) ? entry_count - 5 : 0;
    
    for (int k = start_idx; k < entry_count; k++) {
        printf("%s %s %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
    }
    
    char search_term[64];
    printf("\nEnter search term (or 'quit' to exit): ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        search_term[strcspn(search_term, "\n")] = 0;
        
        if (strcmp(search_term, "quit") != 0) {
            printf("Search results for '%s':\n", search_term);
            int found_count = 0;
            
            struct LogEntry* current = entries;
            struct LogEntry* end = entries + entry_count;
            while (current < end) {
                if (strstr(current->message, search_term) != NULL) {
                    printf("%s %s %s\n", current->timestamp, current->level, current->message);
                    found_count++;
                }
                current++;
            }
            
            if (found_count == 0) {
                printf("No matches found.\n");
            }
        }
    }
    
    return 0;
}