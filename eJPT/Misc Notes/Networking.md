
### DNS Records

- A - Resolves a hostname or domain to an IPv4 address.
- AAAA - Resolves a hostname or domain to an IPv6 address.
- NS - Reference to the domain’s nameserver.
- MX - Resolves a domain to a mail server.
- CNAME - Used for domain aliases.
- TXT - Text record (commonly used for metadata, verification, and security policies).
- HINFO - Host information.
- SOA - Domain authority and zone metadata.
- SRV - Service records that define service locations.
- PTR - Resolves an IP address to a hostname.
- CAA - Specifies which Certificate Authorities are allowed to issue certificates for a domain.
- SPF / DKIM / DMARC - Email authentication mechanisms typically stored as TXT records.

- **DNS Zone Files** - Files that contain DNS records.
  - Zone files define how a domain resolves internally and externally.
  - Misconfigured or exposed zone transfers can leak internal hostnames and network structure.
```
dig axfr @nameserver domain.com
```

---

### Get Network Info

- ### ``ifconfig`` alternative

You can use the ``ip`` command from the ``iproute2`` suite instead of ``ifconfig`` to retrieve network information.
```
ip a s
```
- Stands for `ip addr show`.
- Provides more detailed and modern functionality than ``ifconfig``.

- View routing table:
```
ip route
```

- View active connections and listening services:
```
ss -tunlp
```

---

### Find Hosts

- If `ping` is unable to discover hosts, using `arp-scan` may help.
- `arp-scan` operates at Layer 2 and can identify hosts even when ICMP is blocked.
```
arp-scan -l
```

---

### Subnets Explained

- **Subnet Mask** - A subnet mask is a 32-bit number that divides an IP address into network and host portions. It consists of binary 1s (network portion) and 0s (host portion). The subnet mask determines whether traffic is local or must be routed elsewhere.

- **CIDR (Classless Inter-Domain Routing)** - CIDR notation represents an IP address and its routing prefix length using a slash (`/`) followed by the number of network bits.

The subnet mask `255.255.255.0` in binary is:
```
11111111.11111111.11111111.00000000
```
- This contains 24 consecutive `1` bits.
- Represented as `/24`.
- The first 24 bits define the network portion.
- The remaining 8 bits define the host portion.

Example:
```
192.168.1.0/24
```
- Network address: `192.168.1.0`
- Broadcast address: `192.168.1.255`
- Usable host range: `192.168.1.1 – 192.168.1.254`
- Total addresses: 256
- Usable hosts: 254

---

## What’s a NULL Session?

A [null session](https://www.blumira.com/glossary/null-session/) occurs when a connection is made to a system without providing a username or password. NetBIOS null sessions are a vulnerability found in CIFS/SMB implementations on certain systems.

- Historically used to enumerate:
  - users
  - groups
  - shares
  - policies

- Common enumeration tools:


### enum4linux

`enum4linux` is a Perl-based tool used to enumerate information from Windows and Samba (SMB) systems. It leverages null sessions and SMB-related calls to gather details without authentication when possible.

- Commonly used for **unauthenticated SMB enumeration**
- Useful during early-stage reconnaissance and enumeration

Basic usage:
```
enum4linux <target-ip>
```

Run all available enumeration modules:

```
enum4linux -a <target-ip>
```

Enumerate users:

```
enum4linux -U <target-ip>
```

Enumerate groups:

```
enum4linux -G <target-ip>
```

Enumerate shares:

```
enum4linux -S <target-ip>
```

Enumerate OS and domain information:

```
enum4linux -o <target-ip>
```

- Output may include:
  - local users and groups
  - shared resources
  - domain and hostname information
  - password policy details
- Results depend on SMB configuration and whether null sessions are permitted

  
```
rpcclient -U "" <ip>
```

- Modern systems usually restrict null sessions, but they still appear in legacy environments, misconfigured systems, and penetration testing labs.

- Null session abuse is primarily an **enumeration weakness**, not an exploitation technique.
- Enumeration focuses on extracting information by asking systems questions rather than immediately exploiting them.
