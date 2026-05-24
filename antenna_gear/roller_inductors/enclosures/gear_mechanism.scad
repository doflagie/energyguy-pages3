/**
 * gear_mechanism.scad — TM-ROLLER-001 Rev A
 * Worm Gear Reduction Drive (10:1) for Roller Carriage
 *
 * Worm gear housing with integrated leadscrew support bearings.
 * Worm: 1-start, 20mm pitch diameter, 2mm lead
 * Worm gear: 10-tooth spur → 10:1 reduction
 * Output: drives M5×2mm-pitch leadscrew
 *
 * Print settings:
 *   Material:    PETG
 *   Infill:      60% gyroid (gear housing needs rigidity)
 *   Wall:        4 perimeters
 *   Layer:       0.15mm
 *   Orientation: Split halves flat face down
 *
 * Print envelope: 90mm × 70mm × 55mm per half
 * NOTE: Housing prints as two halves; M3 screws join them.
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// Worm parameters
worm_pitch_dia  = 20.0;  // Worm pitch diameter
worm_lead       = 2.0;   // Lead per revolution (= leadscrew pitch)
worm_len        = 40.0;  // Worm length
worm_shaft_dia  = 5.0;   // Worm input shaft diameter (motor coupling)
worm_bore_dia   = worm_shaft_dia + 0.2;  // Clearance

// Worm gear parameters
worm_gear_teeth = 10;    // Number of worm wheel teeth → 10:1 ratio
worm_gear_pd    = 16.0;  // Worm gear pitch diameter (roughly: teeth × module)
worm_gear_thick = 10.0;  // Gear face width
worm_gear_od    = worm_gear_pd + 4;  // OD including addendum
ls_bore_dia     = 5.2;   // Leadscrew bore (M5 + clearance)

// Housing
housing_len     = worm_len + 20;  // Extra for bearing seats
housing_width   = (worm_pitch_dia/2) + (worm_gear_od/2) + 20;
housing_height  = worm_gear_od + 10;
wall            = 4.0;

// Bearing seats (608ZZ bearings: 8mm bore, 22mm OD, 7mm wide)
bearing_bore    = 8.2;   // 608 bore + clearance
bearing_od      = 22.0;
bearing_width   = 7.0;

// M5 leadscrew bearing seat (625ZZ: 5mm bore, 16mm OD, 5mm wide)
ls_bearing_bore = 5.2;
ls_bearing_od   = 16.2;
ls_bearing_w    = 5.0;

// Housing screws (M3)
screw_dia       = 3.3;
screw_head_dia  = 6.0;
screw_head_h    = 3.0;

// ─── Worm Profile (simplified spur-type for printability) ─────────────────────

module worm(length, pitch_dia, shaft_dia, lead_angle_deg = 8) {
    // Simplified: threaded cylinder approximation
    difference() {
        // Worm body
        cylinder(h = length, d = pitch_dia + 3, $fn = 64, center = true);
        // Shaft bore
        cylinder(h = length + 1, d = shaft_dia, $fn = 32, center = true);
        // Helical thread groove (simplified as helical cuts)
        // In practice: print as smooth cylinder, cut thread on lathe or use
        // commercial worm gear set (e.g., SDP/SI S10T3-GFF10200)
        for (i = [0 : length / lead_angle_deg]) {
            rotate([0, 0, i * 36])
            translate([0, 0, i * lead_angle_deg - length/2])
            rotate([lead_angle_deg, 0, 0])
            translate([pitch_dia/2 + 1.5, 0, 0])
                cube([3, 2, length], center = true);
        }
    }
}

module worm_gear(teeth, pitch_dia, thickness, bore_dia) {
    // Spur gear approximation (actual worm gear needs helical cut)
    // For printing, use a standard involute spur gear at correct pitch dia
    module gear_tooth(r, h) {
        rotate_extrude(angle = 360 / teeth, $fn = 4)
        hull() {
            translate([r - 1, 0]) circle(r = 1.5, $fn = 8);
            translate([r + 2, 0]) circle(r = 0.5, $fn = 8);
        }
    }
    difference() {
        union() {
            cylinder(h = thickness, d = pitch_dia, $fn = 64, center = true);
            for (i = [0 : teeth - 1]) {
                rotate([0, 0, i * (360 / teeth)])
                    translate([pitch_dia/2, 0, 0])
                    cylinder(h = thickness, r = 1.5, $fn = 16, center = true);
            }
        }
        // Bore
        cylinder(h = thickness + 1, d = bore_dia, $fn = 32, center = true);
        // Set screw flat
        translate([bore_dia/2 + 0.5, 0, 0])
            cube([2, bore_dia, thickness + 1], center = true);
        // Weight reduction holes
        for (i = [0 : teeth - 1]) {
            rotate([0, 0, i * (360 / teeth) + 180/teeth])
            translate([pitch_dia/4, 0, 0])
                cylinder(h = thickness + 1, d = 3, $fn = 16, center = true);
        }
    }
}

// ─── Housing ──────────────────────────────────────────────────────────────────

module housing_half_bottom() {
    difference() {
        // Outer body
        hull() {
            translate([-housing_len/2 + 8, -housing_width/2 + 8, 0])
                cylinder(h = housing_height/2, r = 8, $fn = 32);
            translate([ housing_len/2 - 8, -housing_width/2 + 8, 0])
                cylinder(h = housing_height/2, r = 8, $fn = 32);
            translate([-housing_len/2 + 8,  housing_width/2 - 8, 0])
                cylinder(h = housing_height/2, r = 8, $fn = 32);
            translate([ housing_len/2 - 8,  housing_width/2 - 8, 0])
                cylinder(h = housing_height/2, r = 8, $fn = 32);
        }

        // Worm shaft bore (X-axis, through housing width)
        translate([0, 0, housing_height/4])
        rotate([90, 0, 0])
            cylinder(h = housing_width + 1, d = worm_bore_dia,
                     $fn = 32, center = true);

        // Worm shaft bearing seats (both ends)
        for (side = [-1, 1]) {
            translate([0, side * (housing_width/2 - bearing_width/2), housing_height/4])
            rotate([90, 0, 0])
                cylinder(h = bearing_width + 0.5, d = bearing_od,
                         $fn = 32, center = true);
        }

        // Leadscrew bore (along housing length axis)
        translate([0, worm_gear_pd/2 + 4, -0.5])
            cylinder(h = housing_height/2 + 1, d = ls_bore_dia, $fn = 32);

        // Leadscrew bearing seat (bottom)
        translate([0, worm_gear_pd/2 + 4, -0.5])
            cylinder(h = ls_bearing_w + 0.5, d = ls_bearing_od, $fn = 32);

        // Worm gear cavity
        translate([0, worm_gear_pd/2 + 4, housing_height/4])
            cylinder(h = worm_gear_thick + 2, d = worm_gear_od + 2,
                     $fn = 64, center = true);

        // Assembly screw holes (4 corners)
        for (x = [-1, 1]) for (y = [-1, 1]) {
            translate([x * (housing_len/2 - 8), y * (housing_width/2 - 8), -0.5])
                cylinder(h = housing_height/2 + 1, d = screw_dia, $fn = 16);
        }
    }
}

module housing_half_top() {
    translate([0, 0, housing_height/2])
    rotate([180, 0, 0])
    difference() {
        // Mirror of bottom half (without leadscrew exit at bottom)
        hull() {
            translate([-housing_len/2 + 8, -housing_width/2 + 8, 0])
                cylinder(h = housing_height/2, r = 8, $fn = 32);
            translate([ housing_len/2 - 8, -housing_width/2 + 8, 0])
                cylinder(h = housing_height/2, r = 8, $fn = 32);
            translate([-housing_len/2 + 8,  housing_width/2 - 8, 0])
                cylinder(h = housing_height/2, r = 8, $fn = 32);
            translate([ housing_len/2 - 8,  housing_width/2 - 8, 0])
                cylinder(h = housing_height/2, r = 8, $fn = 32);
        }

        // Worm shaft bore
        translate([0, 0, housing_height/4])
        rotate([90, 0, 0])
            cylinder(h = housing_width + 1, d = worm_bore_dia,
                     $fn = 32, center = true);

        // Bearing seats
        for (side = [-1, 1]) {
            translate([0, side * (housing_width/2 - bearing_width/2), housing_height/4])
            rotate([90, 0, 0])
                cylinder(h = bearing_width + 0.5, d = bearing_od,
                         $fn = 32, center = true);
        }

        // Worm gear cavity (top half)
        translate([0, worm_gear_pd/2 + 4, housing_height/4])
            cylinder(h = worm_gear_thick + 2, d = worm_gear_od + 2,
                     $fn = 64, center = true);

        // Screw counterbores for top half
        for (x = [-1, 1]) for (y = [-1, 1]) {
            translate([x * (housing_len/2 - 8), y * (housing_width/2 - 8), -0.5]) {
                cylinder(h = housing_height/2 + 1, d = screw_dia, $fn = 16);
                cylinder(h = screw_head_h + 0.5, d = screw_head_dia, $fn = 16);
            }
        }
    }
}

// ─── Scale Indicator Dial ─────────────────────────────────────────────────────

module position_dial(radius = 20, n_ticks = 20) {
    // Dial plate mounted on worm gear shaft output end
    difference() {
        cylinder(h = 3, d = radius * 2, $fn = 64);
        cylinder(h = 4, d = ls_bore_dia + 0.5, $fn = 32);
        for (i = [0 : n_ticks - 1]) {
            rotate([0, 0, i * 360/n_ticks])
            translate([radius - 2, 0, -0.5])
                cylinder(h = 4, d = 1, $fn = 8);
        }
        // Numeral positions (0, 5, 10, 15, 20)
        for (i = [0, 5, 10, 15]) {
            rotate([0, 0, i * 360/n_ticks])
            translate([radius - 3, 0, -0.5])
                cylinder(h = 4, d = 1.5, $fn = 8);
        }
    }
    // Pointer mark on top face
    translate([radius - 4, 0, 3])
        cylinder(h = 1.5, d1 = 2, d2 = 0.5, $fn = 8);
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 64;

// Full housing assembly (exploded for inspection)
module full_assembly() {
    color("lightblue", 0.7) housing_half_bottom();
    color("lightblue", 0.5)
    translate([0, 0, 0.5]) housing_half_top();

    // Worm gear in position
    color("gold")
    translate([0, worm_gear_pd/2 + 4, housing_height/4])
        worm_gear(worm_gear_teeth, worm_gear_pd, worm_gear_thick, ls_bore_dia);
}

full_assembly();

// To print halves separately:
// housing_half_bottom();
// translate([housing_len + 10, 0, 0]) rotate([0, 180, 0]) housing_half_top();
// translate([housing_len + 80, 0, 0]) position_dial();

// ─── Notes ────────────────────────────────────────────────────────────────────
// COMMERCIAL ALTERNATIVE: SDP/SI L1Z6-LGF10-200 worm gear set (brass, 10:1)
// For highest Q, use commercial brass worm gear — reduces backlash vs. printed.
// Install 608ZZ bearings in worm shaft seats (press fit).
// Install 625ZZ bearings in leadscrew seats.
// Pack bearings with light machine oil (not grease — avoid RF contamination).
