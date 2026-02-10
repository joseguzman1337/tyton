<img align="left" src="https://i.imgur.com/enDjxat.jpg" width="140">

# Tyton

**Linux Kernel-Mode Rootkit Hunter**

[![license](https://img.shields.io/badge/license-GPL-brightgreen.svg)](https://github.com/nbulischeck/tyton/blob/master/LICENSE) ![kernel](https://img.shields.io/badge/kernel-4.4.0--6.18.x-blue.svg) ![arch](https://img.shields.io/badge/arch-x86__64-orange.svg) ![dkms](https://img.shields.io/badge/DKMS-supported-purple.svg)

Actively maintained fork of [nbulischeck/tyton](https://github.com/nbulischeck/tyton) (archived Nov 2021). Reverse-engineered and ported to modern Linux kernels 5.13–6.18+, Feb 2026. Full backward compatibility preserved.

<br clear="left">

---

## What does it detect?

<details>
<summary><b>7 rootkit attack vectors</b> — from hidden modules to IDT hooks</summary>

| Attack Vector | Description |
|:---|:---|
| Hidden Modules | Detects kernel modules hidden from `/proc/modules` and sysfs |
| Syscall Table Hooking | Identifies modified syscall table entries |
| Network Protocol Hooking | Detects hooks in network protocol handlers |
| Netfilter Hooking | Finds unauthorized netfilter hook registrations |
| Zeroed Process Inodes | Detects processes with zeroed-out `/proc` inodes |
| Process Fops Hooking | Identifies hooked file operations in `/proc` |
| IDT Hooking | Detects Interrupt Descriptor Table modifications |

</details>

<details>
<summary><b>Features</b> — real-time scanning, desktop alerts, DKMS</summary>

- **Real-time Scanning** — Configurable interval via `timeout` module parameter
- **Desktop Notifications** — Userland daemon using libnotify for real-time alerts via journald
- **DKMS Support** — Seamless kernel upgrades on rolling-release distributions
- **Wide Kernel Support** — From 4.4.0-31 to 6.18.x with version-guarded compatibility

</details>

---

## Installation

<details>
<summary><b>Garuda / Arch Linux</b> — pacman + DKMS</summary>

```bash
sudo pacman -S linux-headers gcc make libnotify libsystemd pkgconfig gtk3 && git clone https://github.com/joseguzman1337/tyton.git && cd tyton && make && sudo insmod tyton.ko
```

```bash
yay -S tyton-dkms-git   # recommended: auto-rebuilds on kernel upgrades
```

</details>

<details>
<summary><b>Ubuntu / Debian / Kali</b> — apt</summary>

```bash
sudo apt install linux-headers-$(uname -r) gcc make libnotify-dev pkg-config libgtk-3-dev libsystemd-dev && git clone https://github.com/joseguzman1337/tyton.git && cd tyton && make && sudo insmod tyton.ko
```

</details>

<details>
<summary><b>Fedora / CentOS</b> — dnf</summary>

```bash
dnf install kernel-devel gcc make libnotify libnotify-devel systemd-devel gtk3-devel gtk3 && git clone https://github.com/joseguzman1337/tyton.git && cd tyton && make && sudo insmod tyton.ko
```

</details>

<details>
<summary><b>Dependencies table</b> — package names across distros</summary>

| Package | Arch / Garuda | Debian / Ubuntu / Kali | Fedora / CentOS |
|:---|:---|:---|:---|
| Kernel Headers | `linux-headers` | `linux-headers-$(uname -r)` | `kernel-devel` |
| GCC | `gcc` | `gcc` | `gcc` |
| Make | `make` | `make` | `make` |
| Libnotify | `libnotify` | `libnotify-dev` | `libnotify-devel` |
| Libsystemd | `libsystemd` | `libsystemd-dev` | `systemd-devel` |
| Pkg-Config | `pkgconfig` | `pkg-config` | `pkgconfig` |
| GTK3 | `gtk3` | `libgtk-3-dev` | `gtk3-devel` |

</details>

<details>
<summary><b>Module arguments</b> — customize scan interval</summary>

```bash
sudo insmod tyton.ko timeout=X   # scan every X minutes (default: 5)
```

</details>

---

## Kernel 5.13–6.18+ Patches

> The upstream project died on ~kernel 5.x. We brought it back from the dead.
> Patched and tested on Garuda Linux, February 2026.

<details>
<summary><b>Unexported symbols</b> — 3 kernel symbols resolved at runtime via kprobes</summary>

Several kernel symbols (`module_mutex`, `find_module`, `__module_address`) were removed from the export table in 5.13+. This fork resolves them **dynamically at runtime** using kprobe-based `lookup_name()`:

| File | Symbol | Resolution |
|:---|:---|:---|
| `include/core.h` | `module_mutex` | Dynamic lookup via `lookup_name()` |
| `src/util.c` | `__module_address()` | Dynamic lookup via `lookup_name()` |
| `src/module_list.c` | `find_module()` | `compat_find_module()` wrapper with dynamic lookup |

</details>

<details>
<summary><b>Struct layout changes</b> — module memory representation rewrite in 6.4</summary>

| File | Change |
|:---|:---|
| `src/module_list.c` | `mod->core_layout` → `mod->mem[MOD_TEXT]` |

</details>

<details>
<summary><b>API signature changes</b> — bool returns, iterator rework</summary>

| File | Change | Kernel |
|:---|:---|:---|
| `src/proc.c` | `filldir_fn` return: `int` → `bool` | >= 6.1 |
| `src/proc.c` | `f_op->iterate` → `f_op->iterate_shared` | >= 4.7 |

</details>

<details>
<summary><b>Removed interfaces</b> — netfilter mutex and proc_fops gone</summary>

| File | Change |
|:---|:---|
| `src/netfilter_hooks.c` | `nf_hook_mutex` → `rcu_dereference_raw()` for safe hook traversal |
| `src/network_hooks.c` | `proc_fops` detection removed (struct member gone); `seq_ops->show` retained |

</details>

---

## Verified On

Tested on **Garuda Linux** (February 2026) — all kernels installed on the same system, built via DKMS:

| Kernel | Version | Date | Status |
|:---|:---|:---|:---:|
| `linux-zen` | 6.18.8-zen2-1-zen | Feb 10, 2026 | **Builds + loads** |
| `linux` | 6.18.8-arch2-1 | Feb 10, 2026 | **Builds + loads** |
| `linux-hardened` | 6.17.13-hardened1-2-hardened | Feb 10, 2026 | **Builds + loads** |

---

<p align="center">
  Original project by <a href="https://github.com/nbulischeck">Nick Bulischeck</a><br>
  Reverse-engineered and ported to kernel 5.13–6.18+ by <a href="https://github.com/joseguzman1337">@joseguzman1337</a> & <a href="https://claude.ai">Claude</a>
</p>
