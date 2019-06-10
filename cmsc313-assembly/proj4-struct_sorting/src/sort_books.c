#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Following is straight from the project description
 */
#define AUTHOR_LEN      20
#define TITLE_LEN       32
#define SUBJECT_LEN     10

struct book {
    char author[AUTHOR_LEN + 1];    /* first author */
    char title[TITLE_LEN + 1];
    char subject[SUBJECT_LEN + 1];  /* Nonfiction, Fantasy, Mystery, ... */
    unsigned int year;              /* year of e-book release */
};


void sort_books(struct book books[], int numBooks);
void print_books(struct book books[], int numBooks);

/* EXTERNAL FUNCTION DECLARATIONS */
extern int bookcmp(void);


/* Declarations for global variables that need to be accessed from other files */
struct book book1;
struct book book2;


#define MAX_BOOKS 100

int main(int argc, char **argv) {
    struct book books[MAX_BOOKS];
    int numBooks = 0;
        char temp[81] = "";

    int i = 0;
    for (i = 0; i < MAX_BOOKS; i++) {
        /* Sample line: "Breaking Point, Pamela Clare, Romance, 2011" */
        /* Note that for the string fields, it uses the conversion spec
        * "%##[^,]", where "##" is an actual number. This says to read up to
        * a maximum of ## characters (not counting the null terminator!),
        * stopping at the first ','  We have left it up to you to finish
        * out the scanf() call by suppying the remaining arguments specifying
        * where scanf should put the data.  They should be mostly pointers
        * to the fields within the book struct you are filling, e.g.,
        * "&(books[i].year)".  However, note that the first field spec--
        * the title field--specifies 80 chars.  The title field in the
        * struct book is NOT that large, so you need to read it into a
        * temporary buffer first, of an appropriately large size so that
        * scanf() doesn't overrun it.  Again, all the other fields can
        * be read directly into the struct book's members.
        */


        /* scanf returns the actual number of fields matched */
        int numFields = scanf("%80[^,], %20[^,], %10[^,], %u \n",
                (temp), (books[i].author),
                (books[i].subject), (&books[i].year));

        if (numFields == EOF) {
            break;
        }

        if (numFields != 4) {
            /* invalid line input */
            printf("Invalid input, expected 4 fields on line and found %d.\n", numFields);
            exit(1);
        }

        /*printf("read: %s\n", temp);*/
        /* Then, post-process title (see project spec for reason) */
        int k = 0;
        for (k=0; k<80; k++) {
            if (temp[k] != '\0' && k < TITLE_LEN) {
                books[i].title[k] = temp[k];
            }
            temp[k] = '\0'; /* clear array */
        }
        numBooks = numBooks + 1;
    }


    /* testing: appears to be a deep copy */
    /*
    book1 = books[0];
    book1.year = 3005;
    strncpy(book1.author, "J.K. Rowling", 20);

    printf("\n\n\n%s, %s, %s, %d\n\n\n", book1.title, book1.author, book1.subject, book1.year);
    printf("\n\n\n%s, %s, %s, %d\n\n\n", books[0].title, books[0].author, books[0].subject, books[0].year);
    */

    sort_books(books, numBooks);
    print_books(books, numBooks);
    exit(0);
}

/*
 * sort_books(): receives an array of struct book's, of length
 * numBooks.  Sorts the array in-place (i.e., actually modifies
 * the elements of the array).
 *
 * This is almost exactly what was given in the pseudocode in
 * the project spec--need to replace STUBS with real code
 */
void sort_books(struct book books[], int numBooks) {
    int i, j, min;
    int cmpResult;

    for (i = 0; i < numBooks - 1; i++) {
        min = i;
        for (j = i + 1; j < numBooks; j++) {

            /* Copy pointers to the two books to be compared into the
             * global variables book1 and book2 for bookcmp() to see
             */
            book1 = books[min];
            book2 = books[j];

            /* bookcmp returns result in register EAX--above saves it into cmpResult */
            cmpResult = bookcmp();

            /* if book1 COMES AFTER book2 */
            if (cmpResult == 1) {
                min = j;
            }
        }

        if (min != i) {
            /* SWAP books[i], books[min] */
            struct book temp = books[i];
            books[i] = books[min];
            books[min] = temp;
        }
    }
}

/* prints the list of books */
void print_books(struct book books[], int numBooks) {
    int i = 0;
    for (i = 0; i < numBooks; i++) {
        printf("%s, %s, %s, %d\n", books[i].title, books[i].author, books[i].subject, books[i].year);
    }
}
