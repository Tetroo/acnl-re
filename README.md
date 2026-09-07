# acnl-re

Reverse-engineering research and an early native C++20 port groundwork for **Animal Crossing: New Leaf — Welcome amiibo** (Nintendo 3DS, EUR, Title ID `0004000000198F00`).

**Goal:** a native PC reimplementation with 1:1 save-file compatibility (`garden_plus.dat`) and direct loading of original RomFS assets — not an emulator, not a clean-room rewrite from public specs, but a from-scratch native port informed by a real binary analysis of the original game.

This is an early-stage hobby project, produced with heavy AI assistance (Gemini for exploratory analysis, Claude for review/tooling/port code) working against the decompiled binary in Ghidra. See [Data Integrity](#data-integrity--known-issues) below before trusting anything in `functions/` or `systems/` as fact.

---

## What's here

| Folder | Contents |
|---|---|
| `functions/` | One file per analyzed function — address, signature, decompiled pseudocode, findings. Created for complex/key functions; trivial ones are tracked only in `symbols.csv`. |
| `systems/` | Higher-level write-ups per subsystem (weather, time, save format, villagers, museum, economy, etc.), assembled from the function docs. |
| `types/` | C++ struct/class headers matching the game's real binary layout (`#pragma pack`), meant to be reused directly by the PC port. |
| `binary/` | Memory map, heap inventory, known strings, misc binary-level notes. |
| `src/` | The actual PC port code (C++20) — time engine, town generator, RomFS loader, early rendering/world-curvature scaffolding. Very early / partial. |
| `symbols.csv` | Master symbol table: address, name, subsystem, status (`done` / `partial` / `flagged`), description. Source of truth for what's actually verified. |

Internal working notes, session logs, AI-agent instructions, and the local analysis toolchain (Ghidra MCP scripts, ROM extraction, etc.) are kept locally and are not part of this repository.

---

## Data integrity & known issues

Two full audit passes (2026-09-07) went through every file in `functions/` and `systems/`, cross-checking every claim against a live Ghidra decompile of the actual binary. They found that a meaningful portion of the AI-generated documentation contained **fabricated decompiled code and invented formulas** presented with full confidence — e.g. a nonexistent PRNG seed formula for lightning strikes, a stumble-probability mechanic attributed to a function that doesn't exist in the binary, offset tables shifted by copy-paste, a file-size claim that didn't match its own stated hex value. This is a known failure mode of LLM-assisted reverse engineering: the model cannot always distinguish "read from the tool output" from "plausible and self-generated" — both feel equally confident from the inside.

All confirmed fabrications across both passes (~23 files total) were rewritten against the real decompile output, with unconfirmed claims downgraded and marked explicitly. A small number of items remain genuinely unresolved (e.g. one function address that no longer resolves in the current Ghidra database) and are marked `flagged` rather than guessed at.

**Practical implication:** treat `symbols.csv` status as the trust signal —
- `done` = decompiled and semantically confirmed against live Ghidra output.
- `partial` = real function, but part of the analysis is unconfirmed/hypothesis.
- `flagged` = known problem (e.g. address doesn't point to the claimed function), needs rework.

Every `functions/*.md` and `systems/*.md` file has now been through at least one audit pass.

---

## Legal note — no game files included

This repository contains **no copyrighted Nintendo assets**: no ROM, no CIA, no extracted ExeFS/RomFS, no save files. Only original analysis (markdown notes, C++ headers, Python tooling) is tracked — see `.gitignore`. To reproduce the analysis yourself you need your own legally obtained copy of the game.

---

## Target binary

| Field | Value |
|---|---|
| Title | Animal Crossing: New Leaf — Welcome amiibo |
| Title ID | `0004000000198F00` |
| Region | EUR |
| Platform | Nintendo 3DS (ARM11, little-endian, PICA200 GPU) |
| Analysis tool | Ghidra 12.x + a GhidraMCP bridge, letting an AI agent drive analysis (decompile, rename, comment) directly |

---

## Building the port (early / incomplete)

```
cmake -B build
cmake --build build
```

`CMakeLists.txt` currently builds a static core library (`RomfsLoader`, `TownGenerator`, `TimeManager`) plus a minimal test harness. This is scaffolding, not a playable port.

---

## Contributing / prior art

If you're working on ACNL reverse engineering independently, this project would rather coordinate than duplicate effort — open an issue.
