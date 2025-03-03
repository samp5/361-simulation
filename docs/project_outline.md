## requirements
- threads have different roles
- scalability (in that scale of difficulty)
- complex information exchange
- end state
- in C

## teaching lessons
- scheduling of threads and their control flow
- synchronization of threads on shared memory

## ideas
- restaurant 
    - cook threads 
    - waiter threads 
        - waiter takes order
    - customers
        - wait to seat
    
- ships ports 
    - ship thread
    - port thread

## implementation
- start 
- simulation
```
                                            not ended
                                    _______________________
                                   |                       |                ended
start + simulation ----> execute simulation step --> evaluate end condtion ----> end state
|            |
|            |
 ---------> logfile
```
- need a mechanism for debugging and logging errors
    - needed for student debugging

## for next time:
- thread roles
- thread interactions
- start and stop states

### file structure

- email: prof email

```
simulator-app/
├─ simulator/
│  ├─ sim/
│  │  ├─ main.c
│  │  ├─ contollers.h.c
│  ├─controllers.c
│  ├─sim.h
├─ docs/ 
├─ .gitignore
├─ README.md


- simulator
    |- main.c           # all api definitions, starting and managing simulator, calls student implemented functions 
    |- sim.h            # header for student visible api
    |- controllers.c    # funcs that students write for controllers, init function for students to initialize their datastructures
```

- `init` function initializes student memory -- call this function 
