# Function: FlowSeq_ExecuteActionNode (`0x005F305C`)

> **Address:** `0x005F305C`  
> **Subsystem:** Dialogue & Scripting (`script::FlowSeq`)  
> **Source Binary:** `exefs.elf`  
> **Status:** Fully Reverse-Engineered  
> **Related Files:** [`types/Script.h`](../types/Script.h), [`systems/script-engine.md`](../systems/script-engine.md)

---

## 1. Overview

`FlowSeq_ExecuteActionNode` is the primary execution handler for Type 2 (Action/Event) nodes in Nintendo's MSBF (Message Studio Binary Flow) virtual machine. When a conversation or event flowchart reaches an action node (such as rewarding an item, playing a sound effect, granting bells, or modifying a town flag), this function unpacks the node parameters, queries the event handler via the receiver interface, triggers execution, and transitions the flowchart state machine to the subsequent target node.

---

## 2. Function Signature

```cpp
namespace acnl::script {

struct FlowSeqContext;

/**
 * @brief Executes an action node in the active MSBF flowchart
 * @param self Pointer to script::FlowSeq instance (this)
 * @param node_context Node parameter buffer (auStack_30)
 * @param command_payload Pointer to command payload in FLW3 section
 * @return uint32_t Execution status (1 = Completed/Advanced, 2 = Yield/Waiting)
 */
uint32_t FlowSeq_ExecuteActionNode(FlowSeqContext* self, void* node_context, void* command_payload);

}
```

---

## 3. Decompiled C Implementation

```c
undefined1 * FlowSeq_ExecuteActionNode(int param_1, undefined4 param_2, undefined4 param_3)
{
  undefined1 uVar1;
  uint uVar2;
  int iVar3, iVar4, iVar6;
  undefined4 *puVar5;
  undefined4 uVar7;
  code *pcVar8;
  int *piVar9;
  bool bVar10;
  undefined8 uVar11;
  undefined1 auStack_30 [12];
  
  // 1. Unpack node payload into local stack buffer
  FUN_005b078c(auStack_30, param_3);
  piVar9 = *(int **)(param_1 + 0x24);
  
  // 2. Validate receiver interface and flow descriptor
  if ((char)piVar9[2] == '\0') {
    iVar6 = *(int *)(*(int *)(param_1 + 0x20) + 0x1c);
    if (iVar6 != 0) {
      *piVar9 = iVar6;
      iVar6 = *(int *)(iVar6 + 4);
      piVar9[1] = iVar6;
      if (iVar6 != 0) {
        iVar6 = *(int *)(iVar6 + 4);
        iVar3 = FUN_00754a04(auStack_30);
        uVar11 = FUN_007549d0(auStack_30);
        bVar10 = iVar6 != 0;
        iVar6 = (int)((ulonglong)uVar11 >> 0x20);
        if (bVar10) {
          iVar6 = iVar3;
        }
        if ((bVar10 && iVar6 != 0) && (int)uVar11 != 0) {
          // Resolve action opcode and argument index
          uVar7 = FUN_007549ec(auStack_30); // Action Command ID
          iVar3 = FUN_007549f8(auStack_30); // Param ID
          iVar6 = DAT_0095ceb4;
          iVar4 = FUN_001595cc(DAT_0095ceb4 + 0x110, uVar7);
          if ((iVar4 != 0) && (iVar6 = FUN_0075b7f4(iVar6, piVar9 + 0xb), iVar6 != 0)) {
            piVar9[3] = iVar3;
            piVar9[4] = -1;
            *(undefined1 *)piVar9[6] = 0;
            uVar1 = 1;
            *(undefined1 *)(piVar9 + 10) = 0;
            goto LAB_005f316c;
          }
        }
      }
    }
    goto LAB_005f319c;
  }
  
  // 3. Fallback / direct action dispatch
  if ((char)piVar9[2] == '\x05') {
    // Direct receiver check...
  }

LAB_005f319c:
  *(undefined1 *)(piVar9 + 2) = 4; // Mark node as failed/invalid

LAB_005f316c:
  *(undefined1 *)(piVar9 + 2) = uVar1;
  *(undefined1 *)(param_1 + 0x1e5) = 1;

LAB_005f3178:
  // 4. Trigger virtual action dispatch hook
  iVar6 = (uint)*(byte *)(*(int *)(param_1 + 0x24) + 8) * 8;
  uVar2 = *(uint *)(&DAT_0088ff30 + iVar6);
  pcVar8 = *(code **)(&DAT_0088ff2c + iVar6);
  if ((pcVar8 != (code *)0x0) || (((uVar2 & 1) != 0 && (uVar2 != 0)))) {
    if ((uVar2 & 1) != 0) {
      pcVar8 = *(code **)(pcVar8 + *(int *)(*(int *)(param_1 + 0x24) + ((int)uVar2 >> 1)));
    }
    (*pcVar8)(); // Invoke Action Handler
  }
  
  // 5. Query next node ID and transition
  if (*(char *)(param_1 + 0x1e5) == '\0') {
    if (*(int *)(param_1 + 0x1e0) < 0) {
      iVar6 = FUN_00754a20(auStack_30);
      if (iVar6 == 0) {
        *(undefined1 *)(param_1 + 0x1c) = 1; // Mark flow terminated
      }
      else {
        uVar7 = FUN_00754a3c(auStack_30);     // Fetch Next Node Index
        *(undefined4 *)(param_1 + 0x10) = uVar7; // Set Active Node
      }
    }
  }
  return auStack_30;
}
```

---

## 4. Key Takeaways for PC Port

1. **Deterministic Execution:** Unlike message nodes which await player input, action nodes execute synchronously in a single tick unless flagged with a yield condition (e.g. waiting for an item presentation animation).
2. **Decoupled Architecture:** The flow engine does not contain hardcoded gameplay effects; it delegates to the receiver subsystem via `IReceiver` interface (`FUN_0075b7f4`). In the PC port, this maps cleanly to a `std::function<void(uint16_t action_id, uint16_t param)>` dispatch map.
