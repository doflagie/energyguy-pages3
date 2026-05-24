// ============================================================
// PASSIVE FIELD STRENGTH METER ENCLOSURE
// TM-FSM-001 Rev A — Germanium Diode / Analog Meter Version
//
// Handheld form factor: 120 x 65 x 35 mm
// Fits: 60x40mm panel meter, BNC connector, rotary band switch,
//       sensitivity pot, single-sided FR-4 PCB
//
// Print settings:
//   Material: PETG (impact resistant; handles field drops)
//   Layer: 0.2mm, Walls: 3, Infill: 20% gyroid
//   No supports required
// ============================================================

/* [Main Box Dimensions] */
box_w  = 120.0;   // width (left to right)
box_d  = 65.0;    // depth (front to back)
box_h  = 35.0;    // height (body, not lid)
wall   = 2.5;

/* [Lid] */
lid_h    = 8.0;   // lid height including overlap skirt
lid_wall = 2.5;

/* [Analog Meter Window] */
meter_w  = 62.0;  // meter face aperture width
meter_h  = 42.0;  // meter face aperture height
meter_cx = box_w / 2;
meter_cy = box_h / 2;

/* [Connectors — front face Y=0] */
bnc_x  = 15.0;    // BNC probe input
bnc_z  = box_h / 2;

/* [Lid top controls] */
rot_x  = 85.0;    // rotary band switch X
rot_y  = box_d / 2;
sens_x = 30.0;    // sensitivity pot X
sens_y = box_d / 2;

$fn = 48;

// ============================================================
// Shared modules
// ============================================================
module rnd_hole(d, h = wall*3) {
    cylinder(d = d, h = h, center = true);
}

module pcb_boss(x, y, bh = 4.0) {
    translate([x, y, 0])
        cylinder(d = 6.0, h = wall + bh);
    translate([x, y, wall + bh - 0.01])
        cylinder(d = 2.5, h = 8);  // M2.5 tap guide
}

// ============================================================
// BOX BODY
// ============================================================
module box_body() {
    difference() {
        union() {
            cube([box_w, box_d, box_h]);
            // Lid engagement rail
            translate([wall, wall, box_h - 1])
                cube([box_w - 2*wall, box_d - 2*wall, 2]);
        }
        // Interior
        translate([wall, wall, wall])
            cube([box_w - 2*wall, box_d - 2*wall, box_h]);

        // Front face — meter window (large rect cutout)
        translate([meter_cx, -0.01, meter_cy])
            rotate([-90, 0, 0])
                hull() {
                    for (dx = [-meter_w/2 + 3, meter_w/2 - 3])
                    for (dz = [-meter_h/2 + 3, meter_h/2 - 3])
                        translate([dx, dz, 0])
                            cylinder(r = 3, h = wall*2, center = true);
                }

        // BNC connector hole (front face)
        translate([bnc_x, -0.01, bnc_z])
            rotate([-90, 0, 0]) rnd_hole(12.5);

        // Lid bolts: 4x corners M3
        for (x = [10, box_w - 10]) for (y = [10, box_d - 10]) {
            translate([x, y, box_h + 0.5])
                cylinder(d = 3.4, h = 10, center = true);
        }

        // PCB vent slots (bottom)
        for (i = [0:3]) {
            translate([20 + i * 22, box_d / 2, -0.01])
                cube([8, 4, wall + 0.02], center = true);
        }
    }

    // PCB bosses (floor of box)
    translate([0, 0, wall]) {
        pcb_boss(12, 12);
        pcb_boss(box_w - 12, 12);
        pcb_boss(12, box_d - 12);
        pcb_boss(box_w - 12, box_d - 12);
    }

    // Meter glass retainer clips (inside front wall)
    for (side = [-1, 1])
        translate([meter_cx + side * (meter_w/2 + 2), wall/2, meter_cy])
            cube([3, wall, meter_h * 0.6], center = true);
}

// ============================================================
// LID
// ============================================================
module box_lid() {
    translate([0, 0, box_h + 5])  // offset for print preview
    difference() {
        union() {
            cube([box_w, box_d, lid_wall]);
            translate([lid_wall/2, lid_wall/2, -(lid_h - lid_wall)])
                difference() {
                    cube([box_w - lid_wall, box_d - lid_wall, lid_h]);
                    translate([lid_wall, lid_wall, -0.01])
                        cube([box_w - lid_wall*3, box_d - lid_wall*3, lid_h + 0.02]);
                }
        }

        // Rotary band switch hole (10mm shaft, D-flat)
        translate([rot_x, rot_y, 0])
            cylinder(d = 10.5, h = lid_wall*3, center = true);
        translate([rot_x + 5.5, rot_y, 0])
            cube([3, 8, lid_wall*3], center = true);

        // Sensitivity pot hole
        translate([sens_x, sens_y, 0])
            cylinder(d = 7.5, h = lid_wall*3, center = true);

        // Lid bolt holes
        for (x = [10, box_w - 10]) for (y = [10, box_d - 10]) {
            translate([x, y, 0])
                cylinder(d = 3.4, h = lid_wall*3, center = true);
        }

        // Probe strain-relief groove (if cable exits lid)
        translate([15, box_d/2, 0])
            cylinder(d = 5, h = lid_wall*3, center = true);
    }
}

// ============================================================
// Meter bezel / retainer ring (glues over meter face aperture)
// ============================================================
module meter_bezel() {
    translate([meter_cx, -0.1, meter_cy]) rotate([90,0,0])
    difference() {
        // Outer rim matches aperture + 4mm border
        hull() {
            for (dx = [-meter_w/2 - 2 + 3, meter_w/2 + 2 - 3])
            for (dz = [-meter_h/2 - 2 + 3, meter_h/2 + 2 - 3])
                translate([dx, dz, 0]) cylinder(r = 3, h = 1.5);
        }
        // Viewing aperture (slightly smaller for glass retention)
        hull() {
            for (dx = [-meter_w/2 + 1, meter_w/2 - 1])
            for (dz = [-meter_h/2 + 1, meter_h/2 - 1])
                translate([dx, dz, 0]) cylinder(r = 2, h = 2, center = true);
        }
    }
}

// ============================================================
// Rubber foot recesses (bottom, 4x)
// ============================================================
module foot_recesses() {
    for (x = [8, box_w-8]) for (y = [8, box_d-8]) {
        translate([x, y, -0.01])
            cylinder(d = 10, h = 2);
    }
}

// ============================================================
// Render
// ============================================================
box_body();
box_lid();
foot_recesses();

echo("=== PASSIVE FSM ENCLOSURE ===");
echo(str("Box: ", box_w, " x ", box_d, " x ", box_h, " mm"));
echo(str("Lid: ", box_w, " x ", box_d, " x ", lid_wall, " mm + ", lid_h, "mm skirt"));
echo(str("Meter aperture: ", meter_w, " x ", meter_h, " mm"));
echo(str("Fits 200x200 bed: ", max(box_w, box_d) <= 195 ? "YES" : "SPLIT REQUIRED"));
echo("Print: PETG, 3 walls, 20% gyroid, 0.2mm layer");
