## Introduction

General idea is to simulate a very successful borscht restaurant. Borscht is versatile, tasty, and generally very popular. There will be three main roles that a thread can play:

1. Customer, who arrive at the restaurant, wait to be seated, order, eat, then leave.
2. Wait staff, who wait for customers, seat them in their assigned sections, communicate with the cook, and serve customers food
3. Cooks, who receive orders, prepare bowls of borscht and give them to the wait staff

### Thread roles in detail
Each thread has a set of valid interactions with other threads. These interactions are affected by the global state of the restaurant.

#### Customer state
The behavior of a customer is as follows:
- When they arrive they would like to be seated. 
- Once they are seated, they would like to have some number of bowls of borscht, but, being easily embarrassed, would only like to have a single bowl of borscht at their table at any given time.
- Once they have eaten their fill, they leave, making sure to tip the waitstaff before they go (borscht is otherwise provided free of charge to the community).

#### Wait staff state
Each member of the wait staff has a section of seats which they are responsible for. For example,  if the set of seats in the restaurant is 
$\{0,1,2,3\}$ then waiter one may be responsible for $\{0,1\}$ and waiter two for $\{2,3\}$. These sets are *disjoint*.



The behavior of the wait staff is as follows:
- If there are customers waiting, they can seat that customer.
    - Wait staff cannot seat a customer if they have no more seats in their section
    - Everyone on the wait staff is a team player, and knows that service is much more efficient if the workload is evenly distributed.
- Once they seat a customer, they ask the customer how many bowls of borscht they will be having.
    - The waitstaff is very in touch with the community and knows how each customer likes their borscht. See Cook State for more information on dealing with borscht type.
- The wait staff informs the kitchen of the customer orders (it's borscht)
- Once a bowl of borscht is ready for the customer, the wait staff serves the customer until they have had their requested number of bowls of borscht (never bringing another until the customer has finished the last). 
- Once the customer finishes, tips the staff, and leaves, the wait staff must clean the table before seating another customer there 

It is important to note that the above behavior must happen *in sequence* for each *customer*, but not necessarily for the *wait staff*

#### Cook state
Each member of the kitchen is responsible for preparing borscht.
The members of this kitchen are very organized and after many years of preparing borscht, have determined that there are generally a few *distinct*  ways to prepare a bowl. 

The behavior of the cooks is as follows:
- Cooks receive orders from wait staff, prepare some number of bowls borscht, then inform the appropriate member of the wait staff once the order is "up".
    - A cook may prepare multiple bowls of borscht at time, but only if they are of the same *type*
    - For example, a cook may `prepare(1, 3)` for preparing `3` bowls of type `1` or `prepare(1)` for just a single bowl of type `1`. 
    - Notice that it is much more efficient to prepare bowls of `borscht` in batches!



