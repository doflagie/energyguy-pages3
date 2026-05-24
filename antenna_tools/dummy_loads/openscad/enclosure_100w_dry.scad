// ============================================================
// 100W DRY DUMMY LOAD ENCLOSURE
// Forced-air cooled, N or SO-239 connector, fan mount
// Fits 200x200x200mm print bed
//
// Print settings:
//   Material: ASA or PETG (heat resistance required)
//   Infill:   25% gyroid
//   Walls:    3 perimeters
//   Temp:     ASA 255/105 (enclosure recommended)
//
// Assembly: Heatsink mounts externally on flat back.
//           Fan mounts on top. PCB/connector on front panel.
//           Ventilation slots on bottom (intake), top (exhaust).
// ============================================================

/* [Enclosure Dimensions] */
enc_w    = 150.0;   // width (connector axis)
enc_h    = 100.0;   // height
enc_d    = 80.0;    // depth (front to back)
wall     = 3.0;     // wall thickness

/* [Connector Type] */
// 0 = SO-239 (UHF female), 1 = N-type female
connector_type = 1;

// SO-239 panel hole diameter
so239_d  = 15.9;   // hex, use round approximation

// N-type panel hole diameter
ntype_d  = 21.0;   // hex, use round

/* [Heatsink Interface] */
// Heatsink bolt hole pattern (2 holes, horizontal)
hs_bolt_spacing = 100.0;  // center-to-center
hs_bolt_d       = 4.4;    // M4 clearance

/* [Fan Mount] */
fan_size   = 80.0;    // 80mm fan
fan_bolt_d = 3.4;     // M3 clearance
fan_bolt_r = 36.0;    // bolt pattern radius (71.5mm square = 35.75 diagonal)

/* [Ventilation] */
vent_slot_w   = 4.0;
vent_slot_h   = 40.0;
vent_cols     = 8;

$fn = 64;

// ============================================================
// Vented grill pattern
// ============================================================
module vent_slots(cols, slot_w, slot_h, spacing) {
    total_w = cols * (slot_w + spacing) - spacing;
    for (i = [0:cols-1]) {
        x = i * (slot_w + spacing) - total_w/2 + slot_w/2;
        translate([x, 0, 0])
            cube([slot_w, slot_h, wall*3], center=true);
    }
}

// ============================================================
// Main enclosure body
// ============================================================
module main_body() {
    difference() {
        cube([enc_w, enc_d, enc_h]);
        // Interior
        translate([wall, wall, wall])
            cube([enc_w - 2*wall, enc_d - 2*wall, enc_h - 2*wall + 1]);
    }
}

// ============================================================
// Front panel (connector face)
// ============================================================
module front_panel() {
    difference() {
        cube([enc_w, wall, enc_h]);
        // Connector hole (centered horizontally, 30mm from bottom)
        translate([enc_w/2, 0, 30])
            rotate([-90, 0, 0])
                cylinder(d=connector_type == 0 ? so239_d : ntype_d,
                         h=wall*3, center=true);
        // LED indicator hole (5mm, 20mm from connector)
        translate([enc_w/2 + 25, 0, 30])
            rotate([-90, 0, 0])
                cylinder(d=5.2, h=wall*3, center=true);
        // Label emboss area
        translate([enc_w/2 - 50, wall*0.1, enc_h - 25])
            cube([100, wall*0.5, 15]);
    }
}

// ============================================================
// Top panel with fan mount
// ============================================================
module top_panel() {
    translate([0, 0, enc_h - wall])
    difference() {
        cube([enc_w, enc_d, wall]);
        // Fan mount hole (80mm cutout)
        translate([enc_w/2, enc_d/2, 0])
            cylinder(d=fan_size - 4, h=wall*3, center=true);
        // Fan mounting holes
        for (x=[-1,1]) for (y=[-1,1]) {
            translate([enc_w/2 + x*fan_bolt_r/sqrt(2),
                       enc_d/2  + y*fan_bolt_r/sqrt(2), 0])
                cylinder(d=fan_bolt_d, h=wall*3, center=true);
        }
    }
}

// ============================================================
// Bottom panel with intake vents
// ============================================================
module bottom_panel() {
    difference() {
        cube([enc_w, enc_d, wall]);
        translate([enc_w/2, enc_d/2, wall/2])
            vent_slots(vent_cols, vent_slot_w, vent_slot_h, 3.0);
    }
}

// ============================================================
// Back panel with heatsink mounting holes
// ============================================================
module back_panel() {
    translate([0, enc_d - wall, 0])
    difference() {
        cube([enc_w, wall, enc_h]);
        // Heatsink bolt holes (2x M4, horizontal centered)
        for (x = [-1, 1]) {
            translate([enc_w/2 + x*hs_bolt_spacing/2, 0, enc_h/2])
                rotate([-90, 0, 0])
                    cylinder(d=hs_bolt_d, h=wall*3, center=true);
        }
        // Thermal interface opening (heatsink fins protrude through)
        // Optional: cut opening for direct heatsink fin contact
        // translate([enc_w/2 - 60, 0, 20]) cube([120, wall*2, 60]);
    }
}

// ============================================================
// Side panels
// ============================================================
module sides() {
    // Left
    cube([wall, enc_d, enc_h]);
    // Right
    translate([enc_w - wall, 0, 0]) cube([wall, enc_d, enc_h]);
}

// ============================================================
// Fan guard (separate part, print separately)
// ============================================================
module fan_guard() {
    guard_r = fan_size/2 + 3;
    translate([0, 0, -5])
    difference() {
        cylinder(r=guard_r, h=3);
        cylinder(d=fan_size - 4, h=5, center=true);
        // Guard bars
        for (a = [0:45:135]) {
            rotate([0, 0, a])
                cube([fan_size*2, 2, 5], center=true);
        }
        // Bolt holes
        for (a = [0, 90, 180, 270]) {
            rotate([0, 0, a])
                translate([fan_bolt_r/sqrt(2), fan_bolt_r/sqrt(2), 0])
                    cylinder(d=fan_bolt_d, h=5, center=true);
        }
    }
}

// ============================================================
// Cable entry (rear bottom)
// ============================================================
module cable_entry() {
    translate([wall + 15, enc_d - wall, wall + 15])
        rotate([-90, 0, 0])
            cylinder(d=12, h=wall*3, center=true);
}

// ============================================================
// Rubber feet standoffs (4x corners)
// ============================================================
module foot(x, y) {
    translate([x, y, -8])
        difference() {
            cylinder(d=12, h=8);
            translate([0, 0, -1]) cylinder(d=5, h=4);
        }
}

// ============================================================
// Assembly
// ============================================================
difference() {
    union() {
        main_body();
        front_panel();
        top_panel();
        back_panel();
        sides();
    }
    cable_entry();
}

bottom_panel();

// Feet (optional, for stability)
translate([10, 10, 0]) foot(0, 0);
translate([enc_w - 10, 10, 0]) foot(0, 0);
translate([10, enc_d - 10, 0]) foot(0, 0);
translate([enc_w - 10, enc_d - 10, 0]) foot(0, 0);

// Fan guard (move out of way for printing)
translate([enc_w + 20, 0, 0]) fan_guard();

echo(str("=== 100W DRY LOAD ENCLOSURE ==="));
echo(str("Dimensions: ", enc_w, " x ", enc_d, " x ", enc_h, " mm"));
echo(str("Connector:  ", connector_type == 0 ? "SO-239" : "N-type"));
echo(str("Fan:        ", fan_size, "mm"));
echo(str("Fits 200mm bed: ", max(enc_w, enc_d, enc_h) <= 195 ? "YES" : "SPLIT REQUIRED"));
