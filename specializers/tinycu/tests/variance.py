from tinycu import *

class GetSum(TinyCu):
    def __call__(self, x):
        return sum(x)

class GetVariance(TinyCu):
    def __init__(self, avg):
        self.avg = avg
        super(GetVariance, self).__init__()

    def func(self, x):
        return (x-self.avg) * (x-self.avg)

    def __call__(self, x):
        return map(self.func, x)



arr = [1, 2, 3, 4]
sum = GetSum().intercept_call(arr)
avg = sum[0]/len(arr)

result = GetVariance(avg).intercept_call(arr)



print "\n\n Result: ", result