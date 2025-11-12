//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(const char* filename, LogProcessor processor) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line_count++;
        if (line_count > MAX_ENTRIES) {
            fprintf(stderr, "Warning: Maximum entries reached\n");
            break;
        }

        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            fprintf(stderr, "Error: Line too long at line %d\n", line_count);
            continue;
        }

        LogEntry entry;
        int parsed = sscanf(line, "%19s %9s %[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed == 3) {
            for (int i = 0; entry.level[i]; i++) {
                entry.level[i] = toupper(entry.level[i]);
            }
            processor(&entry);
        } else {
            fprintf(stderr, "Warning: Malformed log entry at line %d\n", line_count);
        }
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Warning: Error closing file\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    printf("1. Count all entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Choose option (1-3): ");

    char input[10];
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    
    if (choice < 1 || choice > 3) {
        fprintf(stderr, "Invalid choice\n");
        return 1;
    }

    process_log_file(argv[1], processors[choice - 1]);
    return 0;
}