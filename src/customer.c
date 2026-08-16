#include "console.h"
#include "main.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct {
    int id;
    char date[15];
    char name[50];
    float price;
} Food;

typedef struct {
    int id;
    char date[70];
    char name[70];
    float price;
    int qty;
    float subTotal;
} Cart;

Food f[100];
int count = 0;
int has_items = 0;

void generateBill();
void displayBill();
void saveOrder();
void customerMenu();
void displayMenuToCustomer();
void addToCart(char mode);
void viewCart();
void addMoreItems();
void deleteCartItem();
void loadMenuFromFile();
void clearCartFile();
void updateCartStatus();
void gotoxy(int x, int y);

void loadMenuFromFile() {
    FILE *fp = fopen("data/menu.dat", "rb");
    count = 0;

    if(fp == NULL) {
        return;
    }

    long fileSize;
    if(fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return;
    }
    fileSize = ftell(fp);
    rewind(fp);

    size_t recordSize = sizeof(Food);
    if(recordSize == 0 || fileSize % recordSize != 0) {
        fclose(fp);
        remove("data/menu.dat");
        return;
    }

    while(count < 100 && fread(&f[count], recordSize, 1, fp) == 1) {
        if(f[count].id <= 0 || f[count].name[0] == '\0' ||
           f[count].date[0] == '\0') {
            fclose(fp);
            remove("data/menu.dat");
            count = 0;
            return;
        }
        count++;
    }
    fclose(fp);
}

void clearCartFile() {
    FILE *fp = fopen("data/cart.dat", "wb");
    if(fp != NULL) {
        fclose(fp);
    }
    has_items = 0;
}

void updateCartStatus(void) {
    FILE *fp = fopen("data/cart.dat", "rb");
    if(fp != NULL) {
        Cart cart;
        if(fread(&cart, sizeof(Cart), 1, fp) == 1) {
            has_items = 1;
        } else {
            has_items = 0;
        }
        fclose(fp);
    } else {
        has_items = 0;
    }
}

void customerMenu(void) {
    int choice;
    loadMenuFromFile();
    updateCartStatus();

    while(1) {
        system("cls");
        gotoxy(30, 2);
        printf("========================================================");

        gotoxy(50, 4);
        printf("RESTAURANT BILLING APP");

        gotoxy(30, 6);
        printf("========================================================");

        gotoxy(52, 7);
        printf("+----------------+");
        gotoxy(52, 8);
        printf("| CUSTOMER PANEL |");
        gotoxy(52, 9);
        printf("+----------------+");

        gotoxy(49, 11);
        printf("[ Please Select Option ]");

        gotoxy(49, 13);
        printf("[1] 🍔 Browse Food Menu");
        gotoxy(49, 14);
        printf("[2] 🛒 View Current Cart");
        gotoxy(49, 15);
        printf("[3] 🧾 Checkout & Bills");
        gotoxy(49, 16);
        printf("[4] 🔙 Back to Main Menu");
        gotoxy(30, 17);
        printf("--------------------------------------------------------");

        gotoxy(33, 18);
        printf("Select Option (1-4): ");
        selectionNumInput(&choice, ycord, customerMenu);

        switch(choice) {
        case 1:
            displayMenuToCustomer();
            break;
        case 2:
            viewCart();
            break;
        case 3:
            if(has_items) {
                displayBill();
            } else {
                gotoxy(30, 20);
                printf("⚠️  First add some items");
                gotoxy(30, 21);
                printf("👉 Press ANY KEY to try again...");
                getch();
                customerMenu();
            }
            break;
        case 4:
            showWelcomeScreen();
            return;
        default:
            gotoxy(30, 20);
            printf("⚠️  Invalid Selection");
            gotoxy(30, 21);
            printf("👉 Press ANY KEY to try again...");
            getch();
            customerMenu();
        }
    }
}

void displayMenuToCustomer(void) {
    loadMenuFromFile();
    system("cls");
    gotoxy(20, 3);
    printf("==================================================================="
           "=======");
    gotoxy(20, 4);
    printf("                          AVAILABLE FOOD ITEMS                     "
           "       ");
    gotoxy(20, 5);
    printf("==================================================================="
           "=======");

    if(count == 0) {
        gotoxy(30, 7);
        printf("No food items available in the menu.");
        gotoxy(30, 9);
        printf("Press Enter to go back...");
        getch();
        return;
    }
    gotoxy(20, 7);
    printf("  %-5s  |   %-45s |  %-10s", "ID", "Food Item name", "Price (TK)");
    gotoxy(20, 8);
    printf(
        "---------+-------------------------------------------------+--------"
        "------");

    for(int i = 0; i < count; i++) {
        gotoxy(20, 9 + i);
        printf(" #%-6d |   %-45s |   %-10.2f", f[i].id, f[i].name, f[i].price);
    }

    gotoxy(20, 10 + count);
    printf("==================================================================="
           "=======");
    gotoxy(20, 12 + count);
    printf("Do you want to add an item to cart? (y/n): ");
    char confirm = selectionCharInput();

    if(confirm == 'y' || confirm == 'Y') {
        addToCart('w');
    } else if(confirm == 'n' || confirm == 'N') {
        customerMenu();
    } else {
        gotoxy(30, 14 + count);
        printf("⚠️  Invalid Selection");
        gotoxy(30, 15 + count);
        printf("👉 Press ANY KEY to try again...");
        getch();
        displayMenuToCustomer();
    }
}

void addToCart(char mode) {
    system("cls");
    gotoxy(20, 3);
    printf("==================================================================="
           "=======");
    gotoxy(20, 4);
    printf("                              ADD TO CART                          "
           "       ");
    gotoxy(20, 5);
    printf("==================================================================="
           "=======");

    gotoxy(20, 7);
    printf("  %-5s  |   %-45s |  %-10s", "ID", "Food Item name", "Price (TK)");
    gotoxy(20, 8);
    printf(
        "---------+-------------------------------------------------+--------"
        "------");

    for(int i = 0; i < count; i++) {
        gotoxy(20, 9 + i);
        printf(" #%-6d |   %-45s |  %-10.2f", f[i].id, f[i].name, f[i].price);
    }

    gotoxy(20, 10 + count);
    printf("==================================================================="
           "=======");

    int search_id;
    gotoxy(25, 12 + count);
    printf("Enter Food ID: ");
    if(scanf("%d", &search_id) != 1) {
        while(getchar() != '\n')
            ;
        gotoxy(40, 12 + count);
        printf("⚠️  Invalid input format!");
        gotoxy(25, 13 + count);
        printf("👉 Press [ANY KEY] to try again...");
        getch();
        addToCart('w');
    }

    int target_index = -1;
    for(int i = 0; i < count; i++) {
        if(f[i].id == search_id) {
            target_index = i;
            break;
        }
    }
    while(getchar() != '\n')
        ;

    if(target_index == -1) {
        gotoxy(25, 12 + count);
        printf("Item ID not found! Please check the menu.");
        gotoxy(25, 13 + count);
        printf("👉 Press [ANY KEY] to try again...");
        getch();
        addToCart('w');
    }

    int quantity;
    gotoxy(25, 13 + count);
    printf("Enter Quantity: ");
    if(scanf("%d", &quantity) != 1 || quantity <= 0) {
        while(getchar() != '\n')
            ;
        gotoxy(41, 13 + count);
        printf("⚠️  Invalid quantity!");
        gotoxy(25, 14 + count);
        printf("👉 Press [ANY KEY] to try again...");
        getch();
        addToCart('w');
    }

    int duplicate_found = 0;
    FILE *cart_read = fopen("data/cart.dat", "rb");
    FILE *temp_cart = NULL;

    if(cart_read != NULL) {
        temp_cart = fopen("data/cart_temp.dat", "wb");
    }

    if(cart_read != NULL && temp_cart != NULL) {
        Cart cart;

        while(fread(&cart, sizeof(Cart), 1, cart_read) == 1) {
            if(cart.id == search_id && mode == 'a') {
                cart.qty += quantity;
                cart.subTotal = cart.qty * cart.price;
                duplicate_found = 1;
            }
            fwrite(&cart, sizeof(Cart), 1, temp_cart);
        }
        fclose(cart_read);
        fclose(temp_cart);
        remove("data/cart.dat");
        rename("data/cart_temp.dat", "data/cart.dat");
    } else {
        if(cart_read != NULL)
            fclose(cart_read);
        if(temp_cart != NULL)
            fclose(temp_cart);
    }

    if(!duplicate_found) {
        FILE *fp = fopen("data/cart.dat", "ab");
        if(fp == NULL) {
            gotoxy(30, 18 + count);
            printf("System Error: Could not update cart file.");
            getch();
            return;
        }

        Cart cart;

        cart.id = f[target_index].id;
        strcpy(cart.date, f[target_index].date);
        strcpy(cart.name, f[target_index].name);
        cart.price = f[target_index].price;
        cart.qty = quantity;
        cart.subTotal = cart.price * cart.qty;

        fwrite(&cart, sizeof(Cart), 1, fp);
        fclose(fp);
    }

    has_items = 1;
    viewCart();
}

void viewCart(void) {
    system("cls");
    FILE *fp = fopen("data/cart.dat", "rb");

    gotoxy(12, 3);
    printf("==============================================================="
           "===============================");
    gotoxy(52, 5);
    printf("ORDER VIEW");
    gotoxy(12, 7);
    printf("==============================================================="
           "===============================");

    int i = 0;
    int cart_has_items = 0;

    if(fp != NULL) {
        Cart cart;

        gotoxy(12, 8);
        printf("  %-5s |   %-41s |   %-10s |   %-5s |   %-12s", "ID",
               "Food Item name", "Price", "Qty", "Total (TK)");
        gotoxy(12, 9);
        printf("--------+---------------------------------------------+----"
               "----------+---------+--------------");

        while(fread(&cart, sizeof(Cart), 1, fp) == 1) {
            gotoxy(12, 10 + i);
            printf("  %-5d |   %-41s |   %-10.2f |   %-5d |  "
                   " %-12.2f",
                   cart.id, cart.name, cart.price, cart.qty, cart.subTotal);
            i++;
            cart_has_items = 1;
        }
        fclose(fp);
    }

    if(!cart_has_items) {
        system("cls");

        gotoxy(12, 3);
        printf("======================================================="
               "=======================================");
        gotoxy(52, 5);
        printf("ORDER VIEW");
        gotoxy(12, 7);
        printf("======================================================="
               "=======================================");
        gotoxy(48, 8);
        printf("Your cart is empty.");
        i = 1;
    }

    int gap = (cart_has_items) ? 10 : 8;
    gotoxy(12, gap + i);
    printf("==============================================================="
           "===============================");
    gotoxy(12, gap + 2 + i);
    printf("   [1] Confirm My Order");
    gotoxy(12, gap + 3 + i);
    printf("   [2] Add More Item");
    gotoxy(12, gap + 4 + i);
    printf("   [3] Delete Specific Item From Cart");
    gotoxy(12, gap + 5 + i);
    printf("   [4] Back to Menu");
    gotoxy(12, gap + 6 + i);
    printf("---------------------------------------------------------------"
           "-------------------------------");

    int cart_choice;
    gotoxy(15, gap + 7 + i);
    printf("Select Action (1-4): ");

    selectionNumInput(&cart_choice, ycord, viewCart);

    switch(cart_choice) {
    case 1:
        if(has_items) {
            displayBill();
        } else {
            gotoxy(30, gap + 10 + i);
            printf("⚠️  First add some items");
            gotoxy(30, gap + 11 + i);
            printf("👉 Press ANY KEY to try again...");
            getch();
            viewCart();
        }
        break;
    case 2:
        addMoreItems();
        break;
    case 3:
        if(has_items) {
            deleteCartItem();
        } else {
            gotoxy(30, 20);
            printf("⚠️  First add some items");
            gotoxy(30, 21);
            printf("👉 Press ANY KEY to try again...");
            getch();
            viewCart();
        }
        break;
    case 4:
        customerMenu();
        break;
    default:
        gotoxy(30, gap + 10 + i);
        printf("⚠️  Invalid Selection");
        gotoxy(30, gap + 11 + i);
        printf("👉 Press ANY KEY to try again...");
        getch();
        viewCart();
    }
}

void addMoreItems(void) { addToCart('a'); }

void deleteCartItem(void) {
    system("cls");

    gotoxy(12, 3);
    printf("==================================================================="
           "======================");
    gotoxy(12, 4);
    printf("                                   CURRENT CART ITEMS              "
           "                      ");
    gotoxy(12, 5);
    printf("==================================================================="
           "======================");

    FILE *fp = fopen("data/cart.dat", "rb");
    int i = 0;
    int cart_has_items = 0;

    if(fp != NULL) {
        Cart cart;
        gotoxy(12, 7);
        printf("  %-5s |   %-39s |   %-10s|  %-5s  |  %-12s", "ID",
               "Food Item name", "Price", "Qty", "Total (TK)");
        gotoxy(12, 8);
        printf("--------+-------------------------------------------+----------"
               "---+---------+------------");

        while(fread(&cart, sizeof(Cart), 1, fp) == 1) {
            gotoxy(12, 9 + i);
            printf(" %-6d |   %-39s |  %-10.2f |   %-5d |  "
                   "%-12.2f",
                   cart.id, cart.name, cart.price, cart.qty, cart.subTotal);
            i++;
            cart_has_items = 1;
        }
        fclose(fp);
    }

    int gap = 9;
    gotoxy(12, gap + i);
    printf("==================================================================="
           "======================");

    gotoxy(15, gap + 1 + i);
    printf("Do you want to remove an item? (y/n): ");
    char confirm = selectionCharInput();

    if(confirm == 'y' || confirm == 'Y') {
        int delete_id;

        gotoxy(12, gap + 1 + i);
        printf("                                             ");
        gotoxy(18, gap + 2 + i);
        printf("Enter Food ID to remove: ");
        if(scanf("%d", &delete_id) != 1) {
            while(getchar() != '\n')
                ;
            gotoxy(30, gap + 4 + i);
            printf("⚠️  Invalid ID");
            gotoxy(30, gap + 5 + i);
            printf("👉 Press ANY KEY to try again...");
            getch();
            deleteCartItem();
        }

        fp = fopen("data/cart.dat", "rb");
        FILE *temp = fopen("data/temp.dat", "wb");

        if(fp == NULL || temp == NULL) {
            if(fp != NULL)
                fclose(fp);
            if(temp != NULL)
                fclose(temp);
            return;
        }

        Cart cart;
        int found = 0;

        while(fread(&cart, sizeof(Cart), 1, fp) == 1) {
            if(cart.id == delete_id) {
                found = 1;
                continue;
            }
            fwrite(&cart, sizeof(Cart), 1, temp);
        }

        fclose(fp);
        fclose(temp);

        if(!found) {
            remove("data/temp.dat");
            gotoxy(30, gap + 4 + i);
            printf("⚠️ Invalid ID");
            gotoxy(30, gap + 5 + i);
            printf("👉 Press ANY KEY to try again...");
            getch();
            deleteCartItem();
            return;
        }

        remove("data/cart.dat");
        rename("data/temp.dat", "data/cart.dat");
        viewCart();
        return;
    } else if(confirm == 'n' || confirm == 'N') {
        viewCart();
        return;
    } else {
        gotoxy(30, gap + 4 + i);
        printf("⚠️  Invalid Selection");
        gotoxy(30, gap + 5 + i);
        printf("👉 Press ANY KEY to try again...");
        getch();
        deleteCartItem();
        return;
    }
}
