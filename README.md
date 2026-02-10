<p align="center">
  <img src="https://i.imgur.com/enDjxat.jpg" width="200">
</p>

<h1 align="center">Tyton</h1>
<h3 align="center">Linux Kernel-Mode Rootkit Hunter</h3>

<p align="center">
  <a href="https://github.com/nbulischeck/tyton/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-GPL-brightgreen.svg" alt="License"></a>
  <img src="https://img.shields.io/badge/kernel-4.4.0--6.18.x-blue.svg" alt="Kernel Support">
  <img src="https://img.shields.io/badge/arch-x86__64-orange.svg" alt="Architecture">
  <img src="https://img.shields.io/badge/DKMS-supported-purple.svg" alt="DKMS">
</p>

<p align="center">
  <b>Actively maintained fork</b> of <a href="https://github.com/nbulischeck/tyton">nbulischeck/tyton</a> (archived Nov 2021).<br>
  Patched for modern Linux kernels 5.13 through 6.18+ with full backward compatibility.
</p>

---

## Detected Attacks

| Attack Vector | Description |
|:---|:---|
| Hidden Modules | Detects kernel modules hidden from `/proc/modules` and sysfs |
| Syscall Table Hooking | Identifies modified syscall table entries |
| Network Protocol Hooking | Detects hooks in network protocol handlers |
| Netfilter Hooking | Finds unauthorized netfilter hook registrations |
| Zeroed Process Inodes | Detects processes with zeroed-out `/proc` inodes |
| Process Fops Hooking | Identifies hooked file operations in `/proc` |
| IDT Hooking | Detects Interrupt Descriptor Table modifications |

## Features

- **Real-time Scanning** — Configurable interval scanning via `timeout` module parameter
- **Desktop Notifications** — Userland daemon using libnotify for real-time alerts via journald
- **DKMS Support** — Seamless kernel upgrades on rolling-release distributions
- **Wide Kernel Support** — From 4.4.0-31 to 6.18.x with version-guarded compatibility

---

## Installation

### Dependencies

| Package | Arch / Garuda | Debian / Ubuntu / Kali | Fedora / CentOS |
|:---|:---|:---|:---|
| Kernel Headers | `linux-headers` | `linux-headers-$(uname -r)` | `kernel-devel` |
| GCC | `gcc` | `gcc` | `gcc` |
| Make | `make` | `make` | `make` |
| Libnotify | `libnotify` | `libnotify-dev` | `libnotify-devel` |
| Libsystemd | `libsystemd` | `libsystemd-dev` | `systemd-devel` |
| Pkg-Config | `pkgconfig` | `pkg-config` | `pkgconfig` |
| GTK3 | `gtk3` | `libgtk-3-dev` | `gtk3-devel` |

### Garuda / Arch Linux

```bash
# Install dependencies
sudo pacman -S linux-headers gcc make libnotify libsystemd pkgconfig gtk3

# Clone this fork
git clone https://github.com/joseguzman1337/tyton.git
cd tyton
make
sudo insmod tyton.ko
```

> **Recommended**: Install via DKMS for automatic rebuilds on kernel upgrades:
> ```bash
> yay -S tyton-dkms-git   # or paru, pakku
> ```

### Ubuntu / Debian / Kali

```bash
sudo apt install linux-headers-$(uname -r) gcc make libnotify-dev pkg-config libgtk-3-dev libsystemd-dev
git clone https://github.com/joseguzman1337/tyton.git
cd tyton
make
sudo insmod tyton.ko
```

### Fedora / CentOS

```bash
dnf install kernel-devel gcc make libnotify libnotify-devel systemd-devel gtk3-devel gtk3
git clone https://github.com/joseguzman1337/tyton.git
cd tyton
make
sudo insmod tyton.ko
```

### Module Arguments

```bash
# Set scan interval to X minutes (default: 5)
sudo insmod tyton.ko timeout=X
```

---

## Kernel 5.13–6.18+ Compatibility Patches

The upstream project was archived with support up to ~kernel 5.x. This fork adds **version-guarded patches** for all breaking kernel API changes, maintaining full backward compatibility with older kernels.

### Unexported Symbols (>= 5.13)

Several kernel symbols were removed from the export table. This fork resolves them **dynamically at runtime** using kprobe-based `lookup_name()`:

| File | Symbol | Resolution |
|:---|:---|:---|
| `include/core.h` | `module_mutex` | Dynamic lookup via `lookup_name()` |
| `src/util.c` | `__module_address()` | Dynamic lookup via `lookup_name()` |
| `src/module_list.c` | `find_module()` | `compat_find_module()` wrapper with dynamic lookup |

### Struct Layout Changes (>= 6.4)

| File | Change |
|:---|:---|
| `src/module_list.c` | `mod->core_layout` → `mod->mem[MOD_TEXT]` |

### API Signature Changes (>= 4.7 / 6.1)

| File | Change | Kernel |
|:---|:---|:---|
| `src/proc.c` | `filldir_fn` return: `int` → `bool` | >= 6.1 |
| `src/proc.c` | `f_op->iterate` → `f_op->iterate_shared` | >= 4.7 |

### Removed Interfaces

| File | Change |
|:---|:---|
| `src/netfilter_hooks.c` | `nf_hook_mutex` → `rcu_dereference_raw()` for safe hook traversal |
| `src/network_hooks.c` | `proc_fops` detection removed (struct member gone); `seq_ops->show` retained |

### Verified On

| Kernel | Version | Status |
|:---|:---|:---:|
| Arch Linux (`linux`) | 6.18.8-arch2-1 | Builds + loads |
| Garuda Linux (`linux-zen`) | 6.18.8-zen2-1-zen | Builds + loads |
| Hardened (`linux-hardened`) | 6.17.13-hardened1-2-hardened | Builds + loads |

---

## Credits

- Original project by [Nick Bulischeck](https://github.com/nbulischeck)
- Kernel 5.13–6.18+ patches by [@joseguzman1337](https://github.com/joseguzman1337)
