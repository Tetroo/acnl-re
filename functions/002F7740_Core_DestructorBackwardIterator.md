---
address: 0x002f7740
name: DestructorBackwardIterator
status: fully understood
tags: [save-system, iterator, destructor, helper]
decompiled_at: Session 22b
---

# [[FUN_002f7740|DestructorBackwardIterator]] — Backward Element Iterator (Destructor Helper)

## Summary

Generic backward iterator that calls a function pointer on each element from last to first. Used as the destructor pattern for C++ object arrays in the save buffer.

```c
void FUN_002f7740(void* base, void (*fn)(void*), int elem_size, int count) {
    for (int i = count - 1; i >= 0; i--)
        fn(base + i * elem_size);
}
```

## Notes

- Called by [[FUN_0012cc9c]] (save buffer destructor) to tear down all C++ objects
- Backward iteration = standard C++ destruction order (LIFO)
- Counterpart: [[FUN_00301d9c]] = forward iterator (constructor order)

## See Also

- [[FUN_0012cc9c]] — save buffer destructor; calls this on multiple object arrays
- [[FUN_00301d9c]] — forward iterator (construction)
