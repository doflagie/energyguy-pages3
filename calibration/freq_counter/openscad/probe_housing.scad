/*
 * probe_housing.scad
 * Prescaler Module Housing & Probe Adapters — TM-FC-001 Rev A
 * Material: ASA
 * Print bed: 200 × 200 mm
 *
 * Parts:
 *   1. prescaler_module_box()  — 55 × 35 × 18 mm shielded prescaler enclosure
 *   2. sma_adapter_sleeve()    — SMA male → female right-angle sleeve adapter
 *   3. bnc_sma_adapter_jig()   — alignment jig for BNC-SMA adapter (bench use)
 *   4. tcxo_shield_cap()       — TCXO module shielding/thermal cap
 *   5. gps_antenna_mount()     — NEO-6M patch antenna window/roof mount bracket
 */

$fn = 48;

// ── Utilities ─────────────────────────────────────────────────────────────────
module rounded_box(l, w, h, r=3) {
    hull() {
        for (x = [r, l-r]) for (y = [r, w-r])
            translate([x, y, 0]) cylinder(r=r, h=h);
    }
}

D_SMA   = 10.0;
D_SMA_C = 6.4;    // SMA center conductor hole
WALL    = 1.8;

// ═══════════════════════════════════════════════════════════════════════════════
// 1. PRESCALER MODULE BOX
// ═══════════════════════════════════════════════════════════════════════════════
// Houses ERA-3SM + MB506 prescaler PCB (50 × 30 mm).
// Mounts inside main counter enclosure or as standalone shielded module.
// Two SMA connectors: J_IN (left end) and J_OUT (right end).
// MC pin jumper access window on top.
// Separate lid screws shut (×2 M2 self-tap).

module prescaler_module_box() {
    L = 58;  W = 36;  H = 18;
    PCB_L = 50;  PCB_W = 30;

    difference() {
        rounded_box(L, W, H, 2);
        // Interior
        translate([WALL, WALL, WALL])
            rounded_box(L - 2*WALL, W - 2*WALL, H - WALL + 0.1, 1.5);
        // SMA input hole (left end)
        translate([-0.1, W/2, H/2])
            rotate([0, 90, 0]) cylinder(d=D_SMA, h=WALL + 0.2);
        // SMA output hole (right end)
        translate([L - WALL - 0.1, W/2, H/2])
            rotate([0, 90, 0]) cylinder(d=D_SMA, h=WALL + 0.2);
        // MC jumper access window on top
        translate([L/2 - 8, W/2 - 5, H - WALL - 0.1])
            cube([16, 10, WALL + 0.2]);
        // Lid screw holes (×2, self-tapping M2)
        for (x = [8, L - 8]) {
            translate([x, W/2, H - WALL - 0.1]) cylinder(d=1.8, h=WALL + 0.2);
        }
        // Coax routing notch in bottom (for jumper wire to main PCB)
        translate([L/2 - 3, -0.1, 2]) cube([6, WALL + 0.2, 5]);
    }
    // PCB standoffs
    for (pos = [[4, 3], [4, W-6], [L-7, 3], [L-7, W-6]]) {
        translate([pos[0], pos[1], WALL]) {
            difference() {
                cylinder(d=4, h=3);
                cylinder(d=1.8, h=3);
            }
        }
    }
}

module prescaler_lid() {
    L = 58;  W = 36;
    difference() {
        rounded_box(L, W, WALL + 1, 2);
        // Lip cutout
        translate([WALL + 0.4, WALL + 0.4, -0.1])
            rounded_box(L - 2*WALL - 0.8, W - 2*WALL - 0.8, WALL + 0.2, 1.2);
        // MC window
        translate([L/2 - 8, W/2 - 5, -0.1]) cube([16, 10, WALL + 1.2]);
        // Screw holes
        for (x = [8, L - 8]) translate([x, W/2, -0.1]) cylinder(d=1.8, h=WALL + 1.2);
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// 2. SMA ADAPTER SLEEVE
// ═══════════════════════════════════════════════════════════════════════════════
// Strain relief / alignment sleeve for SMA panel-mount connectors.
// Slips over SMA barrel to reinforce panel mounting for field use.

module sma_adapter_sleeve() {
    OD  = 14.0;
    ID  = 10.2;   // slip fit over SMA barrel
    L   = 18.0;
    HEX = 12.7;   // SMA hex nut across-flats → not modeled, just clearance

    difference() {
        cylinder(d=OD, h=L);
        translate([0, 0, -0.1]) cylinder(d=ID, h=L + 0.2);
        // Flat on two sides (anti-rotation)
        for (a = [0, 180]) rotate([0, 0, a]) translate([4.5, -OD/2, -0.1])
            cube([OD, OD, L + 0.2]);
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// 3. BNC-SMA ADAPTER JIG
// ═══════════════════════════════════════════════════════════════════════════════
// Bench jig to hold BNC-SMA adapter at correct angle during soldering/assembly.

module bnc_sma_adapter_jig() {
    BASE_L = 60;  BASE_W = 40;  BASE_H = 6;
    difference() {
        rounded_box(BASE_L, BASE_W, BASE_H, 3);
        // BNC socket pocket (12.5 mm dia, 15 mm deep)
        translate([15, BASE_W/2, -0.1]) cylinder(d=12.6, h=15);
        // SMA pocket (10 mm dia, 12 mm deep)
        translate([45, BASE_W/2, -0.1]) cylinder(d=10.2, h=12);
        // Center alignment groove
        translate([14, BASE_W/2 - 1, -0.1]) cube([32, 2, BASE_H + 0.2]);
    }
    // Cable strain relief clamp tabs
    for (y = [BASE_W/2 - 10, BASE_W/2 + 6]) {
        translate([BASE_L/2 - 5, y, BASE_H]) cube([10, 4, 4]);
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// 4. TCXO THERMAL/SHIELDING CAP
// ═══════════════════════════════════════════════════════════════════════════════
// Press-fit cap over TCXO module (14-DIP footprint, ~19 × 13 × 9 mm).
// Reduces thermal gradient and provides EMI shielding at PCB level.

module tcxo_shield_cap() {
    TCXO_L = 20;  TCXO_W = 14;  TCXO_H = 10;
    CAP_T  = 1.2;
    difference() {
        // Outer shell
        translate([-CAP_T, -CAP_T, 0])
            rounded_box(TCXO_L + 2*CAP_T, TCXO_W + 2*CAP_T, TCXO_H + CAP_T, 2);
        // Inner cavity (press-fit over TCXO)
        translate([0, 0, -0.1]) rounded_box(TCXO_L, TCXO_W, TCXO_H + 0.2, 1);
        // Pin clearance notches on bottom edge (4 corners)
        for (x = [0, TCXO_L - 3]) for (y = [0, TCXO_W - 3]) {
            translate([x - 0.5, y - 0.5, -CAP_T - 0.1]) cube([4, 4, CAP_T + 0.2]);
        }
        // Vent holes on top (×6)
        for (xi = [4, 10, 16]) for (yi = [3, 8]) {
            translate([xi, yi, TCXO_H]) cylinder(d=1.5, h=CAP_T + 0.2);
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// 5. GPS ANTENNA MOUNT BRACKET
// ═══════════════════════════════════════════════════════════════════════════════
// Mounts NEO-6M patch antenna (25 × 25 mm) on a window sill or equipment top.
// Slots for zip-tie or Velcro strap. SMA pigtail routing channel.

module gps_antenna_mount() {
    ANT_L = 26;  ANT_W = 26;  ANT_H = 4;
    BRKT_L = 60;  BRKT_W = 30;  BRKT_H = 4;

    difference() {
        union() {
            // Base plate
            rounded_box(BRKT_L, BRKT_W, BRKT_H, 3);
            // Antenna pocket raised surround
            translate([(BRKT_L - ANT_L - 2)/2, (BRKT_W - ANT_W - 2)/2, BRKT_H])
                difference() {
                    rounded_box(ANT_L + 2, ANT_W + 2, ANT_H, 2);
                    translate([1, 1, -0.1]) rounded_box(ANT_L, ANT_W, ANT_H + 0.2, 1.5);
                }
        }
        // Zip-tie slots
        for (x = [8, BRKT_L - 12]) {
            translate([x, BRKT_W/2 - 6, -0.1]) cube([4, 12, BRKT_H + 0.2]);
        }
        // Coax routing channel (SMA pigtail, 6 mm dia)
        translate([BRKT_L/2 - 3, -0.1, 1]) cube([6, BRKT_W + 0.2, 3]);
        // Mounting screw holes (×2 M3)
        for (y = [6, BRKT_W - 6]) {
            translate([BRKT_L - 8, y, -0.1]) cylinder(d=3.4, h=BRKT_H + 0.2);
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
// RENDER
// ═══════════════════════════════════════════════════════════════════════════════
// Uncomment one part at a time to export STL.

prescaler_module_box();

// translate([65, 0, 0]) prescaler_lid();
// translate([0, 45, 0]) sma_adapter_sleeve();
// translate([65, 45, 0]) bnc_sma_adapter_jig();
// translate([0, 90, 0]) tcxo_shield_cap();
// translate([65, 90, 0]) gps_antenna_mount();
