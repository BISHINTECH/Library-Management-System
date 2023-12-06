#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char username[50];
    char password[50];
    int isAdmin;
};

struct Book {
    int id;
    char title[100];
    char author[50];
    int quantity;
};

struct Transaction {
    int userId;
    int bookId;
    char action[10];
};

void displayMenu(int isAdmin) {
    printf("\nLibrary Management System\n");
    printf("1. View Books\n");
    printf("2. View Transactions\n");

    if (isAdmin) {
        printf("3. Add User\n");
        printf("4. Add Book\n");
    } else {
        printf("3. Check Out Book\n");
        printf("4. Return Book\n");
    }

    printf("5. Logout\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

void addUser(struct User *users, int *userCount) {
    printf("\nEnter username: ");
    scanf("%s", users[*userCount].username);

    printf("Enter password: ");
    scanf("%s", users[*userCount].password);

    users[*userCount].id = *userCount + 1;
    users[*userCount].isAdmin = 0;
    (*userCount)++;

    printf("User added successfully!\n");
}

void addBook(struct Book *books, int *bookCount) {
    printf("\nEnter book title: ");
    scanf(" %[^\n]s", books[*bookCount].title);

    printf("Enter author: ");
    scanf(" %[^\n]s", books[*bookCount].author);

    printf("Enter quantity: ");
    scanf("%d", &books[*bookCount].quantity);

    books[*bookCount].id = *bookCount + 1;
    (*bookCount)++;

    printf("Book added successfully!\n");
}

void displayBooks(struct Book *books, int bookCount) {
    if (bookCount == 0) {
        printf("No books found.\n");
    } else {
        printf("\nBook ID\tTitle\tAuthor\tQuantity\n");
        for (int i = 0; i < bookCount; ++i) {
            printf("%d\t%s\t%s\t%d\n", books[i].id, books[i].title, books[i].author, books[i].quantity);
        }
    }
}

void displayTransactions(struct Transaction *transactions, int transactionCount) {
    if (transactionCount == 0) {
        printf("No transactions found.\n");
    } else {
        printf("\nUser ID\tBook ID\tAction\n");
        for (int i = 0; i < transactionCount; ++i) {
            printf("%d\t%d\t%s\n", transactions[i].userId, transactions[i].bookId, transactions[i].action);
        }
    }
}

void performTransaction(struct Transaction *transactions, int *transactionCount, int userId, int bookId, const char *action) {
    transactions[*transactionCount].userId = userId;
    transactions[*transactionCount].bookId = bookId;
    strcpy(transactions[*transactionCount].action, action);
    (*transactionCount)++;
}

int main() {
    struct User users[100];
    struct Book books[100];
    struct Transaction transactions[100];

    int userCount = 0;
    int bookCount = 0;
    int transactionCount = 0;

    int currentUser = -1;

    strcpy(users[userCount].username, "admin");
    strcpy(users[userCount].password, "admin");
    users[userCount].id = userCount + 1;
    users[userCount].isAdmin = 1;
    userCount++;

    int choice;
    char username[50];
    char password[50];
    int bookId;

    do {
        if (currentUser == -1) {
            printf("\nLogin Panel\n");
            printf("Enter username: ");
            scanf("%s", username);

            printf("Enter password: ");
            scanf("%s", password);

            for (int i = 0; i < userCount; ++i) {
                if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                    currentUser = i;
                    break;
                }
            }

            if (currentUser == -1) {
                printf("Invalid username or password. Please try again.\n");
                continue;
            }

            printf("Login successful. Welcome, %s!\n", users[currentUser].username);
        }

        displayMenu(users[currentUser].isAdmin);

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayBooks(books, bookCount);
                break;
            case 2:
                displayTransactions(transactions, transactionCount);
                break;
            case 3:
                if (users[currentUser].isAdmin) {
                    addUser(users, &userCount);
                } else {
                    printf("Enter book ID: ");
                    scanf("%d", &bookId);

                    if (bookId < 1 || bookId > bookCount) {
                        printf("Invalid book ID.\n");
                    } else {
                        if (strcmp(transactions[transactionCount - 1].action, "CHECKOUT") == 0 ||
                            books[bookId - 1].quantity > 0) {
                            const char *action = (strcmp(transactions[transactionCount - 1].action, "CHECKOUT") == 0) ? "RETURN" : "CHECKOUT";
                            performTransaction(transactions, &transactionCount, users[currentUser].id, bookId, action);

                            if (strcmp(action, "CHECKOUT") == 0) {
                                books[bookId - 1].quantity--;
                                printf("Book checked out successfully!\n");
                            } else {
                                books[bookId - 1].quantity++;
                                printf("Book returned successfully!\n");
                            }
                        } else {
                            printf("You cannot check out more than one book at a time.\n");
                        }
                    }
                }
                break;
            case 4:
                if (users[currentUser].isAdmin) {
                    addBook(books, &bookCount);
                } else {
                    printf("Enter book ID: ");
                    scanf("%d", &bookId);

                    if (bookId < 1 || bookId > bookCount) {
                        printf("Invalid book ID.\n");
                    } else {
                        if (strcmp(transactions[transactionCount - 1].action, "CHECKOUT") == 0 &&
                            transactions[transactionCount - 1].userId == users[currentUser].id &&
                            transactions[transactionCount - 1].bookId == bookId) {
                            performTransaction(transactions, &transactionCount, users[currentUser].id, bookId, "RETURN");
                            books[bookId - 1].quantity++;
                            printf("Book returned successfully!\n");
                        } else {
                            printf("You can only return the book you have checked out.\n");
                        }
                    }
                }
                break;
            case 5:
                printf("Logout successful. Goodbye, %s!\n", users[currentUser].username);
                currentUser = -1; // Logout the current user
                break;
            case 6:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}
