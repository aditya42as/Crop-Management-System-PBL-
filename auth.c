// auth.c
// Authentication using a simple hash table with chaining.
// Stores username and password in Authenticate.txt (one user per line).
// On successful login sets current_user which other modules should use.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1031
#define MAX_NAME 50
#define MAX_PASS 50

typedef struct UserNode {
    char username[MAX_NAME];
    char password[MAX_PASS];
    struct UserNode *next;
} UserNode;

static UserNode *hash_table[TABLE_SIZE];
static char current_user[MAX_NAME] = "";

// Simple non-cryptographic hash (remainder method)
static unsigned long simple_hash(const char *str) {
    unsigned long h = 0;
    while (*str) {
        h = h * 31 + (unsigned char)(*str);
        str++;
    }
    return h;
}

// Insert user node (no file I/O)
static void insert_user_node(const char *username, const char *password) {
    unsigned long h = simple_hash(username);
    unsigned int idx = (unsigned int)(h % TABLE_SIZE);

    UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));
    if (!newNode) {
        printf("Memory allocation failed while inserting user.\n");
        return;
    }

    strncpy(newNode->username, username, MAX_NAME - 1);
    newNode->username[MAX_NAME - 1] = '\0';
    strncpy(newNode->password, password, MAX_PASS - 1);
    newNode->password[MAX_PASS - 1] = '\0';

    // insert at head of chain
    newNode->next = hash_table[idx];
    hash_table[idx] = newNode;
}

// Find user node pointer by username
static UserNode *find_user_node(const char *username) {
    unsigned long h = simple_hash(username);
    unsigned int idx = (unsigned int)(h % TABLE_SIZE);

    UserNode *cur = hash_table[idx];
    while (cur) {
        if (strcmp(cur->username, username) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

// Remove user from table. Return 1 on success.
static int remove_user_node(const char *username) {
    unsigned long h = simple_hash(username);
    unsigned int idx = (unsigned int)(h % TABLE_SIZE);

    UserNode *cur = hash_table[idx];
    UserNode *prev = NULL;
    while (cur) {
        if (strcmp(cur->username, username) == 0) {
            if (prev == NULL) {
                hash_table[idx] = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

// Load users from Authenticate.txt into hash table.
void load_users_hash() {
    FILE *fp = fopen("Authenticate.txt", "r");
    if (!fp) {
        return;
    }

    char uname[MAX_NAME], pass[MAX_PASS];
    while (fscanf(fp, "%49s %49s", uname, pass) == 2) {
        insert_user_node(uname, pass);
    }

    if (fclose(fp) != 0) {
        // ignore fclose errors
    }
}

// Save all users from hash table back to Authenticate.txt
void save_users_hash() {
    FILE *fp = fopen("Authenticate.txt", "w");
    if (!fp) {
        printf("Error saving users to Authenticate.txt\n");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        UserNode *cur = hash_table[i];
        while (cur) {
            fprintf(fp, "%s %s\n", cur->username, cur->password);
            cur = cur->next;
        }
    }

    if (fclose(fp) != 0) {
        // ignore fclose errors
    }
}

// API: sign up a user. returns 1 on success, 0 on failure.
int signup_user(const char *username, const char *password) {
    if (find_user_node(username)) {
        printf("Username already exists.\n");
        return 0;
    }

    insert_user_node(username, password);
    save_users_hash();
    printf("Account created successfully. Welcome, %s!\n", username);
    return 1;
}

// API: login a user. returns 1 on success, 0 on failure.
int login_user(const char *username, const char *password) {
    UserNode *u = find_user_node(username);
    if (u) {
        if (strcmp(u->password, password) == 0) {
            strncpy(current_user, username, MAX_NAME - 1);
            current_user[MAX_NAME - 1] = '\0';
            return 1;
        }
    }
    return 0;
}

// API: delete a user (verifies password). returns 1 on success, 0 on fail.
int delete_user(const char *username, const char *password) {
    UserNode *u = find_user_node(username);
    if (!u) {
        printf("User not found.\n");
        return 0;
    }

    if (strcmp(u->password, password) != 0) {
        printf("Incorrect password.\n");
        return 0;
    }

    if (remove_user_node(username)) {
        save_users_hash();

        // remove per-user files
        char fname[200];
        snprintf(fname, sizeof(fname), "%s_Crop_data.txt", username);
        if (remove(fname) != 0) {
            // ignore remove error
        }
        snprintf(fname, sizeof(fname), "%s_weather.txt", username);
        if (remove(fname) != 0) {
            // ignore remove error
        }
        snprintf(fname, sizeof(fname), "%s_combined_history.txt", username);
        if (remove(fname) != 0) {
            // ignore remove error
        }
        snprintf(fname, sizeof(fname), "%s_todo.txt", username);
        if (remove(fname) != 0) {
            // ignore remove error
        }

        if (strcmp(current_user, username) == 0) {
            current_user[0] = '\0';
        }

        printf("Account deleted successfully.\n");
        return 1;
    }

    return 0;
}

// Return pointer to current_user string (empty if none).
const char *get_current_user() {
    return current_user;
}

// Logout API: clear current user
void logout_user() {
    current_user[0] = '\0';
}

// Authentication menu to integrate with main
int auth_menu() {
    load_users_hash();

    int choice = 0;
    char username[MAX_NAME];
    char password[MAX_PASS];

    do {
        printf("\n=== Authentication Menu ===\n");
        printf("1. Login\n");
        printf("2. Sign Up\n");
        printf("3. Delete Account\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {
                // consume
            }
            choice = 0;
        }
        getchar(); // consume newline

        switch (choice) {
            case 1: {
                printf("Enter username: ");
                if (fgets(username, sizeof(username), stdin) != NULL) {
                    username[strcspn(username, "\n")] = '\0';
                } else {
                    username[0] = '\0';
                }
                printf("Enter password: ");
                if (fgets(password, sizeof(password), stdin) != NULL) {
                    password[strcspn(password, "\n")] = '\0';
                } else {
                    password[0] = '\0';
                }

                if (login_user(username, password)) {
                    printf("\nLogin successful. Welcome, %s!\n", username);
                    return 1;
                } else {
                    printf("Invalid username or password.\n");
                }
            } break;

            case 2: {
                printf("Enter new username: ");
                if (fgets(username, sizeof(username), stdin) != NULL) {
                    username[strcspn(username, "\n")] = '\0';
                } else {
                    username[0] = '\0';
                }
                printf("Enter password: ");
                if (fgets(password, sizeof(password), stdin) != NULL) {
                    password[strcspn(password, "\n")] = '\0';
                } else {
                    password[0] = '\0';
                }

                signup_user(username, password);
            } break;

            case 3: {
                printf("Enter username to delete: ");
                if (fgets(username, sizeof(username), stdin) != NULL) {
                    username[strcspn(username, "\n")] = '\0';
                } else {
                    username[0] = '\0';
                }
                printf("Enter password: ");
                if (fgets(password, sizeof(password), stdin) != NULL) {
                    password[strcspn(password, "\n")] = '\0';
                } else {
                    password[0] = '\0';
                }

                delete_user(username, password);
            } break;

            case 4: {
                printf("Exiting authentication...\n");
                return 0;
            } break;

            default: {
                printf("Invalid choice.\n");
            } break;
        }
    } while (1);
}
