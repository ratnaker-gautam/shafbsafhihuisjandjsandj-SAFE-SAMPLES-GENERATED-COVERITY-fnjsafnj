//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
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
    char message[MAX_LINE_LEN - 48];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len < 20 || len >= MAX_LINE_LEN) return 0;
    
    int scanned = sscanf(line, "%31s %15s", entry->timestamp, entry->level);
    if (scanned != 2) return 0;
    
    const char* msg_start = strchr(line, ' ');
    if (msg_start == NULL) return 0;
    msg_start = strchr(msg_start + 1, ' ');
    if (msg_start == NULL) return 0;
    msg_start++;
    
    size_t msg_len = strlen(msg_start);
    if (msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_level_occurrences(struct LogEntry* entries, int count, const char* level) {
    int occurrences = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            occurrences++;
        }
    }
    return occurrences;
}

void print_summary(struct LogEntry* entries, int count) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("INFO entries: %d\n", count_level_occurrences(entries, count, "INFO"));
    printf("WARNING entries: %d\n", count_level_occurrences(entries, count, "WARNING"));
    printf("ERROR entries: %d\n", count_level_occurrences(entries, count, "ERROR"));
    printf("DEBUG entries: %d\n", count_level_occurrences(entries, count, "DEBUG"));
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", entries[0].timestamp, entries[0].level, entries[0].message);
        printf("Last entry: %s %s %s\n", entries[count-1].timestamp, entries[count-1].level, entries[count-1].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
            printf("Added entry %d\n", entry_count);
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\n");
    print_summary(entries, entry_count);
    
    char search_level[16];
    printf("\nEnter level to search for (or 'quit' to exit): ");
    if (fgets(search_level, sizeof(search_level), stdin) != NULL) {
        search_level[strcspn(search_level, "\n")] = '\0';
        
        if (strcmp(search_level, "quit") != 0) {
            int found = 0;
            printf("\nEntries with level '%s':\n", search_level);
            for (int i = 0; i < entry_count; i++) {
                if (strcmp(entries[i].level, search_level) == 0) {
                    printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                    found = 1;
                }
            }
            if (!found) {
                printf("No entries found with level '%s'\n", search_level);
            }
        }
    }
    
    return 0;
}