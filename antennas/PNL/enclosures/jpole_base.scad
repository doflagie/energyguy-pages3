// ============================================================
//  jpole_base.scad — Pool Noodle J-Pole Base Mount
//  TM-PNL2-ENC-003 Rev A
//  Mounts pool noodle stub section to mast or tripod,
//  captures matching stub wire, provides coax strain relief.
//  Fits noodle OD=47mm; mast socket: 25mm dia (standard PVC).
//  Print bed: 200×200×200mm  Material: PETG/ASA
// ============================================================

// ---- Parameters ----
NOODLE_OD    = 47.0;
NOODLE_CLR   = 0.7;
MAST_OD      = 25.4;   // 1-inch PVC mast or fiberglass pole
MAST_CLR     = 0.5;
NOODLE_INS   = 55.0;   // noodle insertion depth into base
MAST_INS     = 60.0;   // mast insertion depth
WALL         = 3.5;
BODY_H       = NOODLE_INS + MAST_INS + 20;  // total block height
WIRE_CH_W    = 4.5;    // stub wire capture groove
WIRE_CH_D    = 3.5;    // groove depth
COAX_OD      = 6.5;    // RG-58 / RG-8X OD
HEATSET_OD   = 4.7;    // M3 heat-set inserts
HEATSET_H    = 5.5;
STRAP_W      = 26.0;   // hose clamp strap slot width
STRAP_H      = 8.0;    // strap height
TAP_MARK_D   = 2.0;    // tap point indicator dot diameter
LABEL_DEPTH  = 0.4;

$fn = 64;

NOODLE_ID = NOODLE_OD + NOODLE_CLR * 2;
MAST_ID   = MAST_OD + MAST_CLR * 2;
BODY_OD   = max(NOODLE_ID, MAST_ID) + WALL * 2;

module base_body() {
    difference() {
        union() {
            // Main cylinder
            cylinder(d = BODY_OD, h = BODY_H, center = false);
            // Flat back (index face for wire groove alignment)
            translate([-BODY_OD/2, 0, 0])
                cube([BODY_OD, BODY_OD/2, BODY_H], center = false);
        }
        // Trim flat face flush
        translate([-BODY_OD/2 - 0.1, BODY_OD/4, -0.1])
            cube([BODY_OD + 0.2, BODY_OD, BODY_H + 0.2], center = false);

        // Noodle socket (top)
        translate([0, 0, BODY_H - NOODLE_INS])
            cylinder(d = NOODLE_ID, h = NOODLE_INS + 1, center = false);

        // Mast socket (bottom)
        cylinder(d = MAST_ID, h = MAST_INS + 1, center = false);

        // Wire groove — runs full height on flat back face
        // Captures matching stub wire and radiator wire
        translate([BODY_OD/4, -0.5, -0.1]) {
            cube([WIRE_CH_W, WIRE_CH_D + 1, BODY_H + 0.2], center = false);
        }

        // Coax entry — side, at 20mm from bottom
        translate([BODY_OD/2, 0, 25])
            rotate([0, 90, 0])
                cylinder(d = COAX_OD, h = BODY_OD/2 + 2, center = false);

        // Coax routing channel from side to center
        translate([0, 0, 25])
            cylinder(d = COAX_OD, h = MAST_INS - 15, center = false);

        // Feed tap indication marks — three dots spaced 5%, 10%, 15% of λ/4
        // (field-adjustable tap positions for impedance matching)
        for (h_pct = [0.05, 0.10, 0.15]) {
            translate([BODY_OD/2 - 1.5, -BODY_OD/4, MAST_INS + h_pct * NOODLE_INS])
                cylinder(d = TAP_MARK_D, h = 2, center = false);
        }

        // Strap slot (for hose clamp or velcro strap)
        translate([0, 0, BODY_H/2 - STRAP_H/2])
            difference() {
                cylinder(d = BODY_OD + 4, h = STRAP_H + 0.1, center = true);
                cylinder(d = BODY_OD - 1, h = STRAP_H + 0.2, center = true);
            }

        // Heat-set inserts — lid retention
        for (a = [45, 135, 225, 315])
            rotate([0, 0, a])
                translate([BODY_OD/2 - WALL, 0, BODY_H - HEATSET_H])
                    cylinder(d = HEATSET_OD, h = HEATSET_H + 1, center = false);

        // Label
        translate([BODY_OD/2 - LABEL_DEPTH, 0, 20])
            rotate([0, 90, 0])
                linear_extrude(LABEL_DEPTH + 0.1)
                    text("J-POLE BASE\nTM-PNL2-ENC-003", size = 4,
                         halign = "center", valign = "center", font = "Liberation Sans");
    }
}

module wire_cap_strip() {
    // Snap-in strip to retain wire in groove
    cube([WIRE_CH_W - 0.4, WIRE_CH_D - 0.2, BODY_H - 20], center = false);
}

// ---- Assembly ----
base_body();

// Wire cap strip (print aside)
translate([BODY_OD + 10, 0, 0])
    wire_cap_strip();

// ---- Notes ----
// Print upright (cylinder axis vertical); no supports needed.
// Wire cap: print flat; snap into groove after wire installed.
// Heat-set inserts for optional wire-retention lid (see documentation).
// Feed tap marks: 3 × indentations for 5%, 10%, 15% of stub length.
//   Start at 10% (center mark); adjust for SWR minimum.
// Mast OD: designed for 25.4mm (1in) fiberglass or PVC mast.
//   Adjust MAST_OD parameter for different mast sizes.
