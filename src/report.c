#include "admin.h"
#include "console.h"
#include "main.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int choice;

typedef struct {
    int orderId;
    char date[20];
    char items[100];
    float total;
} Order;

void viewAllOrders(void);
void searchOrder();

// View All Orders
void viewAllOrders(void) {
    FILE *fp;
    Order order;

    fp = fopen("data/orders.dat", "rb");

    if(fp == NULL) {
        system("cls");
        gotoxy(17, 2);
        printf("No order records found!");
        return;
    }

    system("cls");

    gotoxy(17, 2);
    printf("==================================================================="
           "=================");

    gotoxy(49, 3);
    printf("ALL PLACED ORDERS");

    gotoxy(17, 4);
    printf("==================================================================="
           "=================");

    gotoxy(17, 6);
    printf(
        "Order ID | Date       | Items Ordered                              | "
        "Total "
        "Bill (TK)");

    gotoxy(17, 7);
    printf(
        "---------+------------+--------------------------------------------+-"
        "---------------");

    ycord = 8;

    while(fread(&order, sizeof(Order), 1, fp) == 1) {
        gotoxy(17, ycord++);
        printf("#%-7d | %-10s | %-42.42s | %10.2f", order.orderId, order.date,
               order.items, order.total);
    }

    gotoxy(17, ycord + 1);
    printf("==================================================================="
           "=================");

    fclose(fp);
    gotoxy(17, ycord + 3);
    printf("[1] Search Orders");

    gotoxy(17, ycord + 4);
    printf("[2] Back");

    gotoxy(17, ycord + 6);
    printf("➔ Select Option (1-2): ");

    selectionNumInput(&choice, ycord, viewAllOrders);

    switch(choice) {
    case 1:
        searchOrder();
        break;

    case 2:
        adminMenu();
        break;

    default:
        gotoxy(17, ycord + 3);
        printf("⚠️  Invalid Selection");
        gotoxy(17, ycord + 4);
        printf("👉 Press ANY KEY to try again...");
        getch();
        viewAllOrders();
    }
}

// Search Order
void searchOrder() {
    FILE *fp = fopen("data/orders.dat", "rb");

    if(fp == NULL) {
        system("cls");
        gotoxy(17, 2);
        printf("No order records found!");
        return;
    }
    system("cls");

    gotoxy(17, 2);
    printf("==================================================================="
           "=============");

    gotoxy(47, 4);
    printf("SEARCH ORDERS");
    gotoxy(17, 6);
    printf("==================================================================="
           "=============");
    gotoxy(18, 7);
    printf("[💡 Hint: Enter the exact Order ID (e.g., #101) to view details]");
    gotoxy(18, 9);
    printf("➔ Enter Order ID : ");

    Order order;
    int searchId;
    int found = 0;

    scanf("%d", &searchId);
    while(getchar() != '\n')
        ;

    while(fread(&order, sizeof(Order), 1, fp) == 1) {
        if(order.orderId == searchId) {
            gotoxy(18, 9);
            printf("• Status   :  [ ORDER FOUND SUCCESSFUL ]");

            gotoxy(18, 10);
            printf("-----------------------------------------------------------"
                   "-------------------");

            gotoxy(18, 11);
            printf("• Order ID : #%d", order.orderId);

            gotoxy(18, 12);
            printf("• Date     : %s", order.date);

            gotoxy(18, 13);
            printf("• Items    : %s", order.items);

            gotoxy(18, 14);
            printf("• Total    : %.2f TK", order.total);

            gotoxy(18, 15);
            printf("-----------------------------------------------------------"
                   "-------------------");

            gotoxy(17, 17);
            printf("==========================================================="
                   "=====================");

            gotoxy(18, 19);
            printf("Press ANY KEY to return...");
            getch();
            viewAllOrders();

            found = 1;
            break;
        }
    }

    if(!found) {
        gotoxy(18, 9);
        printf("• Status   :  [ ORDER NOT FOUND ]");

        gotoxy(18, 10);
        printf("-----------------------------------------------------------"
               "-------------------");

        gotoxy(18, 12);
        printf("Press ANY KEY to return...");
        getch();
        viewAllOrders();
    }

    fclose(fp);
}

// Sales Report
void generateSalesReport(void) {
    FILE *fp;

    Order order;

    int totalOrders = 0;
    char lastDate[20] = "N/A";
    float totalRevenue = 0;

    fp = fopen("data/orders.dat", "rb");

    if(fp == NULL) {
        system("cls");
        gotoxy(15, 5);
        printf("Unable to open orders.dat");
        return;
    }

    while(fread(&order, sizeof(Order), 1, fp) == 1) {
        totalOrders++;
        totalRevenue += order.total;
        strcpy(lastDate, order.date);
    }

    fclose(fp);

    system("cls");

    gotoxy(35, 2);
    printf("===================================================");

    gotoxy(52, 3);
    printf("SALES REPORT SUMMARY");

    gotoxy(35, 4);
    printf("===================================================");

    gotoxy(38, 6);
    printf("📅 Report Generated On : %s", lastDate);

    gotoxy(38, 7);
    printf("📦 Total Orders Placed : %d Orders", totalOrders);

    gotoxy(38, 8);
    printf("💰 Total Revenue Made  : %.2f TK", totalRevenue);

    gotoxy(35, 10);
    printf("===================================================");

    gotoxy(38, 12);
    printf("-> Press ANY KEY to return...");
    getch();
    adminMenu();
}
