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

    recommendGraph.addEdge("iPhone 14", "AirPods Pro");
    recommendGraph.addEdge("iPhone 14", "iPhone Case");
    recommendGraph.addEdge("MacBook Pro", "Magic Mouse");
    recommendGraph.addEdge("MacBook Pro", "USB-C Hub");
    recommendGraph.addEdge("Samsung Galaxy S23", "Galaxy Buds");
    recommendGraph.addEdge("Dell XPS 15", "Laptop Stand");
    recommendGraph.addEdge("Sony WH-1000XM5", "Audio Cable");
    recommendGraph.addEdge("Kindle Paperwhite", "Kindle Cover");
    recommendGraph.addEdge("Nintendo Switch", "Switch Games");
    recommendGraph.addEdge("PlayStation 5", "PS5 Controller");
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
