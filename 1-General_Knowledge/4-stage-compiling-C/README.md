# 🔧 Demonstration of the 4 Compilation Stages in C Language

This project demonstrates how a C source file is compiled into an executable program through **4 fundamental stages**:

1. **Preprocessing**
2. **Compilation**
3. **Assembly**
4. **Linking**

---

## Usage

### Use the following commands to build or clean the project:

```bash
make        # Build all object files
make clean  # Remove generated files
```
### Makefile:

```makefile
all:
	gcc -c -o main.o main.c -I .
	gcc -c -o helloLinux.o helloLinux.c -I .
	gcc -c -o helloHoang.o helloHoang.c -I .

clean:
	rm -rf main.o helloHoang.o helloLinux.o test
```
---