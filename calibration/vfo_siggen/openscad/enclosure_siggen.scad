/*
 * enclosure_siggen.scad
 * Portable VFO / Signal Generator Enclosure
 * TM-SG-001 Rev A
 * Material: ASA (UV-stable, dimensional stability) or PETG
 * Print bed: 200 × 200 mm; two-part (base + lid); fits 200×200 area each
 *
 * Internal layout (landscape orientation):
 *   Left section (60 mm):   CYD display (75×50 mm window), encoder knob
 *   Center section (80 mm): DDS/Si5351 PCB + ADF4351 PCB stacked
 *   Right section (50 mm):  SMA connectors, controls, binding posts
 *
 * Dimensions:
 *   Outer: 190 × 130 × 52 mm (fits 200×200 print bed)
 *   Wall:  2.5 mm
 *   Lid:   10 mm deep
 *
 * Connectors (front panel):
 *   SMA ×3: HF OUT, VHF OUT, SIG OUT
 *   3.5mm ×2: AF IN, KEY IN
 *   USB-C: power input
 *   Power switch: SPST toggle (6.4 mm)
 *   Encoder: rotary + pushbutton (7 mm shaft, 16 mm bezel)
 *
 * Rear panel:
 *   SMA ×2: EXT REF IN, AUX OUT (Si5351 CLK1)
 *   GPIO header: 2×5 pin 2.54 mm (debug/expansion)
 */

$fn = 48;

BOX_L = 190;
BOX_W = 130;
BOX_H = 52;
WALL  = 2.5;
CORN  = 5.0;
LID_H = 10;

D_SMA   = 6.5;   // SMA panel hole (inner barrel)
D_SMA_M = 8.5;   // SMA mounting hole pattern span
D_35MM  = 6.5;   // 3.5mm jack
D_USBC  = 9.5;   // USB-C hole (width)
H_USBC  = 3.5;   // USB-C hole (height)
D_TOGGLE = 6.4;  // SPST toggle switch
D_ENC   = 7.2;   // Encoder shaft (7mm + clearance)
D_M3    = 3.4;
D_M3_HS = 5.5;   // M3 heat-set insert bore

// ── Utility ───────────────────────────────────────────────────────────────────
module rounded_box(l, w, h, r) {
    hull() for (x=[r, l-r]) for (y=[r, w-r])
        translate([x, y, 0]) cylinder(r=r, h=h);
}

module sma_hole() {
    cylinder(d=D_SMA, h=WALL+1, center=true);
}

// ── Box body ──────────────────────────────────────────────────────────────────
module box_body() {
    difference() {
        rounded_box(BOX_L, BOX_W, BOX_H - LID_H, CORN);

        // Interior pocket
        translate([WALL, WALL, WALL])
            rounded_box(BOX_L - 2*WALL, BOX_W - 2*WALL,
                        BOX_H - LID_H - WALL + 0.1, CORN - WALL);

        // ── FRONT PANEL (y = 0 face) cutouts ─────────────────────────────────
        // SMA row: HF OUT, VHF OUT, SIG OUT at y=0, spaced evenly on right half
        for (x=[BOX_L-145, BOX_L-110, BOX_L-75])
            translate([x, -0.1, BOX_H/2 - LID_H/2])
                rotate([-90, 0, 0]) sma_hole();

        // 3.5mm AF IN and KEY IN
        for (x=[BOX_L-40, BOX_L-20])
            translate([x, -0.1, BOX_H/2 - LID_H/2])
                rotate([-90, 0, 0]) cylinder(d=D_35MM, h=WALL+1, center=true);

        // USB-C slot (rectangular)
        translate([25, -0.1, BOX_H/2 - LID_H/2 - H_USBC/2])
            cube([D_USBC, WALL+1, H_USBC]);

        // Power toggle switch
        translate([42, -0.1, BOX_H/2 - LID_H/2])
            rotate([-90, 0, 0]) cylinder(d=D_TOGGLE, h=WALL+1, center=true);

        // ── REAR PANEL (y = BOX_W face) cutouts ──────────────────────────────
        // EXT REF IN, AUX OUT
        for (x=[30, 55])
            translate([x, BOX_W-WALL-0.1, BOX_H/2 - LID_H/2])
                rotate([90, 0, 0]) sma_hole();

        // GPIO 2×5 header slot (10 pins, 2.54mm pitch)
        translate([BOX_L-60, BOX_W-WALL-0.1, BOX_H/2 - LID_H/2 - 3])
            cube([25.4, WALL+1, 6]);

        // ── TOP face cutouts (for lid area) ──────────────────────────────────
        // Encoder knob hole (rotates through lid)
        translate([30, BOX_W/2, BOX_H - LID_H - 0.1])
            cylinder(d=D_ENC, h=LID_H+0.2);

        // Lid engagement recess
        translate([WALL+0.3, WALL+0.3, BOX_H - LID_H - 0.5])
            rounded_box(BOX_L - 2*WALL - 0.6, BOX_W - 2*WALL - 0.6,
                        LID_H + 0.6, CORN - WALL);
    }

    // PCB standoffs — main board (4× corners, M3)
    for (pos=[[10, 10], [10, BOX_W-15], [BOX_L-15, 10], [BOX_L-15, BOX_W-15]])
        translate([pos[0], pos[1], WALL])
            difference() {
                cylinder(d=6, h=6);
                cylinder(d=D_M3_HS, h=6);
            }

    // PCB standoffs — ADF4351 module (stacked; 12 mm height)
    for (pos=[[90, 15], [90, BOX_W-20], [140, 15], [140, BOX_W-20]])
        translate([pos[0], pos[1], WALL])
            difference() {
                cylinder(d=6, h=12);
                cylinder(d=D_M3_HS, h=12);
            }

    // Lid screw bosses M3 (×4 corners)
    for (pos=[[6, 6], [6, BOX_W-6], [BOX_L-6, 6], [BOX_L-6, BOX_W-6]])
        translate([pos[0], pos[1], BOX_H - LID_H - 1])
            difference() {
                cylinder(d=8, h=LID_H+1);
                translate([0,0,-0.1]) cylinder(d=D_M3_HS, h=5);
            }
}

// ── Lid ───────────────────────────────────────────────────────────────────────
module lid() {
    difference() {
        union() {
            rounded_box(BOX_L, BOX_W, LID_H, CORN);
            // Engagement lip
            translate([WALL+0.5, WALL+0.5, -(LID_H-2)])
                rounded_box(BOX_L - 2*WALL - 1.0, BOX_W - 2*WALL - 1.0,
                            LID_H - 1.5, CORN - WALL);
        }

        // CYD display window (75 × 50 mm; centred left-of-center)
        translate([55, BOX_W/2, -0.1])
            hull() for (x=[-30, 30]) for (y=[-22, 22])
                translate([x, y, 0]) cylinder(r=2, h=LID_H+0.2);

        // Display bezel recess (1 mm deep, 2 mm wider than window)
        translate([55, BOX_W/2, LID_H-1.2])
            hull() for (x=[-32, 32]) for (y=[-24, 24])
                translate([x, y, 0]) cylinder(r=2.5, h=1.3);

        // Encoder shaft clearance hole (top of lid)
        translate([30, BOX_W/2, -0.1]) cylinder(d=D_ENC, h=LID_H+0.2);

        // Encoder bezel recess
        translate([30, BOX_W/2, LID_H-2])
            cylinder(d=18, h=2.1);

        // LED indicator hole (status LED, 3mm)
        translate([12, BOX_W/2+20, -0.1]) cylinder(d=3.2, h=LID_H+0.2);

        // M3 lid screw holes (×4)
        for (pos=[[6, 6], [6, BOX_W-6], [BOX_L-6, 6], [BOX_L-6, BOX_W-6]])
            translate([pos[0], pos[1], -0.1]) cylinder(d=D_M3, h=LID_H+0.2);

        // Label recess for silk-screen or dymo label
        translate([140, BOX_W/2, LID_H-0.8])
            hull() for (x=[-22, 22]) for (y=[-15, 15])
                translate([x, y, 0]) cylinder(r=2, h=0.9);
    }

    // CYD retention clips (×2, press-fit from inside)
    for (side=[-1, 1])
        translate([55+side*33, BOX_W/2, -(LID_H-3)])
            difference() {
                cube([2, 5, 4], center=true);
                // No through-hole; solid clip
            }
}

// ── Front panel label frame ───────────────────────────────────────────────────
// Thin overlay that covers front face; engraved connector labels.
module front_label_frame() {
    difference() {
        translate([0, 0, 0])
            cube([BOX_L, WALL, BOX_H - LID_H]);

        // Mirror all front panel holes (slightly oversize for clearance)
        for (x=[BOX_L-145, BOX_L-110, BOX_L-75])
            translate([x, -0.1, BOX_H/2 - LID_H/2])
                rotate([-90, 0, 0]) cylinder(d=D_SMA+1, h=WALL+0.3, center=true);

        for (x=[BOX_L-40, BOX_L-20])
            translate([x, -0.1, BOX_H/2 - LID_H/2])
                rotate([-90, 0, 0]) cylinder(d=D_35MM+1, h=WALL+0.3, center=true);

        translate([25-0.5, -0.1, BOX_H/2 - LID_H/2 - H_USBC/2 - 0.5])
            cube([D_USBC+1, WALL+0.3, H_USBC+1]);

        translate([42, -0.1, BOX_H/2 - LID_H/2])
            rotate([-90, 0, 0]) cylinder(d=D_TOGGLE+1, h=WALL+0.3, center=true);

        // Engraved text positions (use slicer text tool or Inkscape + SVG import)
        // Manually add connector labels at:
        //   x=BOX_L-145, y=top: "HF"
        //   x=BOX_L-110, y=top: "VHF"
        //   x=BOX_L-75,  y=top: "SIG"
        //   x=BOX_L-40,  y=top: "AF"
        //   x=BOX_L-20,  y=top: "KEY"
        //   x=25,        y=top: "USB"
        //   x=42,        y=top: "PWR"
    }
}

// ── Battery tray (removable; slides in from rear) ─────────────────────────────
module battery_tray() {
    TRAY_L = 50; TRAY_W = 35; TRAY_H = 12;
    difference() {
        cube([TRAY_L, TRAY_W, TRAY_H]);
        // LiPo cell pocket
        translate([2, 2, 2]) cube([TRAY_L-4, TRAY_W-4, TRAY_H]);
        // JST connector slot
        translate([0, TRAY_W/2-3, TRAY_H-3]) cube([TRAY_L+0.2, 6, 3.5]);
    }
    // Rail guides (slides in box bottom)
    for (y=[2, TRAY_W-4])
        translate([0, y, -1.5]) cube([TRAY_L, 2, 1.5]);
}

// ── Render ────────────────────────────────────────────────────────────────────
box_body();
// translate([0, BOX_W+15, LID_H]) rotate([180,0,0]) lid();
// translate([BOX_L+15, 0, 0]) battery_tray();
// translate([BOX_L+15, BOX_W/2, 0]) front_label_frame();
