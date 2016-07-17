# Minimax
Best, Average, Worst: O(b ** ply)

Primitive.

# Negmax
Best, Average, Worst: O(b ** ply)

It doesn't have to consider whether the turn is for the player or the opponent.

# AlphaBeta
Best, Average: O (b ** (ply / 2))

It stops exploring sibling nodes when worst score possible by opponent equals or
exceeds our maximum threshold.
