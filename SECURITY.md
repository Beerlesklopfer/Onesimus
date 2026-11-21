# Security policy

Onesimus is a Qt6 GUI for backup systems. It holds connection profiles with
credentials and talks to the Director over TLS. We take reports about its
security seriously and handle them as described here. This policy also serves
the vulnerability handling required by the EU Cyber Resilience Act
(Regulation (EU) 2024/2847).

## Supported versions

| Version | Security fixes |
|---|---|
| development branch (before v1.0.0) | yes, newest state only |
| 0.x releases | no, update to the newest state |
| from v1.0.0 | the support period is stated with each release |

## Reporting a vulnerability

Please report a vulnerability **privately**, never in a public issue,
discussion or pull request:

- on GitHub: **Security → Report a vulnerability** in this repository
  (private vulnerability reporting), or
- by e-mail to **contact@onesimus.io** with the subject `SECURITY`.

Please include the affected version or commit, the platform, the steps to
reproduce and what an attacker gains. A proof of concept helps; please do not
test against systems that are not yours.

The Rust backup stack (Director `odir`, file daemon `ofd`, storage daemon
`osd`, console `oconsole`) has its own repository and the same policy; you can
report issues for both here.

## What happens next

1. We confirm receipt within **7 days**.
2. We assess the report and tell you whether we consider it a vulnerability
   and how severe it is.
3. We fix it in the supported versions and agree a publication date with you.
   We aim to publish a fix within 90 days.
4. We publish a GitHub Security Advisory (with a CVE where it applies) and
   credit you, unless you prefer not to be named.

Actively exploited vulnerabilities and severe incidents are reported to the
authorities as Article 14 of the Cyber Resilience Act requires.

## Scope

In scope: the Onesimus GUI in this repository, its installers and packages,
and the Rust stack as above. Out of scope: Bareos itself (report to the
Bareos project), third-party libraries (report upstream; tell us if Onesimus is
affected), and findings that need an already compromised machine or Director.
