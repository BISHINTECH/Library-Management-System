#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_BOOK_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_USERS 100
#define MAX_BOOKS 100
#define MAX_TRANSACTIONS 100
#define MAX_ACTION_LENGTH 10

enum UserType
{
    REGULAR_USER,
    ADMIN
};

enum TransactionAction
{
    CHECKOUT,
    RETURN
};

struct User
{
    int id;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    enum UserType userType;
};

struct Book
{
    int id;
    char title[MAX_BOOK_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int quantity;
};

struct Transaction
{
    int userId;
    int bookId;
    enum TransactionAction action;
};

void displayMenu(enum UserType userType);
void addUser(struct User *users, int *userCount);
void addBook(struct Book *books, int *bookCount);
void displayBooks(const struct Book *books, int bookCount);
void displayTransactions(const struct Transaction *transactions, int transactionCount);
void displayUserTransactions(const struct Transaction *transactions, int transactionCount, int currentUserId);
void performTransaction(struct Transaction *transactions, int *transactionCount, int userId, int bookId, enum TransactionAction action);
void loadUsers(struct User *users, int *userCount);
void loadBooks(struct Book *books, int *bookCount);
void loadTransactions(struct Transaction *transactions, int *transactionCount);
void saveUsers(const struct User *users, int userCount);
void saveBooks(const struct Book *books, int bookCount);
void saveTransactions(const struct Transaction *transactions, int transactionCount);

int main()
{
    struct User users[MAX_USERS];
    struct Book books[MAX_BOOKS];
    struct Transaction transactions[MAX_TRANSACTIONS];

    int userCount = 0;
    int bookCount = 0;
    int transactionCount = 0;
    loadUsers(users, &userCount);
    loadBooks(books, &bookCount);
    loadTransactions(transactions, &transactionCount);
    int currentUser = -1;

    // Default admin user
    strcpy(users[userCount].username, "admin");
    strcpy(users[userCount].password, "admin");
    users[userCount].id = userCount + 1;
    users[userCount].userType = ADMIN;
    userCount++;

    int choice;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int bookId;

    do
    {
        if (currentUser == -1)
        {
            printf("\nLogin Panel\n");
            printf("Enter username: ");
            scanf("%49s", username);

            printf("Enter password: ");
            scanf("%49s", password);

            for (int i = 0; i < userCount; ++i)
            {
                if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
                {
                    currentUser = i;
                    break;
                }
            }

            if (currentUser == -1)
            {
                printf("Invalid username or password. Please try again.\n");
                continue;
            }

            printf("Login successful. Welcome, %s!\n", users[currentUser].username);
        }

        displayMenu(users[currentUser].userType);

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayBooks(books, bookCount);
            break;
        case 2:
            if (users[currentUser].userType == ADMIN)
            {
                displayTransactions(transactions, transactionCount);
            }
            else
            {
                displayUserTransactions(transactions, transactionCount, users[currentUser].id);
            }

            break;
        case 3:
            if (users[currentUser].userType == ADMIN)
            {
                addUser(users, &userCount);
            }
            else
            {
                printf("Enter book ID: ");
                scanf("%d", &bookId);

                if (bookId < 1 || bookId > bookCount)
                {
                    printf("Invalid book ID.\n");
                }
                else
                {
                    if (transactions[transactionCount - 1].action == CHECKOUT ||
                        books[bookId - 1].quantity > 0)
                    {
                        enum TransactionAction lastAction = (transactionCount > 0) ? transactions[transactionCount - 1].action : RETURN;
                        enum TransactionAction action = (lastAction == CHECKOUT) ? RETURN : CHECKOUT;

                        performTransaction(transactions, &transactionCount, users[currentUser].id, bookId, action);

                        if (action == CHECKOUT)
                        {
                            books[bookId - 1].quantity--;
                            printf("Book checked out successfully!\n");
                        }
                        else
                        {
                            books[bookId - 1].quantity++;
                            printf("Book returned successfully!\n");
                        }
                    }
                    else
                    {
                        printf("You cannot check out more than one book at a time.\n");
                    }
                }
            }
            break;
        case 4:
            if (users[currentUser].userType == ADMIN)
            {
                addBook(books, &bookCount);
            }
            else
            {
                printf("Enter book ID: ");
                scanf("%d", &bookId);

                if (bookId < 1 || bookId > bookCount)
                {
                    printf("Invalid book ID.\n");
                }
                else
                {
                    if (transactions[transactionCount - 1].action == CHECKOUT &&
                        transactions[transactionCount - 1].userId == users[currentUser].id &&
                        transactions[transactionCount - 1].bookId == bookId)
                    {
                        performTransaction(transactions, &transactionCount, users[currentUser].id, bookId, RETURN);
                        books[bookId - 1].quantity++;
                        printf("Book returned successfully!\n");
                    }
                    else
                    {
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
    saveUsers(users, userCount);
    saveBooks(books, bookCount);
    saveTransactions(transactions, transactionCount);
    return 0;
}

// Function implementations remain unchanged
void displayMenu(enum UserType userType)
{
    printf("\nLibrary Management System\n");
    printf("1. View Books\n");
    printf("2. View Transactions\n");

    if (userType == ADMIN)
    {
        printf("3. Add User\n");
        printf("4. Add Book\n");
    }
    else
    {
        printf("3. Check Out Book\n");
        printf("4. Return Book\n");
    }

    printf("5. Logout\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

// Function to add a new user
void addUser(struct User *users, int *userCount)
{
    if (*userCount < MAX_USERS)
    {
        printf("\nEnter username: ");
        scanf("%49s", users[*userCount].username);

        printf("Enter password: ");
        scanf("%49s", users[*userCount].password);

        users[*userCount].id = *userCount + 1;
        users[*userCount].userType = REGULAR_USER;
        (*userCount)++;

        printf("User added successfully!\n");
    }
    else
    {
        printf("Maximum number of users reached. Cannot add more users.\n");
    }
}

// Function to add a new book
void addBook(struct Book *books, int *bookCount)
{
    if (*bookCount < MAX_BOOKS)
    {
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
    else
    {
        printf("Maximum number of books reached. Cannot add more books.\n");
    }
}

// Function to display books
void displayBooks(const struct Book *books, int bookCount)
{
    if (bookCount == 0)
    {
        printf("No books found.\n");
    }
    else
    {
        printf("\nBook ID\tTitle\tAuthor\tQuantity\n");
        for (int i = 0; i < bookCount; ++i)
        {
            printf("%d\t%s\t%s\t%d\n", books[i].id, books[i].title, books[i].author, books[i].quantity);
        }
    }
}

// Function to display transactions
void displayTransactions(const struct Transaction *transactions, int transactionCount)
{
    if (transactionCount == 0)
    {
        printf("No transactions found.\n");
    }
    else
    {
        printf("\nUser ID\tBook ID\tAction\n");
        for (int i = 0; i < transactionCount; ++i)
        {
            printf("%d\t%d\t%s\n", transactions[i].userId, transactions[i].bookId,
                   (transactions[i].action == CHECKOUT) ? "CHECKOUT" : "RETURN");
        }
    }
}
// Function to display transactions for the logged-in user
void displayUserTransactions(const struct Transaction *transactions, int transactionCount, int currentUserId)
{
    int found = 0; // Flag to check if any transactions are found for the user

    if (transactionCount == 0)
    {
        printf("No transactions found.\n");
    }
    else
    {
        printf("\nUser ID\tBook ID\tAction\n");
        for (int i = 0; i < transactionCount; ++i)
        {
            if (transactions[i].userId == currentUserId)
            {
                found = 1;
                printf("%d\t%d\t%s\n", transactions[i].userId, transactions[i].bookId,
                       (transactions[i].action == CHECKOUT) ? "CHECKOUT" : "RETURN");
            }
        }

        if (!found)
        {
            printf("No transactions found for the current user.\n");
        }
    }
}

// Function to perform a transaction
void performTransaction(struct Transaction *transactions, int *transactionCount, int userId, int bookId, enum TransactionAction action)
{
    if (*transactionCount < MAX_TRANSACTIONS)
    {
        transactions[*transactionCount].userId = userId;
        transactions[*transactionCount].bookId = bookId;
        transactions[*transactionCount].action = action;
        (*transactionCount)++;
    }
    else
    {
        printf("Maximum number of transactions reached. Cannot perform more transactions.\n");
    }
}
void loadUsers(struct User *users, int *userCount)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file 'users.txt'.\n");
        return;
    }

    while (fscanf(file, "%d %s %s %d", &users[*userCount].id, users[*userCount].username, users[*userCount].password, &users[*userCount].userType) == 4)
    {
        (*userCount)++;
        if (*userCount >= MAX_USERS)
        {
            printf("Maximum number of users reached. Cannot load more users.\n");
            break;
        }
    }

    fclose(file);
}

// Function to load books from file
void loadBooks(struct Book *books, int *bookCount)
{
    FILE *file = fopen("books.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file 'books.txt'.\n");
        return;
    }

    while (fscanf(file, "%d %[^\n]s %[^\n]s %d", &books[*bookCount].id, books[*bookCount].title, books[*bookCount].author, &books[*bookCount].quantity) == 4)
    {
        (*bookCount)++;
        if (*bookCount >= MAX_BOOKS)
        {
            printf("Maximum number of books reached. Cannot load more books.\n");
            break;
        }
    }

    fclose(file);
}

// Function to load transactions from file
void loadTransactions(struct Transaction *transactions, int *transactionCount)
{
    FILE *file = fopen("transactions.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file 'transactions.txt'.\n");
        return;
    }

    while (fscanf(file, "%d %d %d", &transactions[*transactionCount].userId, &transactions[*transactionCount].bookId, &transactions[*transactionCount].action) == 3)
    {
        (*transactionCount)++;
        if (*transactionCount >= MAX_TRANSACTIONS)
        {
            printf("Maximum number of transactions reached. Cannot load more transactions.\n");
            break;
        }
    }

    fclose(file);
}

// Function to save users to file
void saveUsers(const struct User *users, int userCount)
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file 'users.txt' for writing.\n");
        return;
    }

    for (int i = 0; i < userCount; ++i)
    {
        fprintf(file, "%d %s %s %d\n", users[i].id, users[i].username, users[i].password, users[i].userType);
    }

    fclose(file);
}

// Function to save books to file
void saveBooks(const struct Book *books, int bookCount)
{
    FILE *file = fopen("books.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file 'books.txt' for writing.\n");
        return;
    }

    for (int i = 0; i < bookCount; ++i)
    {
        fprintf(file, "%d %s %s %d\n", books[i].id, books[i].title, books[i].author, books[i].quantity);
    }

    fclose(file);
}
void saveTransactions(const struct Transaction *transactions, int transactionCount)
{
    FILE *file = fopen("transactions.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file 'transactions.txt' for writing.\n");
        return;
    }
}