from tinycu import *

class DotProduct(TinyCu):
    def __call__(self, x, y):
        return sum(map(self.f, x, y))

    def f(self, a, b):
        return a * b

result =  DotProduct().intercept_call([1,2,3,4], [5,6,7,8])

print "\n\n Result: ", result