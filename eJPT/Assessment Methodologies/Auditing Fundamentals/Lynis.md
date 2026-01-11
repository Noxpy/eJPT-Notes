## Lynis – System Auditing & Hardening

Lynis is a **local security auditing tool** used to evaluate system hardening, configuration weaknesses, and compliance posture. It does not exploit vulnerabilities.

---

## Installation

```bash
sudo apt-get update
sudo apt-get install lynis
```

---

## Run a System Audit

```bash
sudo lynis audit system
```

* Requires root for full coverage
* Safe, read-only audit

---

## Understanding Results

### Test IDs

Example:

```text
KRNL-6000 | AUTH-9408 | SSH-7402 | FIRE-4590 | LOGG-2190
```

* Prefix indicates area:

  * `KRNL` – Kernel
  * `AUTH` – Authentication
  * `SSH` – SSH config
  * `FIRE` – Firewall
  * `LOGG` – Logging

Used for traceability, not manual fixing.

---

### Warnings vs Suggestions

* **Warnings** → real security risk, prioritize remediation
* **Suggestions** → hardening improvements, context-dependent

Skipped tests are **not failures** (service not installed or not applicable).

---

## Reports & Logs

```bash
/var/log/lynis.log
/var/log/lynis-report.dat
```

View warnings:

```bash
grep warning /var/log/lynis-report.dat
```

---

## Hardening Workflow

1. Run audit
2. Fix relevant **Warnings**
3. Re-run Lynis
4. Track Hardening Index for improvement over time
