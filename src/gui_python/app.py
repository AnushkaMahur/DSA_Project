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

        self.filter_min_price = tk.StringVar()
        self.filter_max_price = tk.StringVar()
        self.filter_brand = tk.StringVar()

        self.sort_type = tk.StringVar(value="None")

        self.setup_layout()
        self.show_default_message()

    def setup_layout(self):
        self.top_frame = tk.Frame(self.root, bg="#f8f9fa", height=70)
        self.top_frame.pack(fill=tk.X, side=tk.TOP)

        tk.Label(
            self.top_frame,
            text="🛒 Smart E-Commerce System",
            font=("Arial", 20, "bold"),
            bg="#f8f9fa"
        ).pack(side=tk.LEFT, padx=20)

        self.search_entry = tk.Entry(self.top_frame, font=("Arial", 12), width=40)
        self.search_entry.pack(side=tk.LEFT, padx=10)

        tk.Button(
            self.top_frame,
            text="Search",
            width=12,
            command=self.search_products,
            bg="#4CAF50",
            fg="white",
            font=("Arial", 10, "bold")
        ).pack(side=tk.LEFT, padx=5)

        tk.Button(
            self.top_frame,
            text="🔍 Filters",
            width=12,
            command=self.open_filter_window,
            bg="#1976D2",
            fg="white",
            font=("Arial", 10, "bold")
        ).pack(side=tk.LEFT, padx=5)

        sort_frame = tk.Frame(self.top_frame, bg="#f8f9fa")
        sort_frame.pack(side=tk.LEFT, padx=8)

        tk.Label(sort_frame, text="Sort By:", bg="#f8f9fa").pack(side=tk.LEFT)

        self.sort_dropdown = ttk.Combobox(
            sort_frame,
            textvariable=self.sort_type,
            state="readonly",
            width=22,
            values=[
                "None",
                "Price (Low → High)",
                "Price (High → Low)",
                "Name (A → Z)",
                "Stock (High → Low)"
            ]
        )
        self.sort_dropdown.pack(side=tk.LEFT)

        tk.Button(
            sort_frame,
            text="Apply Sort",
            bg="#795548",
            fg="white",
            font=("Arial", 10, "bold"),
            command=self.apply_sort
        ).pack(side=tk.LEFT, padx=5)

        tk.Button(
            sort_frame,
            text="Refresh",
            bg="#607D8B",
            fg="white",
            font=("Arial", 10, "bold"),
            command=self.refresh_products
        ).pack(side=tk.LEFT, padx=5)

        self.body_frame = tk.Frame(self.root, bg="#ffffff")
        self.body_frame.pack(fill=tk.BOTH, expand=True)

        self.sidebar = tk.Frame(self.body_frame, width=220, bg="#e3f2fd")
        self.sidebar.pack(side=tk.LEFT, fill=tk.Y)

        tk.Label(
            self.sidebar,
            text="📚 Categories",
            font=("Arial", 14, "bold"),
            bg="#e3f2fd"
        ).pack(pady=10)

        tk.Button(
            self.sidebar,
            text="🛍 All Products",
            width=20,
            font=("Arial", 12, "bold"),
            bg="#43A047",
            fg="white",
            relief=tk.RAISED,
            command=self.load_all_products
        ).pack(pady=(5, 15), padx=10)

        for cat in self.categories:
            tk.Button(
                self.sidebar,
                text=cat,
                width=20,
                font=("Arial", 12),
                bg="#2196F3",
                fg="white",
                relief=tk.RAISED,
                command=lambda c=cat: self.load_category_products(c)
            ).pack(pady=5, padx=10)

        self.sidebar_bottom = tk.Frame(self.sidebar, bg="#e3f2fd")
        self.sidebar_bottom.pack(side=tk.BOTTOM, fill=tk.X, pady=20)

        self.cart_sidebar_btn = tk.Button(
            self.sidebar_bottom,
            text="🛒 View Cart (0)",
            width=20,
            bg="#FFEB3B",
            fg="black",
            font=("Arial", 12, "bold"),
            command=self.show_cart
        )
        self.cart_sidebar_btn.pack(pady=10)

        self.rec_sidebar_btn = tk.Button(
            self.sidebar_bottom,
            text="💡 Recommendations",
            width=20,
            bg="#CE93D8",
            fg="black",
            font=("Arial", 12, "bold"),
            command=self.show_recommendations
        )
        self.rec_sidebar_btn.pack(pady=10)

        self.content_frame = tk.Frame(self.body_frame, bg="#f0f0f0")
        self.content_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=10, pady=10)

        self.status_label = tk.Label(
            self.root,
            text="Ready",
            font=("Arial", 10),
            bg="#e0e0e0",
            anchor=tk.W,
            relief=tk.SUNKEN
        )
        self.status_label.pack(side=tk.BOTTOM, fill=tk.X)

    def refresh_products(self):
        if self.current_category:
            self.load_products_for_category(self.current_category)
        else:
            self.load_all_products()

    def open_filter_window(self):
        win = tk.Toplevel(self.root)
        win.title("Filters")
        win.geometry("350x260")

        tk.Label(win, text="Min Price").pack(pady=5)
        tk.Entry(win, textvariable=self.filter_min_price).pack()

        tk.Label(win, text="Max Price").pack(pady=5)
        tk.Entry(win, textvariable=self.filter_max_price).pack()

        tk.Label(win, text="Brand").pack(pady=5)
        tk.Entry(win, textvariable=self.filter_brand).pack()

        tk.Button(
            win,
            text="Apply Filters",
            bg="#4CAF50",
            fg="white",
            command=lambda: [self.apply_filters(), win.destroy()]
        ).pack(pady=20)

    def build_filter_string(self):
        filters = []
        if self.filter_min_price.get().strip():
            filters.append(f"min_price={self.filter_min_price.get().strip()}")
        if self.filter_max_price.get().strip():
            filters.append(f"max_price={self.filter_max_price.get().strip()}")
        if self.filter_brand.get().strip():
            filters.append(f"brand={self.filter_brand.get().strip()}")
        if self.current_category:
            filters.append(f"category={self.current_category}")
        return ";".join(filters)

    def apply_filters(self):
        fs = self.build_filter_string()
        if not fs:
            messagebox.showwarning("Filters", "No filters selected")
            return

        result = self.backend.list_all_filter(fs)
        if "error" in result:
            messagebox.showerror("Error", result["error"])
            return

        products = result.get("products", [])

        self.clear_content()
        tk.Label(
            self.content_frame,
            text="Filtered Results",
            font=("Arial", 18, "bold"),
            bg="#f0f0f0"
        ).pack(pady=10)

        self.load_filtered_table(products)

    def apply_sort(self):
        choice = self.sort_type.get()

        if choice == "None":
            self.refresh_products()
            return

        mapping = {
            "Price (Low → High)": "PRICE_ASC",
            "Price (High → Low)": "PRICE_DESC",
            "Name (A → Z)": "NAME_ASC",
            "Stock (High → Low)": "STOCK_DESC"
        }

        sort_key = mapping.get(choice, "PRICE_ASC")
        cmd = f"SORT {sort_key}"

        if self.current_category:
            cmd += f" {self.current_category}"

        result = self.backend.execute_command(cmd)
        if "error" in result:
            messagebox.showerror("Error", result["error"])
            return

        products = result.get("products", [])

        self.clear_content()
        tk.Label(
            self.content_frame,
            text="Sorted Results",
            font=("Arial", 18, "bold"),
            bg="#f0f0f0"
        ).pack(pady=10)

        self.load_filtered_table(products)

    def load_filtered_table(self, products):
        product_frame = tk.LabelFrame(
            self.content_frame,
            text="Results",
            font=("Arial", 12, "bold"),
            bg="#f0f0f0"
        )
        product_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        columns = ("Name", "Price", "Stock", "Category")
        self.products_tree = ttk.Treeview(
            product_frame,
            columns=columns,
            show="tree headings",
            height=15
        )

        self.products_tree.heading("#0", text="ID")
        self.products_tree.column("#0", width=50)

        for c in columns:
            self.products_tree.heading(c, text=c)
            self.products_tree.column(c, width=150)

        scroll = ttk.Scrollbar(product_frame, orient=tk.VERTICAL, command=self.products_tree.yview)
        self.products_tree.configure(yscroll=scroll.set)
        self.products_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)

        for idx, p in enumerate(products, 1):
            self.products_tree.insert(
                "",
                tk.END,
                text=str(idx),
                values=(
                    p["name"],
                    f"₹{p['price']:.2f}",
                    p["stock"],
                    p.get("category", "")
                )
            )

    def clear_content(self):
        for w in self.content_frame.winfo_children():
            w.destroy()

    def show_default_message(self):
        self.clear_content()
        tk.Label(
            self.content_frame,
            text="Welcome! Select a category from the left sidebar.",
            font=("Arial", 16, "bold"),
            bg="#f0f0f0"
        ).pack(pady=200)

    def load_category_products(self, category):
        self.current_category = category
        self.clear_content()

        tk.Label(
            self.content_frame,
            text=f"Category: {category}",
            font=("Arial", 18, "bold"),
            bg="#f0f0f0"
        ).pack(pady=10)

        product_frame = tk.LabelFrame(
            self.content_frame,
            text="Products",
            font=("Arial", 12, "bold"),
            bg="#f0f0f0"
        )
        product_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        columns = ("Name", "Price", "Stock")
        self.products_tree = ttk.Treeview(
            product_frame,
            columns=columns,
            show="tree headings",
            height=15
        )

        self.products_tree.heading("#0", text="ID")
        self.products_tree.column("#0", width=50)

        for c in columns:
            self.products_tree.heading(c, text=c)
            if c == "Name":
                self.products_tree.column(c, width=350)
            elif c == "Price":
                self.products_tree.column(c, width=120)
            else:
                self.products_tree.column(c, width=100)

        scroll = ttk.Scrollbar(product_frame, orient=tk.VERTICAL, command=self.products_tree.yview)
        self.products_tree.configure(yscroll=scroll.set)
        self.products_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)

        control_frame = tk.Frame(self.content_frame, bg="#f0f0f0")
        control_frame.pack(fill=tk.X, pady=10)

        tk.Label(control_frame, text="Quantity:", font=("Arial", 10), bg="#f0f0f0").pack(side=tk.LEFT)

        self.quantity_var = tk.StringVar(value="1")
        tk.Spinbox(control_frame, from_=1, to=10, textvariable=self.quantity_var, width=5).pack(side=tk.LEFT, padx=5)

        tk.Button(
            control_frame,
            text="➕ Add to Cart",
            command=self.add_to_cart,
            bg="#4CAF50",
            fg="white",
            font=("Arial", 10, "bold")
        ).pack(side=tk.LEFT, padx=5)

        self.load_products_for_category(category)

    def load_products_for_category(self, category):
        result = self.backend.execute_command(f"LISTCAT {category}")
        if "error" in result:
            messagebox.showerror("Error", result["error"])
            return
        self.display_products(result.get("products", []))

    def load_all_products(self):
        self.current_category = None
        self.clear_content()

        tk.Label(
            self.content_frame,
            text="All Available Products",
            font=("Arial", 18, "bold"),
            bg="#f0f0f0"
        ).pack(pady=10)

        product_frame = tk.LabelFrame(
            self.content_frame,
            text="All Products",
            font=("Arial", 12, "bold"),
            bg="#f0f0f0"
        )
        product_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        columns = ("Name", "Price", "Stock", "Category")
        self.products_tree = ttk.Treeview(
            product_frame,
            columns=columns,
            show="tree headings",
            height=15
        )

        self.products_tree.heading("#0", text="ID")
        self.products_tree.column("#0", width=50)

        for c in columns:
            self.products_tree.heading(c, text=c)
            if c == "Name":
                self.products_tree.column(c, width=300)
            elif c == "Price":
                self.products_tree.column(c, width=100)
            elif c == "Stock":
                self.products_tree.column(c, width=80)
            else:
                self.products_tree.column(c, width=150)

        scroll = ttk.Scrollbar(product_frame, orient=tk.VERTICAL, command=self.products_tree.yview)
        self.products_tree.configure(yscroll=scroll.set)
        self.products_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)

        control_frame = tk.Frame(self.content_frame, bg="#f0f0f0")
        control_frame.pack(fill=tk.X, pady=10)

        tk.Label(control_frame, text="Quantity:", font=("Arial", 10), bg="#f0f0f0").pack(side=tk.LEFT)

        self.quantity_var = tk.StringVar(value="1")
        tk.Spinbox(control_frame, from_=1, to=10, textvariable=self.quantity_var, width=5).pack(side=tk.LEFT, padx=5)

        tk.Button(
            control_frame,
            text="➕ Add to Cart",
            command=self.add_to_cart,
            bg="#4CAF50",
            fg="white",
            font=("Arial", 10, "bold")
        ).pack(side=tk.LEFT, padx=5)

        result = self.backend.execute_command("LISTALL")
        self.display_all_products(result.get("products", []))

    def display_all_products(self, products):
        for i in self.products_tree.get_children():
            self.products_tree.delete(i)

        for idx, p in enumerate(products, 1):
            self.products_tree.insert(
                "",
                tk.END,
                text=str(idx),
                values=(
                    p["name"],
                    f"₹{p['price']:.2f}",
                    p["stock"],
                    p.get("category", "")
                )
            )

    def search_products(self):
        query = self.search_entry.get().strip()

        if not query:
            if self.current_category:
                self.load_products_for_category(self.current_category)
            else:
                self.load_all_products()
            return

        if self.current_category:
            cmd = f"SEARCHCAT {self.current_category} {query}"
        else:
            cmd = f"SEARCH {query}"

        result = self.backend.execute_command(cmd)
        products = result.get("products", [])

        if not products:
            messagebox.showinfo("No Results", f"No matches found for '{query}'.")
            return

        self.display_products(products)

    def display_products(self, products):
        for i in self.products_tree.get_children():
            self.products_tree.delete(i)

        for idx, p in enumerate(products, 1):
            self.products_tree.insert(
                "",
                tk.END,
                text=str(idx),
                values=(
                    p["name"],
                    f"₹{p['price']:.2f}",
                    p["stock"]
                )
            )

    def add_to_cart(self):
        selected = self.products_tree.selection()
        if not selected:
            messagebox.showwarning("Select Product", "Select a product to add.")
            return

        item = self.products_tree.item(selected[0])
        name = item["values"][0]
        qty = self.quantity_var.get()

        result = self.backend.execute_command(f"ADD {name} {qty}")

        if result.get("success"):
            messagebox.showinfo("Success", "Added to cart")
            self.update_cart_button()

    def update_cart_button(self):
        result = self.backend.execute_command("SHOWCART")
        if "items" in result:
            count = len(result["items"])
            self.cart_sidebar_btn.config(text=f"🛒 View Cart ({count})")

    def show_cart(self):
        result = self.backend.execute_command("SHOWCART")
        CartWindow(self.root, result, self.backend, self.update_cart_button)

    def show_recommendations(self):
        selected = None
        if hasattr(self, "products_tree"):
            sel = self.products_tree.selection()
            if sel:
                selected = self.products_tree.item(sel[0])["values"][0]

        if not selected:
            messagebox.showwarning("Select Product", "Select a product first.")
            return

        result = self.backend.execute_command(f"RECOMMEND {selected}")
        recs = result.get("recommendations", [])

        if not recs:
            messagebox.showinfo("No Recommendations", "No related products found")
            return

        RecommendationWindow(self.root, selected, recs)


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
                      bg="#757575", fg="white",
                      font=("Arial", 10, "bold")).pack(pady=10)
            return

        frame = tk.Frame(self.window)
        frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        columns = ("Product", "Quantity", "Price", "Subtotal")
        tree = ttk.Treeview(frame, columns=columns, show="headings", height=10)

        for c in columns:
            tree.heading(c, text=c)
            tree.column(c, width=120)

        for item in self.items:
            tree.insert("", tk.END,
                        values=(item["name"],
                                item["quantity"],
                                f"₹{item['price']:.2f}",
                                f"₹{item['subtotal']:.2f}"))

        scroll = ttk.Scrollbar(frame, orient=tk.VERTICAL, command=tree.yview)
        tree.configure(yscroll=scroll.set)
        tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)

        footer = tk.Frame(self.window, bg="#e0e0e0")
        footer.pack(fill=tk.X, padx=10, pady=10)

        tk.Label(footer, text=f"Total: ₹{self.total:.2f}",
                 font=("Arial", 14, "bold"), bg="#e0e0e0").pack(pady=10)

        button_frame = tk.Frame(self.window)
        button_frame.pack(fill=tk.X, padx=10, pady=10)

        tk.Button(
            button_frame,
            text="Remove Selected",
            command=lambda: self.remove_item(tree),
            bg="#f44336",
            fg="white",
            font=("Arial", 10, "bold")
        ).pack(side=tk.LEFT, padx=5)

        tk.Button(
            button_frame,
            text="💳 Checkout",
            command=self.checkout,
            bg="#4CAF50",
            fg="white",
            font=("Arial", 10, "bold")
        ).pack(side=tk.RIGHT, padx=5)

        tk.Button(
            button_frame,
            text="Close",
            command=self.window.destroy,
            bg="#757575",
            fg="white",
            font=("Arial", 10, "bold")
        ).pack(side=tk.RIGHT, padx=5)

    def remove_item(self, tree):
        selection = tree.selection()
        if not selection:
            messagebox.showwarning("Select item", "Please select an item to remove")
            return

        name = tree.item(selection[0])["values"][0]
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
                                f"Checkout successful!\nTotal paid: ₹{result.get('total'):.2f}")
            self.window.destroy()
            self.update_callback()


class RecommendationWindow:
    def __init__(self, parent, product, recommendations):
        self.window = tk.Toplevel(parent)
        self.window.title("Product Recommendations")
        self.window.geometry("500x400")

        tk.Label(
            self.window,
            text=f"💡 Recommendations for:\n{product}",
            font=("Arial", 12, "bold")
        ).pack(pady=10)

        frame = tk.Frame(self.window)
        frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        columns = ("Product", "Price")
        tree = ttk.Treeview(frame, columns=columns, show="headings", height=12)

        tree.heading("Product", text="Product Name")
        tree.heading("Price", text="Price")

        tree.column("Product", width=350)
        tree.column("Price", width=100)

        for rec in recommendations:
            tree.insert("", tk.END, values=(rec["name"], f"₹{rec['price']:.2f}"))

        scroll = ttk.Scrollbar(frame, orient=tk.VERTICAL, command=tree.yview)
        tree.configure(yscroll=scroll.set)
        tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scroll.pack(side=tk.RIGHT, fill=tk.Y)

        tk.Button(
            self.window,
            text="Close",
            command=self.window.destroy,
            bg="#757575",
            fg="white",
            font=("Arial", 10, "bold")
        ).pack(pady=10)


def main():
    root = tk.Tk()
    ECommerceApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
