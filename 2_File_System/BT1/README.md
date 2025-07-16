# Explanation of the `O_APPEND` Flag

### The `O_APPEND` flag is used to:
- Safely append data to the end of a file.
- Prevent overwriting existing data in the file.
- Support concurrent writing in multi-process environments.

When the file `hello.txt` initially contains the content "hello world", the `O_APPEND` flag ensures that the file pointer is automatically moved to the end of the file before each write operation, appending new data without affecting existing content.