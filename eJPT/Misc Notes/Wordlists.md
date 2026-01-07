
````markdown
# Wordlists for Penetration Testing

## Overview

In **Kali Linux** and other penetration testing environments, various **wordlists** are available by default for different attacks. These wordlists are used in tools like **Metasploit**, **Hydra**, **Dirb**, and others to perform brute-force and dictionary-based attacks. 

This document outlines some of the most important wordlists, their locations, and how to use them in popular penetration testing tools like **Metasploit** and **Hydra**.

For more information on wordlists and available tools, visit the official [Kali Wordlists Page](https://www.kali.org/tools/wordlists/).

---

## Common Wordlist Locations

### Default Wordlist Directories

- **`/usr/share/wordlists/`** – The primary location for wordlists used across various tools.
  - **Key Subdirectories**:
    - **`brutespray`**: `/usr/share/brutespray/wordlist`
    - **`dirb`**: `/usr/share/dirb/wordlists`
    - **`dirbuster`**: `/usr/share/dirbuster/wordlists`
    - **`fern-wifi`**: `/usr/share/fern-wifi-cracker/extras/wordlists`
    - **`john.lst`**: `/usr/share/john/password.lst` (used by **John the Ripper**)
    - **`metasploit`**: `/usr/share/metasploit-framework/data/wordlists` (used by **Metasploit**)
    - **`nmap.lst`**: `/usr/share/nmap/nselib/data/passwords.lst`
    - **`rockyou.txt.gz`**: `/usr/share/wordlists/rockyou.txt.gz` (compressed, decompress with `gunzip`)
    - **`seclists`**: `/usr/share/seclists/` (various usernames and passwords)
    - **`sqlmap.txt`**: `/usr/share/sqlmap/data/txt/wordlist.txt`
    - **`wfuzz`**: `/usr/share/wfuzz/wordlist`
    - **`wifite.txt`**: `/usr/share/dict/wordlist-probable.txt` (for wireless cracking)

---

## Using Wordlists in Metasploit

### Metasploit Wordlist Location

Metasploit’s wordlists are located at:
- **`/usr/share/metasploit-framework/data/wordlists/`**
  - Common files include `passwords.txt`, `rockyou.txt`, and others.

### Example 1: SSH Brute-Force with Metasploit

```bash
msfconsole
use auxiliary/scanner/ssh/ssh_login
set RHOSTS <target_ip>                     # Target IP address
set USERNAME_FILE /usr/share/seclists/Usernames/top-usernames-shortlist.txt  # Username list
set PASS_FILE /usr/share/seclists/Passwords/Top1000-passwords.txt  # Password list
set THREADS 10                             # Adjust threads for performance
run
````

### Example 2: HTTP Login Brute-Force with Metasploit

```bash
msfconsole
use auxiliary/scanner/http/http_login
set RHOSTS <target_ip>                     # Target IP address
set TARGETURI /admin                       # Target URI path (e.g., /admin)
set USERNAME_FILE /usr/share/seclists/Usernames/rockyou.txt  # Username list
set PASS_FILE /usr/share/seclists/Passwords/rockyou.txt    # Password list
set THREADS 5                              # Adjust threads to avoid overload
run
```

---

## Using Wordlists in Hydra

**Hydra** is a fast and flexible tool for brute-forcing login services. You can use the wordlists located on Kali Linux to perform attacks against various services like SSH, HTTP, FTP, and more.

### Hydra Example 1: SSH Brute-Force

```bash
hydra -l <username> -P /usr/share/seclists/Passwords/Top1000-passwords.txt ssh://<target_ip>
```

### Hydra Example 2: HTTP Basic Authentication Brute-Force

```bash
hydra -L /usr/share/seclists/Usernames/top-usernames-shortlist.txt -P /usr/share/seclists/Passwords/Top1000-passwords.txt <target_ip> http-get
```

### Hydra Example 3: FTP Brute-Force

```bash
hydra -l <username> -P /usr/share/wordlists/rockyou.txt ftp://<target_ip>
```

---

## Conclusion

Here’s a summary of key wordlists available on **Kali Linux** and how to use them in **Metasploit** and **Hydra**:

* **rockyou.txt**: A highly popular wordlist for password cracking. Found in `/usr/share/wordlists/`.
* **SecLists**: A comprehensive collection of usernames and passwords for various services, available in `/usr/share/seclists/`.
* **Metasploit wordlists**: Located at `/usr/share/metasploit-framework/data/wordlists/`.
* **Hydra**: A tool that integrates well with these wordlists to brute-force various protocols such as SSH, HTTP, and FTP.

For more details, you can always visit the official [Kali Wordlists Page](https://www.kali.org/tools/wordlists/).
