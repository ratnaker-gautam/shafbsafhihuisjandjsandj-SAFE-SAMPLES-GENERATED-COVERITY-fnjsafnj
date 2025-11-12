//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    int i = 0;
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int field = 0;
    int pos = 0;
    int start = i;
    
    while (temp[i] != '\0' && field < 3) {
        if (temp[i] == ' ' && field < 2) {
            int len = i - start;
            if (len > 0) {
                if (field == 0) {
                    if (len >= 19) len = 19;
                    strncpy(entry->timestamp, temp + start, len);
                    entry->timestamp[len] = '\0';
                } else if (field == 1) {
                    if (len >= 9) len = 9;
                    strncpy(entry->level, temp + start, len);
                    entry->level[len] = '\0';
                }
            }
            field++;
            while (temp[i] != '\0' && isspace(temp[i])) i++;
            start = i;
        } else {
            i++;
        }
    }
    
    if (field == 2 && temp[start] != '\0') {
        int len = strlen(temp + start);
        if (len >= MAX_LINE_LEN) len = MAX_LINE_LEN - 1;
        strncpy(entry->message, temp + start, len);
        entry->message[len] = '\0';
        return 1;
    }
    
    return 0;
}

void analyze_log_levels(struct LogEntry* entries, int count) {
    int info_count = 0;
    int warn_count = 0;
    int error_count = 0;
    int other_count = 0;
    
    int i = 0;
    do {
        if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else if (strcmp(entries[i].level, "WARN") == 0) {
            warn_count++;
        } else if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else {
            other_count++;
        }
        i++;
    } while (i < count);
    
    printf("Log Level Summary:\n");
    printf("INFO:  %d\n", info_count);
    printf("WARN:  %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    printf("OTHER: %d\n", other_count);
}

void search_messages(struct LogEntry* entries, int count, const char* keyword) {
    if (keyword == NULL || strlen(keyword) == 0) return;
    
    printf("Messages containing '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        char* pos = entries[i].message;
        while (*pos != '\0') {
            if (strncasecmp(pos, keyword, strlen(keyword)) == 0) {
                printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                found = 1;
                break;
            }
            pos++;
        }
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_log_levels(entries, entry_count);
    printf("\n");
    
    char search_term[MAX_LINE_LEN];
    printf("Enter search term: ");
    if (fgets(search_term, MAX_LINE_LEN, stdin) != NULL) {
        search_term[strcspn(search_term, "\n")] = '\0';
        if (strlen(search_term) > 0) {
            search_messages(entries, entry_count,