import tkinter as tk
from tkinter import ttk, messagebox
import sys
from backend_interface import BackendInterface


class ECommerceApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Smart E-Commerce System")
        self.root.geometry("1200x750")

        backend_path = "../backend_cpp/ecommerce"
        if sys.platform == "win32":
            backend_path = "../backend_cpp/ecommerce.exe"

        self.backend = BackendInterface(
            cpp_executable=backend_path,
            input_file="../backend_cpp/input.txt",
            output_file="../backend_cpp/output.txt"
        )

        self.categories = ["Electronics", "Books", "Gaming", "Accessories",
                           "Audio", "Fitness", "Home", "Appliances"]
        self.current_category = None

        self.setup_layout()
        self.show_default_message()

    def setup_layout(self):
        self.top_frame = tk.Frame(self.root, bg="#f8f9fa", height=70)
        self.top_frame.pack(fill=tk.X, side=tk.TOP)

        tk.Label(self.top_frame, text="🛒 Smart E-Commerce System",
                 font=("Arial", 20, "bold"), bg="#f8f9fa").pack(side=tk.LEFT, padx=20)

        self.search_entry = tk.Entry(self.top_frame, font=("Arial", 12), width=40)
        self.search_entry.pack(side=tk.LEFT, padx=10)
        tk.Button(self.top_frame, text="Search", command=self.search_products,
                  bg="#4CAF50", fg="white", font=("Arial", 10, "bold")).pack(side=tk.LEFT)

        self.cart_btn = tk.Button(self.top_frame, text="🛒 View Cart (0)",
                                  command=self.show_cart, bg="#FF9800",
                                  fg="white", font=("Arial", 10, "bold"))
        self.cart_btn.pack(side=tk.RIGHT, padx=10)

        tk.Button(self.top_frame, text="💡 Recommendations",
                  command=self.show_recommendations, bg="#9C27B0",
                  fg="white", font=("Arial", 10, "bold")).pack(side=tk.RIGHT, padx=10)

        self.body_frame = tk.Frame(self.root, bg="#ffffff")
        self.body_frame.pack(fill=tk.BOTH, expand=True)

        self.sidebar = tk.Frame(self.body_frame, width=220, bg="#e3f2fd")
        self.sidebar.pack(side=tk.LEFT, fill=tk.Y)

        tk.Label(self.sidebar, text="📚 Categories",
                 font=("Arial", 14, "bold"), bg="#e3f2fd").pack(pady=10)

        tk.Button(self.sidebar, text="🛍 All Products", width=20, font=("Arial", 12, "bold"),
                  bg="#43A047", fg="white", relief=tk.RAISED,
                  command=self.load_all_products).pack(pady=(5, 15), padx=10)

        for cat in self.categories:
            tk.Button(self.sidebar, text=cat, width=20, font=("Arial", 12),
                      bg="#2196F3", fg="white", relief=tk.RAISED,
                      command=lambda c=cat: self.load_category_products(c)).pack(pady=5, padx=10)

        self.content_frame = tk.Frame(self.body_frame, bg="#f0f0f0")
        self.content_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=10, pady=10)

        self.status_label = tk.Label(self.root, text="Ready",
                                     font=("Arial", 10), bg="#e0e0e0",
                                     anchor=tk.W, relief=tk.SUNKEN)
        self.status_label.pack(side=tk.BOTTOM, fill=tk.X)

    def clear_content(self):
        for widget in self.content_frame.winfo_children():
            widget.destroy()

    def show_default_message(self):
        self.clear_content()
        tk.Label(self.content_frame,
                 text="Welcome! Select a category from the left sidebar.",
                 font=("Arial", 16, "bold"), bg="#f0f0f0").pack(pady=200)

    def load_category_products(self, category):
        self.current_category = category
        self.clear_content()

        tk.Label(self.content_frame, text=f"Category: {category}",
                 font=("Arial", 18, "bold"), bg="#f0f0f0").pack(pady=10)

        product_frame = tk.LabelFrame(self.content_frame, text="Products",
                                      font=("Arial", 12, "bold"), bg="#f0f0f0")
        product_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        columns = ("Name", "Price", "Stock")
        self.products_tree = ttk.Treeview(product_frame, columns=columns,
                                          show="tree headings", height=15)
        self.products_tree.heading("#0", text="ID")
        self.products_tree.column("#0", width=50)

        for col in columns:
            self.products_tree.heading(col, text=col)
            if col == "Name":
                self.products_tree.column(col, width=350)
            elif col == "Price":
                self.products_tree.column(col, width=120)
            else:
                self.products_tree.column(col, width=100)

        scrollbar = ttk.Scrollbar(product_frame, orient=tk.VERTICAL,
                                  command=self.products_tree.yview)
        self.products_tree.configure(yscroll=scrollbar.set)
        self.products_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        control_frame = tk.Frame(self.content_frame, bg="#f0f0f0")
        control_frame.pack(fill=tk.X, pady=10)

        tk.Label(control_frame, text="Quantity:",
                 font=("Arial", 10), bg="#f0f0f0").pack(side=tk.LEFT, padx=5)
        self.quantity_var = tk.StringVar(value="1")
        tk.Spinbox(control_frame, from_=1, to=10, textvariable=self.quantity_var,
                   width=5, font=("Arial", 10)).pack(side=tk.LEFT, padx=5)

        tk.Button(control_frame, text="➕ Add to Cart",
                  command=self.add_to_cart, bg="#4CAF50",
                  fg="white", font=("Arial", 10, "bold")).pack(side=tk.LEFT, padx=5)

        self.load_products_for_category(category)

    def load_products_for_category(self, category):
        self.status_label.config(text=f"Loading {category} products...")
        result = self.backend.execute_command(f"LISTCAT {category}")
        if "error" in result:
            messagebox.showerror("Error", result["error"])
            return
        self.display_products(result.get("products", []))
        self.status_label.config(text=f"Loaded {len(result.get('products', []))} products")

    def load_all_products(self):
        self.current_category = None
        self.clear_content()

        tk.Label(self.content_frame, text="All Available Products",
                 font=("Arial", 18, "bold"), bg="#f0f0f0").pack(pady=10)

        product_frame = tk.LabelFrame(self.content_frame, text="All Products",
                                      font=("Arial", 12, "bold"), bg="#f0f0f0")
        product_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        columns = ("Name", "Price", "Stock", "Category")
        self.products_tree = ttk.Treeview(product_frame, columns=columns,
                                          show="tree headings", height=15)
        self.products_tree.heading("#0", text="ID")
        self.products_tree.column("#0", width=50)

        for col in columns:
            self.products_tree.heading(col, text=col)
            if col == "Name":
                self.products_tree.column(col, width=300)
            elif col == "Price":
                self.products_tree.column(col, width=100)
            elif col == "Stock":
                self.products_tree.column(col, width=80)
            else:
                self.products_tree.column(col, width=150)

        scrollbar = ttk.Scrollbar(product_frame, orient=tk.VERTICAL,
                                  command=self.products_tree.yview)
        self.products_tree.configure(yscroll=scrollbar.set)
        self.products_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        control_frame = tk.Frame(self.content_frame, bg="#f0f0f0")
        control_frame.pack(fill=tk.X, pady=10)

        tk.Label(control_frame, text="Quantity:",
                 font=("Arial", 10), bg="#f0f0f0").pack(side=tk.LEFT, padx=5)
        self.quantity_var = tk.StringVar(value="1")
        tk.Spinbox(control_frame, from_=1, to=10, textvariable=self.quantity_var,
                   width=5, font=("Arial", 10)).pack(side=tk.LEFT, padx=5)
        tk.Button(control_frame, text="➕ Add to Cart",
                  command=self.add_to_cart, bg="#4CAF50",
                  fg="white", font=("Arial", 10, "bold")).pack(side=tk.LEFT, padx=5)

        self.status_label.config(text="Loading all products...")
        result = self.backend.execute_command("LISTALL")
        if "error" in result:
            messagebox.showerror("Error", result["error"])
            return
        self.display_all_products(result.get("products", []))
        self.status_label.config(text=f"Loaded {len(result.get('products', []))} products")

    def display_all_products(self, products):
        for i in self.products_tree.get_children():
            self.products_tree.delete(i)
        for idx, p in enumerate(products, 1):
            self.products_tree.insert("", tk.END, text=str(idx),
                                      values=(p["name"],
                                              f"₹{p['price']:.2f}",
                                              p["stock"],
                                              p["category"]))

    def search_products(self):
        query = self.search_entry.get().strip()
        if not query:
            if self.current_category:
                self.load_products_for_category(self.current_category)
            else:
                self.load_all_products()
            return

        if self.current_category:
            command = f"SEARCHCAT {self.current_category} {query}"
            context = self.current_category
        else:
            command = f"SEARCH {query}"
            context = "all products"

        self.status_label.config(text=f"Searching '{query}' in {context}...")
        result = self.backend.execute_command(command)
        if "error" in result:
            messagebox.showerror("Error", result["error"])
            return

        products = result.get("products", [])
        if not products:
            messagebox.showinfo("No Results", f"No matches found for '{query}'.")
            if self.current_category:
                self.load_products_for_category(self.current_category)
            else:
                self.load_all_products()
            self.status_label.config(text=f"Found 0 results for '{query}'")
            return
        self.display_products(products)
        self.status_label.config(text=f"Found {len(products)} results for '{query}'")

    def display_products(self, products):
        for i in self.products_tree.get_children():
            self.products_tree.delete(i)
        for idx, p in enumerate(products, 1):
            self.products_tree.insert("", tk.END, text=str(idx),
                                      values=(p["name"],
                                              f"₹{p['price']:.2f}",
                                              p["stock"]))

    def add_to_cart(self):
        if not hasattr(self, "products_tree"):
            messagebox.showwarning("Add to Cart", "Open a product list first.")
            return
        selected = self.products_tree.selection()
        if not selected:
            messagebox.showwarning("Select Product", "Select a product to add.")
            return
        item = self.products_tree.item(selected[0])
        name = item["values"][0]
        qty = self.quantity_var.get()
        result = self.backend.execute_command(f"ADD {name} {qty}")
        if "error" in result:
            messagebox.showerror("Error", result["error"])
        elif result.get("success"):
            messagebox.showinfo("Success", result.get("message", "Added to cart"))
            self.update_cart_button()

    def show_cart(self):
        result = self.backend.execute_command("SHOWCART")
        if "error" in result:
            messagebox.showerror("Error", result["error"])
            return
        CartWindow(self.root, result, self.backend, self.update_cart_button)

    def update_cart_button(self):
        result = self.backend.execute_command("SHOWCART")
        if "items" in result:
            count = len(result["items"])
            self.cart_btn.config(text=f"🛒 View Cart ({count})")

    def show_recommendations(self):
        if not hasattr(self, "products_tree"):
            messagebox.showwarning("Recommendations", "Open a category and select a product.")
            return
        selected = self.products_tree.selection()
        if not selected:
            messagebox.showwarning("Select Product", "Select a product first.")
            return
        item = self.products_tree.item(selected[0])
        name = item["values"][0]
        result = self.backend.execute_command(f"RECOMMEND {name}")
        if "error" in result:
            messagebox.showerror("Error", result["error"])
            return
        recs = result.get("recommendations", [])
        if not recs:
            messagebox.showinfo("No Recommendations", f"No related products for {name}")
            return
        RecommendationWindow(self.root, name, recs)


class CartWindow:
    def __init__(self, parent, cart_data, backend, update_callback):
        self.window = tk.Toplevel(parent)
        self.window.title("Shopping Cart")
        self.window.geometry("600x500")
        self.backend = backend
        self.update_callback = update_callback
        self.items = cart_data.get("items", [])
        self.total = cart_data.get("total", 0.0)
        self.setup_ui()

    def setup_ui(self):
        tk.Label(self.window, text="🛒 Your Shopping Cart",
                 font=("Arial", 16, "bold")).pack(pady=10)

        if not self.items:
            tk.Label(self.window, text="Your cart is empty",
                     font=("Arial", 12)).pack(pady=50)
            tk.Button(self.window, text="Close", command=self.window.destroy,
                      bg="#757575", fg="white", font=("Arial", 10, "bold")).pack(pady=10)
            return

        frame = tk.Frame(self.window)
        frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        cols = ("Product", "Quantity", "Price", "Subtotal")
        tree = ttk.Treeview(frame, columns=cols, show="headings", height=10)
        for c in cols:
            tree.heading(c, text=c)
            tree.column(c, width=120)
        for i in self.items:
            tree.insert("", tk.END, values=(i["name"], i["quantity"],
                                            f"₹{i['price']:.2f}",
                                            f"₹{i['subtotal']:.2f}"))
        scroll = ttk.Scrollbar(frame, orient=tk.VERTICAL, command=tree.yview)
        tree.configure(yscroll=scroll.set)
        tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)

        total_frame = tk.Frame(self.window, bg="#e0e0e0")
        total_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Label(total_frame, text=f"Total: ₹{self.total:.2f}",
                 font=("Arial", 14, "bold"), bg="#e0e0e0").pack(pady=10)

        btn_frame = tk.Frame(self.window)
        btn_frame.pack(fill=tk.X, padx=10, pady=10)
        tk.Button(btn_frame, text="Remove Selected",
                  command=lambda: self.remove_item(tree),
                  bg="#f44336", fg="white", font=("Arial", 10, "bold")).pack(side=tk.LEFT, padx=5)
        tk.Button(btn_frame, text="💳 Checkout", command=self.checkout,
                  bg="#4CAF50", fg="white", font=("Arial", 10, "bold")).pack(side=tk.RIGHT, padx=5)
        tk.Button(btn_frame, text="Close", command=self.window.destroy,
                  bg="#757575", fg="white", font=("Arial", 10, "bold")).pack(side=tk.RIGHT, padx=5)

    def remove_item(self, tree):
        sel = tree.selection()
        if not sel:
            messagebox.showwarning("Select item", "Please select an item to remove")
            return
        name = tree.item(sel[0])["values"][0]
        result = self.backend.execute_command(f"REMOVE {name}")
        if result.get("success"):
            messagebox.showinfo("Removed", "Item removed from cart")
            self.window.destroy()
            self.update_callback()

    def checkout(self):
        confirm = messagebox.askyesno("Confirm",
                                      f"Proceed with checkout?\nTotal: ₹{self.total:.2f}")
        if not confirm:
            return
        result = self.backend.execute_command("CHECKOUT")
        if result.get("success"):
            messagebox.showinfo("Success",
                                f"Checkout successful!\nTotal paid: ₹{result.get('total', 0):.2f}")
            self.window.destroy()
            self.update_callback()


class RecommendationWindow:
    def __init__(self, parent, product, recs):
        self.window = tk.Toplevel(parent)
        self.window.title("Product Recommendations")
        self.window.geometry("500x400")
        tk.Label(self.window, text=f"💡 Recommendations for:\n{product}",
                 font=("Arial", 12, "bold")).pack(pady=10)

        frame = tk.Frame(self.window)
        frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        cols = ("Product", "Price")
        tree = ttk.Treeview(frame, columns=cols, show="headings", height=12)
        tree.heading("Product", text="Product Name")
        tree.heading("Price", text="Price")
        tree.column("Product", width=350)
        tree.column("Price", width=100)
        for r in recs:
            tree.insert("", tk.END, values=(r["name"], f"₹{r['price']:.2f}"))
        scroll = ttk.Scrollbar(frame, orient=tk.VERTICAL, command=tree.yview)
        tree.configure(yscroll=scroll.set)
        tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)
        tk.Button(self.window, text="Close", command=self.window.destroy,
                  bg="#757575", fg="white", font=("Arial", 10, "bold")).pack(pady=10)


def main():
    root = tk.Tk()
    ECommerceApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
