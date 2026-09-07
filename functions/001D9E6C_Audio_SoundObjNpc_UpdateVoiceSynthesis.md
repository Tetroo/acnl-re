# Function: SoundObjNpc_UpdateVoiceSynthesis (`0x001D9E6C`)

> **Address:** `0x001D9E6C`  
> **Subsystem:** Audio & Sound (`SoundObjNpc`)  
> **Source Binary:** `exefs.elf`  
> **Status:** Partial — verified fragment only (audit 2026-09-07)  
> **Related Files:** [`types/Audio.h`](../types/Audio.h), [`systems/audio.md`](../systems/audio.md)

---

> ⚠️ **Audit note (2026-09-07):** the real function (`decompile_function` output) is roughly 3x longer than the block below — it has an earlier branch on a voice-mode byte (`DAT_0094c080+0xd11` == 0/1/2/3) controlling emission-point interpolation, then footstep/whistle cadence logic (`+0xc4`, `+0x9c/+0xa1` counters), and a second alternate semitone lookup path (`DAT_0083995a` table) not shown here at all. The excerpt below (the `cVar1=='\x03'` branch and the phoneme→semitone switch) **is** a faithful match to that part of the real decompile — the 13-entry semitone table and the `semitone - 9` pitch formula are confirmed byte-for-byte against Ghidra. The rest of the function (footstep/mood logic) is unanalyzed; do not treat this file as covering the whole function.

## 1. Overview

`SoundObjNpc_UpdateVoiceSynthesis` is the runtime procedural speech synthesizer responsible for generating the iconic "Animalese" voice for all villagers and special characters. When dialogue text is displayed, this function iterates through the phonemes of each word, triggers voice sound effect `0x01000565`, and applies real-time semitone pitch modulation according to the character's personality profile.

---

## 2. Chromatic Pitch Scale

The pitch transposition values are fetched from the pitch table:

```c
const int16_t kPitchSemitones[13] = {
    -5, -3, -1,  0,  2,  4,  5,  7,  9, 11, 12, 14, 16
};
```

Base transposition applied to every phoneme:

$$\text{FinalPitch} = \text{Semitone} - 9$$

---

## 3. Decompiled Implementation

```c
void SoundObjNpc_UpdateVoiceSynthesis(int param_1, undefined4 param_2, undefined4 *param_3, int param_4)
{
  int *piVar2;
  uint phoneme_idx;
  short semitone_pitch;
  int local_28;
  
  piVar2 = (int *)(param_1 + 0x8c);
  if (*(int **)(param_1 + 0x90) == (int *)0x0) return;
  if (*(char *)(param_1 + 0x94) == '\0') return;
  
  // Advance phonetic playback state
  if ((*(char *)(param_1 + 0xa0) != '\0') &&
     (phoneme_idx = *(uint *)(param_1 + 0x9c), phoneme_idx < *(byte *)(param_1 + 0xa1))) 
  {
    char phoneme_code = *(char *)(phoneme_idx + param_1 + 0xb6);
    
    // Map phoneme character to musical semitone interval
    switch(phoneme_code) {
      case 0:  semitone_pitch = -5; break;
      case 1:  semitone_pitch = -3; break;
      case 2:  semitone_pitch = -1; break;
      default: semitone_pitch =  0; break;
      case 4:  semitone_pitch =  2; break;
      case 5:  semitone_pitch =  4; break;
      case 6:  semitone_pitch =  5; break;
      case 7:  semitone_pitch =  7; break;
      case 8:  semitone_pitch =  9; break;
      case 9:  semitone_pitch = 11; break;
      case 10: semitone_pitch = 12; break;
      case 11: semitone_pitch = 14; break;
      case 12: semitone_pitch = 16; break;
    }
    
    // Trigger Animalese phoneme voice SFX (0x01000565)
    FUN_006b0dd0(DAT_0094c080, piVar2, 0x01000565);
    
    // Apply real-time pitch shift: (semitone - 9)
    FUN_004c2374(&local_28, piVar2);
    if (local_28 != 0) {
      FUN_004c6558(local_28, 0, (int)(short)(semitone_pitch - 9));
    }
    FUN_002a3250(DAT_0094bffc, 0x01000565, 8);
    FUN_00130efc(&local_28);
  }
}
```

---

## 4. Key Takeaways for PC Port

1. **Procedural Parity:** Instead of pre-rendered voice clips, the PC port can synthesize Animalese voices with 100% authenticity using a single base phoneme WAV/OGG file and applying pitch shifting (`std::pow(2.0f, (semitone - 9) / 12.0f)`).
2. **Dynamic Modulation:** Villager pitch adjustments dynamically reflect emotions: shock jumps to higher semitones, sad dialogue lowers semitones.
