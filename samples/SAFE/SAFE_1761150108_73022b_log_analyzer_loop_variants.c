//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int count;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
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
    
    entry->count = 1;
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        struct LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            int found = 0;
            int i = 0;
            while (i < entry_count) {
                if (strcmp(entries[i].message, new_entry.message) == 0 &&
                    strcmp(entries[i].level, new_entry.level) == 0) {
                    entries[i].count++;
                    found = 1;
                    break;
                }
                i++;
            }
            
            if (!found) {
                if (entry_count < MAX_ENTRIES) {
                    entries[entry_count] = new_entry;
                    entry_count++;
                }
            }
        } else {
            printf("Invalid log format. Use: timestamp|level|message\n");
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int total_entries = 0;
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int j = 0;
    while (j < entry_count) {
        total_entries += entries[j].count;
        
        if (strcmp(entries[j].level, "ERROR") == 0) {
            error_count += entries[j].count;
        } else if (strcmp(entries[j].level, "WARNING") == 0) {
            warning_count += entries[j].count;
        } else if (strcmp(entries[j].level, "INFO") == 0) {
            info_count += entries[j].count;
        }
        
        j++;
    }
    
    printf("Total unique log patterns: %d\n", entry_count);
    printf("Total log entries: %d\n", total_entries);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nDuplicate patterns (count > 1):\n");
    int k = 0;
    do {
        if (entries[k].count > 1) {
            printf("Pattern: %s (Level: %s) - Count: %d\n", 
                   entries[k].message, entries[k].level, entries[k].count);
        }
        k++;
    } while (k < entry_count);
    
    return 0;
}