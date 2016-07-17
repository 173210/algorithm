# Depth-First Search
Best, Average, Worst: O(V + E)

Can be applied for other algorithms such as topological sort.

Has no global awareness.

The path may not be the shortest.

# Breadth-First Search
Best, Average, Worst: O(V + E)

Find the shortest path, but may evaluate more nodes.

# Dijkstra's
Best, Average, Worst: O((V + E) * log(V))

For path-finding. Requiring O(V * 3) heap. Negative weights is not applicatable.

# Dijkstra's for Dense Graphs
Best, Average, Worst: O(V ** 2 + E)

Requiring O(V * 2) heap

# Bell-Ford
Best, Average, Worst: O(V * E)

For path-finding. Can be applied for edges with negative weights.

# Floyd-Warshall
Best, Average, Worst O(V *** 3)

For path-finding. Makes a table showing the distance betweeen two points.

# Prim's
Best, Average, Worst: O((V + E) * log(V))

To compute MST (Minimum Spanning Tree).
