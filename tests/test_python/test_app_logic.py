import unittest
from src.gui_python.app import filter_products_by_price

class TestAppLogic(unittest.TestCase):

    def test_price_filter(self):
        products = [
            {"name": "Laptop", "price": 50000},
            {"name": "Mouse", "price": 500},
            {"name": "Keyboard", "price": 1500}
        ]

        filtered = filter_products_by_price(products, 1000)

        self.assertEqual(len(filtered), 1)
        self.assertEqual(filtered[0]["name"], "Mouse")

if __name__ == "__main__":
    unittest.main()
