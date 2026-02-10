# Tyton (Maintained Fork)

> **Note**: This is an actively maintained fork of [nbulischeck/tyton](https://github.com/nbulischeck/tyton), which has been archived since November 2021. This fork adds compatibility with modern Linux kernels (5.13 through 6.18+).

[![license](https://img.shields.io/badge/license-GPL-brightgreen.svg)](https://github.com/nbulischeck/tyton/blob/master/LICENSE)
[![version](https://img.shields.io/badge/linux-4.4.0.31--6.18.x-blue.svg?style=flat)](https://github.com/joseguzman1337/tyton)

<img align="left" src="https://i.imgur.com/enDjxat.jpg" style="padding: 25px">

Linux Kernel-Mode Rootkit Hunter for 4.4.0-31 through 6.18.x.

For more information, visit [Tyton's website](https://nbulischeck.github.io/tyton/).

## Detected Attacks

**Hidden Modules**

**Syscall Table Hooking**
  
**Network Protocol Hooking**

**Netfilter Hooking**

**Zeroed Process Inodes**

**Process Fops Hooking**

**Interrupt Descriptor Table Hooking**

&nbsp;

## Additional Features

**Notifications**: Users (including myself) do not actively monitor their journald logs, so a userland notification daemon has been included to monitor journald logs and display them to the user using libnotify. Notifications are enabled after install by XDG autorun, so if your DM does not have `/etc/xdg/autostart` it will fail.

**DKMS**: Dynamic Kernel Module Support has been added for Arch and Fedora/CentOS (looking to expand in the near future). DKMS allows the (near) seamless upgrading of Kernel modules during kernel upgrades. This is mainly important for distributions that provide rolling releases or upgrade their kernel frequently.

## Installing

### Dependencies

* Linux Kernel 4.4.0-31 or greater
* Corresponding Linux Kernel Headers
* GCC
* Make
* Libnotify
* Libsystemd
* Package Config
* GTK3

### From Source

#### Ubuntu/Debian/Kali

1. `sudo apt install linux-headers-$(uname -r) gcc make libnotify-dev pkg-config libgtk-3-dev libsystemd-dev`
2. `git clone https://github.com/nbulischeck/tyton.git`
3. `cd tyton`
4. `make`
5. `sudo insmod tyton.ko`

> **Note**: For Ubuntu 14.04, libsystemd-dev is named libsystemd-journal-dev.

#### Arch

1. `sudo pacman -S linux-headers gcc make libnotify libsystemd pkgconfig gtk3`
2. `git clone https://github.com/nbulischeck/tyton.git`
3. `cd tyton`
4. `make`
5. `sudo insmod tyton.ko`

> **Note**: It's recommended to install Tyton through the AUR so you can benefit from DKMS.

#### Fedora/CentOS

1. `dnf install kernel-devel gcc make libnotify libnotify-devel systemd-devel gtk3-devel gtk3`
2. `git clone https://github.com/nbulischeck/tyton.git`
3. `cd tyton`
4. `make`
5. `sudo insmod tyton.ko`

##### Kernel Module Arguments

The kernel module can be passed a specific timeout argument on insertion through the command line.

To do this, run the command `sudo insmod tyton.ko timeout=X` where X is the number of minutes you would like the kernel module to wait before executing its scan again.

### AUR

Tyton is available on the AUR [here](https://aur.archlinux.org/packages/tyton-dkms-git/).

You can install it using the AUR helper of your choice:

* `yaourt -S tyton-dkms-git`
* `yay -S tyton-dkms-git`
* `pakku -S tyton-dkms-git`

## Kernel 5.13–6.18+ Compatibility Patches

The upstream project was archived with support only up to ~kernel 5.x. This fork adds version-guarded patches for the following kernel API changes:

### Unexported Symbols (>= 5.13)
Several kernel symbols (`module_mutex`, `find_module`, `__module_address`) are no longer exported to modules. This fork resolves them dynamically at runtime using kprobe-based `lookup_name()`:
- **`include/core.h`** — Dynamic `module_mutex` resolution
- **`src/util.c`** — Dynamic `__module_address()` lookup
- **`src/module_list.c`** — Dynamic `find_module()` lookup via `compat_find_module()`

### Struct Changes (>= 6.4)
- **`src/module_list.c`** — `mod->core_layout` replaced with `mod->mem[MOD_TEXT]` (struct module layout change)

### API Changes (>= 6.1)
- **`src/proc.c`** — `filldir_fn` return type changed from `int` to `bool`
- **`src/proc.c`** — `f_op->iterate` replaced with `f_op->iterate_shared` (>= 4.7)

### Removed Interfaces
- **`src/netfilter_hooks.c`** — `nf_hook_mutex` (static symbol, never exported) replaced with `rcu_dereference_raw()` for safe hook traversal
- **`src/network_hooks.c`** — `proc_fops`-based hook detection removed (struct member removed in recent kernels); `seq_ops->show` verification retained

### Tested On
- `linux 6.18.8-arch2-1`
- `linux-zen 6.18.8-zen2-1-zen`
- `linux-hardened 6.17.13-hardened1-2-hardened`
