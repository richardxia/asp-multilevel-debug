import unittest2 as unittest
from tinycu import *

class SumTests(unittest.TestCase):
    
    def test_full(self):
        class MySumTest(TinyCu):
            def __call__(self, *args):
                return sum([1,2,3,4])

        b = MySumTest()
        result =  b.intercept_call()
        self.assertEqual(result, [10.0])


if __name__ == '__main__':
    unittest.main()

