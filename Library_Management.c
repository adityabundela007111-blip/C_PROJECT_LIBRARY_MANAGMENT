#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book
{
    int bookId;
    char *title;
    char *author;
    int quantity;
};

void addBook();
void showAllBooks();
void searchBook();
void deleteBook();
void issueBook();

void writeString(FILE *fp, char *str);
char *readString(FILE *fp);

int main()
{

    int choice;

    while (1)
    {

        printf("\n LIBRARY MANAGMENT SYSTEM UPES \n");
        printf("1. Add New Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Delete Book\n");
        printf("5. Issue Book\n");
        printf("6. Exit\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid Input\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            showAllBooks();
            break;
        case 3:
            searchBook();
            break;
        case 4:
            deleteBook();
            break;
        case 5:
            issueBook();
            break;
        case 6:
            exit(0);
        default:
            printf("Invalid Choice\n");
        }
    }
}

void writeString(FILE *fp, char *str)
{
    int len = strlen(str);
    fwrite(&len, sizeof(int), 1, fp);
    fwrite(str, sizeof(char), len, fp);
}

char *readString(FILE *fp)
{
    int len;
    fread(&len, sizeof(int), 1, fp);
    char *s = malloc(len + 1);
    fread(s, sizeof(char), len, fp);
    s[len] = '\0';
    return s;
}

void addBook()
{

    struct Book b;
    char temp[200];

    FILE *fp = fopen("library.dat", "ab");

    if (!fp)
    {
        printf("File Error\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &b.bookId);

    printf("Enter Title: ");
    scanf("%s", temp);
    b.title = malloc(strlen(temp) + 1);
    strcpy(b.title, temp);

    printf("Enter Author: ");
    scanf("%s", temp);
    b.author = malloc(strlen(temp) + 1);
    strcpy(b.author, temp);

    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);

    fwrite(&b.bookId, sizeof(int), 1, fp);
    fwrite(&b.quantity, sizeof(int), 1, fp);
    writeString(fp, b.title);
    writeString(fp, b.author);

    fclose(fp);

    printf("Book Added Successfully\n");

    free(b.title);
    free(b.author);
}

void showAllBooks()
{

    FILE *fp = fopen("library.dat", "rb");

    if (!fp)
    {
        printf("No Book Records Found\n");
        return;
    }

    printf("\n--------- BOOK LIST ---------\n");

    int id, qty;

    while (fread(&id, sizeof(int), 1, fp))
    {

        fread(&qty, sizeof(int), 1, fp);
        char *title = readString(fp);
        char *author = readString(fp);

        printf("ID: %d\n", id);
        printf("Title: %s\n", title);
        printf("Author: %s\n", author);
        printf("Quantity: %d\n", qty);
        printf("-----------------------------\n");

        free(title);
        free(author);
    }

    fclose(fp);
}

void searchBook()
{

    int searchId;
    int found = 0;

    printf("Enter Book ID: ");
    scanf("%d", &searchId);

    FILE *fp = fopen("library.dat", "rb");

    if (!fp)
    {
        printf("No Records Available\n");
        return;
    }

    int id, qty;

    while (fread(&id, sizeof(int), 1, fp))
    {

        fread(&qty, sizeof(int), 1, fp);
        char *title = readString(fp);
        char *author = readString(fp);

        if (id == searchId)
        {
            printf("\nBook Found:\n");
            printf("ID: %d\n", id);
            printf("Title: %s\n", title);
            printf("Author: %s\n", author);
            printf("Quantity: %d\n", qty);
            found = 1;
        }

        free(title);
        free(author);
    }

    if (!found)
        printf("Book Not Found\n");

    fclose(fp);
}

void deleteBook()
{

    int deleteId;
    int deleted = 0;

    printf("Enter Book ID to Delete: ");
    scanf("%d", &deleteId);

    FILE *fp = fopen("library.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp)
    {
        printf("File Error\n");
        return;
    }

    int id, qty;

    while (fread(&id, sizeof(int), 1, fp))
    {

        fread(&qty, sizeof(int), 1, fp);
        char *title = readString(fp);
        char *author = readString(fp);

        if (id != deleteId)
        {
            fwrite(&id, sizeof(int), 1, temp);
            fwrite(&qty, sizeof(int), 1, temp);
            writeString(temp, title);
            writeString(temp, author);
        }
        else
        {
            deleted = 1;
        }

        free(title);
        free(author);
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (deleted)
        printf("Book Deleted\n");
    else
        printf("Book Not Found\n");
}

void issueBook()
{

    int issueId;
    int issued = 0;

    printf("Enter Book ID to Issue: ");
    scanf("%d", &issueId);

    FILE *fp = fopen("library.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!fp || !temp)
    {
        printf("File Error\n");
        return;
    }

    int id, qty;

    while (fread(&id, sizeof(int), 1, fp))
    {

        fread(&qty, sizeof(int), 1, fp);
        char *title = readString(fp);
        char *author = readString(fp);

        if (id == issueId && qty > 0)
        {
            qty--;
            issued = 1;
            printf("Book Issued\n");
        }

        fwrite(&id, sizeof(int), 1, temp);
        fwrite(&qty, sizeof(int), 1, temp);
        writeString(temp, title);
        writeString(temp, author);

        free(title);
        free(author);
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (!issued)
        printf("Book Not Found or Out of Stock\n");
}
