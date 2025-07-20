# Explanation:
### Why is `mutex` needed?
-   Without the mutex, multiple threads may try to update `counter` at the same time, leading to race conditions and incorrect results. Mutex ensures mutual exclusion, allowing only one thread to modify `counter` at a time.
### What happens if you remove `mutex`?
-   The final value of `counter` will likely be less than expected (i.e., < 3,000,000), because some increments will be lost due to concurrent access.