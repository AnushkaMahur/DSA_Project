# Code Walkthrough

This document provides a detailed walkthrough of the Smart E-Commerce System codebase, explaining how each component works and how they interact with each other.

---

## Table of Contents
1. [System Overview](#system-overview)
2. [Backend (C++)](#backend-c)
3. [Frontend (Python)](#frontend-python)
4. [Data Flow](#data-flow)
5. [Key Features Implementation](#key-features-implementation)

---

## System Overview

The Smart E-Commerce System is a hybrid desktop application that combines:
- **C++ Backend**: High-performance data processing, search algorithms, and business logic
- **Python GUI**: User-friendly Tkinter interface with modern theming
- **File-based Communication**: Simple text-based protocol for frontend-backend interaction

---

## Backend (C++)

### 1. Product Management (`product.h` / `product.cpp`)

**Core Data Structure:**
```cpp
struct Product {
    string name;
    double price;
    int stock;
    string category;
    string brand;
}
```

**Key Functions:**

- **`loadProducts(filename)`**: Reads product data from `products.txt` file
  - Parses pipe-delimited format: `Name|Price|Stock|Category|Brand`
  - Stores products in an `unordered_map` with lowercase names as keys for fast lookups
  - Handles comments (lines starting with `#`) and empty lines

- **`getProduct(name)`**: Case-insensitive product lookup
  - Converts name to lowercase for consistent matching
  - Returns pointer to product or `nullptr` if not found

- **`getAllProducts()`**: Returns vector of all products for display

- **`getProductsByCategory(category)`**: Filters products by category
  - Case-insensitive category matching
  - Used for category browsing in the UI

- **`updateStock(name, quantity)`**: Updates product stock after checkout
  - Prevents negative stock values
  - Returns `true` on success, `false` on failure

- **`sortProducts(list, type)`**: Implements **Insertion Sort** algorithm
  - Supports multiple sort criteria:
    - `SORT_PRICE_ASC`: Price (Low → High)
    - `SORT_PRICE_DESC`: Price (High → Low)
    - `SORT_NAME_ASC`: Name (A → Z)
    - `SORT_STOCK_DESC`: Stock (High → Low)
  - Uses lambda function for flexible comparison logic

- **`applyFilters(input, filters)`**: Filters products based on criteria
  - Price range filtering (min/max)
  - Category filtering
  - Brand filtering
  - Returns filtered product vector

---

### 2. Trie-Based Autocomplete (`trie.h` / `trie.cpp`)

**Data Structure:**
```cpp
class TrieNode {
    TrieNode* children[26];  // One pointer for each letter a-z
    bool isEndOfWord;
    string fullWord;
}
```

**How It Works:**

The Trie is a tree-like data structure where each node represents a character. It enables efficient prefix-based searching with **O(m)** time complexity, where m is the length of the search prefix.

**Key Functions:**

- **`insert(word)`**: Adds a product name to the Trie
  - Converts word to lowercase
  - Creates nodes for each character
  - Marks end-of-word nodes and stores the full product name

- **`search(word)`**: Checks if exact word exists in Trie
  - Traverses character by character
  - Returns `true` only if path exists and ends at a word marker

- **`autocomplete(prefix)`**: Returns all products starting with prefix
  - Navigates to the prefix node
  - Recursively collects all words reachable from that node
  - Returns empty vector if prefix not found

- **`collectWords(node, results)`**: Recursive helper function
  - Performs depth-first traversal from given node
  - Adds complete words to results vector

**Example:**
```
User types: "app"
Trie finds: ["Apple iPhone 15", "Apple MacBook Air M3", "Apple iPad Pro 12.9", ...]
```

---

### 3. Shopping Cart (`cart.h` / `cart.cpp`)

**Data Structure:**
```cpp
struct CartItem {
    Product* product;  // Pointer to product
    int quantity;      // Number of items
}
```

**Key Functions:**

- **`addItem(product, quantity)`**: Adds items to cart
  - Checks if sufficient stock is available
  - If product already in cart, updates quantity
  - Otherwise, creates new cart item
  - Calls `saveToFile()` to persist cart state

- **`removeItem(productName)`**: Removes product from cart
  - Case-insensitive name matching
  - Updates cart file after removal

- **`showCart()`**: Displays cart contents
  - Prints formatted output: `Name|Quantity|Price|Subtotal`
  - Calculates and displays total price

- **`getTotal()`**: Calculates total cart value
  - Sums up: `quantity × price` for all items

- **`checkout(productManager)`**: Processes order
  - Verifies stock availability for all items
  - Deducts quantities from product stock
  - Saves updated product data to file
  - Clears cart after successful checkout

- **`saveToFile()`**: Persists cart to `cart_data.txt`
  - Format: `ProductName|Quantity`
  - Dynamically determines file path using `getCartFilePath()`

- **`loadFromFile()`**: Restores cart from file
  - Reads saved cart data on application startup
  - Reconstructs cart items using product manager

**File Path Logic:**
```cpp
string getCartFilePath() {
    // Gets current working directory
    // If in gui_python folder, navigates to backend_cpp
    // Returns full path to cart_data.txt
}
```

---

### 4. Recommendation Graph (`graph.h` / `graph.cpp`)

**Data Structure:**
```cpp
class RecommendationGraph {
    map<string, vector<string>> adjacencyList;
}
```

The graph stores product relationships as an **adjacency list**, where each product maps to a list of related products.

**Key Functions:**

- **`addEdge(product1, product2)`**: Creates bidirectional relationship
  - Converts both names to lowercase
  - Adds each product to the other's adjacency list
  - Enables symmetric recommendations (if A relates to B, then B relates to A)

- **`getRecommendations(productName, maxResults=5)`**: Returns related products
  - Finds all directly connected products (neighbors in graph)
  - Uses a `set` to avoid duplicate recommendations
  - Limits results to specified maximum

**Graph Building:**

In `main.cpp`, the graph is populated with product relationships:
```cpp
recommendGraph.addEdge("Apple iPhone 15", "Apple MacBook Air M3");
recommendGraph.addEdge("Apple iPhone 15", "Apple AirPods Pro 2");
// ... hundreds of relationships
```

**Example:**
```
User selects: "Apple iPhone 15"
Recommendations: ["Apple MacBook Air M3", "Apple AirPods Pro 2", "Apple Watch Series 9", ...]
```

---

### 5. Fuzzy Search Implementation (`main.cpp`)

**Levenshtein Distance Algorithm:**

```cpp
int editDistance(const string &a, const string &b) {
    // Dynamic programming approach
    // dp[i][j] = minimum edits to convert a[0..i] to b[0..j]
    
    // Operations: insert, delete, substitute
    // Returns minimum number of edits needed
}
```

**How Fuzzy Search Works:**

1. User enters a query (e.g., "iphne" - typo for "iphone")
2. System splits product names into individual words
3. Calculates edit distance between query and each word
4. If distance ≤ 2, considers it a match
5. Returns all matching products

**Search Function:**
```cpp
void searchCategoryProducts(const string &category, const string &query) {
    // For each product in category:
    //   Split product name into words
    //   Check if any word has edit distance ≤ 2 from query
    //   Also check for exact substring matches
    //   Add matching products to results
}
```

**Example:**
```
Query: "samsng" (typo)
Matches: "Samsung Galaxy S24", "Samsung Galaxy Tab S9", ...
Edit distance between "samsng" and "samsung" = 1 (missing 'u')
```

---

### 6. Main Command Processor (`main.cpp`)

**System Initialization:**

```cpp
void initializeSystem() {
    setWorkingDirectory();           // Navigate to correct folder
    productManager.loadProducts();   // Load product database
    cart.loadFromFile();             // Restore cart state
    
    // Build Trie for autocomplete
    for (auto &product : allProducts) {
        searchTrie.insert(product.name);
    }
    
    // Build recommendation graph
    // (hundreds of addEdge calls)
}
```

**Command Processing:**

The system uses a text-based command protocol:

| Command | Description | Example |
|---------|-------------|---------|
| `AUTOCOMP <prefix>` | Get autocomplete suggestions | `AUTOCOMP app` |
| `SEARCH <query>` | Search all products | `SEARCH laptop` |
| `SEARCHCAT <cat> <query>` | Search within category | `SEARCHCAT Electronics phone` |
| `LISTCAT <category>` | List category products | `LISTCAT Books` |
| `LISTALL` | List all products | `LISTALL` |
| `LISTALLFILTER <filters>` | Apply filters | `LISTALLFILTER min_price=5000;max_price=10000` |
| `SORT <type> [category]` | Sort products | `SORT PRICE_ASC Electronics` |
| `ADD <product> <qty>` | Add to cart | `ADD Apple iPhone 15 2` |
| `REMOVE <product>` | Remove from cart | `REMOVE Apple iPhone 15` |
| `SHOWCART` | Display cart | `SHOWCART` |
| `CHECKOUT` | Process order | `CHECKOUT` |
| `RECOMMEND <product>` | Get recommendations | `RECOMMEND Apple iPhone 15` |

**Command Flow:**
1. Read command from `input.txt`
2. Parse command and extract parameters
3. Execute corresponding function
4. Write formatted output to `output.txt`
5. Python reads output and updates UI

---

## Frontend (Python)

### 1. Backend Interface (`backend_interface.py`)

**Purpose:** Bridges Python GUI and C++ backend

**Key Class:**
```python
class BackendInterface:
    def __init__(self, cpp_executable, input_file, output_file)
    def execute_command(self, command)
    def parse_output(self, output, command)
```

**Execution Flow:**

1. **Write Command**: Python writes command to `input.txt`
```python
with open(self.input_file, 'w') as f:
    f.write(command + '\n')
```

2. **Run Backend**: Execute C++ program
```python
subprocess.run([self.cpp_executable], timeout=5)
```

3. **Read Output**: Parse results from `output.txt`
```python
with open(self.output_file, 'r') as f:
    output = f.read()
return self.parse_output(output, command)
```

**Output Parsers:**

- `_parse_autocomp()`: Extracts autocomplete suggestions
- `_parse_search_results()`: Parses product search results
- `_parse_product_list()`: Handles category listings
- `_parse_cart()`: Extracts cart items and total
- `_parse_checkout()`: Processes checkout response
- `_parse_recommendations()`: Gets related products
- `_parse_sorted_results()`: Handles sorted product lists

**Example Response:**
```python
{
    "products": [
        {"name": "Apple iPhone 15", "price": 79999, "stock": 45, "category": "Electronics"},
        {"name": "Samsung Galaxy S24", "price": 74999, "stock": 60, "category": "Electronics"}
    ]
}
```

---

### 2. Main Application (`app.py`)

**Theme Management:**

The app uses **ttkbootstrap** for modern themes with automatic fallback:

```python
USE_BOOTSTRAP = False
try:
    import ttkbootstrap as tb
    USE_BOOTSTRAP = True
except:
    USE_BOOTSTRAP = False  # Fall back to standard ttk
```

**ThemedFactory Class:**

Provides consistent widget creation regardless of theme library:

```python
class ThemedFactory:
    def button(self, parent, text, command, bootstyle=None, style=None):
        if USE_BOOTSTRAP:
            return ttk.Button(..., bootstyle=bootstyle)
        else:
            return ttk.Button(..., style=style)
```

**UI Layout Structure:**

```
┌─────────────────────────────────────────────────────┐
│ Top Frame (Search, Filters, Sort)                  │
├──────────┬──────────────────────────────────────────┤
│          │                                          │
│ Sidebar  │  Content Frame                          │
│          │  (Product Display Area)                 │
│ - All    │                                          │
│ - Books  │                                          │
│ - Elec   │                                          │
│ - Games  │                                          │
│ - ...    │                                          │
│          │                                          │
│ - Cart   │                                          │
│ - Recs   │                                          │
├──────────┴──────────────────────────────────────────┤
│ Status Bar                                          │
└─────────────────────────────────────────────────────┘
```

---

### 3. Key UI Features

**Autocomplete Search:**

```python
def update_autocomplete(self, event):
    query = self.search_entry.get().strip()
    result = self.backend.execute_command(f"AUTOCOMP {query}")
    suggestions = result.get("suggestions", [])
    self.show_suggestions(suggestions)  # Display dropdown
```

**Dynamic Product Display:**

```python
def load_category_products(self, category):
    # Clear existing content
    self.clear_content()
    
    # Create Treeview table
    columns = ("Name", "Price", "Stock")
    self.products_tree = ttk.Treeview(...)
    
    # Load products from backend
    result = self.backend.execute_command(f"LISTCAT {category}")
    self.display_products(result.get("products", []))
```

**Filter System:**

```python
def build_filter_string(self):
    filters = []
    if self.filter_min_price.get():
        filters.append(f"min_price={self.filter_min_price.get()}")
    if self.filter_max_price.get():
        filters.append(f"max_price={self.filter_max_price.get()}")
    if self.filter_brand.get():
        filters.append(f"brand={self.filter_brand.get()}")
    if self.current_category:
        filters.append(f"category={self.current_category}")
    return ";".join(filters)  # Format: "min_price=5000;max_price=10000;brand=Apple"
```

**Cart Management:**

```python
def add_to_cart(self):
    selected = self.products_tree.selection()
    item = self.products_tree.item(selected[0])
    name = item["values"][0]
    qty = self.quantity_var.get()
    
    result = self.backend.execute_command(f"ADD {name} {qty}")
    
    if result.get("success"):
        self.update_cart_button()  # Update cart count
```

---

### 4. Window Classes

**CartWindow:**
- Displays cart items in table format
- Shows product name, quantity, price, subtotal
- Provides remove and checkout buttons
- Calculates and displays total amount

**RecommendationWindow:**
- Shows related products for selected item
- Displays product names and prices
- Uses graph-based recommendation system

---

## Data Flow

### 1. Product Search Flow

```
User types in search box
    ↓
update_autocomplete() triggered
    ↓
Backend: AUTOCOMP command → Trie search
    ↓
Returns suggestions list
    ↓
Display dropdown with matches
    ↓
User selects suggestion
    ↓
Backend: SEARCH command → Fuzzy match
    ↓
Returns matching products
    ↓
Display in Treeview table
```

### 2. Add to Cart Flow

```
User selects product + quantity
    ↓
Click "Add to Cart"
    ↓
Backend: ADD command
    ↓
Cart: Check stock availability
    ↓
Add/Update cart item
    ↓
Save cart to file
    ↓
Return success message
    ↓
Update cart button count
    ↓
Show success dialog
```

### 3. Checkout Flow

```
User clicks "View Cart"
    ↓
Backend: SHOWCART command
    ↓
Display CartWindow with items
    ↓
User clicks "Checkout"
    ↓
Backend: CHECKOUT command
    ↓
Cart: Verify stock for all items
    ↓
Update product stock quantities
    ↓
Save products.txt
    ↓
Clear cart
    ↓
Save cart_data.txt
    ↓
Return success + total
    ↓
Show confirmation dialog
    ↓
Close cart window
```

---

## Key Features Implementation

### 1. Category-Based Browsing

**Implementation:**
- Sidebar buttons for each category
- Click triggers `load_category_products(category)`
- Backend filters products by category
- Display in dedicated Treeview

### 2. Price Filtering

**Implementation:**
- Filter dialog with min/max price inputs
- Builds filter string: `min_price=X;max_price=Y`
- Backend applies filters in `applyFilters()` function
- Returns only products within price range

### 3. Sorting

**Implementation:**
- Dropdown with sort options
- Maps UI choice to backend sort type
- Backend uses insertion sort algorithm
- Returns sorted product list
- Display in same Treeview format

### 4. Fuzzy Search

**Implementation:**
- User enters query (may contain typos)
- Backend splits product names into words
- Calculates Levenshtein distance for each word
- Matches if distance ≤ 2 or exact substring match
- Returns all matching products

### 5. Product Recommendations

**Implementation:**
- User selects product
- Backend looks up product in graph
- Returns all connected (related) products
- Display in RecommendationWindow
- Based on co-purchase patterns

### 6. Persistent Cart

**Implementation:**
- Cart automatically saves to `cart_data.txt` after changes
- Loads cart on application startup
- Maintains cart state across sessions
- Updates after checkout

---

## File Structure Summary

```
backend_cpp/
├── product.h/cpp          # Product management, filtering, sorting
├── cart.h/cpp             # Shopping cart operations
├── trie.h/cpp             # Autocomplete search
├── graph.h/cpp            # Recommendation system
├── main.cpp               # Command processor, fuzzy search
├── products.txt           # Product database
└── cart_data.txt          # Persistent cart storage

gui_python/
├── app.py                 # Main UI application
├── backend_interface.py   # C++ backend communication
└── assets/                # UI resources
```

---

## Performance Considerations

1. **Trie Search**: O(m) time complexity where m = prefix length
2. **Hash Map Lookups**: O(1) average case for product retrieval
3. **Insertion Sort**: O(n²) worst case, but efficient for small datasets
4. **Graph Recommendations**: O(k) where k = number of related products
5. **Fuzzy Search**: O(n×m×k) where n = products, m = query length, k = word length

---

## Error Handling

**Backend:**
- File not found errors
- Invalid command formats
- Stock unavailability
- All errors prefixed with "ERROR:"

**Frontend:**
- Subprocess timeout (5 seconds)
- File I/O exceptions
- Parse errors with fallback to raw output
- User-friendly error dialogs

---

This walkthrough provides a comprehensive understanding of how the Smart E-Commerce System works, from low-level algorithms to high-level user interactions.
