# Data Structures & Algorithms Documentation

This document provides a structured overview of all Data Structures and Algorithms used in the system, explaining their purpose, complexity, and role within the architecture. Additional justifications are included to clarify why these choices are efficient and appropriate for real-world use.


## 1. Trie (Prefix Tree)

The Trie enables fast and efficient autocomplete functionality for product names.

### Purpose

* Autocomplete
* Fast prefix matching
* Predictable lookup performance (search time depends only on word length, not dataset size)

### Why a Trie?

* More efficient than linear search for prefix queries
* Ideal for search bars and type-ahead features
* **Consistent lookup time** makes it reliable even as data grows
* **Real-world justification:** Lookup cost is based on *L* (word length), which is tiny (usually < 20). This provides instant results for users.

### Operations & Time Complexity

| Operation     | Description                  | Complexity   |
| ------------- | ---------------------------- | ------------ |
| Insert        | Add a word to the Trie       | **O(L)**     |
| Search Prefix | Verify prefix existence      | **O(L)**     |
| Autocomplete  | DFS from matched prefix node | **O(N × L)** |

**Where:** *L* = length of the input word, *N* = number of matches returned
* Word lengths are short → **L is small**
* Only matched prefixes are explored → DFS is **data-dependent**, not full-tree
* Autocomplete typically returns small subsets, making it very fast in practice


## 2. Fuzzy Search (Levenshtein Distance)

Used when the user input does not match anything in the Trie, enabling typo-tolerant search.

### Purpose

* Handle near-matches and misspellings
* Improve overall search experience

### Algorithm

**Levenshtein Edit Distance** (Dynamic Programming)
Allowed operations: Insert, Delete, Replace

### Complexity

| Operation                       | Complexity       |
| ------------------------------- | ---------------- |
| Edit distance computation       | **O(m × n)**     |
| Full fuzzy search over products | **O(P × m × n)** |

**Where:** *m* = input text length, *n* = product name length, *P* = total number of products
* Fuzzy search is **only used as a fallback**, not for every keystroke
* Typical *m* and *n* (user input and names) are small (10–20 characters)
* The number of full fuzzy checks is limited because Trie already filters out perfect matches
* Ensures better UX without slowing down normal search flow


## 3. Graph (Adjacency List)

Represents product similarity clusters and category relationships.

### Structure

```cpp
unordered_map<string, vector<string>> adj;
```

### Operations & Complexity

| Operation      | Complexity               |
| -------------- | ------------------------ |
| Add Edge       | **O(1)**                 |
| List Neighbors | **O(k)** (k = neighbors) |

### Algorithms Used

* Simple BFS/DFS-style traversal
* No heavy graph algorithms (e.g., Dijkstra)

### Why This Is Efficient

* Category and similarity graphs are shallow and small
* Adjacency lists allow instant neighbor lookup
* No costly pathfinding is used → keeps operations lightweight


## 4. Vector-Based Cart

A lightweight shopping cart implementation using C++ vectors.

### Operations & Complexity

| Operation     | Complexity |
| ------------- | ---------- |
| Add Item      | **O(1)**   |
| Remove Item   | **O(N)**   |
| Compute Total | **O(N)**   |

* Cart sizes are extremely small (usually < 30 items)
* Linear operations on small lists are effectively instantaneous
* Vectors provide cache-friendly performance and minimal overhead


## 5. Product Class (OOP Module)

Encapsulates all product-related data.

### Attributes

* `id`
* `name`
* `price`
* `description`

### Benefits

* High modularity
* Better maintainability
* Strong type safety
* Clear separation of concerns
* Easily extendable for future product attributes

---

## Summary

This system integrates several optimized algorithms and data structures:

* **Trie** for fast autocomplete (consistent, scalable lookup)
* **Levenshtein fuzzy search** as a smart fallback (used rarely but improves UX)
* **Graph-based models** for lightweight relationships
* **Vector-based cart** for small-size, high-speed operations
* **OOP product abstraction** for clarity and structure

### Overall System Justification

Despite using multiple DSAs, the system remains **high-performance** because:

* Heavy operations are only triggered when necessary
* Input sizes (product names, user text, cart size) are small
* Most structures provide constant or near-constant time operations
* Architecture balances speed, accuracy, and user experience

The result is a **scalable, efficient, and production-ready backend** suitable for real-time applications.
