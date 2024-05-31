#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *kiri;
    struct Node *kanan;
    int height;
};

int Height(struct Node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int Max(int a, int b)
{
    return (a > b) ? a : b;
}

struct Node *createNode(int data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->kiri = NULL;
    newNode->kanan = NULL;
    newNode->height = 1;
    return newNode;
}

struct Node *rotateRight(struct Node *y)
{
    struct Node *x = y->kiri;
    struct Node *T2 = x->kanan;

    x->kanan = y;
    y->kiri = T2;

    y->height = Max(Height(y->kiri), Height(y->kanan)) + 1;
    x->height = Max(Height(x->kiri), Height(x->kanan)) + 1;

    return x;
}

struct Node *rotateLeft(struct Node *x)
{
    struct Node *y = x->kanan;
    struct Node *T2 = y->kiri;

    y->kiri = x;
    x->kanan = T2;

    x->height = Max(Height(x->kiri), Height(x->kanan)) + 1;
    y->height = Max(Height(y->kiri), Height(y->kanan)) + 1;

    return y;
}

int getBalanceFactor(struct Node *node)
{
    if (node == NULL)
        return 0;
    return Height(node->kiri) - Height(node->kanan);
}

struct Node *insertNode(struct Node *node, int data)
{
    if (node == NULL)
        return createNode(data);

    if (data < node->data)
        node->kiri = insertNode(node->kiri, data);
    else if (data > node->data)
        node->kanan = insertNode(node->kanan, data);
    else
        return node;

    node->height = 1 + Max(Height(node->kiri), Height(node->kanan));

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1 && data < node->kiri->data)
        return rotateRight(node);

    if (balanceFactor < -1 && data > node->kanan->data)
        return rotateLeft(node);

    if (balanceFactor > 1 && data > node->kiri->data)
    {
        node->kiri = rotateLeft(node->kiri);
        return rotateRight(node);
    }

    if (balanceFactor < -1 && data < node->kanan->data)
    {
        node->kanan = rotateRight(node->kanan);
        return rotateLeft(node);
    }

    return node;
}

struct Node *findMinNode(struct Node *node)
{
    struct Node *current = node;
    while (current->kiri != NULL)
        current = current->kiri;
    return current;
}

struct Node *deleteNode(struct Node *root, int data)
{
    if (root == NULL)
        return root;

    if (data < root->data)
        root->kiri = deleteNode(root->kiri, data);
    else if (data > root->data)
        root->kanan = deleteNode(root->kanan, data);
    else
    {
        if (root->kiri == NULL && root->kanan == NULL)
        {
            free(root);
            root = NULL;
        }
        else if (root->kiri == NULL || root->kanan == NULL)
        {
            struct Node *temp = (root->kiri != NULL) ? root->kiri : root->kanan;
            *root = *temp;
            free(temp);
        }
        else
        {
            struct Node *temp = findMinNode(root->kanan);
            root->data = temp->data;
            root->kanan = deleteNode(root->kanan, temp->data);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + Max(Height(root->kiri), Height(root->kanan));

    int balanceFactor = getBalanceFactor(root);

    if (balanceFactor > 1 && getBalanceFactor(root->kiri) >= 0)
        return rotateRight(root);

    if (balanceFactor > 1 && getBalanceFactor(root->kiri) < 0)
    {
        root->kiri = rotateLeft(root->kiri);
        return rotateRight(root);
    }

    if (balanceFactor < -1 && getBalanceFactor(root->kanan) <= 0)
        return rotateLeft(root);

    if (balanceFactor < -1 && getBalanceFactor(root->kanan) > 0)
    {
        root->kanan = rotateRight(root->kanan);
        return rotateLeft(root);
    }

    return root;
}

void deleteValue(struct Node **root, int data)
{
    if (*root == NULL)
    {
        printf("Data not found!\n");
        return;
    }

    struct Node *current = *root;
    struct Node *parent = NULL;
    int found = 0;

    while (current != NULL)
    {
        if (data == current->data)
        {
            found = 1;
            break;
        }
        parent = current;
        if (data < current->data)
            current = current->kiri;
        else
            current = current->kanan;
    }

    if (found)
    {
        if (current == *root)
            *root = deleteNode(*root, data);
        else if (data < parent->data)
            parent->kiri = deleteNode(parent->kiri, data);
        else
            parent->kanan = deleteNode(parent->kanan, data);
        printf("Data Found\n");
        printf("Value %d was deleted\n", data);
    }
    else
    {
        printf("Data not found\n");
    }
}

void preorderTraversal(struct Node *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
        preorderTraversal(root->kiri);
        preorderTraversal(root->kanan);
    }
}

void inorderTraversal(struct Node *root)
{
    if (root != NULL)
    {
        inorderTraversal(root->kiri);
        printf("%d ", root->data);
        inorderTraversal(root->kanan);
    }
}

void postorderTraversal(struct Node *root)
{
    if (root != NULL)
    {
        postorderTraversal(root->kiri);
        postorderTraversal(root->kanan);
        printf("%d ", root->data);
    }
}

int main()
{
    struct Node *root = NULL;
    int choice, data;

    do
    {
        printf("\nAVL Tree Operations\n");
        printf("1. Insertion\n");
        printf("2. Deletion\n");
        printf("3. Traversal\n");
        printf("4. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Insert: ");
            scanf("%d", &data);
            root = insertNode(root, data);
            break;
        case 2:
            printf("Delete: ");
            scanf("%d", &data);
            deleteValue(&root, data);
            root = deleteNode(root, data);
            break;
        case 3:
            printf("Preorder Traversal: ");
            preorderTraversal(root);
            printf("\n");
            printf("Inorder Traversal: ");
            inorderTraversal(root);
            printf("\n");
            printf("Postorder Traversal: ");
            postorderTraversal(root);
            printf("\n");
            break;
        case 4:
            printf("Thank You\n");
            break;
        default:
            printf("Invalid choice! Please enter a valid option.\n");
            break;
        }
    } while (choice != 4);

    return 0;
}