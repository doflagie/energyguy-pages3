// ============================================================
//  dipole_center.scad — Dipole Center Insulator + Coil Form
//  TM-PNL2-ENC-002 Rev A
//  Central section: mounts loading coil, supports feed balun,
//  provides strain relief for coax and arm wires.
//  Print bed: 200×200×200mm  Material: PETG/ASA
// ============================================================

// ---- Parameters ----
NOODLE_OD    = 47.0;   // mm
NOODLE_CLR   = 0.8;    // socket clearance
INSERT_D     = 40.0;   // insertion depth each side (mm)
BODY_W       = 80.0;   // total width of center block (mm)
BODY_H       = 60.0;   // height
BODY_D       = 55.0;   // depth
WALL         = 3.2;    // wall thickness
COAX_ID      = 6.5;    // coax cable OD + clearance
WIRE_SLOT_W  = 5.0;    // arm wire slot width
WIRE_SLOT_H  = 4.0;    // arm wire slot height
M3_BORE      = 3.3;    // M3 bolt clearance
M3_HEAD      = 6.2;    // M3 head diameter
BALUN_DIA    = 30.0;   // space for ferrite toroids (balun core)
BALUN_H      = 22.0;   // balun core height
HEATSET_OD   = 4.7;    // M3 heat-set insert OD
HEATSET_H    = 5.5;    // insert height
LID_H        = 8.0;    // snap lid height
LABEL_D      = 0.5;    // label depth

$fn = 64;

SOC_ID = NOODLE_OD + NOODLE_CLR * 2;

module center_body() {
    difference() {
        // Main block — rounded box
        minkowski() {
            cube([BODY_W - 6, BODY_D - 6, BODY_H - 6], center = true);
            sphere(r = 3, $fn = 24);
        }

        // Noodle arm sockets — horizontal, left and right
        for (x = [-BODY_W/2 + INSERT_D/2 - 0.5, BODY_W/2 - INSERT_D/2 + 0.5]) {
            translate([x, 0, 0])
                rotate([0, 90, 0])
                    cylinder(d = SOC_ID, h = INSERT_D + 1, center = true);
        }

        // Hollow interior (save material, reduce weight)
        cube([BODY_W - WALL*2 - 20, BODY_D - WALL*2, BODY_H - WALL*2 - 4], center = true);

        // Coax entry — bottom center
        translate([0, 0, -BODY_H/2])
            cylinder(d = COAX_ID, h = BODY_H, center = false);

        // Coax strain relief slot
        translate([0, -BODY_D/2, -BODY_H/2 + WALL])
            cube([COAX_ID + 1, WALL + 2, 20], center = true);

        // Arm wire channels — top face, perpendicular to noodle sockets
        translate([0, 0, BODY_H/2 - WIRE_SLOT_H/2 + 0.5])
            cube([BODY_W + 2, WIRE_SLOT_W, WIRE_SLOT_H + 1], center = true);

        // Balun core cavity (interior, mount ferrite cores)
        translate([0, 0, 5])
            cylinder(d = BALUN_DIA, h = BALUN_H, center = true);

        // Lid mating recess
        translate([0, BODY_D/2 - WALL/2, BODY_H/4])
            cube([BODY_W - WALL*2, WALL + 1, BODY_H/2], center = true);

        // Heat-set insert holes for lid (4× corners)
        for (xy = [[-BODY_W/2 + 10, 0], [BODY_W/2 - 10, 0]])
            translate([xy[0], BODY_D/2 - HEATSET_H/2, BODY_H/4])
                cylinder(d = HEATSET_OD, h = HEATSET_H + 1, center = true);

        // Label top face
        translate([0, 0, BODY_H/2 - LABEL_D/2])
            linear_extrude(LABEL_D + 0.1)
                text("PNILD CENTER  TM-PNL2-ENC-002", size = 3.5,
                     halign = "center", font = "Liberation Sans");
    }
}

module front_lid() {
    // Removable front panel for coil access
    difference() {
        cube([BODY_W - WALL*2 - 0.6, WALL + 1, BODY_H/2 - 0.6], center = true);
        // M3 bolt holes
        for (x = [-BODY_W/2 + 10, BODY_W/2 - 10])
            translate([x, 0, 0])
                cylinder(d = M3_BORE, h = WALL + 3, center = true);
    }
}

// ---- Assembly ----
center_body();

// Print lid separately (moved aside)
translate([0, BODY_D + 15, 0])
    front_lid();

// ---- Print notes ----
// Body: print with front face down (flat reference surface)
// Support: internal overhangs only (slicer tree supports)
// Lid: flat face down, no supports
// Heat-set inserts: install with soldering iron at 180°C
// Post-print: drill coax hole to final size with 6.5mm bit if needed
// Material: PETG; avoid PLA (heat near coil from RF loss)
