import unittest
from src.gui_python.backend_interface import BackendInterface

class TestBackendInterface(unittest.TestCase):

    def setUp(self):
        self.backend = BackendInterface()

    def test_search_returns_list(self):
        results = self.backend.search_products("lap")
        self.assertIsInstance(results, list)

    def test_backend_command(self):
        response = self.backend.run_command("SEARCH laptop")
        self.assertTrue(isinstance(response, str))

if __name__ == "__main__":
    unittest.main()
