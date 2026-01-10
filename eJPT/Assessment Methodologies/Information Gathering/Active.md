## DNS Zone Transfer

- Performing zone transfer with `dnsenum`
````
dnsenum pokemon.com
````

* Performing zone transfer with `dig`

```bash
dig axfr @nsztm1.digi.ninja zonetransfer.me
```

`axfr` – switch for zone transfer
`nsztm1.digi.ninja` – authoritative nameserver

* Performing zone transfer with `fierce`

```bash
fierce --domain pokemon.com
```

##### Notes

* Zone transfers only work if explicitly allowed
* Most production domains block AXFR

---

## dig (Basic DNS Queries)

* Basic lookup

```bash
dig example.com
```

* Specific record

```bash
dig example.com MX
```

* Query specific DNS server

```bash
dig @8.8.8.8 example.com A
```

---

## Netdiscover

Uses ARP to discover devices on the local network (IP, MAC, vendor).

```bash
sudo netdiscover -i eth0 -r 192.168.2.0/24
```

---

## Nmap

* Skip host discovery (ICMP blocked on many systems, especially Windows)

```bash
nmap -Pn 10.4.19.214
```

* SYN scan (default, stealthier)

```bash
nmap -sS target
```

* Timing control (slower = quieter)

```bash
nmap -Pn -sS -T2 target
```

* Decoy scanning (obfuscate source)

```bash
nmap -D RND:5 target
```

* Scan specific ports (example)

```bash
nmap -Pn -p 22,80,443 target
```

* Service/version detection

```bash
nmap -Pn -p 80 -sV target
```

---

## Nmap NSE Scripts

* Default scripts

```bash
nmap -sC target
```

* Discovery scripts

```bash
nmap --script discovery target
```

* Vulnerability detection

```bash
nmap --script vuln target
```

* Intrusive scripts

```bash
nmap --script intrusive target
```

---

## Saving Nmap Output

* Normal output

```bash
nmap -Pn target -oN scan.txt
```

* Grepable output

```bash
nmap -Pn target -oG scan.grep
```

* XML output

```bash
nmap -Pn target -oX scan.xml
```

* All formats at once

```bash
nmap -Pn target -oA scan
```

Creates:

* `scan.nmap`
* `scan.gnmap`
* `scan.xml`

---

## Importing Nmap Output

* Grep results

```bash
grep open scan.gnmap
```

* Convert XML to HTML

```bash
xsltproc scan.xml -o scan.html
```

* Import XML into tools:

  * Metasploit
  * Dradis
  * Faraday
  * Nessus (scan reference)

---

## Gobuster (Directory / DNS Brute Force)

* Directory enumeration

```bash
gobuster dir -u http://target -w /usr/share/wordlists/dirb/common.txt
```

* DNS subdomain enumeration

```bash
gobuster dns -d target.com -w subdomains.txt
```

* Ignore status codes

```bash
gobuster dir -u http://target -w wordlist.txt -b 403,404
```

---

## Common 200‑Level HTTP Status Codes (APIs)

| Code | Meaning         |
| ---- | --------------- |
| 200  | OK (success)    |
| 201  | Created         |
| 202  | Accepted        |
| 204  | No Content      |
| 206  | Partial Content |

---

## Notes

* AXFR success = misconfiguration
* `-Pn` bypasses ICMP filtering
* Lower `-T` = quieter scans
* NSE adds logic, not just port checks
* Save outputs early for reuse
