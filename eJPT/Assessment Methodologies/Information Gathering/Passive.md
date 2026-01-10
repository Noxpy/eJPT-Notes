### Things to Look For

- IPs, directories, names, emails, phone numbers, addresses  
- Web technologies in use  

---

## DNS Lookup
```
host example.com
```

* Resolves domain to IP(s)
* Can be blocked behind FW, proxy, or Cloudflare

---

## Hidden Directories

* `robots.txt` → lists disallowed paths for search engines
* `sitemap.xml` → full site structure for indexing

---

## Web Tech Fingerprinting

```bash
whatweb example.com
```

* Browser plugins: BuiltWith, Wappalyzer

---

## Website Download / Scraping

```bash
sudo apt install webhttrack
httrack http://example.com
```

---

## Domain Ownership Lookup

```bash
whois example.com
```

* Registrar, creation/expiry dates, nameservers

---

## Passive Recon / Internet-Wide

* **Shodan:** search IPs, ports, services, tech, SSL/TLS

```text
hostname:example.com
net:8.8.8.8
http.title:"admin"
product:nginx
```

* **DNSDumpster:** dnsdumpster.com
* **NetCraft:** searchdns.netcraft.com / sitereport.netcraft.com

---

## WAF Detection

```bash
wafw00f example.com
wafw00f example.com -a   # check all possible WAFs
```

---

## Subdomain Enumeration

* **Sublist3r (passive)**

```bash
sublist3r -d example.com
sublist3r -d example.com -e Netcraft  # enumerate via Netcraft
```

---

## Google Dorks

* Search operators:

```
site:example.com
inurl:login
intitle:"index of"
filetype:pdf
cache:example.com
```

* Look for sensitive files/directories: auth_user_file.txt, passwd.txt, wp-config.bak

---

## Historical Site View

* Wayback Machine: [http://web.archive.org/](http://web.archive.org/)

---

## TheHarvester (emails, subdomains, IPs)

```bash
theHarvester -d example.com -b duckduckgo -f report.html
```

* `-f` → save results (HTML/XML/JSON)

---

## DNS Recon

* **dnsrecon**

```bash
dnsrecon -d example.com
```

* Records: A, AAAA, MX, NS, SOA, TXT, SRV, PTR
* Zone transfer:

```bash
dig @ns1.example.com example.com AXFR
```

* AXFR = full zone transfer, IXFR = incremental

---

## Key DNS Record Types

| Record | Purpose               |
| ------ | --------------------- |
| A      | IPv4                  |
| AAAA   | IPv6                  |
| CNAME  | Alias                 |
| MX     | Mail server           |
| NS     | Name server           |
| SOA    | Zone metadata         |
| TXT    | SPF/DKIM/verification |
| SRV    | Service locator       |
| PTR    | Reverse DNS           |


### ProxyChains

- Edit config:
  
```
sudo nano /etc/proxychains4.conf
````
or

```
sudo vi /etc/proxychains4.conf
```

* Run tools through proxy:

```
proxychains dnsrecon -d example.com
```

```
proxychains nmap -Pn target
```

