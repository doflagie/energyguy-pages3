// ============================================================
//  loop_connector.scad — Pool Noodle 90-degree Corner Connector
//  TM-PNL-ENC-002 Rev A
//  Pool Noodle Magnetic Loop Antenna Project
//  Print: PETG (UV/moisture resistant), 0.2mm layer, 4 perimeters
//  Bed footprint: ~100 x 100 x 62mm — fits 200x200 bed
//  Print orientation: lay flat with corner elbow face up
// ============================================================

// ---- Parameters ----
NOODLE_OD    = 47.0;   // pool noodle outer diameter (mm)
NOODLE_ID    = 25.0;   // pool noodle inner diameter
SOCKET_DEPTH = 42.0;   // socket insertion depth
SOCKET_CL    = 0.6;    // diametral clearance (press fit: 0.3, slide: 0.6)
WIRE_CH_W    = 4.8;    // wire channel width
WIRE_CH_D    = 3.8;    // wire channel depth (into wall)
CORNER_SQ    = 32.0;   // corner post side length
WALL         = 3.2;    // minimum wall thickness
INSERT_OD    = 6.0;    // M3 heat-set insert OD (e.g., Ruthex M3x5.7)
INSERT_L     = 6.0;    // insert length
PASS_D       = 5.5;    // wire pass-through hole diameter
$fn          = 72;

SOCKET_OD = NOODLE_OD + 2 * WALL;  // outer diameter of socket tube

// ---- Single socket arm (cylinder with channel) ----
module socket_arm(length) {
    difference() {
        // Outer tube
        cylinder(h=length, d=SOCKET_OD);
        // Noodle bore
        translate([0, 0, -0.1])
            cylinder(h=length + 0.2, d=NOODLE_OD + SOCKET_CL);
        // Wire channel — runs along outer surface (positive-X side)
        translate([NOODLE_OD/2 + WALL - WIRE_CH_D, -WIRE_CH_W/2, -0.1])
            cube([WIRE_CH_D + 0.1, WIRE_CH_W, length + 0.2]);
        // Retention ridges (2x bumps inside bore to grip noodle) — cut negative
        for (z = [10, SOCKET_DEPTH - 10])
            translate([0, 0, z])
                rotate_extrude()
                    translate([NOODLE_OD/2 - 0.4, 0, 0])
                        circle(r=0.5);
    }
}

// ---- Wire channel snap cover (separate part, press fit) ----
module channel_cover(length) {
    // Thin strip that presses into the wire channel to retain wire
    translate([NOODLE_OD/2 + WALL - WIRE_CH_D + 0.3, -WIRE_CH_W/2 + 0.4, 3])
        difference() {
            cube([WIRE_CH_D - 0.6, WIRE_CH_W - 0.8, length - 6]);
            // Hollow inside for flex
            translate([0.8, 0.8, -0.1])
                cube([WIRE_CH_D - 2.2, WIRE_CH_W - 2.4, length - 4]);
        }
}

// ---- Corner elbow post ----
module corner_post() {
    difference() {
        // Solid square post
        cube([CORNER_SQ, CORNER_SQ, CORNER_SQ], center=true);
        // Wire pass-through (axial, Z direction — connects two channel sides)
        cylinder(h=CORNER_SQ + 0.2, d=PASS_D, center=true);
        // M3 heat-set insert holes (2x, for accessory mounting)
        for (rot = [0, 90])
            rotate([0, 0, rot])
                translate([CORNER_SQ/2 - 4, 0, 0])
                    rotate([0, 90, 0])
                        cylinder(h=INSERT_L + 1, d=INSERT_OD, center=true);
        // Wire channel continuation groove across post face (X arm)
        translate([CORNER_SQ/2, -WIRE_CH_W/2, -CORNER_SQ/2])
            cube([0.2, WIRE_CH_W, CORNER_SQ]);
        // Wire channel continuation groove across post face (Y arm)
        translate([-CORNER_SQ/2, NOODLE_OD/2 + WALL - WIRE_CH_D - CORNER_SQ/2, -CORNER_SQ/2])
            cube([CORNER_SQ, WIRE_CH_D + 0.1, WIRE_CH_W]);
    }
}

// ---- Assembled connector ----
// Corner post centered at origin
translate([0, 0, NOODLE_OD/2 + WALL])
    corner_post();

// Arm 1: extends along +X axis
translate([CORNER_SQ/2, 0, NOODLE_OD/2 + WALL])
    rotate([0, -90, 0])
        socket_arm(SOCKET_DEPTH);

// Arm 2: extends along +Y axis
translate([0, CORNER_SQ/2, NOODLE_OD/2 + WALL])
    rotate([90, 0, 0])
        socket_arm(SOCKET_DEPTH);

// Wire channel covers (shown in place; print separately)
color("LimeGreen", 0.7) {
    translate([CORNER_SQ/2, 0, NOODLE_OD/2 + WALL])
        rotate([0, -90, 0])
            channel_cover(SOCKET_DEPTH);
    translate([0, CORNER_SQ/2, NOODLE_OD/2 + WALL])
        rotate([90, 0, 0])
            channel_cover(SOCKET_DEPTH);
}

// ---- Usage Notes ----
// 4 connectors needed per loop (one per corner).
// Print with 100% infill at corners for strength.
// Insert M3 heat-set inserts with soldering iron before assembly.
// Slide pool noodle sections into both socket arms; the retention
// ridges provide friction hold. Tape or cable-tie for wind resistance.
// Thread loop wire through channel before pressing cover into place.
// Wire pass-through hole aligns wire from one arm channel to the other
// through the post interior, forming a continuous channel around the corner.
