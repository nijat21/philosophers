# Philosophers (Dining Philosophers Problem)

![philosophers](/Dining_philosophers.png)

## Problem Description

The Dining Philosophers problem is a classic synchronization problem in computer science. It involves a group of philosophers sitting around a circular table, each with a plate of spaghetti and a fork between them. To eat, a philosopher needs to pick up both the fork to their left and the right. However, there are exactly as many forks as philosophers, leading to a potential deadlock or starvation scenario.

The challenge is to design a solution where:

- Philosophers can eat without starving
- No philosopher dies from hunger
- The system avoids deadlock (all philosophers waiting indefinitely)
- The simulation runs efficiently with multiple threads

## Solution Overview

This implementation uses multithreading in C with POSIX threads (pthreads) and mutexes to solve the Dining Philosophers problem:

- **Each philosopher** is represented by a separate thread
- **Each fork** is protected by a mutex to ensure exclusive access
- **Synchronization mechanisms** prevent deadlock and ensure fairness
- **A monitor thread** continuously checks for philosopher deaths
- **Thread-safe operations** for shared data access

### Key Features:

- Handles any number of philosophers (including edge cases like 1 philosopher)
- Configurable timing parameters for eating, sleeping, and time-to-die
- Optional parameter for minimum meals per philosopher
- Colored output for better visualization
- Robust error handling and input validation

## How to Compile and Run

### Prerequisites

- GCC compiler
- POSIX threads library (usually included in Linux/macOS)

### Compilation

```bash
make
```

### Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Parameters

- `number_of_philosophers`: Number of philosophers and forks (1-200)
- `time_to_die`: Time in milliseconds after which a philosopher dies if they haven't eaten
- `time_to_eat`: Time in milliseconds it takes for a philosopher to eat
- `time_to_sleep`: Time in milliseconds a philosopher spends sleeping
- `number_of_times_each_philosopher_must_eat`: Optional parameter - simulation stops when all philosophers have eaten this many times

### Example

```bash
./philo 5 800 200 200 7
```

This runs a simulation with 5 philosophers, 800ms time to die, 200ms eat time, 200ms sleep time, and each philosopher must eat 7 times.

### Output

The program outputs timestamped messages showing philosopher actions:

- Taking forks
- Eating
- Sleeping
- Thinking
- Death (if a philosopher dies)

## Technical Implementation

- **Language**: C with POSIX threads
- **Synchronization**: Mutexes for fork access and shared data protection
- **Deadlock Prevention**: Alternating fork pickup order for philosophers
- **Starvation Prevention**: Careful timing and synchronization in the eating cycle
- **Monitoring**: Dedicated thread for death detection
- **Memory Management**: Proper cleanup of threads and mutexes

The solution ensures thread safety, prevents race conditions, and provides a realistic simulation of the classic concurrency problem.
