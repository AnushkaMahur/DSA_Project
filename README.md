# Smart E-Commerce System

<p align="center">
  <img src="https://img.shields.io/badge/Python-3.x-blue">
  <img src="https://img.shields.io/badge/C++-17-red">
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey">
  <img src="https://img.shields.io/badge/Status-Active-success">
</p>


A full desktop-based e-commerce application built using C++ (backend) and Python Tkinter (frontend).
The system handles core e-commerce operations such as product browsing, searching, filtering, and cart management, and demonstrates practical application of Data Structures & Algorithms.

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

## Features
-  Product browsing with category-based filtering
-  Keyword search and sorting (price, rating, etc.)
-  Hash-based fast lookup using product IDs
-  Shopping cart management
-  Persistent storage using text files
-  Smooth interaction between Python GUI and C++ backend
-  Modular, DSA-focused implementation

## Getting Started
**Clone the repository:**

   ```
      git clone https://github.com/your-username/smart-ecommerce-system.git
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
     cd backend_cpp
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


src/\
|\
|___backend_cpp/\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___cart.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___cart.h\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___graph.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___graph.h\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___main.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___product.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___product.h\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___products.txt\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___trie.cpp\
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___trie.h\
|\
|\
|___gui_python/\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___app.py\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|___backend_interface.py
##  License
This project is part of a university course. You may reuse portions with proper acknowledgment.



