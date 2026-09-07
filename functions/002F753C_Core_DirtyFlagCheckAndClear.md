---
addresses: [0x002F753C, 0x002F7544]
tags: [function, state, helper]
status: understood
---

# [[FUN_002F753C]] / [[FUN_002F7544]] — Dirty Flag Check / Clear

These two always appear as a pair: check if dirty, then clear after processing.

---

## [[FUN_002F753C]] — Read Dirty Flag

```c
int FUN_002f753c(int obj) {
    return (int)(*(char *)(obj + 0x10));
}
```

Returns the byte at `+0x10` of an object. Non-zero = dirty / pending update.

---

## [[FUN_002F7544]] — Clear Dirty Flag

```c
void FUN_002f7544(int obj) {
    FUN_001329CC();       // unknown — notify / callback before clear
    *(obj + 0x10) = 0;   // clear dirty flag
}
```

Calls `FUN_001329CC` before clearing — likely notifies a listener or triggers a callback.

---

## Usage Pattern

```c
if (FUN_002F753C(obj)) {
    FUN_002F7544(obj);   // process + clear
}
```

Seen 3× in [[FUN_00100684]] — one standalone, one standalone, one inside loop ×2.

## See Also

[[FUN_00100684]]
