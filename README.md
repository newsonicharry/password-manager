# Password Manager

A terminal-based password manager with a TUI (Terminal User Interface), built in C++23. All data is stored locally and encrypted with industry-standard cryptography. Each user has their own encrypted vault file, and existing passwords can be imported from Bitwarden.

---

## Features

- Vaults are encrypted using AEGIS-256 via libsodium, with Argon2id key derivation
- Wipes all memory storing secure data after use
- File integrity is verified via a MAC with libsodium using associated data
- Parses and converts Bitwarden JSON exports into a custom vault format
- Fully navigable by keyboard
- Generates strong random passwords with configurable options
- Rates passwords from Very Weak to Very Strong
- Each user account gets its own `.encrypted` file on disk


---

## Usage

todo on finished version

---

## Dependencies

| Library | Purpose |
|---|---|
| [libsodium](https://libsodium.gitbook.io/doc/installation/) | Encryption, key hashing, and random number generation |
| [FTXUI](https://github.com/ArthurSonzogni/FTXUI) | TUI rendering |
| [nlohmann/json](https://github.com/nlohmann/json) | Parsing Bitwarden JSON exports |

**nlohmann/json** is already fetched automatically by CMake, though **FTXUI** must be installed within`dependencies/FTXUI/` and **libsodium** must be installed on the system.

---

## Building

### Prerequisites

- CMake 3.20+
- C++23 compiler
- libsodium
- pkg-config

**Libsodium installation:** 
Follow the installation directions and install it from [here](https://libsodium.gitbook.io/doc/installation), directly from libsodium


### Clone and Build

```bash
git clone https://github.com/newsonicharry/password-manager.git
cd password-manager

# installs FTXUI
mkdir dependencies
git clone https://github.com/ArthurSonzogni/FTXUI dependencies/FTXUI

# configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# run
./build/password_manager
```

---

## Data Storage

Vault files are stored in a platform-specific location:

| Platform | Path |
|---|---|
| Linux | `~/.local/share/password-manager/passwords-list/` |
| Windows | `%APPDATA%\password-manager\passwords-list\` |

Each user's vault is stored as `<username>.encrypted`.

---

## Encryption Architecture

The vault file uses a custom binary format with authenticated encryption:

**File Header (67 bytes total):**
| Field | Size | Description |
|---|---|---|
| Magic bytes | 8 bytes | `\0Encrypt` — identifies the file format |
| Nonce | 32 bytes | Randomly generated per encryption |
| Salt | 16 bytes | Used for Argon2id key derivation |
| Iterations | 1 byte | Argon2id iteration count |
| Entry count | 2 bytes | Number of stored entries |
| Message size | 8 bytes | Size of the encrypted payload |

- **Cipher:** `AEGIS-256` (via libsodium)  
- **Key derivation:** `Argon2id` (via libsodium) 
- **Associated data:** All header fields except the magic bytes (59 bytes total) are authenticated but not encrypted
- **Sensitive data in memory:** Passwords and key material are stored in `SecureBuffer`, which zeroes memory on destruction

**Per-entry binary format:**

Each entry begins with a `0x00` initial magic byte, followed by typed fields. Each field is identified by a 1-byte magic identifier and a 2-byte length prefix:

| Magic ID | Field |
|---|---|
| `1` | Site |
| `2` | Username |
| `3` | Password |
| `4` | Notes |
| `5` | Date created (timestamp) |
| `6` | Date modified (timestamp) |

Unknown magic identifiers are silently ignored

---

## Bitwarden Import

On first setup you can import a Bitwarden **unencrypted JSON export**:

1. In Bitwarden, go to **Tools -> Export Vault** and choose JSON format.
2. Launch the password manager and during the setup put the *exact* path (if not it will just fail).

The converter will parse only the `login` entries, any special ones such as `card`, `identity`, `note` or `SSH key` will be ignored.

