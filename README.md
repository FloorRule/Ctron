# Ctron

Application built from scratch that compiles a C file to an EXE of the same code.
Ctron translates a simplified C-like language into a Windows executable (`.exe`).
It uses Flex & Bison to generate LLVM IR, compiles it to assembly with `llc.exe`, and finally produces an executable using `gcc`.

---

## Writing C Code for Ctron

Ctron currently supports a restricted subset of C. Follow these rules when writing code:

### Supported & Required Syntax

* `"\d"` is interpreted as `"\n"`.
* The keyword `global` must appear before declaring any global variables.
* Use `//` to start a single-line comment.
* Only the `int` type is fully functional (and `void` for functions).

### Functions

`void` functions must end with:

```c
return;
```

### Operators

**Fully functional:**

* `==`, `||`, `&&`, `<`, `>`, `<=`, `>=`

**Not functional yet:**

* `!`, `!=`
* `+=`, `-=`, `++`, `--`, `*=`, `/=`

Use the expanded form instead, for example:

```c
x = x + 1;   // instead of x++
x = x - 1;   // instead of x--
x = x * 2;   // instead of x *= 2
x = x / 2;   // instead of x /= 2
```

---

## Required Files & Environment

The following files **must be located in the same folder** as `Ctron.exe` (or `Main.cpp` during development):

* `Flex_BisonV6.exe`
* `llc.exe`

Additionally, the computer must have **gcc** installed (MinGW recommended).

* If `gcc` is missing, Ctron should produce an initial error indicating that the environment is not set up correctly.

---

## Running the Ctron Compiler (Ctron)

Open **cmd** in the folder that contains `Ctron.exe` (or the built binary).

### Help

Type:

```cmd
Ctron -help
```

or:

```cmd
Ctron -h
```

Example output:

```text
Usage:
  Ctron [options]
  Ctron [source_file] [command]

Options:
        -h                       Display this information.

Commands:
        -o <file>                Place the output into <file>.
        -o                       Place the output into [source_file.exe].
```

---

### Compiling a File with Ctron

To compile `test.c` and output an EXE with the same base name (`test.exe`):

```cmd
Ctron test.c -o
```

To compile `test.c` and output an EXE with a custom file name:

```cmd
Ctron test.c -o custom.exe
```

---

## Running Bison + Flex Manually

These steps show the internal pipeline when you want to run Flex/Bison and LLVM tools directly.

1. Generate `out.ll` (LLVM IR) using Flex + Bison:

   ```cmd
   Flex_BisonV6.exe test.c
   ```

   This generates:

   ```text
   out.ll
   ```

2. Convert `out.ll` to x86 assembly using `llc.exe`:

   ```cmd
   llc.exe -march=x86 --x86-asm-syntax=intel out.ll
   ```

   This generates:

   ```text
   out.s
   ```

3. Compile the assembly file to an executable using `gcc`:

   ```cmd
   gcc out.s -o test.exe
   ```

4. Run the resulting executable:

   ```cmd
   test.exe
   ```
