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

int count_entries_by_level(struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
        i++;
    }
    return total;
}

void print_level_summary(struct LogEntry* entries, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    printf("Log Level Summary:\n");
    int j = 0;
    do {
        int cnt = count_entries_by_level(entries, count, levels[j]);
        printf("%s: %d\n", levels[j], cnt);
        j++;
    } while (j < level_count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (timestamp level message). Enter 'END' to finish:\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Use: timestamp level message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    for (int k = 0; k < entry_count; k++) {
        printf("%s %s %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
    }
    
    printf("\n");
    print_level_summary(entries, entry_count);
    
    printf("\nSearch for entries containing text: ");
    char search_term[128];
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        search_term[strcspn(search_term, "\n")] = 0;
        
        if (strlen(search_term) > 0) {
            printf("Matching entries:\n");
            int found = 0;
            int m = 0;
            for (; m < entry_count; m++) {
                if (strstr(entries[m].message, search_term) != NULL) {
                    printf("%s %s %s\n", entries[m].timestamp, entries[m].level, entries[m].message);
                    found = 1;
                }
            }
            if (!found) {
                printf("No entries found containing '%s'\n", search_term);
            }
        }
    }
    
    return 0;
}