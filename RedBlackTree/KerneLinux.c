#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } color;

typedef struct vm_area_struct {
    unsigned long vm_start;
    unsigned long vm_end;
    color color;
    struct vm_area_struct *parent, *left, *right;
} vm_area_struct;

typedef struct mm_struct {
    vm_area_struct *root;
    vm_area_struct *mmap_cache;
} mm_struct;

vm_area_struct *create_vma(unsigned long start, unsigned long end) {
    vm_area_struct *vma = malloc(sizeof(vm_area_struct));
    vma->vm_start = start;
    vma->vm_end = end;
    vma->color = RED;
    vma->parent = vma->left = vma->right = NULL;
    return vma;
}

void rotate_left(mm_struct *process_mm, vm_area_struct *x) {
    vm_area_struct *y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        process_mm->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotate_right(mm_struct *process_mm, vm_area_struct *x) {
    vm_area_struct *y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        process_mm->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void fix_insert(mm_struct *process_mm, vm_area_struct *z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            vm_area_struct *y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(process_mm, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(process_mm, z->parent->parent);
            }
        } else {
            vm_area_struct *y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(process_mm, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(process_mm, z->parent->parent);
            }
        }
    }
    process_mm->root->color = BLACK;
}

void insert_vma(mm_struct *process_mm, vm_area_struct *vma) {
    vm_area_struct *y = NULL;
    vm_area_struct *x = process_mm->root;

    while (x) {
        y = x;
        if (vma->vm_start < x->vm_start)
            x = x->left;
        else
            x = x->right;
    }
    vma->parent = y;
    if (!y)
        process_mm->root = vma;
    else if (vma->vm_start < y->vm_start)
        y->left = vma;
    else
        y->right = vma;

    fix_insert(process_mm, vma);
}

vm_area_struct *find_vma(mm_struct *process_mm, unsigned long addr) {
    vm_area_struct *vma = process_mm->mmap_cache;
    if (vma && vma->vm_end > addr && vma->vm_start <= addr)
        return vma;

    vm_area_struct *node = process_mm->root;
    vma = NULL;

    while (node) {
        if (node->vm_end > addr) {
            vma = node;
            if (node->vm_start <= addr)
                break;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    if (vma)
        process_mm->mmap_cache = vma;
    return vma;
}

int main() {
    // Cria e inicializa a estrutura de memória do processo
    mm_struct process_mm;
    process_mm.root = NULL;
    process_mm.mmap_cache = NULL;

    // Cria uma nova área de memória virtual em create_vma (inicio, fim)
    // e insere na RBT
    insert_vma(&process_mm, create_vma(0x1000, 0x2000));
    insert_vma(&process_mm, create_vma(0x3000, 0x4000));
    insert_vma(&process_mm, create_vma(0x5000, 0x6000));
    insert_vma(&process_mm, create_vma(0x2500, 0x2800));
    insert_vma(&process_mm, create_vma(0x7000, 0x8000));

    // Procura onde o endereço addr está mapeado
    unsigned long addr = 0x2600;
    vm_area_struct *found = find_vma(&process_mm, addr);


    // Impressão
    if (found)
        printf("\nEndereço 0x%lx está em VMA [%lx - %lx]\n",
               addr, found->vm_start, found->vm_end);
    else
        printf("\nEndereo 0x%lx não pertence a nenhuma VMA.\n", addr);

    return 0;
}
