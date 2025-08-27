
# Philosophers Project: Solution Architecture Design

## 1. Problem Definition and Constraints

The Dining Philosophers Problem is a classic synchronization problem in computer science that illustrates the challenges of resource allocation among multiple concurrent processes or threads. In this project, we aim to simulate this problem with specific constraints imposed by the 42 School curriculum and the user's request:

*   **Entities:** A fixed number of philosophers and an equal number of forks arranged in a circle.
*   **Philosopher Actions:** Each philosopher cycles through states: thinking, eating, and sleeping. To eat, a philosopher requires two forks (one to their left and one to their right).
*   **Argument Count:** The program must accept either 5 or 6 command-line arguments.
*   **Argument Validation:** All arguments must be valid non-negative numbers. Invalid or negative arguments should result in an error.
*   **Philosopher Death:** A philosopher dies if they do not eat within a specified `time_to_die` period. This condition must be accurately monitored and reported.
*   **Synchronization:** The solution must use at least two mutexes for managing philosophers and forks to prevent data races and race conditions.
*   **Deadlock Prevention:** The implementation must actively prevent deadlocks, a common issue in this problem where all philosophers might starve due to circular waiting for resources.
*   **42 Norm Rules:** The entire codebase must strictly adhere to the 42 School's coding standards and norm rules, including function length, line width, naming conventions, and forbidden functions/operators.
*   **Code Uniqueness:** The implementation should strive for uniqueness in its approach while meeting all functional and norm requirements.
*   **Testing:** The solution should be testable with open-source testers.

## 2. Argument Parsing and Validation

The program will accept command-line arguments in the following format:

`./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]`

### Validation Steps:

1.  **Argument Count:** Check if the `argc` (argument count) is either 5 or 6. If not, print an error message and exit.
2.  **Numeric Conversion:** Attempt to convert each argument (from `argv`) to a long integer. Use a robust conversion function that handles potential non-numeric characters or overflow.
3.  **Non-Negative Check:** After successful conversion, verify that all numeric arguments are strictly positive (greater than 0). The `number_of_philosophers` must also be at least 1.
4.  **Error Handling:** If any validation fails, print a clear, informative error message to `stderr` and exit with a non-zero status code.

## 3. Data Structures

To manage the state of the simulation, the following core data structures will be defined. These structures will be designed to facilitate efficient access and synchronization while adhering to 42 Norm rules (e.g., `s_` prefix for structs, `t_` for typedefs).

### `t_program_data` (Main Program Data)

This structure will hold all global simulation parameters and shared resources, acting as the central context for the philosophers and forks. It will be passed to threads to avoid global variables where possible.

```c
typedef struct s_program_data
{
    long            t_num_philosophers;
    long            t_time_to_die;
    long            t_time_to_eat;
    long            t_time_to_sleep;
    long            t_num_times_to_eat; // Optional, -1 if not provided
    long            t_start_time;
    int             t_simulation_running; // Flag to stop simulation
    pthread_mutex_t t_write_mutex; // Mutex for printing messages
    pthread_mutex_t t_meal_check_mutex; // Mutex for checking meal counts
    pthread_mutex_t t_death_mutex; // Mutex for handling philosopher death
    struct s_philosopher *t_philosophers; // Array of philosophers
    pthread_mutex_t *t_forks; // Array of fork mutexes
} t_program_data;
```

*   `t_num_philosophers`: Total number of philosophers (and forks).
*   `t_time_to_die`: Time in milliseconds after which a philosopher dies if they haven't eaten.
*   `t_time_to_eat`: Time in milliseconds a philosopher spends eating.
*   `t_time_to_sleep`: Time in milliseconds a philosopher spends sleeping.
*   `t_num_times_to_eat`: (Optional) The number of times each philosopher must eat. If not provided, it will be set to -1 to indicate an infinite number of meals.
*   `t_start_time`: Timestamp when the simulation begins, used for relative time calculations.
*   `t_simulation_running`: A flag (boolean-like integer) to signal all threads to stop. This will be protected by a mutex to prevent race conditions when accessed by multiple threads.
*   `t_write_mutex`: A mutex to protect `printf` operations, ensuring that messages are printed atomically and do not interleave, preventing messy output and data races on `stdout`.
*   `t_meal_check_mutex`: A mutex to protect access to meal count variables, ensuring accurate tracking of how many times each philosopher has eaten.
*   `t_death_mutex`: A mutex to protect the `t_simulation_running` flag and any death-related state, ensuring that death is handled correctly and only once.
*   `t_philosophers`: A pointer to an array of `t_philosopher` structures.
*   `t_forks`: A pointer to an array of `pthread_mutex_t` representing the forks. Each fork is a mutex.

### `t_philosopher` (Philosopher Data)

Each philosopher will have their own structure containing their unique state and a reference to the shared program data.

```c
typedef struct s_philosopher
{
    int             t_id;
    long            t_last_meal_time;
    int             t_meals_eaten;
    pthread_t       t_thread;
    pthread_mutex_t *t_left_fork;
    pthread_mutex_t *t_right_fork;
    t_program_data  *t_program_data; // Pointer to shared program data
} t_philosopher;
```

*   `t_id`: Unique identifier for the philosopher (e.g., 1 to `number_of_philosophers`).
*   `t_last_meal_time`: Timestamp of the philosopher's last meal. Used to check for starvation.
*   `t_meals_eaten`: Counter for the number of meals the philosopher has consumed. Used for the optional `number_of_times_each_philosopher_must_eat` argument.
*   `t_thread`: The `pthread_t` identifier for the philosopher's thread.
*   `t_left_fork`: Pointer to the mutex representing the left fork.
*   `t_right_fork`: Pointer to the mutex representing the right fork.
*   `t_program_data`: A pointer back to the main `t_program_data` structure, allowing philosophers to access shared simulation parameters and mutexes.

## 4. Core Logic and Synchronization

### 4.1. Initialization

1.  **Parse Arguments:** Call a dedicated function to parse and validate command-line arguments, populating the `t_program_data` structure.
2.  **Initialize Mutexes:** Initialize all necessary `pthread_mutex_t` instances:
    *   `t_write_mutex`
    *   `t_meal_check_mutex`
    *   `t_death_mutex`
    *   An array of `t_num_philosophers` fork mutexes.
3.  **Initialize Philosophers:** Allocate memory for `t_philosopher` structures and initialize each one:
    *   Assign `t_id`.
    *   Set `t_last_meal_time` to the `t_start_time`.
    *   Set `t_meals_eaten` to 0.
    *   Assign pointers to their respective `t_left_fork` and `t_right_fork` mutexes. The fork assignment will be crucial for deadlock prevention.
    *   Assign a pointer to the shared `t_program_data`.

### 4.2. Thread Creation and Lifecycle

*   **Create Philosopher Threads:** For each philosopher, create a `pthread` that will execute the philosopher's routine.
*   **Monitoring Thread:** A separate monitoring thread will be created to periodically check if any philosopher has starved. This thread will be responsible for setting the `t_simulation_running` flag to `0` if a philosopher dies.
*   **Thread Join:** The main thread will `pthread_join` all philosopher threads and the monitoring thread to wait for their completion.

### 4.3. Philosopher Routine (`philo_routine`)

Each philosopher thread will execute a routine that simulates their actions:

1.  **Thinking:** Philosophers start by thinking.
2.  **Taking Forks:** This is the critical section. To prevent deadlock, we will implement a resource hierarchy strategy:
    *   **Odd-numbered philosophers:** Pick up the right fork first, then the left fork.
    *   **Even-numbered philosophers:** Pick up the left fork first, then the right fork.
    This breaks the circular dependency that leads to deadlock. Alternatively, a 


waiter strategy or limiting holding time could be considered, but the resource hierarchy is a common and effective solution for this problem.
    *   **Mutex Locking:** Use `pthread_mutex_lock()` to acquire forks and `pthread_mutex_unlock()` to release them.
3.  **Eating:** Once both forks are acquired, the philosopher eats for `time_to_eat` milliseconds. During this time, update `t_last_meal_time` and increment `t_meals_eaten`.
4.  **Sleeping:** After eating, the philosopher releases both forks and sleeps for `time_to_sleep` milliseconds.
5.  **Loop:** The philosopher continues this cycle (thinking, eating, sleeping) until the simulation ends (either a philosopher dies or all philosophers have eaten `number_of_times_each_philosopher_must_eat` times).

### 4.4. Monitoring Thread (`monitor_routine`)

This thread will periodically check the state of all philosophers to determine if any have starved or if the optional meal count has been reached.

1.  **Loop:** Continuously iterate through all philosophers.
2.  **Starvation Check:** For each philosopher, calculate the time elapsed since their `t_last_meal_time`. If this time exceeds `t_time_to_die`, the philosopher has starved.
    *   **Death Notification:** If a philosopher starves, print a death message (e.g., `<timestamp> <philosopher_id> died`).
    *   **Stop Simulation:** Set the `t_simulation_running` flag to `0` (protected by `t_death_mutex`) to signal all other threads to terminate.
3.  **Meal Count Check (Optional):** If `t_num_times_to_eat` is specified, check if all philosophers have eaten the required number of times. If so, set `t_simulation_running` to `0`.
4.  **Sleep:** The monitor thread will sleep for a short interval (e.g., 1 millisecond) to avoid busy-waiting and reduce CPU usage.

### 4.5. Time Management

*   **`gettimeofday`:** Use `gettimeofday()` for precise time measurements in milliseconds. This will be used to calculate `t_start_time`, `t_last_meal_time`, and elapsed times.
*   **`usleep`:** Use `usleep()` for pausing threads to simulate eating, sleeping, and thinking times. Remember that `usleep` takes microseconds, so convert milliseconds to microseconds (milliseconds * 1000).

## 5. Error Handling and Logging

*   **Argument Errors:** As detailed in Section 2, invalid arguments will lead to an error message and program exit.
*   **System Call Errors:** Check return values of system calls like `pthread_create`, `pthread_join`, `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`, and `malloc`. Handle errors appropriately (e.g., print error message to `stderr` and exit).
*   **Logging:** All state changes (taking a fork, eating, sleeping, thinking, dying) will be logged to `stdout` with a timestamp and philosopher ID, as per project requirements. The `t_write_mutex` will ensure that these messages are printed without interleaving.

## 6. Uniqueness Considerations

To ensure the uniqueness of the code, beyond adhering to the 42 Norm, the following aspects will be considered during implementation:

*   **Modular Design:** While the 42 Norm limits function length, a highly modular design with well-defined, single-purpose functions will be prioritized. This promotes readability and maintainability, and can lead to a distinct code structure.
*   **Custom Utility Functions:** Instead of relying solely on standard library functions for common tasks (e.g., time conversion, printing messages), custom utility functions will be implemented where appropriate, adhering to Norm rules.
*   **Error Handling Implementation:** The error handling mechanism will be robust and consistent, providing clear and specific error messages for different failure scenarios.
*   **Deadlock Prevention Strategy:** While resource hierarchy is a common solution, the specific implementation details, such as the order of fork acquisition for odd/even philosophers, can be uniquely structured.
*   **Monitoring Logic:** The monitoring thread's logic for checking starvation and meal counts will be carefully crafted to be efficient and accurate, potentially incorporating subtle optimizations or unique timing checks.
*   **Code Style and Readability:** Beyond the strict Norm rules, attention will be paid to overall code readability, logical flow, and consistent commenting style to create a clean and unique codebase.

## 7. Testing Strategy

After implementation, the program will be tested using open-source testers. This will involve:

*   **Functional Testing:** Verifying that the program behaves as expected under various valid argument combinations.
*   **Error Handling Testing:** Ensuring that the program correctly handles invalid arguments and other error conditions.
*   **Concurrency Testing:** Using tools designed to detect race conditions, deadlocks, and other concurrency issues. This might involve running the simulation multiple times with different parameters to expose subtle timing-dependent bugs.
*   **Norm Compliance Testing:** Using the `norminette` tool to ensure full adherence to 42 Norm rules.

## 8. Project Structure (Proposed)

```
philo/
├── includes/
│   └── philo.h
├── src/
│   ├── main.c
│   ├── parsing.c
│   ├── init.c
│   ├── philo_routine.c
│   ├── monitor_routine.c
│   ├── utils.c
│   └── error.c
└── Makefile
```

*   `philo.h`: Main header file containing all necessary structure definitions, function prototypes, and include guards.
*   `main.c`: Entry point of the program, handles argument parsing, initialization, thread creation, and joining.
*   `parsing.c`: Contains functions for argument parsing and validation.
*   `init.c`: Contains functions for initializing program data, mutexes, and philosophers.
*   `philo_routine.c`: Contains the main routine executed by each philosopher thread.
*   `monitor_routine.c`: Contains the routine executed by the monitoring thread.
*   `utils.c`: Contains general utility functions (e.g., time-related functions, printing messages).
*   `error.c`: Contains functions for error handling and printing error messages.
*   `Makefile`: Standard 42 Makefile for compiling the project, including `all`, `clean`, `fclean`, and `re` rules.

This architecture provides a clear separation of concerns, making the code easier to develop, debug, and maintain, while strictly adhering to the project requirements and 42 Norm rules. The emphasis on modularity and custom utility functions will contribute to the uniqueness of the solution.

