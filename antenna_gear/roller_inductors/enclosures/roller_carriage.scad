/**
 * roller_carriage.scad — TM-ROLLER-001 Rev A
 * Roller Contact Carriage Assembly
 *
 * Slides along two 6mm guide rods parallel to the coil former.
 * Spring-loaded silver-plated roller bearing presses against coil wire.
 * M5 leadscrew nut drives carriage along coil axis.
 *
 * Print settings:
 *   Material:    PETG (dimensional stability)
 *   Infill:      50% grid
 *   Wall:        4 perimeters (heavy walls for rod holes)
 *   Layer:       0.15mm (tight tolerances on rod bores)
 *   Orientation: Flat face down
 *   Supports:    Yes, for spring pocket
 *
 * Print envelope: 95mm × 50mm × 40mm
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// Guide rods
rod_dia         = 6.0;   // Rod diameter
rod_clearance   = 0.15;  // Clearance for sliding fit
rod_bore_dia    = rod_dia + 2 * rod_clearance;  // 6.3mm
rod_spacing     = 85.0;  // Center-to-center distance between rods
                          // = former_od + 2×10mm side offsets = 63.5 + 21.5 = 85mm

// Carriage body
body_width      = rod_spacing + rod_dia + 6;  // Total body width (~97mm)
body_depth      = 35.0;   // Depth (parallel to former axis)
body_height     = 30.0;   // Height of carriage body

// Roller assembly
roller_dia      = 8.0;    // Roller bearing OD (silver-plated)
roller_width    = 4.0;    // Roller contact width
roller_shaft    = 3.0;    // Roller shaft (M3 stainless)
roller_arm_len  = 20.0;   // Length of spring-loaded arm
roller_arm_thick = 4.0;

// Spring pocket (compression spring)
spring_od       = 5.0;
spring_id_bore  = 5.2;    // Bore for spring
spring_depth    = 15.0;   // Pocket depth

// Spring pre-load: ~0.5N contact force
// Spring: 5mm OD, 15mm free length, ~0.1N/mm rate → 5mm compression = 0.5N

// Leadscrew nut
ls_pitch        = 2.0;    // Leadscrew pitch (mm)
ls_dia          = 5.0;    // M5 leadscrew diameter
ls_nut_od       = 9.0;    // M5 hex nut flat-to-flat
ls_nut_thick    = 4.0;    // M5 nut height

// Wiper terminal
wiper_od        = 3.3;    // M3 screw hole for wiper lug
wiper_boss_h    = 5.0;    // Boss height for wiper mount

// Coil centerline height (from rod centerline to coil OD surface)
// Rods are 10mm outside the former OD (31.75 + 10 = 41.75mm from center)
// Rod center is at y = former_od/2 + rail_offset = 31.75 + 10 = 41.75mm
// Coil OD surface is at y = former_od/2 = 31.75mm from coil axis
// Roller must reach coil OD: carriage needs to position roller at y = 31.75mm

coil_surface_y  = 31.75 + 4.0;  // Distance from rod line to coil surface + arm clearance

// ─── Modules ──────────────────────────────────────────────────────────────────

module rod_bore() {
    // Linear bearing bore (IGUS RJUM-01-06 or plain bore with PTFE lining)
    cylinder(h = body_depth + 1, d = rod_bore_dia, $fn = 32);
}

module leadscrew_nut_pocket() {
    // Hex pocket for M5 anti-backlash nut (or simple hex nut)
    translate([0, 0, body_depth / 2])
    union() {
        cylinder(h = ls_nut_thick + 1, d = ls_nut_od + 0.4, $fn = 6);
        // Through hole for leadscrew
        translate([0, 0, -body_depth/2 - 0.5])
            cylinder(h = body_depth + 1, d = ls_dia + 0.3, $fn = 32);
    }
}

module spring_pocket() {
    cylinder(h = spring_depth, d = spring_id_bore, $fn = 24);
}

module roller_arm_slot() {
    // Slot for roller arm to pivot through body
    cube([roller_arm_thick + 0.4, roller_arm_len + 2, body_height / 2], center = true);
}

module carriage_body() {
    difference() {
        // Main body block
        hull() {
            translate([-body_width/2 + 6, 0, 0])
                cylinder(h = body_height, r = 6, $fn = 32);
            translate([ body_width/2 - 6, 0, 0])
                cylinder(h = body_height, r = 6, $fn = 32);
            translate([-body_width/2 + 6, body_depth - 6, 0])
                cylinder(h = body_height, r = 6, $fn = 32);
            translate([ body_width/2 - 6, body_depth - 6, 0])
                cylinder(h = body_height, r = 6, $fn = 32);
        }

        // Left rod bore
        translate([-rod_spacing/2, body_depth/2, -0.5])
            rod_bore();

        // Right rod bore
        translate([rod_spacing/2, body_depth/2, -0.5])
            rod_bore();

        // Leadscrew hole (center, along axis)
        translate([0, body_depth/2, -0.5])
            leadscrew_nut_pocket();

        // Spring pocket (top center, for roller arm spring)
        translate([0, body_depth/2, body_height - spring_depth])
            spring_pocket();

        // Roller arm slot
        translate([0, body_depth/2, body_height * 0.6])
            roller_arm_slot();

        // Wiper terminal boss cutout (to be filled with boss)
        translate([15, body_depth - 5, body_height - 3])
            cylinder(h = wiper_boss_h + 1, d = wiper_od + 4, $fn = 16);
    }

    // Wiper terminal boss
    translate([15, body_depth - 5, body_height])
    difference() {
        cylinder(h = wiper_boss_h, d = wiper_od + 6, $fn = 16);
        translate([0, 0, -0.5])
            cylinder(h = wiper_boss_h + 1, d = wiper_od, $fn = 16);
    }
}

module roller_arm() {
    // Pivoting arm that holds roller bearing, spring-loaded downward
    difference() {
        union() {
            // Main arm
            cube([roller_arm_thick, roller_arm_len, 5], center = false);
            // Roller mount fork at bottom
            translate([0, 0, 0])
            union() {
                translate([0, -5, 0]) cube([roller_arm_thick, 5, 8]);
                translate([roller_arm_thick + 3, -5, 0]) cube([roller_arm_thick, 5, 8]);
            }
        }
        // Pivot pin hole (top of arm)
        translate([roller_arm_thick/2, roller_arm_len - 3, -0.5])
            cylinder(h = 6, d = 3.2, $fn = 16);
        // Roller shaft hole (bottom of arm, fork)
        translate([roller_arm_thick/2, -2, 3])
            rotate([90, 0, 0])
            translate([0, 0, -roller_arm_thick - 4])
            cylinder(h = roller_arm_thick * 2 + 8, d = roller_shaft + 0.2, $fn = 16);
    }
}

module roller_bearing() {
    // Simplified roller representation (actual: silver-plated ball bearing or
    // turned brass roller, silver-plated, on M3 stainless shaft)
    color("silver")
    difference() {
        cylinder(h = roller_width, d = roller_dia, $fn = 32, center = true);
        // Shaft bore
        cylinder(h = roller_width + 1, d = roller_shaft + 0.1, $fn = 16, center = true);
        // Chamfer edges
        translate([0, 0,  roller_width/2])
            cylinder(h = 0.5, d1 = roller_dia - 1, d2 = roller_dia + 1, $fn = 32);
        translate([0, 0, -roller_width/2])
            cylinder(h = 0.5, d1 = roller_dia + 1, d2 = roller_dia - 1, $fn = 32);
    }
}

// ─── Position Indicator Groove ────────────────────────────────────────────────

module indicator_pointer() {
    // V-shaped pointer on carriage front face, aligns with position scale
    translate([0, -0.5, body_height/2])
    linear_extrude(height = 2)
    polygon(points = [[0, 0], [-3, 8], [3, 8]]);
}

// ─── Assembly ─────────────────────────────────────────────────────────────────

module carriage_assembly(show_roller = true) {
    carriage_body();
    indicator_pointer();

    if (show_roller) {
        // Roller arm in position (pointing down toward coil)
        translate([0, body_depth/2, body_height * 0.4])
        rotate([0, 0, 0]) {
            color("gray")
            translate([-roller_arm_thick/2, 0, 0])
                roller_arm();
            // Roller at bottom of arm
            translate([0, -2, 3])
                rotate([90, 0, 0])
                translate([0, 0, 0])
                roller_bearing();
        }
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 32;

// Print carriage body and arm separately
module print_carriage_body() {
    carriage_body();
}

module print_roller_arm() {
    translate([0, 0, 0])
    roller_arm();
}

// Default: show full assembly
carriage_assembly(show_roller = true);

// To print just the body:
// print_carriage_body();

// To print just the roller arm:
// translate([120, 0, 0]) print_roller_arm();

// ─── Notes ────────────────────────────────────────────────────────────────────
// After printing:
// 1. Press M5 hex nut into leadscrew pocket (light press-fit, secure with CA glue)
// 2. Install IGUS RJUM-01-06 linear bearings in rod bores (or use plain bore + grease)
// 3. Insert M3×20 stainless pivot pin through body and arm top
// 4. Thread roller shaft (M3×20) through fork and roller bearing
// 5. Install compression spring in spring pocket
// 6. Adjust spring pre-load: roller should press coil with ~0.5N force
// 7. Silver-plate roller: coat with silver conductive paint or use
//    purchased silver-plated brass roller bearing (McMaster 5912K713 equiv)
// 8. Attach wiper terminal lug at boss with M3 screw
// 9. Connect flexible wire from wiper terminal to main terminal
