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
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
    int i = 0;
    while (i < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid format, skipping: %s\n", line);
        }
        i++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
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
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent entries (last 5):\n");
    int start_idx = (entry_count > 5) ? entry_count - 5 : 0;
    
    for (int k = start_idx; k < entry_count; k++) {
        printf("%s | %s | %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
    }
    
    char search_level[16];
    printf("\nEnter level to search (ERROR/WARNING/INFO): ");
    if (fgets(search_level, 16, stdin) != NULL) {
        size_t slen = strlen(search_level);
        if (slen > 0 && search_level[slen - 1] == '\n') {
            search_level[slen - 1] = '\0';
        }
        
        int found_count = 0;
        int m = 0;
        while (m < entry_count) {
            if (strcmp(entries[m].level, search_level) == 0) {
                if (found_count == 0) {
                    printf("Entries with level '%s':\n", search_level);
                }
                printf("%s | %s | %s\n", entries[m].timestamp, entries[m].level, entries[m].message);
                found_count++;
            }
            m++;
        }
        
        if (found_count == 0) {
            printf("No entries found with level '%s'\n", search_level);
        }
    }
    
    return 0;
}