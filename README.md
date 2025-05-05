## Introduction

The general idea of this simulation is to simulate a very successful borscht restaurant. 
Borscht is versatile, tasty, and generally very popular.
There will be three main roles that a thread can play in the simulation:

1. Customer, who arrive at the restaurant, wait to be seated, order, eat, then leave.
2. Wait staff, who wait for customers, seat those customers in their assigned sections, and serve customers food
3. Cooks, who receive orders, prepare bowls of borscht and give them to the wait staff

### Thread roles in detail
Each thread has a set of valid interactions with other threads. These interactions are affected by the global state of the restaurant.
The specifics of how a thread may manipulate the state of the restaurant is located in the function comments in `sim.h`.

#### Customer state
The behavior of a customer is as follows:

- When they arrive they need to be seated. See the wait staff thread information for this API call.

- Like all restaurants, there is a queue and (barring some absurd social behaviors) the customer at the front of the queue is always seated first.
The current state of the queue at the restaurant is accessible through the API via `get_waiting_line(customer_id**, size_t*)`.

- Once a customer is seated, they would like to have some number of bowls of borscht, $n$, of some preparation type, $t$.
    - However, being easily embarrassed, customers would only like to have a **single** bowl of borscht at their table at any given time.
    - That is, a customer must **finish eating** the current bowl of borscht before a member of the wait staff brings another bowl (if needed).
    - A customer thread can eat via `eat(customer_id)`.
    - A customer **must** eat $n$ bowls of borscht before leaving.

- Once they have eaten their fill, they leave, making sure to tip the waitstaff before they go (borscht is otherwise provided free of charge to the community).
    - A customer thread can leave via `leave(customer_id)`.

#### Wait staff state
Each member of the wait staff has a section of seats in the restaurant for which they are responsible. 
For example,  if the set of seats in the restaurant is 
$\{0,1,2,3\}$ and there are two members of the wait staff then one may be responsible for $\{0,1\}$ and the other for $\{2,3\}$. These sets are *disjoint*.


Waitstaff actions are much different than the customer actions in that they can be in any order as long as no action violates the state of the restaurant.

The possible actions of the wait staff are as follows:
- They can seat a customer via `seat(waitstaff_id, customer_id, table_id)`
    - A member of the wait staff can only seat a customer in *their* section. 
    - They must seat the customer at the front of the queue
    - They **cannot** seat a customer at a dirty table.
    - They **cannot** seat a customer at a table which is already occupied
    - *Hint:* Everyone on the wait staff is a team player, and knows that service is much more efficient if the workload is evenly distributed.

- Being very in touch with their customer base (and the community at large), they can intuit a customers borscht preference via `intuit_preference(customer_id)`
    - Note that this customer need not be in any particular state other than **arrived at the restaurant**.

- They can take a customers order via `take_order(waitstaff_id, customer_id, int)`
    - Although they know what *type* of borscht a customer likes, the waitstaff is not so experienced that they can intuit how hungry a customer is
    - In order for a customer's order to be taken they must
        - Be seated in this waitstaff's section
        - Not have already ordered 


- They can pick up borscht from the kitchen via `pick_up_borsht(waitstaff_id, borscht_type, int)`
    - This call will result in an invalid state if the kitchen does not have the requested quantity of the specified borscht type available.

- They can serve borscht to a customer via `serve(waitstaff_id, table_id, borscht_type)`
    - This customer must
        - Not be eating
        - Have ordered
        - Be seated in this wait staff member's section
        - Not yet have eaten their desired quantity of borscht
    - The member of the wait staff must be carrying at least one bowl of the specified borscht type. See `pick_up_borsht(waitstaff_id, borscht_type, int)`

- They can clean a table via `clean_table(waitstaff_id, table_id)`
    - This table must:
        - Not be occupied


Note that members of the wait staff must inform the kitchen of the customer preference and desired quantity in some way, However,

- There are **no** API calls for this.
- A data structure to track orders must be created 
- *Note:* A cook thread's call to `prepare(borscht_type, int)` must not exceed the ordered amount of borscht at that time. See the Cook thread description for more detailed information.

#### Cook state
Each member of the kitchen is responsible for preparing borscht.
The members of this kitchen are very organized and after many years of preparing borscht, have determined that there are generally a few *distinct*  ways to prepare a bowl, this is the `borscht_type` seen throughout the API. The number of types of borscht can be obtained via `types_of_borsht()`

The behavior of the cooks is as follows:
- Cooks receive orders from wait staff through some data structure, prepare some number of bowls borscht, then inform the appropriate member of the wait staff once the order is "up".
    - A cook may prepare multiple bowls of borscht at time, but only if they are of the same *type*
    - For example, a cook may `prepare(1, 3)` for preparing `3` bowls of type `1` or `prepare(1)` for just a single bowl of type `1`. 
    - *Note* that it is much more efficient to prepare bowls of `borscht` in batches!


This API call will fail if *the total number of bowls prepared of some specified type after this call will exceed the total number of bowls of that type ordered*.
This rule is to prevent front loading of the call! Being able to prepare, say, 10,000 bowls of each type right when the restaurant opens would sure make that `prepare` call efficient, but, might result in (unforgivable) w@st#d borscht! 

Consider the small sequence of actions for clarification on when this call may fail:
- The restaurant has 1 customer, `customer1`, and one waiter `waitstaff1`.
- `waitstaff1` seats `customer1` then seats `customer2`
- A call to `prepare` with a `quantity` argument greater than 0 will **fail** 
- `waitstaff1` takes the order of `customer1`, they want `2` bowls of type `1`.
- A call to `prepare` with a `quantity` of `2` and type `1` will **not** fail
- A subsequent call to `prepare` with a `quantity` of `2` and type `1` will **fail**
- A call to `prepare` with a `quantity` greater than zero and any type other than `1` will **fail**



