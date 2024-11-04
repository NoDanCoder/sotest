# sotest

`sotest` is a C-based console application developed for testing shared function libraries on GNU/Linux (x86 processor architecture). It includes a custom scripting language for loading shared libraries and calling library functions. The application conforms to POSIX.1-2001 standards and is designed to run in a terminal environment, making it lightweight and highly portable.
## Features

- **Load shared libraries** dynamically at runtime.
- **Invoke library functions** via a custom scripting language.
- **Error handling** for syntax errors, missing libraries, and unavailable functions.
- **Interactive mode** for direct command input.
- **Comment support** in scripts (`#`).


## Installation

Clone this repository and navigate to the project directory:

```bash
$ git clone https://github.com/yourusername/sotest.git
$ cd sotest
```
## Compilation

A Makefile is included to automate the compilation and cleanup process.

- Compile only main program:
```bash
$ make
```

- Compile with some shared libraries to try out program:
```bash
$ make taste
```

- Clean Up:
```bash
$ make clean
```
## Usage/Examples

### File reading

`sotest` reads a script file to load shared libraries and execute their functions. Each line in the script file corresponds to a command, with syntax validation and error handling.

1. Load Library
```bash
use <so_name>
```
- `<so_name>`: The path to the shared library (e.g., test.so).
2. Call Function
```bash
call <func_name>
```
- `<func_name>`: The name of the function to call within the loaded library.
3. Comments
```bash
# This is a comment
```
- Comments: Lines starting with # are treated as comments and ignored by the interpreter.

Below is an example script named test.sc (included in the curernt repo), which loads libraries and calls functions.

```bash
# Load test library
use test.so
call example
call test1_func

# Load another library
use other.so
call func1
```

### Interactive console

There is also another way to load shared libraries an run its methods. This way have the same commands that **file reading** commented above.

```bash
(sc) use test.so

(sc) call example
Successfully loaded: test.so
Running method [example] from lib [test.so]:

This is example() from test.so
Finished process of running library with status 0
```

## Comments

- If you are loading a library using a relative path use a relative search based on your current position `./`.
- Support for inline comments in code is not enabled yet.
- Any try to use a `use` command after already loaded a library will load the new library, even if command fails, on which case you will have to load a library again to start.

## Authors

- [@NoDanCoder](https://www.github.com/nodancoder)


## License

[MIT](https://choosealicense.com/licenses/mit/)
