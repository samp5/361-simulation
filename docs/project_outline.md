## project requirements
- threads have different roles
- scalability (in terms of difficulty)
- complex information exchange between threads
- clearly defined end state
- in C

## teaching lessons
- scheduling of threads and their control flow
- synchronization of threads on shared memory

## ideas
- restaurant 
    - cook threads 
        - chef waits for orders       
        - can cook so many orders at any given time
    - waiter threads 
        - wait to seat customers
        - waiter takes order
        - takes orders to chef
    - customers
        - wait to be seated

## general flow

```
                                        if not ended
                                    -<---------<---------<
                                   |                       |               ended
start, simulation ----> execute simulation step --> evaluate end condtion ------> end state
|            |                     |                       |
|            |                     |                       |
 -------------------------------------------------------------->logfile
```
- need a mechanism for debugging and logging errors
    - needed for student debugging

## for next time:
- thread roles
- thread interactions
- start and stop states

### file structure

```
simulator-app/
├─ simulator
│  ├─ sim/  
│  │  ├─ main.c          // all api defintions, starting and stopping sim
│  │  ├─ contollers.h  // function declarations for student functions
│  ├─controllers.c       // student code + init function
│  ├─sim.h               // api function declarations
├─ docs/                 // documentation and updates
├─ .gitignore            
├─ README.md

```

- `init` function initializes allows students to do any setup needed for their data structures
