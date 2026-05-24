// ============================================================
//  coil_housing.scad — Pool Noodle Loading Coil Housing
//  TM-PNL2-ENC-001 Rev A
//  Houses pool noodle loading coil + wire-pass terminals
//  Print bed: 200×200×200mm  Material: PETG/ASA
// ============================================================

// ---- Parameters ----
NOODLE_OD     = 47.0;   // pool noodle outer diameter (mm)
NOODLE_CLR    = 0.8;    // socket clearance (mm)
INSERT_DEPTH  = 45.0;   // noodle insertion depth each end (mm)
WALL          = 3.0;    // wall thickness (mm)
WIRE_CH_W     = 5.5;    // wire channel width (mm)
WIRE_CH_H     = 4.5;    // wire channel height (mm)
TERM_OD       = 8.0;    // terminal post outer dia (mm)
TERM_BORE     = 3.5;    // M3 thread tap bore (mm)
LID_CLIP_H    = 6.0;    // snap-fit clip height (mm)
LID_CLIP_W    = 2.0;    // snap-fit clip width (mm)
LABEL_DEPTH   = 0.4;    // embossed text depth (mm)

$fn = 64;

// ---- Derived ----
SOCK_ID  = NOODLE_OD + NOODLE_CLR * 2;   // socket inner diameter
BODY_OD  = SOCK_ID + WALL * 2;            // housing outer diameter
BODY_LEN = INSERT_DEPTH * 2 + 20;         // total housing length (mm)
// 20mm center section provides space for coil access window

module coil_housing_body() {
    difference() {
        // Outer cylinder
        cylinder(d = BODY_OD, h = BODY_LEN, center = true);

        // Noodle socket bores
        translate([0, 0,  BODY_LEN/2 - INSERT_DEPTH/2 + 0.5])
            cylinder(d = SOCK_ID, h = INSERT_DEPTH + 1, center = true);
        translate([0, 0, -BODY_LEN/2 + INSERT_DEPTH/2 - 0.5])
            cylinder(d = SOCK_ID, h = INSERT_DEPTH + 1, center = true);

        // Center inspection window (access to coil taps)
        cube([BODY_OD + 2, WIRE_CH_W * 3, 20], center = true);

        // Wire exit channels — top and bottom faces
        translate([0, 0,  BODY_LEN/2 - 1])
            cube([WIRE_CH_W, WIRE_CH_H, 4], center = true);
        translate([0, 0, -BODY_LEN/2 + 1])
            cube([WIRE_CH_W, WIRE_CH_H, 4], center = true);

        // Terminal holes (2× M3 screw terminals, tap and arm connection)
        for (y = [-8, 8]) {
            translate([0, y, 0])
                cylinder(d = TERM_BORE, h = BODY_OD + 2, center = true);
        }

        // Label recess
        translate([BODY_OD/2 - LABEL_DEPTH/2, 0, 15])
            rotate([0, 90, 0])
                linear_extrude(LABEL_DEPTH + 0.1)
                    text("PNILD COIL", size = 3.5, halign = "center", font = "Liberation Sans");
        translate([BODY_OD/2 - LABEL_DEPTH/2, 0, 8])
            rotate([0, 90, 0])
                linear_extrude(LABEL_DEPTH + 0.1)
                    text("TM-PNL2-ENC-001", size = 2.5, halign = "center", font = "Liberation Sans");
    }
}

module terminal_post(y_offset) {
    translate([0, y_offset, 0]) {
        difference() {
            cylinder(d = TERM_OD, h = WALL + 4, center = false);
            cylinder(d = TERM_BORE, h = WALL + 5, center = false);
        }
    }
}

module snap_lid() {
    // Snap-fit cover for inspection window
    difference() {
        union() {
            cube([BODY_OD - 0.4, WIRE_CH_W * 3 + WALL * 2, 4], center = true);
            // Snap tabs
            for (y = [-(WIRE_CH_W*1.5 + WALL), (WIRE_CH_W*1.5 + WALL)]) {
                translate([0, y, -LID_CLIP_H/2])
                    cube([BODY_OD - 4, LID_CLIP_W, LID_CLIP_H], center = true);
            }
        }
        // Wire clearance slots
        for (x = [-10, 10]) {
            translate([x, 0, 0])
                cylinder(d = WIRE_CH_W + 1, h = 6, center = true);
        }
    }
}

// ---- Assembly ----
translate([0, 0, BODY_LEN/2])
    coil_housing_body();

// Terminal posts
translate([0, 0, BODY_LEN/2])
    for (y = [-8, 8])
        terminal_post(y);

// Lid (translate aside for printing)
translate([BODY_OD + 10, 0, 2])
    snap_lid();

// ---- Print notes ----
// Print orientation: cylinder axis vertical (upright)
// No supports needed for BODY if printed upright
// Lid: flat face down, no supports
// Layer height: 0.2mm recommended
// Infill: 30% gyroid
// Material: PETG preferred (UV and heat resistant)
// Both parts print within 200×200×200mm bed
