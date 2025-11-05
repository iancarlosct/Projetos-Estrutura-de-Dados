#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } color;

// Estrutura da VMA
typedef struct vm_area_struct {
    unsigned long vm_start;
    unsigned long vm_end;
    color color;
    struct vm_area_struct *parent, *left, *right; // RBT
    struct vm_area_struct *vm_next, *vm_prev;     // Lista encadeada
} vm_area_struct;

// Estrutura do processo (mm_struct)
typedef struct mm_struct {
    vm_area_struct *root;       // Árvore rubro-negra
    vm_area_struct *mmap_cache; // Cache da última busca
    vm_area_struct *mmap;       // Lista encadeada de VMAs
} mm_struct;

vm_area_struct *create_vma(unsigned long start, unsigned long end) {
  vm_area_struct *vma = malloc(sizeof(vm_area_struct));
  vma->vm_start = start;
  vma->vm_end = end;
  vma->color = RED;
  vma->parent = vma->left = vma->right = NULL;
  vma->vm_next = vma->vm_prev = NULL;
  return vma;
}

void insert_vma_list(mm_struct *mm, vm_area_struct *vma) {
  vm_area_struct *curr = mm->mmap, *prev = NULL;

  // Inserção ordenada por vm_start
  while (curr && curr->vm_start < vma->vm_start) {
      prev = curr;
      curr = curr->vm_next;
  }

  vma->vm_next = curr;
  vma->vm_prev = prev;
  if (prev) {
    prev->vm_next = vma;
  } else {
    mm->mmap = vma;
  }
  if (curr) {
    curr->vm_prev = vma;
  }
}

void rotate_left(mm_struct *mm, vm_area_struct *x) {
  vm_area_struct *y = x->right;
  x->right = y->left;
  if (y->left) y->left->parent = x;
  y->parent = x->parent;
  if (!x->parent){
    mm->root = y;
  } else if (x == x->parent->left){
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void rotate_right(mm_struct *mm, vm_area_struct *x) {
  vm_area_struct *y = x->left;
  x->left = y->right;
  if (y->right) y->right->parent = x;
  y->parent = x->parent;
  if (!x->parent){
    mm->root = y;
  } else if (x == x->parent->right){
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void fix_insert(mm_struct *mm, vm_area_struct *z) {
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
                rotate_left(mm, z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            rotate_right(mm, z->parent->parent);
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
                rotate_right(mm, z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            rotate_left(mm, z->parent->parent);
          }
      }
  }
  mm->root->color = BLACK;
}

void insert_vma_tree(mm_struct *mm, vm_area_struct *vma) {
  vm_area_struct *y = NULL;
  vm_area_struct *x = mm->root;

  while (x) {
    y = x;
    if (vma->vm_start < x->vm_start){
        x = x->left;
    } else{
        x = x->right;
    }
  }

  vma->parent = y;
  if (!y)
    mm->root = vma;
  else if (vma->vm_start < y->vm_start)
    y->left = vma;
  else
    y->right = vma;

  fix_insert(mm, vma);
}

vm_area_struct *find_vma_list(mm_struct *mm, unsigned long addr, int *ops) {
  vm_area_struct *vma = mm->mmap;
  while (vma) {
      (*ops)++;
      if (vma->vm_start <= addr && addr < vma->vm_end)
          return vma;
      vma = vma->vm_next;
  }
  return NULL;
}

vm_area_struct *find_vma_tree(mm_struct *mm, unsigned long addr, int *operations) {
  vm_area_struct *vma = mm->mmap_cache;
  if (vma && vma->vm_end > addr && vma->vm_start <= addr)
      return vma;

  vm_area_struct *node = mm->root;
  vma = NULL;

  while (node) {
    (*operations)++;
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
      mm->mmap_cache = vma;
  return vma;
}

int main() {
  mm_struct mm = {0};
  
  unsigned long ranges[8][2] = {
      {0x1000, 0x2000},
      {0x2000, 0x3000},
      {0x3000, 0x4000},
      {0x4000, 0x5000},
      {0x5000, 0x6000},
      {0x6000, 0x7000},
      {0x7000, 0x8000},
      {0x8000, 0x9000},
  };

  for (int i = 0; i < 8; i++) {
      vm_area_struct *v = create_vma(ranges[i][0], ranges[i][1]);
      insert_vma_list(&mm, v);
      insert_vma_tree(&mm, create_vma(ranges[i][0], ranges[i][1]));
  }

  unsigned long addr = 0x7500;
  int ops_list = 0, ops_tree = 0;

  vm_area_struct *vmaL = find_vma_list(&mm, addr, &ops_list);
  vm_area_struct *vmaT = find_vma_tree(&mm, addr, &ops_tree);

  printf("address 0x%lx:\n", addr);
  printf("Linked List: %d Comparations:\n", ops_list);
  if (vmaL) printf("[%lx - %lx]\n", vmaL->vm_start, vmaL->vm_end);
  else printf("Not found\n");

  printf("Red-Black Tree: %d Comparations:\n", ops_tree);
  if (vmaT) printf("[%lx - %lx]\n", vmaT->vm_start, vmaT->vm_end);
  else printf("Not found\n");

  return 0;
}
