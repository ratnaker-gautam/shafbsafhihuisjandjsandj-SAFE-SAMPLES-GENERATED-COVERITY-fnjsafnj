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
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 31) return 0;
    if (strlen(level) >= 15) return 0;
    if (strlen(message) >= 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->count = 1;
    
    return 1;
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (empty line to finish):\n");
    
    int i = 0;
    while (i < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        struct LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            int found = 0;
            int j = 0;
            while (j < entry_count) {
                if (strcmp(entries[j].message, new_entry.message) == 0) {
                    entries[j].count++;
                    found = 1;
                    break;
                }
                j++;
            }
            
            if (!found) {
                if (entry_count < MAX_ENTRIES) {
                    entries[entry_count] = new_entry;
                    entry_count++;
                }
            }
        }
        i++;
    }
    
    printf("\nLog Summary:\n");
    printf("============\n");
    
    int k = 0;
    do {
        if (k >= entry_count) break;
        printf("Message: %s\n", entries[k].message);
        printf("Level: %s\n", entries[k].level);
        printf("Count: %d\n", entries[k].count);
        printf("---\n");
        k++;
    } while (k < entry_count);
    
    int total_entries = 0;
    for (int m = 0; m < entry_count; m++) {
        total_entries += entries[m].count;
    }
    printf("Total log entries processed: %d\n", total_entries);
    
    return 0;
}