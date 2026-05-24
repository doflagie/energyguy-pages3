// TM-VCAP-ENC-007 Rev A — Stack-On Modular Flat-Plate Capacitor Unit Tray
// Carrier tray for CSM copper-foil/polymer capacitor units (TM-VCAP-CAL-006,
// TM-VCAP-SCH-005). Provides velcro slots, rubber band grooves, terminal tab
// recess, compression ridge, and label area. Array layout prints 6 units/bed.
//
// USAGE: Set UNIT_ID to desired CSM unit (see table below), generate STL.
// Units are carried in a 4×2 grid in a pill organiser or small Pelican case.

// ─── CSM Unit Dimensions (plate W × L in mm) ─────────────────────────────────
//  Unit   W      L     C(pF)  Layers  Band
//  A2     50     35    248    2       160m
//  B2     40     28    159    2        80m
//  C2     32     22    100    2        40m
//  D      22     16    100    1        30m
//  E      16     12     54    1        20m
//  F      12      9     31    1        15m
//  G       9      7     18    1        10m
//  VHF     7      5     10    1       VHF

// Select unit here ↓
UNIT_ID = "E";   // "A2","B2","C2","D","E","F","G","VHF"

// Look-up table: [plate_W, plate_L, n_layers, C_pF]
function unit_specs(id) =
    id == "A2"  ? [50, 35, 2, 248] :
    id == "B2"  ? [40, 28, 2, 159] :
    id == "C2"  ? [32, 22, 2, 100] :
    id == "D"   ? [22, 16, 1, 100] :
    id == "E"   ? [16, 12, 1,  54] :
    id == "F"   ? [12,  9, 1,  31] :
    id == "G"   ? [ 9,  7, 1,  18] :
    id == "VHF" ? [ 7,  5, 1,  10] :
                  [16, 12, 1,  54];  // default to E

SPECS = unit_specs(UNIT_ID);
PLATE_W    = SPECS[0];
PLATE_L    = SPECS[1];
N_LAYERS   = SPECS[2];
C_VALUE    = SPECS[3];

// ─── Tray Parameters ──────────────────────────────────────────────────────────
TRAY_WALL       = 1.2;     // mm — wall thickness
TRAY_FLOOR      = 0.8;     // mm — floor thickness
TAB_EXTEND      = 6;       // mm — wire tab area at front edge
VELCRO_W        = 6;       // mm — velcro slot width (for 6mm or 12mm velcro strip)
VELCRO_D        = 1.5;     // mm — velcro slot depth (hook side compresses to this)
RUBBER_BAND_W   = 2.5;     // mm — rubber band groove width
RUBBER_BAND_D   = 1.5;     // mm — rubber band groove depth
RIDGE_H         = 0.8;     // mm — compression ridge height (ensures foil contact)
LABEL_H         = 4;       // mm — label emboss area height on end wall
CORNER_R        = 1.0;     // mm — corner fillet radius

// Computed tray interior = plate footprint + tab area
TRAY_INT_W = PLATE_W + 2*TRAY_WALL;
TRAY_INT_L = PLATE_L + TAB_EXTEND + 2*TRAY_WALL;

// Total tray outer dimensions
TRAY_W = TRAY_INT_W;
TRAY_L = TRAY_INT_L;
// Height: floor + plate assembly + lid compression (1 or 2 layers of 0.3mm foil + 0.1mm polymer each)
ASSEMBLY_H = N_LAYERS * (0.1 + 0.1) + 1.0 + 0.5;  // dielectric + foil + backing + margin
TRAY_H = TRAY_FLOOR + ASSEMBLY_H + RIDGE_H + 0.5;

$fn = 36;

// ─── Modules ──────────────────────────────────────────────────────────────────

module tray_body() {
    // Open-top tray that holds the CSM foil+polymer+PVC sandwich.
    difference() {
        // Outer shell
        cube([TRAY_W, TRAY_L, TRAY_H]);

        // Interior pocket — plate area (back portion, not tab area)
        translate([TRAY_WALL, TAB_EXTEND + TRAY_WALL, TRAY_FLOOR])
            cube([PLATE_W, PLATE_L, TRAY_H]);

        // Tab pocket at front: shallower floor so tabs splay out horizontally
        translate([TRAY_WALL, TRAY_WALL, TRAY_FLOOR])
            cube([PLATE_W, TAB_EXTEND, TRAY_H]);

        // Velcro slot on BACK wall (long face) — centred, full width
        translate([TRAY_W/2 - VELCRO_W/2, TRAY_L - TRAY_WALL - 0.1, TRAY_H/2])
            cube([VELCRO_W, TRAY_WALL + 0.2, VELCRO_D]);

        // Rubber band grooves: two grooves on each SIDE wall, 20% and 80% along length
        for (y = [TRAY_L*0.25, TRAY_L*0.75]) {
            // Left side wall
            translate([-0.1, y - RUBBER_BAND_W/2, TRAY_H/2 - RUBBER_BAND_D/2])
                cube([TRAY_WALL + 0.2, RUBBER_BAND_W, RUBBER_BAND_D]);
            // Right side wall
            translate([TRAY_W - TRAY_WALL - 0.1, y - RUBBER_BAND_W/2, TRAY_H/2 - RUBBER_BAND_D/2])
                cube([TRAY_WALL + 0.2, RUBBER_BAND_W, RUBBER_BAND_D]);
        }
    }

    // Compression ridge: thin rail running along the inside top rim of plate pocket
    // Presses foil layers together for solid electrical contact when stacked.
    translate([TRAY_WALL + 1, TAB_EXTEND + TRAY_WALL, TRAY_FLOOR + ASSEMBLY_H])
        cube([PLATE_W - 2, PLATE_L, RIDGE_H]);
}

module tab_terminal_post(side) {
    // Small round boss inside the tab pocket; wire solders to foil tab, then
    // wraps around this post. Provides strain relief and alignment.
    // side: 0 = top foil, 1 = bottom foil (offset 3mm from edge per TM-VCAP-SCH-005)
    Y_OFFSET = (side == 0) ? 1.5 : 4.0;
    translate([TRAY_W/2, Y_OFFSET, TRAY_FLOOR])
        cylinder(h=ASSEMBLY_H + 0.5, d=1.8);
}

module label_emboss() {
    // Recessed label area on the back face of the tray (visible when stacked).
    // Dimensions: TRAY_W × LABEL_H rectangle, 0.6mm deep.
    translate([2, TRAY_L - TRAY_WALL + 0.1, TRAY_H - LABEL_H - 1])
        cube([TRAY_W - 4, TRAY_WALL - 0.3, LABEL_H]);
}

module unit_tray() {
    difference() {
        union() {
            tray_body();
            tab_terminal_post(0);  // top foil post
            tab_terminal_post(1);  // bottom foil post
        }
        label_emboss();
    }
}

// ─── Print Array Layout ────────────────────────────────────────────────────────
// 6 trays per print in 2-row × 3-column grid (fits 220×220mm bed for small units).
// For A2 (50×41mm tray): fits 3×2 on 200mm bed.
// For VHF (7×11mm tray): fits 12×8 on 200mm bed — just print one column of 6.

ARRAY_COLS = 3;
ARRAY_ROWS = 2;
GAP        = 3;  // mm gap between trays in array

for (row = [0 : ARRAY_ROWS - 1])
    for (col = [0 : ARRAY_COLS - 1])
        translate([col * (TRAY_W + GAP), row * (TRAY_L + GAP), 0])
            unit_tray();

// ─── Notes ────────────────────────────────────────────────────────────────────
// ASSEMBLY GUIDE (referenced in TM-VCAP-SCH-005):
//
// 1. Print tray in PLA or PETG. Tray width/length = plate + wall clearance.
//
// 2. Build CSM sandwich (outside the tray):
//    a. Cut PVC backing to PLATE_W × PLATE_L mm.
//    b. Apply copper foil tape to BOTTOM face — 5mm tab extends beyond front edge.
//    c. Place polymer sheet (OHP transparency) on top of bottom foil.
//       Sheet is PLATE_W × PLATE_L — does NOT extend to front tab area.
//    d. Apply second copper foil to TOP face of polymer sheet — 5mm tab extends
//       front edge but OFFSET 3mm down from plate edge (can't short bottom tab).
//    e. Solder 20AWG wire lead to each tab.
//
// 3. Drop sandwich into tray. The compression ridge presses foil layers together.
//
// 4. Apply self-adhesive velcro (hook side) to tray back wall slot.
//    Matching velcro (loop side) on adjacent units or on tuner chassis panel.
//
// 5. For small units (F, G, VHF): wrap rubber band into side grooves — no velcro needed.
//
// 6. Dymo-label each tray end: "CSM-E / 54 pF / 20m"
//
// STACKING:
//   All top-foil leads clip/tie to a common T₁ bus (one terminal).
//   All bottom-foil leads clip/tie to a common T₂ bus (other terminal).
//   Add units to increase C; remove units to decrease C.
//   Spring contact strip (0.15mm phosphor-bronze) optional at tab edge.
