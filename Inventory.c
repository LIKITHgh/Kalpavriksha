#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

typedef struct {
    int id;
    char name[MAX_NAME];
    float price;
    int quantity;
} Product;


void addProduct(Product **list, int *total);
void viewProducts(Product *list, int total);
void updateQuantity(Product *list, int total);
void findByID(Product *list, int total);
void findByName(Product *list, int total);
void findByPrice(Product *list, int total);
void removeProduct(Product **list, int *total);

int main() {
    int totalProducts, choice;
    Product *inventory = NULL;

    printf("Enter initial number of products: ");
    scanf("%d", &totalProducts);
    getchar(); 

    inventory = (Product *)calloc(totalProducts, sizeof(Product));

    for (int i = 0; i < totalProducts; i++) {
        printf("\nEnter details for product %d:\n", i + 1);
        printf("Product ID: ");
        scanf("%d", &inventory[i].id);
        printf("Product Name: ");
        fgets(inventory[i].name, MAX_NAME, stdin);
        printf("Product Price: ");
        scanf("%f", &inventory[i].price);
        printf("Product Quantity: ");
        scanf("%d", &inventory[i].quantity);
    }

    do {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addProduct(&inventory, &totalProducts); break;
            case 2: viewProducts(inventory, totalProducts); break;
            case 3: updateQuantity(inventory, totalProducts); break;
            case 4: findByID(inventory, totalProducts); break;
            case 5: findByName(inventory, totalProducts); break;
            case 6: findByPrice(inventory, totalProducts); break;
            case 7: removeProduct(&inventory, &totalProducts); break;
            case 8:
                printf("\nFreeing memory and exiting...\n");
                free(inventory);
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while (choice != 8);

    return 0;
}

void addProduct(Product **list, int *total) {
    *list = realloc(*list, (*total + 1) * sizeof(Product));

    if (*list == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    Product *p = &((*list)[*total]);
    printf("\nEnter new product details:\n");
    printf("Product ID: ");
    scanf("%d", &p->id);
    printf("Product Name: ");
    fgets(p->name, MAX_NAME, stdin);
    p->name[strcspn(p->name, "\n")] = '\0';
    printf("Product Price: ");
    scanf("%f", &p->price);
    printf("Product Quantity: ");
    scanf("%d", &p->quantity);

    (*total)++;
    printf("Product added successfully!\n");
}

void viewProducts(Product *list, int total) {
    if (total == 0) {
        printf("\nNo products in inventory.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int i = 0; i < total; i++) {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               list[i].id, list[i].name, list[i].price, list[i].quantity);
    }
}

void updateQuantity(Product *list, int total) {
    int id, newQty, found = 0;
    printf("Enter Product ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < total; i++) {
        if (list[i].id == id) {
            printf("Enter new Quantity: ");
            scanf("%d", &newQty);
            list[i].quantity = newQty;
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Product not found!\n");
}

void findByID(Product *list, int total) {
    int id, found = 0;
    printf("Enter Product ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < total; i++) {
        if (list[i].id == id) {
            printf("Product Found ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   list[i].id, list[i].name, list[i].price, list[i].quantity);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No product found with this ID.\n");
}

void findByName(Product *list, int total) {
    char search[MAX_NAME];
    int found = 0;

    printf("Enter name to search (partial allowed): ");
    fgets(search, MAX_NAME, stdin);
    search[strcspn(search, "\n")] = '\0';

    printf("\nMatching Products:\n");
    for (int i = 0; i < total; i++) {
        if (strstr(list[i].name, search)) {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   list[i].id, list[i].name, list[i].price, list[i].quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No products matched that name.\n");
}

void findByPrice(Product *list, int total) {
    float min, max;
    int found = 0;

    printf("Enter minimum price: ");
    scanf("%f", &min);
    printf("Enter maximum price: ");
    scanf("%f", &max);

    printf("\nProducts within price range:\n");
    for (int i = 0; i < total; i++) {
        if (list[i].price >= min && list[i].price <= max) {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   list[i].id, list[i].name, list[i].price, list[i].quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No products found in this price range.\n");
}

void removeProduct(Product **list, int *total) {
    int id, found = 0;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *total; i++) {
        if ((*list)[i].id == id) {
            for (int j = i; j < *total - 1; j++) {
                (*list)[j] = (*list)[j + 1];
            }
            *list = realloc(*list, (*total - 1) * sizeof(Product));
            (*total)--;
            printf("Product deleted successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Product ID not found!\n");
}
