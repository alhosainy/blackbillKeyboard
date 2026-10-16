# CONTEXT — blackbillKeyboard (custom QMK keyboard)

> This file mirrors the project's canonical `CONTEXT.md` (kept in the T3 project
> directory). Keep both in sync when anything changes.

## What this project is

`blackbillKeyboard` (keyboard ID / folder `blackbillkeyboard`) is a **custom, hand-wired full-size ANSI mechanical keyboard** (104 ANSI keys)
built as a keyboard definition for
[QMK Firmware](https://github.com/qmk/qmk_firmware). It has dual-strip RGB underglow.

- **Manufacturer / maintainer:** `alhosainy`
- **MCU / dev board:** Black Pill (STM32F401, `blackpill_f401`)
- **Diode direction:** `COL2ROW`
- **Keys:** 104 ANSI (all keys are in the diode matrix).
- **Matrix:** 6 rows × 21 cols
- **Extras:** dual WS2812B underglow (`B0`, 16 LEDs total)
- **FN key:** momentary layer 1 on the Fn key (matrix `[5,10]`, `MO(1)`)

## Where the code lives

The working copy of this keyboard is inside a QMK firmware checkout:

```
F:\keyboard\keyboards\blackbillkeyboard\        <- the keyboard definition (QMK repo working tree)
├── keyboard.json                   <- QMK info.json replacement (matrix, pins, layouts)
├── keymaps\default\keymap.c        <- default keymap (LAYOUT_fullsize_ansi)
├── keymaps\default\rules.mk        <- keymap options (empty)
├── keymaps\diag\keymap.c           <- wiring diagnostic keymap (types each pressed key's RxCy)
├── readme.md                       <- QMK keyboard readme
├── matrix_wiring_guide.html        <- matrix + pin wiring guide (generated)
├── keyboard_guide.html             <- features + layer guide (generated)
└── full_size_keyboard_hand_wiring_guide(1).html  <- standalone build/wiring guide
```

`F:\keyboard` is a clone of `qmk/qmk_firmware` (`origin` and `upstream` both point at
`https://github.com/qmk/qmk_firmware`). `keyboards/blackbillkeyboard/` is currently **untracked**
in that repo (`git status` shows `?? keyboards/blackbillkeyboard/`).

This T3 project directory is a separate scaffold (README + `assets/icon.svg` only).

## Build & flash

The QMK toolchain lives under `D:\kayboard\QMK_MSYS` (qmk 1.2.0, arm-none-eabi-gcc).
It is not on PATH by default; invoke it from `F:\keyboard` with:

```powershell
$env:MSYSTEM='MINGW64'; $env:QMK_HOME='F:\keyboard'
$env:SHELL='D:\kayboard\QMK_MSYS\usr\bin\bash.exe'; $env:PYTHONUTF8='1'
$env:Path = 'D:\kayboard\QMK_MSYS\mingw64\bin;D:\kayboard\QMK_MSYS\usr\bin;D:\kayboard\QMK_MSYS\opt\qmk\bin;' + $env:Path
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' compile -kb blackbillkeyboard -km default
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' lint -kb blackbillkeyboard -km default
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' flash -kb blackbillkeyboard -km default
```

(`make blackbillkeyboard:default` / `make blackbillkeyboard:default:flash` also work inside a QMK MSYS shell.)
`PYTHONUTF8=1` is required or lint crashes on the box-drawing characters in `keymap.c`.
Build artifacts land in `.build/blackbillkeyboard_default.{bin,hex,elf}` (and copied to `qmk_firmware/`).

Bootloader entry: Bootmagic reset (hold the (0,0) key while plugging in), the physical
reset button, or the `QK_BOOT` keycode (FN + Pause, or FN + Esc).

## Hardware plan

| Item | Spec |
|------|------|
| Matrix | 6 rows × 21 cols (keys per physical row: 16/21/21/16/17/13) |
| Row pins | `A0 A1 A2 A3 A4 A5` |
| Col pins | `A6 A7 A8 A9 B4 A15 B1 C15 B5 B6 B7 B8 B9 B10 B3 B12 B13 B14 B15 C14 C13` |
| RGB underglow | `ws2812.pin = B0`, `rgblight.led_count = 16` (2 × 8-LED boards chained) |
| Protection | 330Ω resistor on RGB data line, 1000µF cap across 5V/GND |
| Pull-ups | ~5.1k (5.6k fine) from `A9` (col 3) to 3V3 so the VBUS-sense pin works as a column. `A10` is now unused, so it needs no pull-up |
| Left free | `B2` (BOOT1 — avoid), `A10` (avoided). `A11`/`A12` are USB; `A13`/`A14` (SWD) are not broken out on this board |

## Layout notes / known gaps

- `keyboard.json` defines exactly **104 keys** in `LAYOUT_fullsize_ansi` (standard 104-key
  ANSI fullsize on KLE spacing). There is no encoder entry in the layout.
- The layout coordinates come from the user's pasted `LAYOUT_fullsize_ansi`: Esc restored
  at `[0,0]` (the paste had put a placeholder key there), F-row/PrtSc/Pause on
  KLE-standard spacing, numpad `Enter` at `[5,20]` (h2) and `+` at `[3,20]` (h2).
- The four extra keys above the numpad (`F13`–`F16`) the paste carried were **removed**
  at the user's request (`F13 F14 F15 F16 does nothing`) — the layout is now a plain
  full-size ANSI 104. Matrix positions `[0,17]`–`[0,20]` are unwired.
- The **rotary encoder was dropped** because its `B3` pin was needed as the replacement for
  the nonexistent `B11`. `B3` is now matrix **column 14**.
- `keymaps/default/keymap.c` has two layers:
  - `_BASE`: ANSI keys; bottom-right cluster left→right is `KC_RALT (Alt), MO(1) (Fn), KC_APP (Menu), KC_RCTL`
    (matrix `[5,9]`, `[5,10]`, `[5,12]`, `[5,13]`), so Alt=`[5,9]`, Fn=`[5,10]`, Menu=`[5,12]`, Ctrl=`[5,13]`.
  - `_FN` (hold the Fn key `[5,10]`): `KC_PAUS` and `KC_ESC`→`QK_BOOT`; `KC_F12`→`KC_KLOCK`
    (custom keycode that toggles a keyboard lock — all keys are ignored until Fn+F12 is pressed
    again); numpad keys drive RGB underglow
    (`UG_TOGG`/`UG_NEXT`/`UG_PREV`/hue/sat/val/speed); arrows drive media
    (`KC_MSTP`/`KC_MPRV`/`KC_MPLY`/`KC_MNXT`); Space = `KC_MUTE`.
- **Pin plan & measured wiring:** the row/column pin assignments were chosen to satisfy
  QMK's unique-pin rule (the original file had duplicate placeholder pins);
  `matrix_wiring_guide.html` is the authoritative reference and must stay in sync with
  `keyboard.json`. As of 2026-10 the board is hand-wired and scans, and the physical wiring
  differed from the original KLE/matrix plan, so `keyboard.json` was re-mapped to match it
  (verified with the `keymaps/diag` diagnostic keymap that types each key's `RxCy` coordinate):
  - Row 4 (`ZXCV` row) letters sit one column left of the original plan: `Shift`=`[4,0]`,
    `Z`=`[4,1]` … `/`=`[4,10]`, `RShift`=`[4,13]`, `Up`=`[4,15]`.
  - Bottom-row right cluster: `RAlt`=`[5,9]`, `Fn`=`[5,10]`, `Menu`=`[5,12]`, `RCtrl`=`[5,13]`
    (column `[5,11]` is unwired).
  - Numpad `+`=`[3,20]`, `Enter`=`[5,20]`, `0`=`[5,17]` (moved off `[2,20]`/`[4,20]`/`[5,18]`,
    which did not scan).
  - `keymap.c` layout-arg order is unchanged — only matrix coordinates were remapped.
- **48-pin WeAct Black Pill pin caveats (per https://docs.qmk.fm/platformdev_blackpill_f4x1):**
  `B11` is not bonded (hence col 14 = `B3`); `A11`/`A12` are USB (unusable); `B2` is BOOT1 —
  the doc says it should not be used, so it was moved off col 7 and is now left unused (col 7 =
  `C15`); `A9` (col 3) is VBUS-sense with an internal pull-down — a ~5.1k
  pull-up to 3V3 makes it work (5.6k confirmed working on hardware); **`A10` was tried as col 4
  but did not scan reliably** (the vendor doc says any connection on it can block DFU entry),
  so col 4 was moved to `B4` and `A10` is now left unused; `A4` is
  the SPI-flash CS (fine while the SOI8 footprint is unused); `C13`–`C15` are input-only
  (columns only: `C13` = col 20, `C14` = col 19, `C15` = col 7; `C13` also drives the onboard
  LED). `A13`/`A14`
  (SWD) are not broken out on this board, so recovery/flashing is via BOOT0 + DFU.
- `keyboard.json` `features`: bootmagic, extrakey, mousekey, nkro, rgblight.
- `url` is set to `https://github.com/alhosainy/blackbillKeyboard` (schema requires a non-empty URL).

## Conventions

- QMK keyboard definition: `keyboard.json` + `keymaps/<name>/keymap.c`.
- Keep the layout-macro key order in `keymap.c` in sync with the `layouts` array order
  in `keyboard.json`.
- No comments unless asked; match existing QMK file style.

## Roadmap / TODO

- **v1 (current, shipped):** upstream QMK (`qmk/qmk_firmware`) `master` branch.
  Build with `qmk compile -kb blackbillkeyboard -km default` (and `qmk lint -kb blackbillkeyboard -km default`).
  Static keymap: no Vial/VIA live remapping.
- **v2 — Vial (web remapping) — TODO, not started:** add [Vial](https://vial.rocks)
  support so the keymap and RGB can be remapped live from the browser (vial.rocks, WebHID,
  no install needed), instead of recompiling firmware.
  - Vial is a **QMK fork**, not an upstream feature: this checkout has no `quantum/vial`.
    The fork is `https://github.com/vial-kb/vial-qmk`.
  - Already prepared during exploration: git remote `vial` added and the `vial` branch
    fetched (`vial/vial` tip `dd43959`, 2026-07-26; QMK base 2025-06-21); a local `vial`
    branch tracks it. v1 stays on `master` (`git checkout master` to return).
  - Remaining work:
    1. Add `keyboards/blackbillkeyboard/vial.json` (matrix / layouts / keymap metadata for the web
       app) mirroring `keyboard.json` order.
    2. In `keyboards/blackbillkeyboard/rules.mk` add `VIAL_ENABLE = yes` and (for RGB) `VIALRGB_ENABLE = yes`.
    3. Build on the `vial` branch and flash, then open vial.rocks.
  - Gotcha: switching QMK branches requires submodule updates, and `core.autocrlf=true`
    makes submodules look dirty so a plain `git submodule update` aborts ("Aborting") —
    force with `git -C lib/<sub> checkout -f <sha>` or `git submodule update --force`.
  - Trade-off: the `vial` branch is based on older QMK (2025-06) than v1 `master`
    (2026-10), so v2 gives up newer upstream changes.
