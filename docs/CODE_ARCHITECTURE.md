# Code Walkthrough

This document is guide to the Smart E-Commerce System key implementations.


## System Overview

**Architecture:**
- **C++ Backend**: Data processing, search algorithms, business logic
- **Python GUI**: Tkinter interface with ttkbootstrap theming
- **Communication**: File-based text protocol (`input.txt` / `output.txt`)


## Backend Components

### 1. Product Management (`product.h/cpp`)

**Data Structure:**
```cpp
struct Product {
    string name, category, brand;
    double price;
    int stock;
}
```

**Key Functions:**
- `loadProducts()`: Parses `products.txt` (format: `Name|Price|Stock|Category|Brand`)
- `getProduct(name)`: Case-insensitive O(1) lookup via hash map
- `getProductsByCategory()`: Filters by category
- `sortProducts()`: Insertion sort with multiple criteria (price, name, stock)
- `applyFilters()`: Price range, category, and brand filtering

### 2. Trie Autocomplete (`trie.h/cpp`)

**Structure:** 26-child tree (a-z), each node has `isEndOfWord` flag and `fullWord` string.

**Operations:**
- `insert(word)`: O(m) - adds product name
- `autocomplete(prefix)`: O(k) - returns all matching products
- Recursive DFS collection of suggestions

**Example:** User types "app" → Returns ["Apple iPhone 15", "Apple MacBook Air M3", ...]

### 3. Shopping Cart (`cart.h/cpp`)

**Structure:**
```cpp
struct CartItem {
    Product* product;
    int quantity;
}
```

**Operations:**
- `addItem()`: Validates stock, updates quantity, persists to file
- `checkout()`: Verifies stock, deducts inventory, clears cart
- `saveToFile()` / `loadFromFile()`: Persists cart state to `cart_data.txt`

### 4. Recommendation Graph (`graph.h/cpp`)

**Structure:** Adjacency list storing product relationships.

**Operations:**
- `addEdge(p1, p2)`: Creates bidirectional relationship
- `getRecommendations(product)`: Returns up to 5 related products

### 5. Fuzzy Search (`main.cpp`)

**Algorithm:** Levenshtein distance (edit distance) with dynamic programming.

**Logic:**
- Splits product names into words
- Calculates edit distance for each word vs. query
- Matches if distance ≤ 2 or exact substring match

**Example:** "samsng" matches "Samsung" (distance = 1)

### 6. Command Processor (`main.cpp`)

**Protocol:**

| Command | Description |
|---------|-------------|
| `AUTOCOMP <prefix>` | Autocomplete suggestions |
| `SEARCH <query>` | Fuzzy search all products |
| `SEARCHCAT <cat> <query>` | Search within category |
| `LISTCAT <category>` | List category products |
| `LISTALLFILTER <filters>` | Apply filters (format: `min_price=X;brand=Y`) |
| `SORT <type> [category]` | Sort by price/name/stock |
| `ADD <product> <qty>` | Add to cart |
| `SHOWCART` | Display cart |
| `CHECKOUT` | Process order |
| `RECOMMEND <product>` | Get recommendations |

**Flow:** Read `input.txt` → Process command → Write `output.txt`


## Frontend Components

### 1. Backend Interface (`backend_interface.py`)

**Purpose:** Manages C++ subprocess communication.

**Process:**
1. Write command to `input.txt`
2. Execute C++ binary via `subprocess.run()`
3. Parse `output.txt` response
4. Return structured data to UI

**Output Parsers:** Separate methods for autocomplete, search, cart, recommendations, etc.

### 2. Main Application (`app.py`)

**UI Layout:**
```
┌─────────────────────────────────────────┐
│ Top Frame (Search, Filters, Sort)       │
├──────────┬──────────────────────────────┤
│ Sidebar  │ Content Frame                │
│          │ (Treeview Product Display)   │
│ - All    │                              │ 
│ - Books  │                              │
│ - Elec   │                              │
│ - Cart   │                              │
│ - Recs   │                              │
├──────────┴──────────────────────────────┤
│ Status Bar                              │
└─────────────────────────────────────────┘
```

**Key Features:**
- **ThemedFactory:** Provides consistent widgets with ttkbootstrap fallback
- **Autocomplete:** Real-time Trie-based suggestions dropdown
- **Filters:** Dialog for price range and brand selection
- **Cart Window:** Separate window for cart management
- **Recommendation Window:** Displays related products


## Data Flow Examples

### Search Flow
```
User types → update_autocomplete() → AUTOCOMP command → Trie search
→ Suggestions dropdown → User selects → SEARCH command → Display results
```

### Add to Cart Flow
```
Select product + quantity → ADD command → Validate stock → Update cart
→ Save to file → Update cart badge → Success dialog
```

### Checkout Flow
```
View Cart → SHOWCART command → Display CartWindow → Checkout button
→ CHECKOUT command → Verify stock → Update inventory → Clear cart
→ Confirmation dialog
```


## File Structure

```
backend_cpp/
├── product.h/cpp      # Product management, filtering, sorting
├── cart.h/cpp         # Shopping cart operations
├── trie.h/cpp         # Autocomplete search
├── graph.h/cpp        # Recommendation system
├── main.cpp           # Command processor, fuzzy search
├── products.txt       # Product database
└── cart_data.txt      # Persistent cart storage

gui_python/
├── app.py                 # Main UI application
├── backend_interface.py   # Backend communication
└── assets/                # UI resources
```


## Performance & Complexity

- **Trie Search:** O(m) where m = prefix length
- **Product Lookup:** O(1) average (hash map)
- **Insertion Sort:** O(n²) worst case
- **Graph Recommendations:** O(k) where k = related products
- **Fuzzy Search:** O(n×m×k) where n = products, m = query length, k = word length


## Error Handling

**Backend:** File errors, invalid commands, stock issues (prefixed with "ERROR:")  
**Frontend:** Subprocess timeouts (5s), parse errors, user-friendly dialogs
