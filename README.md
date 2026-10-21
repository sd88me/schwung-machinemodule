# Machinemodule for Schwung (Ableton Move)

The Elektron Machinedrum SPS-1 UW sound engine on Ableton Move, as a [Schwung](https://github.com/charlesvestal/schwung) sound
generator. It is the same engine as the [desktop plugin and the MPC builds](https://github.com/sd88me/mpc-vst-machinedrum): the
Machinedrum's own DSP code and OS running in an emulator, with its track effects, mixer and master effects. The 16 tracks sit on
Move's 16 drum pads, and the pages follow the pad you hit, in the style of [DR32](https://github.com/legsmechanical/schwung-dr32).

Not affiliated with, endorsed by or sponsored by Elektron. **No firmware is included:** you upload your own Machinedrum OS file.

## Install

Requires Schwung 1.7.3 or later (tested on 1.7.3, on a standard Move).

1. Download `machinemodule-module.tar.gz` from the [Releases](../../releases) page. In Schwung Manager, open **Modules** and use
   **Install from File**.
2. On Machinemodule's page in Schwung Manager, upload your files:
   - **os** (required): the Machinedrum UW OS 1.63 `.syx`, a free download from Elektron's Machinedrum support page.
   - **flash** (optional): the Machinedrum UW flash image `.bin`. It gives you the 16 factory kits and the ROM machines' samples.
     The first start after the upload imports it by booting the emulated Machinedrum once: about 6 minutes, with the module silent
     meanwhile. After that it starts in about 2 seconds.
   - **kits** (optional): your own kit dumps (`.syx`, one or many kits per file).
3. Restart Move, add **Machinemodule** to a track and play the pads.

## Playing it

Tracks 1-16 are the 16 drum pads (notes 36-51). Hit a pad and every track page shows that track. While a pattern runs, only a pad
you hit with your finger moves the pages, not the notes the sequencer plays.

Jog moves through the pages:

| Page | Knobs |
|---|---|
| **Kit** | a list: Init, the factory kits, then your own kits ("User:"). Choosing one loads it |
| **Track** | TRK (the track on screen), MACH (its machine), LEVEL, MUTE |
| **Synth** | the machine's eight SYN knobs, with the machine's own names (CTR-8P: its targets instead) |
| **Amp/Filter** | AMD AMF EQF EQG FLTF FLTW FLTQ SRR |
| **Mix** | DIST VOL PAN DEL REV |
| **LFO** | TRACK PARAM SHP1 SHP2 UPDTE SPEED DEPTH SHMIX |
| **Echo** | TIME MOD MFRQ FB FLTF FLTW MONO LEV |
| **Reverb** | DVOL PRED DEC DAMP HP LP GATE LEV |
| **EQ** | LF LG HF HG PF PG PQ GAIN |
| **Dynamix** | ATCK REL TRHD RTIO KNEE HP OUTG MIX |
| **Master** | FX, LTNCY, VELMD, ACTHR, ACAMT, VOICS, LOAD, PEAK |

The machines are all of the UW's except MIDI, RAM and INP: there is no MIDI out, sampling or audio input here. CTR-AL, CTR-8P and
CTR-RE/GB/EQ/DX work as on the Machinedrum. The ROM machines need the flash image.

**Master page**

- **FX**: the Machinedrum's master effects. *Full* is all of them. *Rev+Echo* skips the EQ and Dynamix, which saves about a quarter of
  their CPU; it sounds louder, because the Dynamix is a compressor. *Off* saves all of it.
- **LTNCY**: how far ahead the engine renders, in blocks of 2.9 ms. Default 2 (5.8 ms).
- **VELMD**: *MD* plays the Machinedrum's way: a hit at or above **ACTHR** is accented, a softer one plays **ACAMT** softer. *VEL*
  makes velocity set the level, like Move's own instruments.
- **VOICS**: how many tracks may sound at once (4-16; 16 = no limit). Default 10. Past it, the oldest voice is cut. A ROM machine
  counts as two.
- **LOAD**: the engine's CPU use now. **PEAK**: the worst moment of the last 5 seconds. Under 100% plays without clicks.

**Buses:** each track can go to its own Schwung bus. It then leaves the main mix with its level and pan, and gets that bus's
effects instead of the Machinedrum's master section.

**Saving:** a Schwung set saves the whole kit and the settings. Copy and Delete of a track use the host's hold-and-hit gesture.

## CPU

The emulation is heavy: a busy kit with ROM machines and the full master effects is close to what a Move can do. If it clicks,
watch **PEAK** on the Master page while it plays, and in this order:

1. Lower **VOICS** (8 or 6).
2. Set **FX** to *Rev+Echo*, or *Off*.
3. Raise **LTNCY** to 3.

Measured on a standard Move (CM4): a typical beat uses about 40-60% of one block's time; a busy 16-track sequence with ROM machines
plays clean at the defaults (LTNCY 2, VOICS 10, FX Full).

## Building

```sh
git clone --recursive https://github.com/sd88me/schwung-machinemodule
cd schwung-machinemodule
scripts/build.sh                          # Docker (ubuntu 22.04's aarch64 toolchain): dist/machinemodule-module.tar.gz
scripts/install.sh ableton@move.local     # or Schwung Manager's "Install from File" with the tarball
```

The engine is [sd88me/mpc-vst-machinedrum](https://github.com/sd88me/mpc-vst-machinedrum), a submodule in `libs/machinemodule`.
This repo holds the Move-only parts:

| | |
|---|---|
| `src/plugin/md_plugin.cpp` | `dsp.so`, inside Move: the kit table, pages, MIDI, kits, state and buses. It starts and watches the engine |
| `src/child/md_engine.cpp` | `md-engine`, the engine process (cores 0-2, made real-time by the plugin) |
| `src/common/md_shm.h` | the shared memory between the two |
| `module/` | `module.json`, `help.json` |
| `tests/md_hosttest.cpp` | loads `dsp.so` as Schwung does and plays it in real time on a Move (run as root for Move's priorities) |
| `tests/md_busybench.cpp` | a kit's CPU cost per block on a Move, for each engine setting |
| `tools/md_watch.cpp` | the running engine's late blocks and block times, live (as root on the Move) |

[PROPOSAL.md](PROPOSAL.md) has the design and the measurements behind it.

## Licence

AGPL-3.0, as the engine. `src/host_api/plugin_api_v1.h` is Schwung's (MIT, see `src/host_api/README`).
