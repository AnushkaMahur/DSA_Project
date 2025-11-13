# ecommerce_gui_themable.py
import tkinter as tk
from tkinter import ttk, messagebox
import sys
import os
from backend_interface import BackendInterface

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
BACKEND_DIR = os.path.join(BASE_DIR, "..", "backend_cpp")

# Try to use ttkbootstrap for modern themes. If not installed, fall back to ttk.
USE_BOOTSTRAP = False
try:
    import ttkbootstrap as tb
    from ttkbootstrap.constants import *
    USE_BOOTSTRAP = True
except Exception:
    USE_BOOTSTRAP = False

class ThemedFactory:
    """
    Small wrapper to create themed widgets consistently whether ttkbootstrap is available
    or not. It centralizes creation so you can switch style names easily.
    """
    def __init__(self, root, style_name=None):
        self.root = root
        if USE_BOOTSTRAP:
            self.style = tb.Style(theme="flatly")

        # ---- Treeview selection color patch ----
            self.style.configure("Treeview",
                            rowheight=28,
                            font=("Arial", 11))
            self.style.map("Treeview",
                    background=[("selected", "#cce7ff")],
                    foreground=[("selected", "black")])

        else:
            self.style = ttk.Style()
            self.style.theme_use("clam")   # better control

        # ---- Treeview selection color patch ----
            self.style.configure("Treeview",
                                 background="white",
                                fieldbackground="white",
                                foreground="black",
                                rowheight=28)
            self.style.map("Treeview",
                        background=[("selected", "#cce7ff")],
                        foreground=[("selected", "black")])

    def label(self, parent, **kwargs):
        if USE_BOOTSTRAP:
            return ttk.Label(parent, **kwargs)
        return ttk.Label(parent, **kwargs)

    def button(self, parent, text, command=None, width=None, bootstyle=None, style=None, **kwargs):
        if USE_BOOTSTRAP:
            # use bootstyle to control color (e.g., 'success', 'primary')
            bs = bootstyle or "secondary"
            return ttk.Button(parent, text=text, command=command, width=width, bootstyle=bs, **kwargs)
        else:
            st = style or "Accent.TButton"
            if width:
                return ttk.Button(parent, text=text, command=command, width=width, style=st, **kwargs)
            return ttk.Button(parent, text=text, command=command, style=st, **kwargs)

    def entry(self, parent, **kwargs):
        return ttk.Entry(parent, **kwargs)

    def combobox(self, parent, **kwargs):
        return ttk.Combobox(parent, **kwargs)

    def spinbox(self, parent, **kwargs):
        # ttk Spinbox is available in recent tkinter - fallback to tk.Spinbox if needed
        try:
            return ttk.Spinbox(parent, **kwargs)
        except Exception:
            return tk.Spinbox(parent, **kwargs)

class ECommerceApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Smart E-Commerce System")
        self.root.geometry("1200x750")

        # theme: if ttkbootstrap available, default to 'flatly' (clean). otherwise keep default.
        if USE_BOOTSTRAP:
            self.style = tb.Style(theme="flatly")
        else:
            self.style = ttk.Style()

        # Configure some fallback styles when ttkbootstrap isn't present
        if not USE_BOOTSTRAP:
            # Accent button
            try:
                self.style.configure("Accent.TButton",
                                     foreground="white",
                                     font=("Arial", 10, "bold"))
                self.style.map("Accent.TButton",
                               foreground=[('active', 'white')],
                               background=[('active', '#4CAF50')])
            except Exception:
                pass

        self.factory = ThemedFactory(root)

        backend_path = os.path.join(BACKEND_DIR, "ecommerce.exe")
        input_file = os.path.join(BACKEND_DIR, "input.txt")
        output_file = os.path.join(BACKEND_DIR, "output.txt")

        self.backend = BackendInterface(
            cpp_executable=backend_path,
            input_file=input_file,
            output_file=output_file
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
        # Top frame - give it subtle background by using a Frame with relief
        self.top_frame = tk.Frame(self.root, bg="#f5f7fa", height=70)
        self.top_frame.pack(fill=tk.X, side=tk.TOP)

        title_label = self.factory.label(
            self.top_frame,
            text="🛒 Smart E-Commerce System",
            font=("Arial", 20, "bold"),
            background="#f5f7fa"
        )
        title_label.pack(side=tk.LEFT, padx=20)

        self.search_entry = self.factory.entry(self.top_frame, font=("Arial", 12), width=40)
        self.search_entry.pack(side=tk.LEFT, padx=10)
        self.suggestion_box = None
        self.search_entry.bind("<KeyRelease>", self.update_autocomplete)


        search_btn = self.factory.button(self.top_frame, text="Search", width=12, command=self.search_products,
                                         bootstyle="success") if USE_BOOTSTRAP else self.factory.button(self.top_frame, text="Search", width=12, command=self.search_products, style="Accent.TButton")
        search_btn.pack(side=tk.LEFT, padx=5)

        filters_btn = self.factory.button(self.top_frame, text="🔍 Filters", width=12, command=self.open_filter_window,
                                          bootstyle="primary" if USE_BOOTSTRAP else None)
        filters_btn.pack(side=tk.LEFT, padx=5)

        sort_frame = tk.Frame(self.top_frame, bg="#f5f7fa")
        sort_frame.pack(side=tk.LEFT, padx=8)
        tk.Label(sort_frame, text="Sort By:", bg="#f5f7fa").pack(side=tk.LEFT)

        self.sort_dropdown = self.factory.combobox(
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

        apply_sort_btn = self.factory.button(sort_frame, text="Apply Sort", command=self.apply_sort,
                                             bootstyle="warning" if USE_BOOTSTRAP else None)
        apply_sort_btn.pack(side=tk.LEFT, padx=5)

        refresh_btn = self.factory.button(sort_frame, text="Refresh", command=self.refresh_products,
                                          bootstyle="secondary" if USE_BOOTSTRAP else None)
        refresh_btn.pack(side=tk.LEFT, padx=5)

        # Body
        self.body_frame = tk.Frame(self.root, bg="#ffffff")
        self.body_frame.pack(fill=tk.BOTH, expand=True)

        # Sidebar
        self.sidebar = tk.Frame(self.body_frame, width=240, bg="#e9f4ff")
        self.sidebar.pack(side=tk.LEFT, fill=tk.Y)

        sidebar_title = self.factory.label(self.sidebar, text="📚 Categories",
                                           font=("Arial", 14, "bold"),
                                           background="#e9f4ff")
        sidebar_title.pack(pady=12)

        all_btn = self.factory.button(self.sidebar, text="🛍 All Products", width=22,
                                      command=self.load_all_products,
                                      bootstyle="success" if USE_BOOTSTRAP else None)
        all_btn.pack(pady=(5, 12), padx=12)

        # Category buttons
        for cat in self.categories:
            btn = self.factory.button(self.sidebar, text=cat, width=22,
                                      command=lambda c=cat: self.load_category_products(c),
                                      bootstyle="info" if USE_BOOTSTRAP else None)
            btn.pack(pady=6, padx=12)

        # Bottom sidebar actions
        self.sidebar_bottom = tk.Frame(self.sidebar, bg="#e9f4ff")
        self.sidebar_bottom.pack(side=tk.BOTTOM, fill=tk.X, pady=18)

        self.cart_sidebar_btn = self.factory.button(
            self.sidebar_bottom,
            text="🛒 View Cart (0)",
            width=22,
            command=self.show_cart,
            bootstyle="warning" if USE_BOOTSTRAP else None
        )
        self.cart_sidebar_btn.pack(pady=8)

        self.rec_sidebar_btn = self.factory.button(
            self.sidebar_bottom,
            text="💡 Recommendations",
            width=22,
            command=self.show_recommendations,
            bootstyle="secondary" if USE_BOOTSTRAP else None
        )
        self.rec_sidebar_btn.pack(pady=8)

        # Content area
        self.content_frame = tk.Frame(self.body_frame, bg="#f6f6f6")
        self.content_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=12, pady=12)

        # Status bar
        self.status_label = ttk.Label(self.root, text="Ready", font=("Arial", 10), anchor=tk.W, relief=tk.SUNKEN)
        self.status_label.pack(side=tk.BOTTOM, fill=tk.X)


    # --- existing logic unchanged, only minor adjustments to use factory widgets where needed ---
    def update_autocomplete(self, event):
        query = self.search_entry.get().strip()
        if not query:
            self.hide_suggestions()
            return

        result = self.backend.execute_command(f"AUTOCOMP {query}")
        suggestions = result.get("suggestions", [])

        if not suggestions:
            self.hide_suggestions()
            return

        self.show_suggestions(suggestions)

    def hide_suggestions(self):
        if self.suggestion_box:
            self.suggestion_box.destroy()
            self.suggestion_box = None

    def show_suggestions(self, suggestions):
        self.hide_suggestions()
        box = tk.Listbox(self.top_frame, height=min(6, len(suggestions)))
        for s in suggestions:
            box.insert(tk.END, s)
        box.place(x=self.search_entry.winfo_x(), y=self.search_entry.winfo_y() + 30)
        box.bind("<<ListboxSelect>>", lambda e: self._select_suggestion(box))
        self.suggestion_box = box

    def _select_suggestion(self, box):
        sel = box.curselection()
        if not sel:
            return
        value = box.get(sel[0])
        self.search_entry.delete(0, tk.END)
        self.search_entry.insert(0, value)
        self.hide_suggestions()
        self.search_products()

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
        ttk.Entry(win, textvariable=self.filter_min_price).pack()

        tk.Label(win, text="Max Price").pack(pady=5)
        ttk.Entry(win, textvariable=self.filter_max_price).pack()

        tk.Label(win, text="Brand").pack(pady=5)
        ttk.Entry(win, textvariable=self.filter_brand).pack()

        apply_btn = self.factory.button(
            win, text="Apply Filters",
            command=lambda: [self.apply_filters(), win.destroy()],
            bootstyle="success" if USE_BOOTSTRAP else None
        )
        apply_btn.pack(pady=20)

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
        ttk.Label(self.content_frame, text="Filtered Results", font=("Arial", 18, "bold"), background="#f6f6f6").pack(pady=10)

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
        ttk.Label(self.content_frame, text="Sorted Results", font=("Arial", 18, "bold"), background="#f6f6f6").pack(pady=10)

        self.load_filtered_table(products)

    def load_filtered_table(self, products):
        product_frame = ttk.LabelFrame(self.content_frame, text="Results")
        product_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        columns = ("Name", "Price", "Stock", "Category")
        self.products_tree = ttk.Treeview(product_frame, columns=columns, show="tree headings", height=15)

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
            self.products_tree.insert("", tk.END, text=str(idx),
                                      values=(p["name"], f"₹{p['price']:.2f}", p["stock"], p.get("category", "")))

    def clear_content(self):
        for w in self.content_frame.winfo_children():
            w.destroy()

    def show_default_message(self):
        self.clear_content()
        ttk.Label(self.content_frame, text="Welcome! Select a category from the left sidebar.",
                  font=("Arial", 16, "bold")).pack(pady=200)

    def load_category_products(self, category):
        self.current_category = category
        self.clear_content()

        ttk.Label(self.content_frame, text=f"Category: {category}", font=("Arial", 18, "bold")).pack(pady=10)

        product_frame = ttk.LabelFrame(self.content_frame, text="Products")
        product_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        columns = ("Name", "Price", "Stock")
        self.products_tree = ttk.Treeview(product_frame, columns=columns, show="tree headings", height=15)

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

        control_frame = tk.Frame(self.content_frame, bg="#f6f6f6")
        control_frame.pack(fill=tk.X, pady=10)

        ttk.Label(control_frame, text="Quantity:", font=("Arial", 10)).pack(side=tk.LEFT)

        self.quantity_var = tk.StringVar(value="1")
        sb = self.factory.spinbox(control_frame, from_=1, to=10, textvariable=self.quantity_var, width=5)
        sb.pack(side=tk.LEFT, padx=5)

        add_btn = self.factory.button(control_frame, text="➕ Add to Cart", command=self.add_to_cart,
                                      bootstyle="success" if USE_BOOTSTRAP else None)
        add_btn.pack(side=tk.LEFT, padx=5)

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

        ttk.Label(self.content_frame, text="All Available Products", font=("Arial", 18, "bold")).pack(pady=10)

        product_frame = ttk.LabelFrame(self.content_frame, text="All Products")
        product_frame.pack(fill=tk.BOTH, expand=True, pady=10)

        columns = ("Name", "Price", "Stock", "Category")
        self.products_tree = ttk.Treeview(product_frame, columns=columns, show="tree headings", height=15)

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

        control_frame = tk.Frame(self.content_frame, bg="#f6f6f6")
        control_frame.pack(fill=tk.X, pady=10)

        ttk.Label(control_frame, text="Quantity:", font=("Arial", 10)).pack(side=tk.LEFT)

        self.quantity_var = tk.StringVar(value="1")
        sb = self.factory.spinbox(control_frame, from_=1, to=10, textvariable=self.quantity_var, width=5)
        sb.pack(side=tk.LEFT, padx=5)

        add_btn = self.factory.button(control_frame, text="➕ Add to Cart", command=self.add_to_cart,
                                      bootstyle="success" if USE_BOOTSTRAP else None)
        add_btn.pack(side=tk.LEFT, padx=5)

        result = self.backend.execute_command("LISTALL")
        self.display_all_products(result.get("products", []))

    def display_all_products(self, products):
        if not hasattr(self, "products_tree"):
            return

        for i in self.products_tree.get_children():
            self.products_tree.delete(i)

        for idx, p in enumerate(products, 1):
            self.products_tree.insert("", tk.END, text=str(idx),
                                      values=(p["name"], f"₹{p['price']:.2f}", p["stock"], p.get("category", "")))

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
            self.products_tree.insert("", tk.END, text=str(idx),
                                      values=(p["name"], f"₹{p['price']:.2f}", p["stock"]))

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
        ttk.Label(self.window, text="🛒 Your Shopping Cart", font=("Arial", 16, "bold")).pack(pady=10)

        if not self.items:
            ttk.Label(self.window, text="Your cart is empty", font=("Arial", 12)).pack(pady=50)
            close_btn = ttk.Button(self.window, text="Close", command=self.window.destroy)
            close_btn.pack(pady=10)
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

        ttk.Label(footer, text=f"Total: ₹{self.total:.2f}", font=("Arial", 14, "bold")).pack(pady=10)

        button_frame = tk.Frame(self.window)
        button_frame.pack(fill=tk.X, padx=10, pady=10)

        remove_btn = ttk.Button(button_frame, text="Remove Selected", command=lambda: self.remove_item(tree))
        remove_btn.pack(side=tk.LEFT, padx=5)
        checkout_btn = ttk.Button(button_frame, text="💳 Checkout", command=self.checkout)
        checkout_btn.pack(side=tk.RIGHT, padx=5)
        close_btn = ttk.Button(button_frame, text="Close", command=self.window.destroy)
        close_btn.pack(side=tk.RIGHT, padx=5)

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

        ttk.Label(self.window, text=f"💡 Recommendations for:\n{product}", font=("Arial", 12, "bold")).pack(pady=10)

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

        close_btn = ttk.Button(self.window, text="Close", command=self.window.destroy)
        close_btn.pack(pady=10)

def main():
    root = tk.Tk()
    ECommerceApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
