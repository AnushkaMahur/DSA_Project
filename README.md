# Smart E-Commerce System

<p align="center">
  <img src="https://img.shields.io/badge/Python-3.x-blue">
  <img src="https://img.shields.io/badge/C++-17-red">
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey">
  <img src="https://img.shields.io/badge/Status-Active-success">
</p>


A full desktop-based hybrid e-commerce application built by combining C++ (high-performance backend) with a Python GUI (Tkinter).
The system handles core e-commerce operations such as product browsing, searching, filtering, and cart management.
The application demonstrates practical usage of core Data Structures and Algorithms by implementing:

- Fast product search using Trie

- Fuzzy search using Levenstein Distance for typo-tolerant search

- Product graph relationships for category grouping, item association and recommendation system

- Clean OOP design for product and cart management

- Price-based filtering and sorting, allowing users to sort products by ascending/descending price and quickly view items within a specific price range

The backend efficiently handles all computational and data-intensive operations, while the GUI delivers a responsive, user-friendly interface for browsing, searching, and interacting with products.

## Tech Stack
- Backend: C++
- Frontend: Python (Tkinter GUI)
- Data Storage: Text-based file handling
- Interfacing: Python ↔ C++ executable communication

## Prerequisites
To run this project locally, you need to have the following software installed:

- Python 3.x
- GCC / MinGW or any C++ compiler (only if rebuilding backend)
- Git
- **ttkbootstrap** (optional ,for enhanced UI themes - automatically falls back to standard ttk if not available)


## Features

C++ Backend
- Product management using OOP
- Trie-based autocomplete system
- Fuzzy search (Levenshtein distance)
- Graph-based product relationship mapping
- Shopping cart implementation
- File-based product database

Python GUI
- Modern, clean interface using the **Flatly theme** (via ttkbootstrap)
- Automatic fallback to standard ttk theme if ttkbootstrap is not installed
- Tkinter UI for user-friendly interface
- Search with autocomplete + fuzzy matching
- Product display and selection
- Cart management
- Communication with the C++ backend

## Getting Started
**Clone the repository:**

   ```
   git clone https://github.com/AnushkaMahur/DSA_Project.git
   cd smart-ecommerce-system
   ```

## Backend Setup (C++)
The backend contains the core logic:

  - Product storage
  - Searching, sorting, filtering
  - Cart operations
  - File handling

**Build:**\
**Compile the backend:**
  ```
  cd src/backend_cpp
  g++ -std=c++17 *.cpp -o ecommerce.exe
  ```

## Frontend Setup:

**Navigate to the GUI directory:**

  ```
  cd src/gui_python
  ```

**Install required Python modules**

Most Tkinter installations are bundled with Python.\
If using virtual environment:

  ```
  pip install tk
  ```
Install Libraries:

  ```
  pip install ttkbootstrap
  ```

**Run the application:**

   ```
   python app.py
   ```

The GUI will launch and automatically communicate with the C++ backend.


## Application Properties

The backend expects product data in a `.txt` file.
To change file paths or customize dataset:
1.	Go to src\backend_cpp\products.txt
2.	Update input/output file paths as needed.
   
**Example:**

  ```
  products.txt  
  ```
Save the file and recompile if changed.

## Accessing the Application
Once running, the application window appears locally at:
-  Desktop Tkinter Window (no browser needed)
You can browse products, add items to cart, apply filters, search, and place orders.

## Project Structure


Smart E-Commerce System\
|\
|___assets/\
|\
|___docs/\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___CODE_WALKTHROUGH.md\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___ARCHITECTURE.md\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___DATA_STRUCTURES_AND_ALGORITHMS.md\
|\
|___src/\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___backend_cpp/\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___product.h\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___cart.h\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___trie.h\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___graph.h\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___product.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___cart.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___trie.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___graph.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___main.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___products.txt\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___gui_python/\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___app.py\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___backend_interface.py\
|\
|\
|___tests/\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___tests_cpp/\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___test_cart.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___test_graph.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___test_trie.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;| ___test_python/\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___test_app_logic.py\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___test_backend_interface.py\
|____.gitignore\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\
|____README.md\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\
|____Report.pdf\


##  License
This project is part of a university course. You may reuse portions with proper acknowledgment.



