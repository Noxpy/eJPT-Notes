# Nessus Installation & Audit Cheat Sheet

## Download Nessus

```
https://www.tenable.com/downloads/nessus
```

* Linux (Debian/Ubuntu): `.deb`
* Windows: `.msi`

---

## Install Nessus (Linux – Debian/Ubuntu)

```bash
sudo dpkg -i Nessus-*.deb
sudo systemctl start nessusd
sudo systemctl enable nessusd
```

Web UI:

```text
https://localhost:8834
```

---

## Install Nessus (Windows)

* Run `.msi`
* Complete installer
* Open:

```text
https://localhost:8834
```

---

## Initial Setup (Once)

* Create admin user
* Activate (Essentials / license)
* Wait for plugin sync

---

## Nmap – Asset Discovery (Audit Scope Known)

Scan subnet:

```bash
nmap 192.168.100.0/24
```

Scan specific hosts (detailed):

```bash
nmap 192.168.100.1,2,3,4 -A
```

---

## Nmap → Nessus (Recommended Workflow)

Export Nmap results:

```bash
nmap -sS -sV -oX nmap_scan.xml 192.168.100.0/24
```

Import into Nessus:

```
Scans → Import → nmap_scan.xml
```

Why:

* Faster scans
* Accurate service mapping
* Less network noise

---

## Run Nessus Scan

1. **New Scan**
2. **Advanced Scan**
3. Targets → IPs or imported data
4. Launch

---

## Export Results

* PDF → reporting
* HTML → review
* CSV → analysis

---

## Mental Model (Audit)

```
Nmap  → what is exposed
Nessus → what is vulnerable / misconfigured
```

No exploitation. No evasion. Known scope.

---

## One-Page Workflow

```text
Install Nessus
↓
Discover hosts (nmap)
↓
Export nmap XML
↓
Import into Nessus
↓
Advanced Scan
↓
Report
```

