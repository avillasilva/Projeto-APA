# capacitated-vehicle-routing-problem

In this problem, a fleet of vehicles is used to visit a set of customers delivering products.  Each vehicle has the same capacity and each customer requires a certain amount of products. The goal is to create a set of routes (one for each vehicle) in which each customer is visited exactly once, all demands are met without exceeding vehicle capacity, and transportation costs are minimized. 
Note that this cost is given by the total distance traveled. Each vehicle must start and end its route at the same point, called a deposit.

### Test instances

- The number of vertices can be found in the DIMENSION field.
- The capacity of each vehicle can be consulted in the CAPACITY field.
- The cost matrix can be found in the EDGE_WEIGHT_SECTION field.
- Vertex 0 is the deposit.
- Consider that there is no limit to the number of vehicles.
