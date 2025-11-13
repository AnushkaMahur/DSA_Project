#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <direct.h>
#include "product.h"
#include "trie.h"
#include "cart.h"
#include "graph.h"

using namespace std;

ProductManager productManager;
Trie searchTrie;
ShoppingCart cart;
RecommendationGraph recommendGraph;

void setWorkingDirectory() {
    char buffer[1024];
    _getcwd(buffer, sizeof(buffer)); // Get current working directory
    string currentPath = string(buffer);

    size_t pos = currentPath.find("gui_python");
    if (pos != string::npos) {
        string backendPath = currentPath.substr(0, pos) + "backend_cpp";
        _chdir(backendPath.c_str());
    }
}

static inline string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

ProductFilters parseFilterString(const string &s) {
    ProductFilters f;
    if (s.empty()) return f;
    stringstream ss(s);
    string piece;
    while (getline(ss, piece, ';')) {
        if (piece.empty()) continue;
        size_t eq = piece.find('=');
        if (eq == string::npos) continue;
        string key = trim(piece.substr(0, eq));
        string val = trim(piece.substr(eq + 1));
        if (key == "min_price") {
            try { f.min_price = stod(val); } catch(...) { f.min_price = -1.0; }
        } else if (key == "max_price") {
            try { f.max_price = stod(val); } catch(...) { f.max_price = -1.0; }
        } else if (key == "min_rating") {
            try { f.min_rating = stod(val); } catch(...) { f.min_rating = -1.0; }
        } else if (key == "brand" || key == "brands") {
            string tmp;
            stringstream ss2(val);
            while (getline(ss2, tmp, ',')) {
                string br = trim(tmp);
                if (!br.empty()) f.brands.push_back(br);
            }
        } else if (key == "category") {
            f.category = val;
        }
    }
    return f;
}

void initializeSystem() {
    setWorkingDirectory();
    productManager.loadProducts("products.txt");
    cart.loadFromFile();

    vector<Product> allProducts = productManager.getAllProducts();
    for (const auto &product : allProducts) {
        searchTrie.insert(product.name);
    }

    recommendGraph.addEdge("Apple iPhone 15", "Apple MacBook Air M3");
    recommendGraph.addEdge("Apple iPhone 15", "Apple iPad Pro 12.9");
    recommendGraph.addEdge("Apple iPhone 15", "Apple Watch Series 9");
    recommendGraph.addEdge("Apple iPhone 15", "Apple AirTag 4-Pack");
    recommendGraph.addEdge("Apple iPhone 15", "Apple AirPods Pro 2");
    recommendGraph.addEdge("Apple iPhone 15", "Apple Watch Ultra 2");
    recommendGraph.addEdge("Apple iPhone 15", "Apple MagSafe Charger");
    recommendGraph.addEdge("Apple iPhone 15", "Samsung 45W Charger");
    recommendGraph.addEdge("Apple iPhone 15", "Boat Type-C Cable");
    recommendGraph.addEdge("Apple iPhone 15", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Apple iPhone 15", "OtterBox Defender Case");
    recommendGraph.addEdge("Apple iPhone 15", "HP USB-C Dock");
    recommendGraph.addEdge("Apple iPhone 15", "Anker Wireless Charger");
    recommendGraph.addEdge("Apple iPhone 15", "Belkin HDMI Cable");
    recommendGraph.addEdge("Apple iPhone 15", "JBL Audio Cable");
    recommendGraph.addEdge("Apple iPhone 15", "Apple Pencil 2");
    recommendGraph.addEdge("Apple iPhone 15", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Apple iPhone 15", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Apple iPhone 15", "Baseus Car Charger");
    recommendGraph.addEdge("Apple iPhone 15", "Ugreen USB Adapter");
    recommendGraph.addEdge("Apple iPhone 15", "Apple Lightning Cable");
    recommendGraph.addEdge("Samsung Galaxy S24", "Samsung Galaxy Tab S9");
    recommendGraph.addEdge("Samsung Galaxy S24", "Samsung Galaxy Watch 6");
    recommendGraph.addEdge("Samsung Galaxy S24", "Samsung Galaxy Fit 3");
    recommendGraph.addEdge("Samsung Galaxy S24", "Samsung 340L Refrigerator");
    recommendGraph.addEdge("Samsung Galaxy S24", "Samsung Microwave 28L");
    recommendGraph.addEdge("Samsung Galaxy S24", "Apple MagSafe Charger");
    recommendGraph.addEdge("Samsung Galaxy S24", "Samsung 45W Charger");
    recommendGraph.addEdge("Samsung Galaxy S24", "Boat Type-C Cable");
    recommendGraph.addEdge("Samsung Galaxy S24", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Samsung Galaxy S24", "OtterBox Defender Case");
    recommendGraph.addEdge("Samsung Galaxy S24", "HP USB-C Dock");
    recommendGraph.addEdge("Samsung Galaxy S24", "Anker Wireless Charger");
    recommendGraph.addEdge("Samsung Galaxy S24", "Belkin HDMI Cable");
    recommendGraph.addEdge("Samsung Galaxy S24", "JBL Audio Cable");
    recommendGraph.addEdge("Samsung Galaxy S24", "Apple Pencil 2");
    recommendGraph.addEdge("Samsung Galaxy S24", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Samsung Galaxy S24", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Samsung Galaxy S24", "Baseus Car Charger");
    recommendGraph.addEdge("Samsung Galaxy S24", "Ugreen USB Adapter");
    recommendGraph.addEdge("Samsung Galaxy S24", "Apple Lightning Cable");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Apple MagSafe Charger");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Samsung 45W Charger");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Boat Type-C Cable");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Google Pixel 9 Pro", "OtterBox Defender Case");
    recommendGraph.addEdge("Google Pixel 9 Pro", "HP USB-C Dock");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Anker Wireless Charger");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Belkin HDMI Cable");
    recommendGraph.addEdge("Google Pixel 9 Pro", "JBL Audio Cable");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Apple Pencil 2");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Google Pixel 9 Pro", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Baseus Car Charger");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Ugreen USB Adapter");
    recommendGraph.addEdge("Google Pixel 9 Pro", "Apple Lightning Cable");
    recommendGraph.addEdge("OnePlus 12", "Apple MagSafe Charger");
    recommendGraph.addEdge("OnePlus 12", "Samsung 45W Charger");
    recommendGraph.addEdge("OnePlus 12", "Boat Type-C Cable");
    recommendGraph.addEdge("OnePlus 12", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("OnePlus 12", "OtterBox Defender Case");
    recommendGraph.addEdge("OnePlus 12", "HP USB-C Dock");
    recommendGraph.addEdge("OnePlus 12", "Anker Wireless Charger");
    recommendGraph.addEdge("OnePlus 12", "Belkin HDMI Cable");
    recommendGraph.addEdge("OnePlus 12", "JBL Audio Cable");
    recommendGraph.addEdge("OnePlus 12", "Apple Pencil 2");
    recommendGraph.addEdge("OnePlus 12", "Samsung Stylus S Pen");
    recommendGraph.addEdge("OnePlus 12", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("OnePlus 12", "Baseus Car Charger");
    recommendGraph.addEdge("OnePlus 12", "Ugreen USB Adapter");
    recommendGraph.addEdge("OnePlus 12", "Apple Lightning Cable");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Apple MagSafe Charger");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Samsung 45W Charger");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Boat Type-C Cable");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Xiaomi 14 Pro", "OtterBox Defender Case");
    recommendGraph.addEdge("Xiaomi 14 Pro", "HP USB-C Dock");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Anker Wireless Charger");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Belkin HDMI Cable");
    recommendGraph.addEdge("Xiaomi 14 Pro", "JBL Audio Cable");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Apple Pencil 2");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Xiaomi 14 Pro", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Baseus Car Charger");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Ugreen USB Adapter");
    recommendGraph.addEdge("Xiaomi 14 Pro", "Apple Lightning Cable");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Sony WH-1000XM5");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Sony WF-1000XM5");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Sony XB13 Portable Speaker");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Sony PlayStation 5");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Apple MagSafe Charger");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Samsung 45W Charger");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Boat Type-C Cable");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "OtterBox Defender Case");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "HP USB-C Dock");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Anker Wireless Charger");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Belkin HDMI Cable");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "JBL Audio Cable");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Apple Pencil 2");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Baseus Car Charger");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Ugreen USB Adapter");
    recommendGraph.addEdge("Sony Bravia 55-inch 4K TV", "Apple Lightning Cable");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Apple MagSafe Charger");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Samsung 45W Charger");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Boat Type-C Cable");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "OtterBox Defender Case");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "HP USB-C Dock");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Anker Wireless Charger");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Belkin HDMI Cable");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "JBL Audio Cable");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Apple Pencil 2");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Samsung Stylus S Pen");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Baseus Car Charger");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Ugreen USB Adapter");
    recommendGraph.addEdge("LG OLED C3 65-inch TV", "Apple Lightning Cable");
    recommendGraph.addEdge("Dell XPS 13", "Dell Inspiron 15");
    recommendGraph.addEdge("Dell XPS 13", "Apple MagSafe Charger");
    recommendGraph.addEdge("Dell XPS 13", "Samsung 45W Charger");
    recommendGraph.addEdge("Dell XPS 13", "Boat Type-C Cable");
    recommendGraph.addEdge("Dell XPS 13", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Dell XPS 13", "OtterBox Defender Case");
    recommendGraph.addEdge("Dell XPS 13", "HP USB-C Dock");
    recommendGraph.addEdge("Dell XPS 13", "Anker Wireless Charger");
    recommendGraph.addEdge("Dell XPS 13", "Belkin HDMI Cable");
    recommendGraph.addEdge("Dell XPS 13", "JBL Audio Cable");
    recommendGraph.addEdge("Dell XPS 13", "Apple Pencil 2");
    recommendGraph.addEdge("Dell XPS 13", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Dell XPS 13", "Dell Laptop Sleeve 15");
    recommendGraph.addEdge("Dell XPS 13", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Dell XPS 13", "Baseus Car Charger");
    recommendGraph.addEdge("Dell XPS 13", "Ugreen USB Adapter");
    recommendGraph.addEdge("Dell XPS 13", "Apple Lightning Cable");
    recommendGraph.addEdge("HP Spectre x360", "HP Envy 13");
    recommendGraph.addEdge("HP Spectre x360", "Dyson Air Purifier HP07");
    recommendGraph.addEdge("HP Spectre x360", "Apple MagSafe Charger");
    recommendGraph.addEdge("HP Spectre x360", "Samsung 45W Charger");
    recommendGraph.addEdge("HP Spectre x360", "Boat Type-C Cable");
    recommendGraph.addEdge("HP Spectre x360", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("HP Spectre x360", "OtterBox Defender Case");
    recommendGraph.addEdge("HP Spectre x360", "HP USB-C Dock");
    recommendGraph.addEdge("HP Spectre x360", "Anker Wireless Charger");
    recommendGraph.addEdge("HP Spectre x360", "Belkin HDMI Cable");
    recommendGraph.addEdge("HP Spectre x360", "JBL Audio Cable");
    recommendGraph.addEdge("HP Spectre x360", "Apple Pencil 2");
    recommendGraph.addEdge("HP Spectre x360", "Samsung Stylus S Pen");
    recommendGraph.addEdge("HP Spectre x360", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("HP Spectre x360", "Baseus Car Charger");
    recommendGraph.addEdge("HP Spectre x360", "Ugreen USB Adapter");
    recommendGraph.addEdge("HP Spectre x360", "Apple Lightning Cable");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Apple MagSafe Charger");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Samsung 45W Charger");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Boat Type-C Cable");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "OtterBox Defender Case");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "HP USB-C Dock");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Anker Wireless Charger");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Belkin HDMI Cable");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "JBL Audio Cable");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Apple Pencil 2");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Baseus Car Charger");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Ugreen USB Adapter");
    recommendGraph.addEdge("Lenovo ThinkPad X1 Carbon", "Apple Lightning Cable");
    recommendGraph.addEdge("Asus ZenBook 14", "Asus ROG Zephyrus G14");
    recommendGraph.addEdge("Asus ZenBook 14", "ASUS ROG Ally");
    recommendGraph.addEdge("Asus ZenBook 14", "Asus TUF Gaming Monitor 27");
    recommendGraph.addEdge("Asus ZenBook 14", "Apple MagSafe Charger");
    recommendGraph.addEdge("Asus ZenBook 14", "Samsung 45W Charger");
    recommendGraph.addEdge("Asus ZenBook 14", "Boat Type-C Cable");
    recommendGraph.addEdge("Asus ZenBook 14", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Asus ZenBook 14", "OtterBox Defender Case");
    recommendGraph.addEdge("Asus ZenBook 14", "HP USB-C Dock");
    recommendGraph.addEdge("Asus ZenBook 14", "Anker Wireless Charger");
    recommendGraph.addEdge("Asus ZenBook 14", "Belkin HDMI Cable");
    recommendGraph.addEdge("Asus ZenBook 14", "JBL Audio Cable");
    recommendGraph.addEdge("Asus ZenBook 14", "Apple Pencil 2");
    recommendGraph.addEdge("Asus ZenBook 14", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Asus ZenBook 14", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Asus ZenBook 14", "Baseus Car Charger");
    recommendGraph.addEdge("Asus ZenBook 14", "Ugreen USB Adapter");
    recommendGraph.addEdge("Asus ZenBook 14", "Apple Lightning Cable");
    recommendGraph.addEdge("Acer Swift 5", "Apple MagSafe Charger");
    recommendGraph.addEdge("Acer Swift 5", "Samsung 45W Charger");
    recommendGraph.addEdge("Acer Swift 5", "Boat Type-C Cable");
    recommendGraph.addEdge("Acer Swift 5", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Acer Swift 5", "OtterBox Defender Case");
    recommendGraph.addEdge("Acer Swift 5", "HP USB-C Dock");
    recommendGraph.addEdge("Acer Swift 5", "Anker Wireless Charger");
    recommendGraph.addEdge("Acer Swift 5", "Belkin HDMI Cable");
    recommendGraph.addEdge("Acer Swift 5", "JBL Audio Cable");
    recommendGraph.addEdge("Acer Swift 5", "Apple Pencil 2");
    recommendGraph.addEdge("Acer Swift 5", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Acer Swift 5", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Acer Swift 5", "Baseus Car Charger");
    recommendGraph.addEdge("Acer Swift 5", "Ugreen USB Adapter");
    recommendGraph.addEdge("Acer Swift 5", "Apple Lightning Cable");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Apple MagSafe Charger");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Samsung 45W Charger");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Boat Type-C Cable");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "OtterBox Defender Case");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "HP USB-C Dock");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Anker Wireless Charger");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Belkin HDMI Cable");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "JBL Audio Cable");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Apple Pencil 2");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Baseus Car Charger");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Ugreen USB Adapter");
    recommendGraph.addEdge("Microsoft Surface Laptop 5", "Apple Lightning Cable");
    recommendGraph.addEdge("Apple MacBook Air M3", "Apple iPad Pro 12.9");
    recommendGraph.addEdge("Apple MacBook Air M3", "Apple Watch Series 9");
    recommendGraph.addEdge("Apple MacBook Air M3", "Apple AirTag 4-Pack");
    recommendGraph.addEdge("Apple MacBook Air M3", "Apple AirPods Pro 2");
    recommendGraph.addEdge("Apple MacBook Air M3", "Apple Watch Ultra 2");
    recommendGraph.addEdge("Apple MacBook Air M3", "Apple MagSafe Charger");
    recommendGraph.addEdge("Apple MacBook Air M3", "Samsung 45W Charger");
    recommendGraph.addEdge("Apple MacBook Air M3", "Boat Type-C Cable");
    recommendGraph.addEdge("Apple MacBook Air M3", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Apple MacBook Air M3", "OtterBox Defender Case");
    recommendGraph.addEdge("Apple MacBook Air M3", "HP USB-C Dock");
    recommendGraph.addEdge("Apple MacBook Air M3", "Anker Wireless Charger");
    recommendGraph.addEdge("Apple MacBook Air M3", "Belkin HDMI Cable");
    recommendGraph.addEdge("Apple MacBook Air M3", "JBL Audio Cable");
    recommendGraph.addEdge("Apple MacBook Air M3", "Apple Pencil 2");
    recommendGraph.addEdge("Apple MacBook Air M3", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Apple MacBook Air M3", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Apple MacBook Air M3", "Baseus Car Charger");
    recommendGraph.addEdge("Apple MacBook Air M3", "Ugreen USB Adapter");
    recommendGraph.addEdge("Apple MacBook Air M3", "Apple Lightning Cable");
    recommendGraph.addEdge("Dell Inspiron 15", "Apple MagSafe Charger");
    recommendGraph.addEdge("Dell Inspiron 15", "Samsung 45W Charger");
    recommendGraph.addEdge("Dell Inspiron 15", "Boat Type-C Cable");
    recommendGraph.addEdge("Dell Inspiron 15", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Dell Inspiron 15", "OtterBox Defender Case");
    recommendGraph.addEdge("Dell Inspiron 15", "HP USB-C Dock");
    recommendGraph.addEdge("Dell Inspiron 15", "Anker Wireless Charger");
    recommendGraph.addEdge("Dell Inspiron 15", "Belkin HDMI Cable");
    recommendGraph.addEdge("Dell Inspiron 15", "JBL Audio Cable");
    recommendGraph.addEdge("Dell Inspiron 15", "Apple Pencil 2");
    recommendGraph.addEdge("Dell Inspiron 15", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Dell Inspiron 15", "Dell Laptop Sleeve 15");
    recommendGraph.addEdge("Dell Inspiron 15", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Dell Inspiron 15", "Baseus Car Charger");
    recommendGraph.addEdge("Dell Inspiron 15", "Ugreen USB Adapter");
    recommendGraph.addEdge("Dell Inspiron 15", "Apple Lightning Cable");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "ASUS ROG Ally");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Asus TUF Gaming Monitor 27");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Apple MagSafe Charger");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Samsung 45W Charger");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Boat Type-C Cable");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "OtterBox Defender Case");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "HP USB-C Dock");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Anker Wireless Charger");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Belkin HDMI Cable");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "JBL Audio Cable");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Apple Pencil 2");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Baseus Car Charger");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Ugreen USB Adapter");
    recommendGraph.addEdge("Asus ROG Zephyrus G14", "Apple Lightning Cable");
    recommendGraph.addEdge("HP Envy 13", "Dyson Air Purifier HP07");
    recommendGraph.addEdge("HP Envy 13", "Apple MagSafe Charger");
    recommendGraph.addEdge("HP Envy 13", "Samsung 45W Charger");
    recommendGraph.addEdge("HP Envy 13", "Boat Type-C Cable");
    recommendGraph.addEdge("HP Envy 13", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("HP Envy 13", "OtterBox Defender Case");
    recommendGraph.addEdge("HP Envy 13", "HP USB-C Dock");
    recommendGraph.addEdge("HP Envy 13", "Anker Wireless Charger");
    recommendGraph.addEdge("HP Envy 13", "Belkin HDMI Cable");
    recommendGraph.addEdge("HP Envy 13", "JBL Audio Cable");
    recommendGraph.addEdge("HP Envy 13", "Apple Pencil 2");
    recommendGraph.addEdge("HP Envy 13", "Samsung Stylus S Pen");
    recommendGraph.addEdge("HP Envy 13", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("HP Envy 13", "Baseus Car Charger");
    recommendGraph.addEdge("HP Envy 13", "Ugreen USB Adapter");
    recommendGraph.addEdge("HP Envy 13", "Apple Lightning Cable");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Apple Watch Series 9");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Apple AirTag 4-Pack");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Apple AirPods Pro 2");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Apple Watch Ultra 2");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Apple MagSafe Charger");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Samsung 45W Charger");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Boat Type-C Cable");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "OtterBox Defender Case");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "HP USB-C Dock");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Anker Wireless Charger");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Belkin HDMI Cable");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "JBL Audio Cable");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Apple Pencil 2");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Baseus Car Charger");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Ugreen USB Adapter");
    recommendGraph.addEdge("Apple iPad Pro 12.9", "Apple Lightning Cable");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Samsung Galaxy Watch 6");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Samsung Galaxy Fit 3");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Samsung 340L Refrigerator");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Samsung Microwave 28L");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Apple MagSafe Charger");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Samsung 45W Charger");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Boat Type-C Cable");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "OtterBox Defender Case");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "HP USB-C Dock");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Anker Wireless Charger");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Belkin HDMI Cable");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "JBL Audio Cable");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Apple Pencil 2");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Baseus Car Charger");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Ugreen USB Adapter");
    recommendGraph.addEdge("Samsung Galaxy Tab S9", "Apple Lightning Cable");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Apple MagSafe Charger");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Samsung 45W Charger");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Boat Type-C Cable");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Amazon Kindle Oasis", "OtterBox Defender Case");
    recommendGraph.addEdge("Amazon Kindle Oasis", "HP USB-C Dock");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Anker Wireless Charger");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Belkin HDMI Cable");
    recommendGraph.addEdge("Amazon Kindle Oasis", "JBL Audio Cable");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Apple Pencil 2");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Amazon Kindle Oasis", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Baseus Car Charger");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Ugreen USB Adapter");
    recommendGraph.addEdge("Amazon Kindle Oasis", "Apple Lightning Cable");
    recommendGraph.addEdge("Apple Watch Series 9", "Apple AirTag 4-Pack");
    recommendGraph.addEdge("Apple Watch Series 9", "Apple AirPods Pro 2");
    recommendGraph.addEdge("Apple Watch Series 9", "Apple Watch Ultra 2");
    recommendGraph.addEdge("Apple Watch Series 9", "Apple MagSafe Charger");
    recommendGraph.addEdge("Apple Watch Series 9", "Samsung 45W Charger");
    recommendGraph.addEdge("Apple Watch Series 9", "Boat Type-C Cable");
    recommendGraph.addEdge("Apple Watch Series 9", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Apple Watch Series 9", "OtterBox Defender Case");
    recommendGraph.addEdge("Apple Watch Series 9", "HP USB-C Dock");
    recommendGraph.addEdge("Apple Watch Series 9", "Anker Wireless Charger");
    recommendGraph.addEdge("Apple Watch Series 9", "Belkin HDMI Cable");
    recommendGraph.addEdge("Apple Watch Series 9", "JBL Audio Cable");
    recommendGraph.addEdge("Apple Watch Series 9", "Apple Pencil 2");
    recommendGraph.addEdge("Apple Watch Series 9", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Apple Watch Series 9", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Apple Watch Series 9", "Baseus Car Charger");
    recommendGraph.addEdge("Apple Watch Series 9", "Ugreen USB Adapter");
    recommendGraph.addEdge("Apple Watch Series 9", "Apple Lightning Cable");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Samsung Galaxy Fit 3");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Samsung 340L Refrigerator");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Samsung Microwave 28L");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Apple MagSafe Charger");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Samsung 45W Charger");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Boat Type-C Cable");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "OtterBox Defender Case");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "HP USB-C Dock");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Anker Wireless Charger");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Belkin HDMI Cable");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "JBL Audio Cable");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Apple Pencil 2");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Baseus Car Charger");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Ugreen USB Adapter");
    recommendGraph.addEdge("Samsung Galaxy Watch 6", "Apple Lightning Cable");
    recommendGraph.addEdge("Google Nest Hub 2", "Apple MagSafe Charger");
    recommendGraph.addEdge("Google Nest Hub 2", "Samsung 45W Charger");
    recommendGraph.addEdge("Google Nest Hub 2", "Boat Type-C Cable");
    recommendGraph.addEdge("Google Nest Hub 2", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Google Nest Hub 2", "OtterBox Defender Case");
    recommendGraph.addEdge("Google Nest Hub 2", "HP USB-C Dock");
    recommendGraph.addEdge("Google Nest Hub 2", "Anker Wireless Charger");
    recommendGraph.addEdge("Google Nest Hub 2", "Belkin HDMI Cable");
    recommendGraph.addEdge("Google Nest Hub 2", "JBL Audio Cable");
    recommendGraph.addEdge("Google Nest Hub 2", "Apple Pencil 2");
    recommendGraph.addEdge("Google Nest Hub 2", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Google Nest Hub 2", "Mi 10000mAh Power Bank");
    recommendGraph.addEdge("Google Nest Hub 2", "Dell Laptop Sleeve 15");
    recommendGraph.addEdge("Google Nest Hub 2", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Google Nest Hub 2", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Google Nest Hub 2", "Baseus Car Charger");
    recommendGraph.addEdge("Google Nest Hub 2", "Ugreen USB Adapter");
    recommendGraph.addEdge("Google Nest Hub 2", "Kingston 128GB Flash Drive");
    recommendGraph.addEdge("Google Nest Hub 2", "Sandisk 1TB SSD");
    recommendGraph.addEdge("Google Nest Hub 2", "Apple Lightning Cable");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Apple AirPods Pro 2");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Apple Watch Ultra 2");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Apple MagSafe Charger");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Samsung 45W Charger");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Boat Type-C Cable");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "OtterBox Defender Case");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "HP USB-C Dock");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Anker Wireless Charger");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Belkin HDMI Cable");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "JBL Audio Cable");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Apple Pencil 2");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Baseus Car Charger");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Ugreen USB Adapter");
    recommendGraph.addEdge("Apple AirTag 4-Pack", "Apple Lightning Cable");
    recommendGraph.addEdge("Anker PowerCore 20000", "Anker Soundcore Life Q30");
    recommendGraph.addEdge("Anker PowerCore 20000", "Apple MagSafe Charger");
    recommendGraph.addEdge("Anker PowerCore 20000", "Samsung 45W Charger");
    recommendGraph.addEdge("Anker PowerCore 20000", "Boat Type-C Cable");
    recommendGraph.addEdge("Anker PowerCore 20000", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Anker PowerCore 20000", "OtterBox Defender Case");
    recommendGraph.addEdge("Anker PowerCore 20000", "HP USB-C Dock");
    recommendGraph.addEdge("Anker PowerCore 20000", "Anker Wireless Charger");
    recommendGraph.addEdge("Anker PowerCore 20000", "Belkin HDMI Cable");
    recommendGraph.addEdge("Anker PowerCore 20000", "JBL Audio Cable");
    recommendGraph.addEdge("Anker PowerCore 20000", "Apple Pencil 2");
    recommendGraph.addEdge("Anker PowerCore 20000", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Anker PowerCore 20000", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Anker PowerCore 20000", "Baseus Car Charger");
    recommendGraph.addEdge("Anker PowerCore 20000", "Ugreen USB Adapter");
    recommendGraph.addEdge("Anker PowerCore 20000", "Apple Lightning Cable");
    recommendGraph.addEdge("Sony WH-1000XM5", "Sony WF-1000XM5");
    recommendGraph.addEdge("Sony WH-1000XM5", "Sony XB13 Portable Speaker");
    recommendGraph.addEdge("Sony WH-1000XM5", "Sony PlayStation 5");
    recommendGraph.addEdge("Sony WH-1000XM5", "JBL Audio Cable");
    recommendGraph.addEdge("Bose QuietComfort Ultra", "Bose SoundLink Revolve+");
    recommendGraph.addEdge("Bose QuietComfort Ultra", "JBL Audio Cable");
    recommendGraph.addEdge("Apple AirPods Pro 2", "Apple Watch Ultra 2");
    recommendGraph.addEdge("Apple AirPods Pro 2", "Apple MagSafe Charger");
    recommendGraph.addEdge("Apple AirPods Pro 2", "JBL Audio Cable");
    recommendGraph.addEdge("Apple AirPods Pro 2", "Apple Pencil 2");
    recommendGraph.addEdge("Apple AirPods Pro 2", "Apple Lightning Cable");
    recommendGraph.addEdge("JBL Charge 5 Speaker", "JBL Tune 760NC");
    recommendGraph.addEdge("JBL Charge 5 Speaker", "JBL Audio Cable");
    recommendGraph.addEdge("Marshall Emberton II", "JBL Audio Cable");
    recommendGraph.addEdge("Sony WF-1000XM5", "Sony XB13 Portable Speaker");
    recommendGraph.addEdge("Sony WF-1000XM5", "Sony PlayStation 5");
    recommendGraph.addEdge("Sony WF-1000XM5", "JBL Audio Cable");
    recommendGraph.addEdge("Sennheiser Momentum 4", "JBL Audio Cable");
    recommendGraph.addEdge("boAt Rockerz 550", "JBL Audio Cable");
    recommendGraph.addEdge("Zebronics Soundbar Z900", "JBL Audio Cable");
    recommendGraph.addEdge("Sony XB13 Portable Speaker", "Sony PlayStation 5");
    recommendGraph.addEdge("Sony XB13 Portable Speaker", "JBL Audio Cable");
    recommendGraph.addEdge("JBL Tune 760NC", "JBL Audio Cable");
    recommendGraph.addEdge("Logitech Z407 Speaker", "Logitech G Pro Wireless Mouse");
    recommendGraph.addEdge("Logitech Z407 Speaker", "Logitech G923 Racing Wheel");
    recommendGraph.addEdge("Logitech Z407 Speaker", "JBL Audio Cable");
    recommendGraph.addEdge("Logitech Z407 Speaker", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Anker Soundcore Life Q30", "Anker Wireless Charger");
    recommendGraph.addEdge("Anker Soundcore Life Q30", "JBL Audio Cable");
    recommendGraph.addEdge("Beats Studio Pro", "JBL Audio Cable");
    recommendGraph.addEdge("Bose SoundLink Revolve+", "JBL Audio Cable");
    recommendGraph.addEdge("Philips TAH8506BK", "JBL Audio Cable");
    recommendGraph.addEdge("Jabra Elite 8 Active", "JBL Audio Cable");
    recommendGraph.addEdge("Realme Buds Air 5", "JBL Audio Cable");
    recommendGraph.addEdge("Skullcandy Hesh ANC", "JBL Audio Cable");
    recommendGraph.addEdge("Noise Evolve 3", "JBL Audio Cable");
    recommendGraph.addEdge("Sony PlayStation 5", "Boat Type-C Cable");
    recommendGraph.addEdge("Sony PlayStation 5", "Belkin HDMI Cable");
    recommendGraph.addEdge("Sony PlayStation 5", "JBL Audio Cable");
    recommendGraph.addEdge("Sony PlayStation 5", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Sony PlayStation 5", "Apple Lightning Cable");
    recommendGraph.addEdge("Microsoft Xbox Series X", "Boat Type-C Cable");
    recommendGraph.addEdge("Microsoft Xbox Series X", "Belkin HDMI Cable");
    recommendGraph.addEdge("Microsoft Xbox Series X", "JBL Audio Cable");
    recommendGraph.addEdge("Microsoft Xbox Series X", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Microsoft Xbox Series X", "Apple Lightning Cable");
    recommendGraph.addEdge("Nintendo Switch OLED", "Boat Type-C Cable");
    recommendGraph.addEdge("Nintendo Switch OLED", "Belkin HDMI Cable");
    recommendGraph.addEdge("Nintendo Switch OLED", "JBL Audio Cable");
    recommendGraph.addEdge("Nintendo Switch OLED", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Nintendo Switch OLED", "Apple Lightning Cable");
    recommendGraph.addEdge("ASUS ROG Ally", "Asus TUF Gaming Monitor 27");
    recommendGraph.addEdge("ASUS ROG Ally", "Boat Type-C Cable");
    recommendGraph.addEdge("ASUS ROG Ally", "Belkin HDMI Cable");
    recommendGraph.addEdge("ASUS ROG Ally", "JBL Audio Cable");
    recommendGraph.addEdge("ASUS ROG Ally", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("ASUS ROG Ally", "Apple Lightning Cable");
    recommendGraph.addEdge("Steam Deck 512GB", "Boat Type-C Cable");
    recommendGraph.addEdge("Steam Deck 512GB", "Belkin HDMI Cable");
    recommendGraph.addEdge("Steam Deck 512GB", "JBL Audio Cable");
    recommendGraph.addEdge("Steam Deck 512GB", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Steam Deck 512GB", "Apple Lightning Cable");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Apple MagSafe Charger");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Samsung 45W Charger");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Boat Type-C Cable");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "OtterBox Defender Case");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "HP USB-C Dock");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Anker Wireless Charger");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Belkin HDMI Cable");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "JBL Audio Cable");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Apple Pencil 2");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Mi 10000mAh Power Bank");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Dell Laptop Sleeve 15");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Baseus Car Charger");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Ugreen USB Adapter");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Kingston 128GB Flash Drive");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Sandisk 1TB SSD");
    recommendGraph.addEdge("Razer BlackWidow V4 Keyboard", "Apple Lightning Cable");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Logitech G923 Racing Wheel");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Apple MagSafe Charger");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Samsung 45W Charger");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Boat Type-C Cable");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Spigen iPhone 15 Case");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "OtterBox Defender Case");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "HP USB-C Dock");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Anker Wireless Charger");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Belkin HDMI Cable");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "JBL Audio Cable");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Apple Pencil 2");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Mi 10000mAh Power Bank");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Dell Laptop Sleeve 15");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "HyperDrive USB-C Hub");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Baseus Car Charger");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Ugreen USB Adapter");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Kingston 128GB Flash Drive");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Sandisk 1TB SSD");
    recommendGraph.addEdge("Logitech G Pro Wireless Mouse", "Apple Lightning Cable");
    recommendGraph.addEdge("Corsair K70 RGB MK.2", "Boat Type-C Cable");
    recommendGraph.addEdge("Corsair K70 RGB MK.2", "Belkin HDMI Cable");
    recommendGraph.addEdge("Corsair K70 RGB MK.2", "JBL Audio Cable");
    recommendGraph.addEdge("Corsair K70 RGB MK.2", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Corsair K70 RGB MK.2", "Apple Lightning Cable");
    recommendGraph.addEdge("Cosmic Byte GS410 Headset", "Boat Type-C Cable");
    recommendGraph.addEdge("Cosmic Byte GS410 Headset", "Belkin HDMI Cable");
    recommendGraph.addEdge("Cosmic Byte GS410 Headset", "JBL Audio Cable");
    recommendGraph.addEdge("Cosmic Byte GS410 Headset", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Cosmic Byte GS410 Headset", "Apple Lightning Cable");
    recommendGraph.addEdge("Alienware Aurora R16", "Boat Type-C Cable");
    recommendGraph.addEdge("Alienware Aurora R16", "Belkin HDMI Cable");
    recommendGraph.addEdge("Alienware Aurora R16", "JBL Audio Cable");
    recommendGraph.addEdge("Alienware Aurora R16", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Alienware Aurora R16", "Apple Lightning Cable");
    recommendGraph.addEdge("Razer Kraken V3", "Boat Type-C Cable");
    recommendGraph.addEdge("Razer Kraken V3", "Belkin HDMI Cable");
    recommendGraph.addEdge("Razer Kraken V3", "JBL Audio Cable");
    recommendGraph.addEdge("Razer Kraken V3", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Razer Kraken V3", "Apple Lightning Cable");
    recommendGraph.addEdge("Asus TUF Gaming Monitor 27", "Boat Type-C Cable");
    recommendGraph.addEdge("Asus TUF Gaming Monitor 27", "Belkin HDMI Cable");
    recommendGraph.addEdge("Asus TUF Gaming Monitor 27", "JBL Audio Cable");
    recommendGraph.addEdge("Asus TUF Gaming Monitor 27", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Asus TUF Gaming Monitor 27", "Apple Lightning Cable");
    recommendGraph.addEdge("Logitech G923 Racing Wheel", "Boat Type-C Cable");
    recommendGraph.addEdge("Logitech G923 Racing Wheel", "Belkin HDMI Cable");
    recommendGraph.addEdge("Logitech G923 Racing Wheel", "JBL Audio Cable");
    recommendGraph.addEdge("Logitech G923 Racing Wheel", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Logitech G923 Racing Wheel", "Apple Lightning Cable");
    recommendGraph.addEdge("HyperX Cloud Alpha", "Boat Type-C Cable");
    recommendGraph.addEdge("HyperX Cloud Alpha", "Belkin HDMI Cable");
    recommendGraph.addEdge("HyperX Cloud Alpha", "JBL Audio Cable");
    recommendGraph.addEdge("HyperX Cloud Alpha", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("HyperX Cloud Alpha", "Apple Lightning Cable");
    recommendGraph.addEdge("MSI Katana GF66 Laptop", "Boat Type-C Cable");
    recommendGraph.addEdge("MSI Katana GF66 Laptop", "Belkin HDMI Cable");
    recommendGraph.addEdge("MSI Katana GF66 Laptop", "JBL Audio Cable");
    recommendGraph.addEdge("MSI Katana GF66 Laptop", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("MSI Katana GF66 Laptop", "Apple Lightning Cable");
    recommendGraph.addEdge("Acer Nitro 5 Laptop", "Boat Type-C Cable");
    recommendGraph.addEdge("Acer Nitro 5 Laptop", "Belkin HDMI Cable");
    recommendGraph.addEdge("Acer Nitro 5 Laptop", "JBL Audio Cable");
    recommendGraph.addEdge("Acer Nitro 5 Laptop", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Acer Nitro 5 Laptop", "Apple Lightning Cable");
    recommendGraph.addEdge("Zotac RTX 4070 GPU", "Boat Type-C Cable");
    recommendGraph.addEdge("Zotac RTX 4070 GPU", "Belkin HDMI Cable");
    recommendGraph.addEdge("Zotac RTX 4070 GPU", "JBL Audio Cable");
    recommendGraph.addEdge("Zotac RTX 4070 GPU", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Zotac RTX 4070 GPU", "Apple Lightning Cable");
    recommendGraph.addEdge("Razer Seiren Mini Mic", "Boat Type-C Cable");
    recommendGraph.addEdge("Razer Seiren Mini Mic", "Belkin HDMI Cable");
    recommendGraph.addEdge("Razer Seiren Mini Mic", "JBL Audio Cable");
    recommendGraph.addEdge("Razer Seiren Mini Mic", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Razer Seiren Mini Mic", "Apple Lightning Cable");
    recommendGraph.addEdge("Elgato Stream Deck", "Boat Type-C Cable");
    recommendGraph.addEdge("Elgato Stream Deck", "Belkin HDMI Cable");
    recommendGraph.addEdge("Elgato Stream Deck", "JBL Audio Cable");
    recommendGraph.addEdge("Elgato Stream Deck", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("Elgato Stream Deck", "Apple Lightning Cable");
    recommendGraph.addEdge("BenQ Zowie XL2546K", "Boat Type-C Cable");
    recommendGraph.addEdge("BenQ Zowie XL2546K", "Belkin HDMI Cable");
    recommendGraph.addEdge("BenQ Zowie XL2546K", "JBL Audio Cable");
    recommendGraph.addEdge("BenQ Zowie XL2546K", "Logitech Mouse Pad XL");
    recommendGraph.addEdge("BenQ Zowie XL2546K", "Apple Lightning Cable");
    recommendGraph.addEdge("Apple Watch Ultra 2", "Apple MagSafe Charger");
    recommendGraph.addEdge("Apple Watch Ultra 2", "Apple Pencil 2");
    recommendGraph.addEdge("Apple Watch Ultra 2", "Apple Lightning Cable");
    recommendGraph.addEdge("Samsung Galaxy Fit 3", "Samsung 340L Refrigerator");
    recommendGraph.addEdge("Samsung Galaxy Fit 3", "Samsung Microwave 28L");
    recommendGraph.addEdge("Samsung Galaxy Fit 3", "Samsung 45W Charger");
    recommendGraph.addEdge("Samsung Galaxy Fit 3", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Dyson Air Purifier HP07", "HP USB-C Dock");
    recommendGraph.addEdge("Samsung 340L Refrigerator", "Samsung Microwave 28L");
    recommendGraph.addEdge("Samsung 340L Refrigerator", "Samsung 45W Charger");
    recommendGraph.addEdge("Samsung 340L Refrigerator", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Samsung Microwave 28L", "Samsung 45W Charger");
    recommendGraph.addEdge("Samsung Microwave 28L", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Apple MagSafe Charger", "Apple Pencil 2");
    recommendGraph.addEdge("Apple MagSafe Charger", "Apple Lightning Cable");
    recommendGraph.addEdge("Samsung 45W Charger", "Samsung Stylus S Pen");
    recommendGraph.addEdge("Apple Pencil 2", "Apple Lightning Cable");
}

void listCategoryProducts(const string &category) {
    vector<Product> allProducts = productManager.getAllProducts();
    cout << "CATEGORY_PRODUCTS" << endl;
    for (const auto &p : allProducts) {
        if (p.category == category) {
            cout << p.name << "|" << p.price << "|" << p.stock << "|" << p.category
                 << "|" << p.rating << "|" << p.brand << endl;
        }
    }
    cout << "CATEGORY_PRODUCTS_END" << endl;
}

void searchCategoryProducts(const string &category, const string &query) {
    vector<Product> allProducts = productManager.getAllProducts();
    string lowerQuery = query;
    transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    vector<Product> results;
    for (const auto &p : allProducts) {
        string name = p.name;
        string cat = p.category;
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (cat == category && name.find(lowerQuery) != string::npos) {
            results.push_back(p);
        }
    }

    if (results.empty()) {
        cout << "NO_RESULTS" << endl;
        return;
    }

    cout << "CATEGORY_SEARCH_RESULTS" << endl;
    for (const auto &p : results) {
        cout << p.name << "|" << p.price << "|" << p.stock << "|" << p.category
             << "|" << p.rating << "|" << p.brand << endl;
    }
    cout << "CATEGORY_SEARCH_END" << endl;
}

void processCommand(const string &command) {
    stringstream ss(command);
    string action;
    ss >> action;
    transform(action.begin(), action.end(), action.begin(), ::toupper);

    if (action == "SEARCH") {
        string query;
        getline(ss, query);
        if (!query.empty() && query[0] == ' ') query.erase(0,1);

        vector<string> results = searchTrie.autocomplete(query);
        if (results.empty()) {
            cout << "NO_RESULTS" << endl;
        } else {
            cout << "SEARCH_RESULTS" << endl;
            for (const string &name : results) {
                Product *p = productManager.getProduct(name);
                if (p) {
                    cout << p->name << "|" << p->price << "|" << p->stock << "|" << p->category
                         << "|" << p->rating << "|" << p->brand << endl;
                }
            }
            cout << "SEARCH_END" << endl;
        }
    }

    else if (action == "SEARCHCAT") {
        string category, query;
        ss >> category;
        getline(ss, query);
        if (!query.empty() && query[0] == ' ') query.erase(0, 1);
        searchCategoryProducts(category, query);
    }

    else if (action == "LISTCAT") {
        string category;
        getline(ss, category);
        if (!category.empty() && category[0] == ' ') category.erase(0, 1);
        listCategoryProducts(category);
    }

    else if (action == "ADD") {
        string productName;
        int quantity = 1;
        getline(ss, productName);
        if (!productName.empty() && productName[0] == ' ') productName.erase(0,1);

        size_t lastSpace = productName.find_last_of(' ');
        if (lastSpace != string::npos) {
            try {
                quantity = stoi(productName.substr(lastSpace + 1));
                productName = productName.substr(0, lastSpace);
            } catch (...) {}
        }

        Product *p = productManager.getProduct(productName);
        if (p)
            cart.addItem(p, quantity);
        else
            cout << "ERROR: Product not found" << endl;
    }

    else if (action == "REMOVE") {
        string productName;
        getline(ss, productName);
        if (!productName.empty() && productName[0] == ' ') productName.erase(0,1);
        cart.removeItem(productName);
    }

    else if (action == "SHOWCART") {
        cart.showCart();
    }

    else if (action == "CHECKOUT") {
        cart.checkout(productManager);
    }

    else if (action == "RECOMMEND") {
        string productName;
        getline(ss, productName);
        if (!productName.empty() && productName[0] == ' ') productName.erase(0,1);

        vector<string> recommendations = recommendGraph.getRecommendations(productName);
        if (recommendations.empty()) {
            cout << "NO_RECOMMENDATIONS" << endl;
        } else {
            cout << "RECOMMENDATIONS" << endl;
            for (const string &recName : recommendations) {
                Product *p = productManager.getProduct(recName);
                if (p)
                    cout << p->name << "|" << p->price << endl;
            }
            cout << "RECOMMEND_END" << endl;
        }
    }

    else if (action == "LISTALL") {
        vector<Product> allProducts = productManager.getAllProducts();
        cout << "ALL_PRODUCTS" << endl;
        for (const auto &p : allProducts) {
            cout << p.name << "|" << p.price << "|" << p.stock << "|" << p.category
                 << "|" << p.rating << "|" << p.brand << endl;
        }
        cout << "PRODUCTS_END" << endl;
    }

    else if (action == "LISTALLFILTER") {
        string filters;
        getline(ss, filters);
        if (!filters.empty() && filters[0] == ' ') filters.erase(0,1);
        ProductFilters pf = parseFilterString(filters);
        vector<Product> allProducts = productManager.getAllProducts();
        vector<Product> out = productManager.applyFilters(allProducts, pf);
        if (out.empty()) {
            cout << "NO_RESULTS" << endl;
        } else {
            cout << "ALL_PRODUCTS" << endl;
            for (const auto &p : out) {
                cout << p.name << "|" << p.price << "|" << p.stock << "|" << p.category
                     << "|" << p.rating << "|" << p.brand << endl;
            }
            cout << "PRODUCTS_END" << endl;
        }
    }

    else if (action == "SEARCHFILTER") {
        string rest;
        getline(ss, rest);
        if (!rest.empty() && rest[0] == ' ') rest.erase(0,1);
        string qpart;
        string fpart;
        size_t bar = rest.find('|');
        if (bar != string::npos) {
            qpart = trim(rest.substr(0, bar));
            fpart = trim(rest.substr(bar + 1));
        } else {
            qpart = trim(rest);
            fpart = "";
        }

        vector<string> results = searchTrie.autocomplete(qpart);
        vector<Product> matched;
        for (const string &name : results) {
            Product *p = productManager.getProduct(name);
            if (p) matched.push_back(*p);
        }

        ProductFilters pf = parseFilterString(fpart);
        vector<Product> out = productManager.applyFilters(matched, pf);
        if (out.empty()) {
            cout << "NO_RESULTS" << endl;
        } else {
            cout << "SEARCH_RESULTS" << endl;
            for (const auto &p : out) {
                cout << p.name << "|" << p.price << "|" << p.stock << "|" << p.category
                     << "|" << p.rating << "|" << p.brand << endl;
            }
            cout << "SEARCH_END" << endl;
        }
    }

    else {
        cout << "ERROR: Unknown command" << endl;
    }
}

int main() {
    initializeSystem();

    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "ERROR: Cannot open input/output files" << endl;
        return 1;
    }

    streambuf *coutBuf = cout.rdbuf();
    cout.rdbuf(outputFile.rdbuf());

    string command;
    while (getline(inputFile, command)) {
        if (!command.empty() && command[0] != '#') {
            processCommand(command);
        }
    }

    cout.rdbuf(coutBuf);
    inputFile.close();
    outputFile.close();

    cart.saveToFile();
    return 0;
}
