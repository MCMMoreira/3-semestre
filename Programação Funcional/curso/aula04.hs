import Data.Time.Format.ISO8601 (yearFormat)
resto a b 
    |a > b = resto (a-b) b
    |a == b = 0
    |a < b = a

multi a b 
    |b > 1 = a + multi a (b-1)
    |b == 0 = 0
    |b == 1 = a

mdc x y
    |x > y = mdc(x-y) y
    |x < y = mdc y x
    |x == y = x 
    
fib n a1 a2
    |n == 0 = a1
    |n == 1 = a2
    |n > 1 = fib (n-1) a2 (a1+a2)
