import unittest2 as unittest
from tinycu import *

class MapTests(unittest.TestCase):
    
    def test_full(self):
        class MyMapTest(TinyCu):
            def func(self, a):
                return 2*a
            def __call__(self, *args):
                return map(self.func, [1,2,3,4])

        b = MyMapTest()
        print b.intercept_call()
        self.assertEqual(b(), [2,4,6,8])


    def test_2arity(self):
        class MyMapTest(TinyCu):
            def func(self, a, b):
                return a + b

            def __call__(self, *args):
                return map(self.func, [1,2,3,4], [5,6,7,8])

        b = MyMapTest()
        self.assertEqual(b.intercept_call(), [6,8,10,12])


if __name__ == '__main__':
    unittest.main()

