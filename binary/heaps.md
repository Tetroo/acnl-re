---
tags: [binary, memory, heaps]
aliases: [heaps, heap inventory, HeapManager]
status: confirmed from data_items
---

# Heap Inventory

All named heaps share the same parent object at `0x00951C34` (HeapManager).
Each heap entry in `.text` has the pattern:
```
string ref  → heap name (e.g. "WeatherHeap")
DAT_xxx     → 0x00951C34  (HeapManager pointer)
```

Heap allocation size is passed as first arg to `FUN_002F6E5C` at init time.

---

## Known Heaps

| Name | Size (known) | Init function | Notes |
|------|-------------|---------------|-------|
| `DLLHeap` | ? | near `0x0010007C` | |
| `NetHeap` | `0x215800` (2.1 MB) | near `0x001000B4` | largest known |
| `UgcHeap` | ? | near `0x001000F8` | UGC = user generated content? |
| `FontHeap` | ? | near `0x00100138` | |
| `SaveHeap` | ? | near `0x0010017C` | see [[systems/save]] (backlog) |
| `WorkHeap` | ? | near `0x001001C0` | |
| `FaderHeap` | ? | near `0x00100204` | screen fader/transition |
| `FieldHeap` | `0x133000` (1.2 MB) | near `0x00100248` | field/map data |
| `PhotoHeap` | ? | near `0x00100294` | in-game camera photos |
| `SoundHeap` | ? | near `0x001002D8` | |
| `StageHeap` | `0xEB8000` (14.7 MB) | near `0x0010031C` | **largest** — stage/world data |
| `ScriptHeap` | ? | near `0x00100368` | |
| `NetGameHeap` | `0xD4800` (0.8 MB) | near `0x001003AC` | |
| `WeatherHeap` | `0x7600` (29.5 KB) | [[FUN_001003BC]] | fully reversed |
| `NoDevice` | ? | near `0x00100434` | unclear — maybe placeholder |
| `RealEstateHeap` | ? | near `0x0010047C` | real estate = HHA / exterior shop |
| `ModelEffectUnitHeap` | `0x2710` (10 KB) | near `0x001004C0` | |
| `SeadParticleHeap` | `0x23FC00` (2.2 MB) | near `0x00100510` | Nintendo sead particle engine |
| `HumanAnimBankHeap` | `0x238333` (2.2 MB) | near `0x0010055C` | character animations |
| `ModelEffectResHeap` | ? | near `0x001005A8` | model effect resources |
| `ParticleHandleHeap` | ? | near `0x001005F0` | |

---

## HeapManager Object

- **Address:** `0x00951C34` (in `.bss`)
- All `DAT_xxx` values near heap init code equal `0x00951C34`
- `DAT_001003FC == DAT_001009F8 == 0x00951C34` — confirmed same object for WeatherHeap init and cleanup
- Individual heap pointers stored as offsets within this object (e.g. WeatherHeap at `+0x10`)

---

## Notes

- `StageHeap` at 14.7 MB is by far the largest — holds the entire game world
- `HumanAnimBankHeap` and `SeadParticleHeap` both ~2.2 MB — animation heavy
- `NetHeap` at 2.1 MB — significant online infrastructure
- `FieldHeap` likely holds active map/acre data separately from StageHeap

## See Also

[[memory-map]], [[FUN_001003BC]], [[FUN_002F6E48]]
