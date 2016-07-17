# Insertion
Best: O(n) Average, Worst: O(n ** 2)

For (small | partially) sorted data.

# Selection
Best, Average, Worst: O((n * (n + 1) / 2) - 1)

Slow, but the principle to pick up the maximum element can be applied as heap
sort.

# Heap
Best, Average, Worst: O(n * log(n))

Works in cases Quicksort performs badly (e.g. an initially sorted array)

# Quick
Best, Average: O(n * log(n)), Worst: O(n ** 2)

Best in average, but not always good. Consumes stack as partitions get created.

# Bucket
Bes, Average, Worst: O(n)

Efficient if hash algorithm can distribute contents equally.

# Merge
Best, Average, Worst: O(n * log(n))

Always O(n log n), but requires stack and O(n) heap.
