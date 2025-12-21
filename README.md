# C++ Data Structures & Algorithms (No STL Implementation)

> **"What I cannot create, I do not understand."** — Richard Feynman

This repository demonstrates a deep dive into core data structures and algorithms using **pure C++**. All implementations are built from scratch **without the Standard Template Library (STL)** to ensure a profound understanding of memory management, pointer arithmetic, and algorithmic efficiency.

## 🚀 Project Highlights

This is not just a collection of homework. It focuses on **performance optimization** and **low-level mechanics** relevant to Quantitative Finance and High-Performance Computing (HPC).

### 1. Manual Memory Management & Optimization
* **Static Memory Pool**: In `05_Trees/BST_Rank_Ops.cpp`, I implemented a static memory pool (`Node pool[MAXN]`) instead of repeated `new/delete`. This prevents memory fragmentation and significantly reduces allocation overhead in high-frequency operations.
* **Cache-Friendly Access**: In `03_Matrix/SparseMatrix.cpp`, I optimized sparse matrix multiplication by **transposing the matrix** first. This converts column-wise access into row-wise access, maximizing **CPU cache hits** and linear memory scanning.

### 2. Custom Data Structures
* **Hand-Written Heaps**: Implemented binary min-heaps manually (`06_Heap`) to support Priority Queues for **Huffman Coding** and **Prim’s Algorithm**, replacing `std::priority_queue`.
* **Chain Forward Star**: Used array-based adjacency lists (Chain Forward Star) in `07_Graph` for graph representation, which is more memory-efficient than standard vector-based adjacency lists (`std::vector<std::vector<int>>`).

### 3. Algorithm Implementation
* **Sorting**: Implemented **Quick Sort** and **Merge Sort** (for Linked Lists) from scratch to handle custom comparators and stability requirements.
* **Graph Algorithms**: Implemented **Kruskal’s** (with Disjoint Set Union) and **Prim’s** algorithms for MST.

---

## 📂 Repository Catalog

| Module | Description & Key Files |
| :--- | :--- |
| **00_Basics** | **Recursion & Bitwise Ops**: DFS for subset XOR sums and combinations. |
| **01_LinearList** | **Pointers & memory**: `SeqList`, `SinglyLinkedList`, and List Merging. |
| **02_StackQueue** | **Buffers**: Circular Queue (Ring Buffer) and Expression Evaluation (Stack). |
| **03_Matrix** | **Sparse Matrix**: Triple representation, Transpose optimization, and Multiplication. |
| **04_HashTable** | **Hash Map**: Implementation of **Open Addressing** (Linear Probing) & **Chaining**. |
| **05_Trees** | **BST & Reconstruction**: Pre/In-order reconstruction, Rank operations with **Memory Pool**. |
| **06_Heap** | **Priority Queue**: Manual Min-Heap implementation & Huffman Coding. |
| **07_Graph** | **Graph Theory**: BFS/DFS, MST (Prim/Kruskal) using custom data structures. |
| **08_Sorting** | **Sort Algorithms**: Template-based Sorts & Linked List Merge Sort. |

---

## 🛠️ Build & Run

Since no external libraries are used, you can compile any file using `g++`.

**Example: Running the Sparse Matrix module**
```bash
cd 03_Matrix
g++ SparseMatrix.cpp -o matrix_test -O2
./matrix_test
