
# Lab: Perform a Fuzzing Test with Metasploit

## Estimated Time
30 minutes

## Overview

This lab demonstrates the fundamentals of fuzzing by targeting a deliberately simple and fragile HTTP service. Fuzzing is a vulnerability discovery technique that sends unexpected, malformed, or oversized input to an application to uncover crashes, unhandled exceptions, or other anomalous behavior.

You will simulate a vulnerable HTTP service using Python and use Metasploit’s HTTP fuzzing modules to send malformed requests. The lab is designed to be safe, local, and reproducible.

## Learning Objectives

By completing this lab, you will be able to:

- Explain the purpose of fuzzing in vulnerability discovery
- Deploy a mock vulnerable HTTP service
- Use Metasploit auxiliary fuzzing modules against an HTTP endpoint
- Observe and interpret abnormal application behavior
- Perform proper cleanup after a security test

---

## Environment Requirements

- Kali Linux (local VM recommended)
- Metasploit Framework
- Python 3
- Root or sudo privileges

---

## Exercise 1: Set Up the Vulnerable HTTP Service

This exercise creates a low-privilege user and runs a simple Python HTTP server that accepts arbitrary input without validation.

### Create the Victim User

```bash
sudo adduser victim --disabled-password --gecos ""
sudo mkdir -p /home/victim
````

### Create the Vulnerable HTTP Server Script

```bash
sudo tee /home/victim/vuln_http.py > /dev/null << 'EOF'
from http.server import BaseHTTPRequestHandler, HTTPServer

class FuzzTestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        print(f"Received GET request: {self.path}")
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"OK")

server = HTTPServer(('127.0.0.1', 8080), FuzzTestHandler)
print("Starting HTTP server on port 8080...")
server.serve_forever()
EOF
```

### Set Permissions

```bash
sudo chmod +x /home/victim/vuln_http.py
sudo chown victim:victim /home/victim/vuln_http.py
```

### Run the Server as the Victim User

Open a new terminal:

```bash
sudo -u victim python3 /home/victim/vuln_http.py &
```

The server should now be listening on `127.0.0.1:8080`.

---

## Exercise 2: Launch Metasploit and Fuzz the Server

### Start Metasploit

```bash
msfconsole
```

### Load the HTTP URI String Fuzzer

```bash
use auxiliary/fuzzers/http/http_get_uri_strings
set RHOSTS 127.0.0.1
set RPORT 8080
run
```

Metasploit will begin sending increasingly malformed HTTP GET requests to the target service.

---

## Exercise 3: Observe and Document Behavior

### Victim Terminal

Watch for output such as:

```
Received GET request: /AAAAAAAAAAAAAAAAAAAA...
BrokenPipeError: [Errno 32] Broken pipe
```

These messages indicate that the server is struggling to handle the malformed input.

### Metasploit Console

Observe fuzzing progress:

```
[*] 127.0.0.1:8080 - Fuzzing with iteration 22700 using fuzzer_string_uris_giant
```

Stop the fuzzer when finished:

```bash
Ctrl + C
```

### Interpretation

While this simple Python server is unlikely to suffer memory corruption, real-world services written in C/C++ often crash or expose vulnerabilities under similar conditions. The goal is to identify unexpected behavior, not guaranteed exploitation.

---

## Exercise 4: Challenge Tasks

Experiment with different configurations:

* Change the listening port:

  ```bash
  set RPORT 9090
  ```
* Simulate a virtual host:

  ```bash
  set VHOST test.local
  ```
* Explore additional HTTP fuzzing modules:

  ```bash
  search auxiliary/fuzzers/http
  ```

Iterative experimentation is central to real-world fuzzing workflows.

---

## Exercise 5: Clean Up the Environment

Terminate the HTTP server and remove the test user:

```bash
sudo pkill -f vuln_http.py
sudo deluser --remove-home victim
```

This restores the system to its pre-lab state.

---

## Summary

In this lab, you:

* Deployed a mock vulnerable HTTP service
* Used Metasploit to fuzz an application endpoint
* Observed server-side failures caused by malformed input
* Experimented with fuzzer configuration options
* Practiced ethical cleanup procedures

---

## Notes on Real-World Fuzzing

* Effective fuzzing often requires custom payloads and long runtimes
* Crashes should be correlated with logs, core dumps, and debuggers
* Always fuzz systems you own or have explicit permission to test

Fuzzing is less about instant success and more about patient pressure applied to brittle assumptions in code.
