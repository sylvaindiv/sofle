# Sofle RGB — Vial firmware & editor

Vial configuration for a **Sofle Choc** split keyboard with per-key RGB, ready to fork.

This repo is public and free to reuse to build your own Sofle with per-key RGB.

## Contents

- `*.uf2` — precompiled Vial firmware to flash onto the keyboard
- `index.html`, `vial-rgb-editor_1.html` — per-key RGB editor (standalone, opens in a browser)
- `sofle-layout-map.json` — keyboard layout mapping
- `netlify-deploy/` — version deployed on Netlify
- `firmware-source/` — full firmware source code (see its [README](firmware-source/keebart-sofle-choc-pro-vial/README.md))

## Flashing the firmware

**Flash both halves — this is a split keyboard, each half runs its own copy of the firmware.**

1. **Disconnect the two halves from each other** (unplug the cable between them) before flashing either one — flashing while they're connected can interfere with the bootloader/USB enumeration on the half you're not flashing.
2. Put the half you're flashing into bootloader mode (reset button / double-tap, depending on the PCB).
3. Copy the `.uf2` file onto the drive that appears (`RPI-RP2` or similar).
4. The half reboots automatically with the new firmware.
5. Repeat steps 2–4 for the **other half**.
6. Reconnect the two halves once both are flashed.

## Configuring RGB

The firmware supports [Vial](https://get.vial.today/) for keymaps. For per-key RGB, open `index.html` (or `vial-rgb-editor_1.html`) in a browser — no install required.

## Forking

Fork this repo, replace the `.uf2` with your own build if needed, and adapt `sofle-layout-map.json` to your layout.

## Rebuilding the firmware / adapting it to a different split keyboard

The full source code (not just the binary) is in `firmware-source/`. If you want to rebuild it or adapt it to a different VialRGB-compatible split keyboard, start with [`firmware-source/keebart-sofle-choc-pro-vial/TROUBLESHOOTING.md`](firmware-source/keebart-sofle-choc-pro-vial/TROUBLESHOOTING.md) — it documents every bug encountered (source loss, Direct-mode RGB not showing on a split board, layer changes not synced to the slave half) and a porting checklist.
