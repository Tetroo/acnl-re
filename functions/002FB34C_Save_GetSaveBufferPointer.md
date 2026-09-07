---
address: 0x002FB34C
name: GetSaveBufferPointer
status: fully understood
tags: [save-system, accessor, singleton, compiler-artifact]
decompiled_at: Session 5, corrected Session 25, re-corrected Session 25b (TownAcres sweep)
---

# [[FUN_002FB34C]] — Save Buffer Base Pointer Getter (generic accessor)

## Summary

```c
undefined4 FUN_002fb34c(void)
{
    return *DAT_002FB358;
}
```

Disassembly (Session 25):

```
002fb34c: ldr r0,[0x2fb358]   ; r0 = literal pool word at 0x2fb358 = 0x0095AF74
002fb350: ldr r0,[r0,#0x0]    ; r0 = *(void**)0x0095AF74  (save buffer base pointer)
002fb354: bx lr
```

> [!IMPORTANT]
> **`DAT_002FB358` is a literal-pool constant whose VALUE is `0x0095AF74`** — the same save-buffer pointer-to-pointer address documented in `memory-bank/activeContext.md` (garden_plus.dat runtime pointer, set by [[FUN_005c9dd4]]). This is **not** a separate "time singleton" — it's the same double-indirection pattern seen in [[FUN_005c9a24]] (`DAT_005c9a38 = 0x0095AF74`), just a different literal-pool copy embedded near this function's own code.
>
> **This is the central, most widely-used accessor for the save buffer base pointer.** `get_xrefs_to(0x002fb34c)` returns **200+ distinct call sites** spread across nearly the whole `.text` range (`0x0010xxxx`–`0x0076xxxx`) — far more than the 15 direct-literal-pool readers of `0x0095AF74` enumerated in Session 24.

> [!IMPORTANT] Session 25b — the 200+ callers are (almost entirely) NOT genuine data reads
> A systematic sweep of **39 of the 247 unique caller functions** — chosen for diversity (save cluster `0x005cxxxx`, save/file cluster `0x001bxxxx`–`0x001cxxxx`, state machine `0x001dexxx`, date/weather cluster `0x0030xxxx`–`0x0033xxxx`, villager AI `0x0010ccbc`, network `0x006c9a28`, village menu `0x0058bc6c`/`0x0021xxxx`/`0x0069dec8`/`0x0024a8c0`) — found **100% of sampled sites use it as a naked tail-call inside an error/default/"unhandled enum value" branch**, e.g.:
> ```c
> if (uVar12 == 0x4f) {
>                     /* WARNING: Subroutine does not return */
>   FUN_002fb34c();
> }
> ```
> The call's return value is **never captured into a variable and never dereferenced with an offset** at any of the 39 sites checked — contrast with a genuine reader like [[FUN_005c9a24]], which does `iVar = get_ptr(); *(ushort*)(iVar + 0x292A4 + ...)`. Ghidra's decompiler marks the enclosing statement "does not return" precisely because these are unconditional `B` (branch, not `BL`) tail-calls at the end of `if`/`switch` arms — i.e. `return FUN_002fb34c()` used purely as a generic "return *some* non-null pointer" fallback for an unreachable/default case, not as a load-bearing data access. `FUN_002faec0` (previously identified, Session 24, as an abort/assert handler) shows up right alongside it in the same role in several of these functions (e.g. `FUN_001c0290`), reinforcing that both are reused trap/fallback targets rather than accessors in this context.
>
> **Practical consequence:** hunting through this xref list for a TownAcres reader is a low-yield strategy — the 200+ call count is inflated by this trap-reuse pattern, not by genuine save-data consumers. See `research/town-generation.md` Session 25b for the full negative result and the list of 39 functions checked. This does not contradict the disassembly above (the function genuinely is a simple getter) — it only means that in practice almost nobody *uses* the returned pointer; they just needed *a* call that returns control flow to their caller in a `B`-not-`BL` friendly way.

## Reinterpreting the Session 5 finding

Session 5 observed this function called from the weather/time period-index pipeline ([[FUN_0062EF1C]], [[FUN_0062EEE8]], [[FUN_0031872C]]) and concluded it was a "second time singleton getter" returning a date/period value directly. That is corrected: this function returns the **save buffer pointer**, not a scalar. The weather/time callers almost certainly read a small-offset field *off of* the returned pointer (e.g. a date/time header living near the start of the raw save block, offset `0x00`–`0x292A4`) rather than using the return value directly as a time value. The exact offset those callers read has not been re-derived yet — flagged as follow-up.

> [!WARNING] Discarded
> "Second time singleton getter returning a scalar time value" (Session 5) — the function returns a pointer (save buffer base), not a time value. Also discarded: "`DAT_002FB358` written from a `.cro` module" — it's a literal-pool constant equal to `0x0095AF74`, written the same way the save buffer pointer itself is (by [[FUN_005c9dd4]]).
> Non-returning function attribute (Ghidra tail-call artefact) — also discarded, function returns normally.

## Open Questions

- ~~Which callers among the 200+ read TownAcres (raw block, offset `< 0x292A4`)?~~ **Likely none** — Session 25b sampled 39 diverse callers, all trap/fallback pattern, zero genuine dereferences. Remaining ~208 unchecked callers not exhaustively ruled out, but the pattern is strong enough that further sweeping this specific list is not recommended as a primary strategy.
- What offset do the weather/time callers ([[FUN_0062EF1C]] etc.) read off the returned buffer pointer? Still open, but re-examine in light of Session 25b: `FUN_0062ef1c` itself wasn't in the 39-sample; if revisited, check whether it's also a trap-pattern call rather than a genuine offset read.

## See Also

- [[FUN_005c9a24]] — villager reader, uses its own private literal-pool copy of `0x0095AF74` instead of this wrapper
- [[FUN_005c9dd4]] — save buffer constructor, writes `0x0095AF74`
- `research/town-generation.md` — TownAcres search status
- [[FUN_0062EF1C]] / [[FUN_0062EEE8]] / [[FUN_0031872C]] — weather/time callers, likely read a near-zero offset off the save buffer
