**WARNING**: This code demonstrates a **buffer overflow vulnerability** in C. **Do not run this on production systems**. Always ensure you're testing in a **controlled environment** (such as a virtual machine or isolated container) to avoid potential system instability, security risks, or unintended consequences.

### Adding More Detail on **Buffer Overflow**:

A simple **buffer overflow** might occur in a function like this:

```c
#include <string.h>

void vulnerable_function() {
    char buffer[64];
    gets(buffer);  // unsafe function that doesn't check buffer size
}

int main() {
    vulnerable_function();
    return 0;
}
