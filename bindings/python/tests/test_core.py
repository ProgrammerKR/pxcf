import unittest
import pxcf.core
import pxcf

class TestPxcf(unittest.TestCase):
    def test_load_string(self):
        source = """
        server {
            host: "localhost"
            port: 8080
            active: true
            ssl: null
        }
        """
        doc = pxcf.loads(source)
        self.assertEqual(doc["server"]["host"], "localhost")
        self.assertEqual(doc["server"]["port"], 8080)
        self.assertTrue(doc["server"]["active"])
        self.assertIsNone(doc["server"]["ssl"])

    def test_invalid_syntax(self):
        with self.assertRaises(SyntaxError):
            pxcf.loads("invalid { { }")

    def test_arrays(self):
        doc = pxcf.loads("arr: [1, 2, 3, 4.5]")
        self.assertEqual(doc["arr"], [1, 2, 3, 4.5])

    def test_duplicate_keys(self):
        with self.assertRaises(SyntaxError) as context:
            pxcf.loads("server: 1 server: 2")
        self.assertTrue("Duplicate key" in str(context.exception))
        
    def test_dumps(self):
        obj = {
            "server": {
                "port": 8080,
                "host": "localhost",
                "active": True,
                "ssl": None
            },
            "arr": [1, 2, 3]
        }
        out = pxcf.dumps(obj)
        self.assertTrue("port: 8080" in out)
        self.assertTrue("active: true" in out)
        
        # Roundtrip
        doc = pxcf.loads(out)
        self.assertEqual(doc["server"]["port"], 8080)
        self.assertEqual(doc["arr"], [1, 2, 3])

if __name__ == '__main__':
    unittest.main()
