#include "admin.h"
#include "console.h"
#include "customer.h"
#include "main.h"
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

typedef struct {
    int orderId;
    char date[20];
    char items[100];
    float total;
} Order;

typedef struct {
    int cart_id;
    char date[70];
    char item[70];
    float price;
    int qty;
    float subTotal;
} Cart;

void userReview(void);
void viewUserReview(void);
void showThankYouScreen(void);
void saveOrder(char item[], float price);
void confirmOrder(char item[], float price, int ycod);

float subTotal;
float fullSubTotal;
float vat, vatRate = 0.1, discount, discountRate = 0.15, grandTotal;

float generateBill(float fullSubTotal, float *vat, float vatRate,
                   float *discount, float discountRate) {
    *vat = fullSubTotal * vatRate;
    *discount = fullSubTotal * discountRate;

    return fullSubTotal - (*vat + *discount);
}

int getLastOrderID(void) {
    FILE *fp = fopen("data/orders.dat", "rb");

    if(fp == NULL) {
        return 1000;
    }

    Order order;
    int lastID = 1000;

    while(fread(&order, sizeof(Order), 1, fp) == 1) {
        lastID = order.orderId;
    }

    fclose(fp);
    return lastID;
}

void displayBill() {
    system("cls");
    int i;

    Cart cart;

    time_t t;
    struct tm *date;

    t = time(NULL);
    date = localtime(&t);

    FILE *fp = fopen("data/cart.dat", "rb");

    gotoxy(17, 2);
    printf("==================================================================="
           "==");
    gotoxy(45, 4);
    printf("FINAL INVOICE\n");
    gotoxy(17, 6);
    printf("==================================================================="
           "==");

    gotoxy(17, 7);
    printf("Date : %02d-%02d-%04d\t\t\t\t  Order ID : %d\n", date->tm_mday,
           date->tm_mon + 1, date->tm_year + 1900, getLastOrderID() + 1);
    gotoxy(17, 8);
    printf("------------------------------------------+------+---------+-------"
           "--");
    gotoxy(17, 9);
    printf("%-41s |%-5s |%-8s |%-8s\n", "Item Name", " Qty", " Rate", " Total");
    gotoxy(17, 10);
    printf("------------------------------------------+------+---------+-------"
           "--");
    int ycod = 11;

    if(fp == NULL) {
        gotoxy(17, ycod);
        printf("Your cart is empty or cart data is unavailable.\n");
        gotoxy(17, ycod + 2);
        printf("👉 Press ANY KEY to return...");
        getch();
        showWelcomeScreen();
        return;
    }

    fullSubTotal = 0;
    char allItems[500] = "";
    int hasItems = 0;

    while(fread(&cart, sizeof(Cart), 1, fp) == 1) {
        gotoxy(17, ycod);
        printf("%-41s | %-5d| %-8.2f| %-8.2f\n", cart.item, cart.qty,
               cart.price, cart.subTotal);

        if(strlen(allItems) > 0)
            strcat(allItems, ", ");

        strcat(allItems, cart.item);

        fullSubTotal += cart.subTotal;
        ycod++;
        hasItems = 1;
    }

    fclose(fp);

    if(!hasItems) {
        gotoxy(17, ycod + 1);
        printf("Your cart is empty. Add items before checkout.\n");
        gotoxy(17, ycod + 3);
        printf("👉 Press ANY KEY to return...");
        getch();
        showWelcomeScreen();
        return;
    }

    grandTotal =
        generateBill(fullSubTotal, &vat, vatRate, &discount, discountRate);

    gotoxy(17, ycod + 1);
    printf("-------------------------------------------------------------------"
           "--");

    gotoxy(17, ycod + 2);
    printf("Sub Total\t\t:   \t\t\t    %.2f TK\n", fullSubTotal);

    gotoxy(17, ycod + 3);
    printf("VAT (%.0f%%)\t\t:  \t\t\t    %.2f TK\n", vatRate * 100, vat);

    gotoxy(17, ycod + 4);
    printf("Discount (%.0f%%) \t:   \t\t\t    %.2f TK\n", discountRate * 100,
           discount);

    gotoxy(17, ycod + 5);
    printf("-------------------------------------------------------------------"
           "--");

    gotoxy(17, ycod + 6);
    printf("Grand Total \t\t: \t\t\t    %.2f TK", grandTotal);

    gotoxy(17, ycod + 7);
    printf("==================================================================="
           "==");

    fclose(fp);
    confirmOrder(cart.item, cart.price, ycod);
}

void saveOrder(char item[], float price) {
    int orderID = getLastOrderID() + 1;

    time_t now = time(NULL);
    struct tm *date = localtime(&now);

    FILE *cartFp = fopen("data/cart.dat", "rb");
    if(cartFp == NULL) {
        return;
    }

    char allItems[100] = "";
    char tempItem[120];
    float totalPrice = 0;
    Cart cart;
    int firstItem = 1;

    while(fread(&cart, sizeof(Cart), 1, cartFp) == 1) {
        totalPrice += cart.subTotal;

        if(firstItem) {
            snprintf(tempItem, sizeof(tempItem), "%s(x%d)", cart.item,
                     cart.qty);
            firstItem = 0;
        } else {
            snprintf(tempItem, sizeof(tempItem), ", %s(x%d)", cart.item,
                     cart.qty);
        }

        if(strlen(allItems) + strlen(tempItem) < sizeof(allItems)) {
            strncat(allItems, tempItem,
                    sizeof(allItems) - strlen(allItems) - 1);
        }
    }

    fclose(cartFp);

    if(strlen(allItems) == 0) {
        return;
    }

    FILE *fp = fopen("data/orders.dat", "ab");

    if(fp != NULL) {
        Order order;
        order.orderId = orderID;

        strftime(order.date, sizeof(order.date), "%d-%m-%Y", date);

        strncpy(order.items, allItems, sizeof(order.items) - 1);
        order.items[sizeof(order.items) - 1] = '\0';

        order.total = totalPrice;

        fwrite(&order, sizeof(Order), 1, fp);
        fclose(fp);
    }
}

void confirmOrder(char item[], float price, int ycod) {
    float vat, discount, grandTotal;
    int choice;

    gotoxy(17, ycod + 9);
    printf("[1] Confirm Order\n");

    gotoxy(17, ycod + 10);
    printf("[2] Back\n");

    gotoxy(17, ycod + 12);
    printf("➔ Select Option (1-2): ");

    selectionNumInput(&choice, ycod + 11, displayBill);

    switch(choice) {
    case 1:
        saveOrder(item, price);
        clearCartFile();
        userReview();
        break;
    case 2:
        viewCart();
        break;
    default:
        gotoxy(17, ycod + 14);
        printf("⚠️  Invalid Selection");
        gotoxy(17, ycod + 15);
        printf("👉 Press ANY KEY to try again...");
        getch();
        displayBill();
    }
}

void showThankYouScreen(void) {
    system("cls");

    gotoxy(30, 4);
    printf("========================================================");

    gotoxy(43, 5);
    printf("+------------------------------+");

    gotoxy(43, 6);
    printf("|       THANK YOU FOR YOUR     |");
    gotoxy(43, 7);
    printf("|            ORDER!            |");
    gotoxy(43, 8);
    printf("+------------------------------+");
    gotoxy(46, 9);
    printf("Hope to see you again soon!");
    gotoxy(30, 10);
    printf("========================================================");

    gotoxy(30, 12);
    printf("👉 Press [ANY KEY] to return to Home Screen...");
    getch();
    showWelcomeScreen();
}

void userReview() {
    system("cls");

    char review_line[150];

    gotoxy(30, 5);
    printf("========================================================");
    gotoxy(49, 7);
    printf("Customer Review");
    gotoxy(30, 9);
    printf("========================================================");

    gotoxy(30, 11);
    printf("Enter Review: ");

    fgets(review_line, sizeof(review_line), stdin);
    review_line[strcspn(review_line, "\n")] = '\0';

    int orderID = getLastOrderID();
    FILE *freview = fopen("data/reviews.dat", "ab");
    if(freview == NULL) {
        gotoxy(25, 12);
        printf("Unable to open reviews.dat");
        getch();
        return;
    }

    fprintf(freview, "Order ID : %d\n", orderID);
    fprintf(freview, "Review   : %s\n", review_line);
    fprintf(freview,
            "----------------------------------------------------------\n");

    fclose(freview);

    showThankYouScreen();
}

void viewUserReview(void) {
    system("cls");

    FILE *fp = fopen("data/reviews.dat", "r");
    if(fp == NULL) {
        gotoxy(30, 10);
        printf("No customer reviews found.\n");
        gotoxy(30, 12);
        printf("👉 Press ANY KEY to return...");
        getch();
        adminMenu();
        return;
    }

    gotoxy(30, 2);
    printf("==============================================================");
    gotoxy(52, 4);
    printf("CUSTOMER REVIEWS");
    gotoxy(30, 6);
    printf("==============================================================");

    char line[256];
    int row = 8;
    int page = 0;
    int hasReview = 0;

    while(fgets(line, sizeof(line), fp) != NULL) {
        if(row > 20) {
            gotoxy(30, 22);
            printf("👉 Press ANY KEY to continue...");
            getch();
            system("cls");
            gotoxy(30, 2);
            printf("==========================================================="
                   "===");
            gotoxy(52, 4);
            printf("CUSTOMER REVIEWS");
            gotoxy(30, 6);
            printf("==========================================================="
                   "===");
            row = 8;
            page++;
        }

        int len = strlen(line);
        int start = 0;
        int firstLine = 1;

        while(start < len) {
            char part[61];
            int size;
            if(firstLine) {
                size = 60;
                gotoxy(32, row++);
                firstLine = 0;
            } else {
                size = 49;
                gotoxy(43, row++);
            }

            strncpy(part, line + start, size);
            part[size] = '\0';
            printf("%s", part);
            start += size;

            if(row > 20) {
                gotoxy(30, 22);
                printf("👉 Press ANY KEY to continue...");
                getch();

                system("cls");
                gotoxy(30, 2);
                printf("======================================================="
                       "=======");
                gotoxy(52, 4);
                printf("CUSTOMER REVIEWS");
                gotoxy(30, 6);
                printf("======================================================="
                       "=======");

                row = 8;
            }
        }

        hasReview = 1;
    }

    fclose(fp);

    if(!hasReview) {
        gotoxy(30, 4);
        printf("No customer reviews available yet.\n");
    }

    gotoxy(52, row);
    printf("No more reviews");
    gotoxy(30, row + 2);
    printf("==============================================================");
    gotoxy(32, row + 4);
    printf("👉 Press ANY KEY to return...");
    getch();
    adminMenu();
}