# Smart E-Commerce System

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
- Node.js
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
1. Clone the repository:

   ```shell
   git clone https://github.com/your-username/smart-ecommerce-system.git
    cd smart-ecommerce-system
   ```

2. Backend Setup (C++)
The backend contains the core logic:

  - Product storage
  - Searching, sorting, filtering
  - Cart operations
  - File handling

**Build:**\
Compile the backend:
  ```shell
  cd backend_cpp
  g++ -std=c++17 *.cpp -o ecommerce.exe
  ```

3. Frontend Setup:

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

The backend expects product data in a .txt file.
To change file paths or customize dataset:
1.	Open the config section inside ecommerce.cpp.
2.	Update input/output file paths as needed.\
   
**Example:**

  ```
    products.txt  
    orders.txt
  ```
Save the file and recompile if changed.

## Accessing the Application
Once running, the application window appears locally at:
-  Desktop Tkinter Window (no browser needed)
You can browse products, add items to cart, apply filters, search, and place orders.

## Project Structure

##  License
This project is part of a university course. You may reuse portions with proper acknowledgment.



