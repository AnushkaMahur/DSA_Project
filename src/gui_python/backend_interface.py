import subprocess
import os
import time

class BackendInterface:
    def __init__(self, cpp_executable="../backend_cpp/ecommerce.exe",
                 input_file="../backend_cpp/input.txt",
                 output_file="../backend_cpp/output.txt"):
        self.cpp_executable = cpp_executable
        self.input_file = input_file
        self.output_file = output_file
        
    def execute_command(self, command):
        try:
            with open(self.input_file, 'w', encoding='utf-8') as f:
                f.write(command.strip() + '\n')
            
            with open(self.output_file, 'w', encoding='utf-8') as f:
                f.write('')
            
            result = subprocess.run([self.cpp_executable],
                                   capture_output=True,
                                   text=True,
                                   timeout=5)
            
            if result.returncode != 0:
                return {"error": f"Backend error: {result.stderr.strip() or 'Execution failed'}"}
            
            time.sleep(0.1)
            
            with open(self.output_file, 'r', encoding='utf-8') as f:
                output = f.read()
            
            return self.parse_output(output, command)
        
        except subprocess.TimeoutExpired:
            return {"error": "Backend timeout"}
        except FileNotFoundError:
            return {"error": f"C++ executable not found: {self.cpp_executable}"}
        except Exception as e:
            return {"error": f"Execution error: {str(e)}"}
    
    def parse_output(self, output, command):
        lines = output.strip().split('\n')
        if not lines or not output.strip():
            return {"error": "No output from backend"}
        if lines[0].startswith("ERROR"):
            return {"error": lines[0].replace("ERROR: ", "")}
        
        cmd = command.strip().upper()

        if "SEARCHCAT" in cmd:
            return self._parse_search_results(lines)
        elif "LISTCAT" in cmd:
            return self._parse_product_list(lines)
        elif "SEARCH" in cmd:
            return self._parse_search_results(lines)
        elif "SHOWCART" in cmd:
            return self._parse_cart(lines)
        elif "CHECKOUT" in cmd:
            return self._parse_checkout(lines)
        elif "RECOMMEND" in cmd:
            return self._parse_recommendations(lines)
        elif "LISTALL" in cmd:
            return self._parse_product_list(lines)
        elif "ADD" in cmd or "REMOVE" in cmd:
            return self._parse_cart_action(lines)
        
        return {"output": output}
    
    def _parse_search_results(self, lines):
        if lines[0] == "NO_RESULTS":
            return {"products": []}
        products = []
        if lines[0] in ("SEARCH_RESULTS", "CATEGORY_SEARCH_RESULTS"):
            for line in lines[1:]:
                if line in ("SEARCH_END", "CATEGORY_SEARCH_END"):
                    break
                parts = line.split('|')
                if len(parts) >= 4:
                    products.append({
                        "name": parts[0],
                        "price": float(parts[1]),
                        "stock": int(parts[2]),
                        "category": parts[3]
                    })
        return {"products": products}
    
    def _parse_cart(self, lines):
        if lines[0] == "CART_EMPTY":
            return {"items": [], "total": 0.0}
        items = []
        total = 0.0
        if lines[0] == "CART_START":
            for line in lines[1:]:
                if line == "CART_END":
                    continue
                if line.startswith("TOTAL:"):
                    total = float(line.split(":")[1].strip())
                    break
                parts = line.split('|')
                if len(parts) >= 4:
                    items.append({
                        "name": parts[0],
                        "quantity": int(parts[1]),
                        "price": float(parts[2]),
                        "subtotal": float(parts[3])
                    })
        return {"items": items, "total": total}
    
    def _parse_checkout(self, lines):
        if lines[0] == "CHECKOUT_SUCCESS":
            total = 0.0
            if len(lines) > 1 and lines[1].startswith("TOTAL_PAID:"):
                total = float(lines[1].split(":")[1].strip())
            return {"success": True, "total": total}
        return {"success": False, "error": lines[0]}
    
    def _parse_recommendations(self, lines):
        if lines[0] == "NO_RECOMMENDATIONS":
            return {"recommendations": []}
        recs = []
        if lines[0] == "RECOMMENDATIONS":
            for line in lines[1:]:
                if line == "RECOMMEND_END":
                    break
                parts = line.split('|')
                if len(parts) >= 2:
                    recs.append({
                        "name": parts[0],
                        "price": float(parts[1])
                    })
        return {"recommendations": recs}
    
    def _parse_product_list(self, lines):
        products = []
        if lines[0] in ("ALL_PRODUCTS", "CATEGORY_PRODUCTS"):
            for line in lines[1:]:
                if line in ("PRODUCTS_END", "CATEGORY_PRODUCTS_END"):
                    break
                parts = line.split('|')
                if len(parts) >= 4:
                    products.append({
                        "name": parts[0],
                        "price": float(parts[1]),
                        "stock": int(parts[2]),
                        "category": parts[3]
                    })
        return {"products": products}
    
    def _parse_cart_action(self, lines):
        if lines[0].startswith("SUCCESS"):
            return {"success": True, "message": lines[0].replace("SUCCESS: ", "")}
        elif lines[0].startswith("ERROR"):
            return {"success": False, "error": lines[0].replace("ERROR: ", "")}
        return {"output": '\n'.join(lines)}
