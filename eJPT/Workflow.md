# Pentesting Workflow (eJPT-Oriented)


## 1. Reconnaissance / Information Gathering

### Passive Recon (No Direct Interaction)

**Tools**
- theHarvester
- Shodan
- Netcraft
- DNSDumpster
- Google Dorks
- Wayback Machine

**Examples**
```bash
theHarvester -d example.com -b google
site:example.com intitle:admin
````

**Output**

* Domains
* Subdomains
* Emails
* IPs
* Technology stack

---

### Active Recon (Direct Interaction)

**Tools**

* Nmap
* Netdiscover
* dnsrecon
* dig
* host

**Examples**

```bash
nmap -Pn -sS -sV target
dnsrecon -d example.com
dig example.com A
```

**Output**

* Live hosts
* Open ports
* Services
* DNS records

---

## 2. Enumeration

### DNS Enumeration

**Tools**

* dnsrecon
* dnsenum
* dig
* fierce

**Examples**

```bash
dig axfr @ns1.example.com example.com
dnsenum example.com
```

---

### Service Enumeration

**Tools**

* Nmap NSE
* Netcat
* smbclient
* enum4linux

**Examples**

```bash
nmap -sV --script=default target
nc target 80
```

---

### Web Enumeration

**Tools**

* Gobuster
* Dirb
* Nikto
* WhatWeb

**Examples**

```bash
gobuster dir -u http://target -w wordlist.txt
nikto -h http://target
```

---

## 3. Vulnerability Identification

**Tools**

* Nmap NSE (vuln)
* Searchsploit
* Nikto

**Examples**

```bash
nmap --script=vuln target
searchsploit apache 2.4
```

**Output**

* CVEs
* Misconfigurations
* Weak services

---

## 4. Exploitation

### Exploit Execution

**Tools**

* Metasploit Framework
* Exploit-DB

**Examples**

```bash
msfconsole
search type:exploit service
set RHOSTS target
run
```

---

### Payload Handling

**Tools**

* Meterpreter
* Netcat

**Output**

* Shell access
* Active session

---

## 5. Post-Exploitation

**Tools**

* Meterpreter
* Native Linux / Windows commands

**Examples**

```bash
sysinfo
getuid
ipconfig
ifconfig
```

**Goals**

* Internal reconnaissance
* Pivot discovery

---

## 6. Privilege Escalation

**Tools**

* LinPEAS
* WinPEAS
* Manual enumeration

**Examples**

```bash
./linpeas.sh
```

**Output**

* Root / Administrator access

---

## 7. Credential Access

**Tools**

* Mimikatz
* Meterpreter (hashdump)
* John the Ripper

**Examples**

```bash
hashdump
john hashes.txt
```

---

## 8. Lateral Movement (Basic)

**Tools**

* Metasploit
* SSH
* SMB

**Examples**

```bash
use auxiliary/scanner/smb/smb_login
```

---

## 9. Reporting & Cleanup

### Reporting

* Markdown notes
* Screenshots
* Command logs

### Cleanup

* Remove shells
* Remove users
* Delete temporary files

---

## Core Principles

* Always have authorization
* Prefer passive before active
* Enumerate before exploit
* Stay within scope
* Document everything!!!
