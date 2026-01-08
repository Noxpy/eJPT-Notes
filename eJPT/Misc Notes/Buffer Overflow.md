## Buffer Overflow (VERY Brief)

This in my opionion is the bedrock of exploits, holding an importnat history.

A **buffer overflow** is a programming error where data written to a buffer exceeds its allocated size, causing adjacent memory to be overwritten. Because memory regions often contain control data (such as saved frame pointers or return addresses), this can lead to crashes or redirection of program execution.

Buffers are fixed-size memory regions used to temporarily hold data. If input size is not validated, malformed or oversized input can overwrite nearby memory, corrupt program state, or alter control flow.

---

### Stack vs Heap (High-Level)

**Stack**
- Fixed-size, LIFO memory region
- Holds local variables, saved registers, return addresses
- Overflows can overwrite saved EBP/RBP and saved return address (EIP/RIP)
- Common mitigations: stack canaries, ASLR, NX/DEP, PIE

**Heap**
- Dynamically allocated memory (malloc/new)
- Overflows can corrupt adjacent allocations or allocator metadata
- Related bugs: heap overflow, use-after-free, double-free
- Mitigations: hardened allocators, ASLR, pointer integrity checks

---

### Stack vs Heap (Growth Visualization)

**Conceptual growth direction:**

```
High memory addresses 0xFFFFFFFF etc. <- Value changes if 32 bit (0xFFFFFFFF) Or 64 bit (0xFFFFFFFFFFFFFFFF)
+----------------------------+
|                            | <-- Commandline or Function Argument
|----------------------------|
|                            |
|           Stack            |  ↓ grows downward (function calls)
|                            |
|~~~~~~~~~~~~~~~~~~~~~~~~~~~~| <-- Stack pointer
|----------------------------|
|                            |
|   Saved frame pointers     |
|----------------------------|
| local buffers overflowable |
|----------------------------|
| return address vulnerable  |
| to overwrite via buffer    |
|          overflow          |
|----------------------------|
|             |              |
|             V              |
|                            |
|                            |
|             ^              |
|             |              |
|----------------------------|
|                            |
|           Heap             |  ↑ grows upward (malloc / new)
|                            |
|dynamically allocated memory|
|----------------------------|
|                            |
|        Data                |
|                            |
+----------------------------+
Low memory addresses 0X00000000 etc. <- Value changes if 32 bit (0x00000000) Or 64 bit (0x0000000000000000)
```

**Key relationship to exploitation:**

- Stack-based buffer overflows abuse **downward-growing stack frames**
- Heap overflows target **adjacent allocations or allocator metadata**
- Overwrites occur when growth boundaries are crossed without bounds checking

This layout explains why stack overflows can overwrite:
- saved frame pointers
- saved return addresses

…and why heap overflows can corrupt:
- neighboring objects
- function pointers
- allocator control structures

---


### Minimal Stack-Based Overflow Example (C)

```c
char a[8] = "";
unsigned short b = 1979;

strcpy(a, "excessive");  // 10 bytes including null terminator
```

Memory layout (simplified):

```
| Variable | Stored Bytes            |
| -------- | ----------------------- |
| a        | 65 78 63 65 73 73 69 76 |
| b        | 65 00 (overwritten)     |
```

- `"excessive"` exceeds the 8-byte buffer
- Extra bytes overwrite `b`
- Program behavior changes due to memory corruption

---

### Why This Matters for Exploitation

On the stack, local buffers are typically placed **below** saved frame data:

```
Higher addresses
+-------------------------+
| Saved return address    |
+-------------------------+
| Saved frame pointer     |
+-------------------------+
| Local buffer            |  <-- overflow starts here
+-------------------------+
Lower addresses
```

Writing past the buffer advances into higher addresses, eventually overwriting the saved return address. When the function returns, execution jumps to the overwritten value.



---


### NOP Sled (Concept)

A **NOP sled** is a sequence of no‑operation instructions (`0x90`) placed before a payload in memory.

- Any jump into the sled will “slide” execution forward
- Increases reliability by expanding the valid landing zone
- Payload executes once the sled ends

Conceptual layout:

```
[ padding ][ RET overwrite ][ NOP NOP NOP ... ][ payload ]
```

---

### NOP Sled Visualization

```
Memory layout (conceptual):

Higher addresses
+-------------------------+
| Saved return address    |  <- overwritten to point into NOP sled
+-------------------------+
| Saved frame pointer     |
+-------------------------+
| Local buffer            |
|  ...                    |
|  [payload begins here]  |
+-------------------------+
| NOP NOP NOP NOP ...     |  <- NOP sled (0x90)
+-------------------------+
Lower addresses
```

- Execution landing anywhere in the NOP sled slides forward
- CPU eventually reaches the payload
- Larger sled = higher reliability

Example address targeting:

```
NOP sled:  0x7fffd020 - 0x7fffd047
Payload:   0x7fffd050

Overwrite return address with any value inside sled range
```

---

### Key Constraints (Modern Systems)

For injected code to execute, all must be true:

1. Instruction pointer is redirected
2. Target memory is executable
3. Address is predictable/reachable

Modern defenses (NX/DEP, ASLR, stack canaries) intentionally break these assumptions.

---

### Recommended Learning Resources

- **Running a Buffer Overflow Attack – Computerphile**  
  https://www.youtube.com/watch?v=1S0aBV-Waeo

- **Reverse Polish Notation and The Stack – Computerphile**  
  https://www.youtube.com/watch?v=7ha78yWRDlE

- **Stack vs Heap. What’s the Difference and Why Should I Care? - Linux Foundation**  
  https://www.linux.com/training-tutorials/stack-vs-heap-whats-difference-and-why-should-i-care/

---

### Secure Coding Takeaways (Language-Specific)

- **Validate input length**  
  Always check buffer sizes before writing. In C, unchecked functions like `strcpy`, `gets`, or `sprintf` write directly to memory, which can overwrite adjacent stack or heap memory.  

- **Use bounded functions in C/C++**  
  Prefer `strlcpy`, `snprintf`, `memcpy_s` to explicitly limit bytes written. These functions help prevent stack or heap corruption by stopping writes beyond buffer capacity.  
  - Example:  
    ```c
    char buf[16];
    strlcpy(buf, input, sizeof(buf)); // prevents overflow
    ```

- **Understand low-level implications (Assembly/Memory Layout)**  
  High-level C code maps almost directly to machine instructions:  
  - Local variables → stack frame offsets  
  - Function calls → `call` / `ret` instructions  
  - Buffer overflows → overwrite saved frame pointer or return address  
  Knowing assembly and calling conventions clarifies **how memory corruption translates into control-flow hijacking**.

- **Enable compiler and OS mitigations**  
  - Stack canaries (`-fstack-protector`) detect corruption of saved return addresses  
  - NX/DEP marks memory regions non-executable  
  - ASLR randomizes stack, heap, and libraries  
  These measures make direct memory exploits harder.

- **Static & dynamic analysis tools**  
  Use tools like AddressSanitizer (ASan), Valgrind, and UBSan during development to catch out‑of‑bounds writes, use‑after‑free bugs, and undefined behavior. These tools instrument memory accesses and correlate high‑level C code with the exact assembly instructions performing unsafe reads or writes.

- **Favor memory‑safe languages when possible**  
  Languages like Rust, Go, and Java enforce bounds checking, ownership rules, or garbage collection at compile time or runtime. These constraints prevent entire classes of memory‑corruption bugs by design, breaking the direct path from high‑level mistakes to assembly‑level control‑flow manipulation.

---

### eJPT Specific Examples


### **CVE-2014-6271: Shellshock**

- **Description**: A buffer overflow vulnerability in **Bash** (the default shell in many Linux systems) allows remote attackers to execute arbitrary code. This vulnerability is caused by how **Bash** interprets function definitions passed through environment variables.
- **Impact**: This vulnerability can lead to remote code execution (RCE) on affected systems when an attacker is able to pass malicious function definitions to a vulnerable Bash shell.
- **Exploitability**: An attacker can exploit this vulnerability through web servers, SSH, or other services that use Bash and pass user input through environment variables.
- **Relevance to eJPT**: Shellshock is a clear example of how a **buffer overflow** can allow remote code execution in a widely used application. Understanding how the vulnerability works, and identifying it using penetration testing tools, is crucial in an eJPT context.
- **Link**: [CVE-2014-6271](https://nvd.nist.gov/vuln/detail/CVE-2014-6271)

---

### **CVE-2017-0144: WannaCry**

- **Description**: This is a **buffer overflow** vulnerability in **SMBv1** (Server Message Block protocol version 1) in Microsoft Windows. It was exploited by the **WannaCry** ransomware, leading to widespread attacks in May 2017. The vulnerability allows remote code execution through crafted SMB packets.
- **Impact**: WannaCry exploits this vulnerability to spread ransomware across networks, encrypting files and demanding a ransom for decryption. It caused massive disruptions to businesses and public services worldwide.
- **Exploitability**: This vulnerability was primarily exploited via **SMBv1** by using the **EternalBlue** exploit, which allowed attackers to execute arbitrary code on vulnerable systems.
- **Relevance to eJPT**: Understanding how buffer overflows in network protocols like SMBv1 can lead to widespread exploitation is important for identifying and mitigating such vulnerabilities during penetration testing.
- **Link**: [CVE-2017-0144](https://nvd.nist.gov/vuln/detail/CVE-2017-0144)




