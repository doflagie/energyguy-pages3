/**
 * motor_mount.scad — TM-ROLLER-001 Rev A
 * NEMA 17 Stepper Motor Mount Bracket
 *
 * Mounts NEMA 17 stepper motor in line with worm gear input shaft.
 * Flexible coupler (jaw coupler) connects motor shaft to worm input.
 * Adjustable slot allows belt tension compensation and coupler alignment.
 *
 * NEMA 17 standard:
 *   Face: 42.3mm × 42.3mm
 *   Bolt pattern: 31.0mm × 31.0mm (M3, 4-bolt)
 *   Pilot boss: 22mm diameter, 2mm protrusion
 *   Shaft: 5mm diameter, 24mm long
 *
 * Print settings:
 *   Material:    PETG
 *   Infill:      60%
 *   Wall:        4 perimeters
 *   Layer:       0.2mm
 *   Orientation: Flat face on bed
 *
 * Print envelope: 75mm × 75mm × 30mm
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// NEMA 17 dimensions
nema17_face     = 42.3;   // Face dimension
nema17_bolt_sp  = 31.0;   // Bolt hole spacing (center to center)
nema17_pilot    = 22.2;   // Pilot boss clearance hole
nema17_shaft    = 5.2;    // Shaft clearance

// Mount plate
plate_thick     = 6.0;    // Plate thickness
plate_w         = nema17_face + 12;  // 54.3mm
plate_h         = nema17_face + 12;

// Adjustment slots (for motor alignment)
slot_len        = 4.0;    // Slot extra length (motor can shift ±2mm)
slot_w          = 3.4;    // M3 bolt slot width

// Coupler guard
guard_od        = 26.0;   // Flexible coupler OD + 2mm clearance
guard_len       = 20.0;   // Guard length
guard_thick     = 2.5;

// Standoff (motor face to gear housing face)
standoff_h      = 15.0;   // Height of standoff pillars
standoff_d      = 8.0;    // Standoff pillar diameter
standoff_bore   = 3.3;    // M3 clearance

// Gear housing attachment holes (M3, on gear housing face)
// Match housing corner bolt positions from gear_mechanism.scad
gh_bolt_x       = 38.0;   // Half-spacing in X (housing_len/2 - 8 ≈ 42mm? adjust)
gh_bolt_y       = 26.0;   // Half-spacing in Y

// ─── Modules ──────────────────────────────────────────────────────────────────

module motor_mount_plate() {
    difference() {
        // Plate with rounded corners
        hull() {
            for (x = [-1, 1]) for (y = [-1, 1]) {
                translate([x * (plate_w/2 - 5), y * (plate_h/2 - 5), 0])
                    cylinder(h = plate_thick, r = 5, $fn = 32);
            }
        }

        // Pilot boss clearance
        translate([0, 0, -0.5])
            cylinder(h = plate_thick + 1, d = nema17_pilot, $fn = 64);

        // Shaft clearance
        translate([0, 0, -0.5])
            cylinder(h = plate_thick + 1, d = nema17_shaft, $fn = 32);

        // Motor bolt slots (M3, 4 positions with adjustment slots)
        for (x = [-1, 1]) for (y = [-1, 1]) {
            translate([x * nema17_bolt_sp/2, y * nema17_bolt_sp/2, -0.5]) {
                // Circular base
                cylinder(h = plate_thick + 1, d = slot_w, $fn = 16);
                // Radial slot for alignment
                translate([0, 0, 0])
                hull() {
                    cylinder(h = plate_thick + 1, d = slot_w, $fn = 16);
                    translate([x * slot_len, 0, 0])
                        cylinder(h = plate_thick + 1, d = slot_w, $fn = 16);
                }
            }
        }

        // Gear housing attachment holes (align to housing screw positions)
        for (x = [-1, 1]) for (y = [-1, 1]) {
            translate([x * gh_bolt_x/2, y * gh_bolt_y/2, -0.5])
                cylinder(h = plate_thick + 1, d = 3.3, $fn = 16);
        }
    }
}

module standoff_pillars() {
    // Four standoff pillars from plate to gear housing
    for (x = [-1, 1]) for (y = [-1, 1]) {
        translate([x * gh_bolt_x/2, y * gh_bolt_y/2, plate_thick]) {
            difference() {
                cylinder(h = standoff_h, d = standoff_d, $fn = 16);
                translate([0, 0, -0.5])
                    cylinder(h = standoff_h + 1, d = standoff_bore, $fn = 16);
            }
        }
    }
}

module coupler_guard() {
    // Half-cylinder guard around flexible jaw coupler
    translate([0, 0, plate_thick + standoff_h])
    difference() {
        cylinder(h = guard_len, d = guard_od + guard_thick * 2, $fn = 64);
        cylinder(h = guard_len + 1, d = guard_od, $fn = 64);
        // Access slot (half-open for cable routing / inspection)
        translate([-guard_od, 0, -0.5])
            cube([guard_od, guard_od * 2 + guard_thick * 2, guard_len + 1]);
    }
}

module cable_clip() {
    // Motor cable strain relief clip
    clip_w = 12;
    clip_h = 8;
    clip_d = 6;
    cable_d = 5;
    translate([plate_w/2 - 8, -plate_h/2 - 2, 0])
    difference() {
        cube([clip_w, clip_d, clip_h]);
        translate([clip_w/2, -0.5, clip_h/2])
        rotate([-90, 0, 0])
            cylinder(h = clip_d + 1, d = cable_d, $fn = 16);
        // M2.5 screw for strap
        translate([clip_w/2, clip_d + 0.5, clip_h * 0.6])
        rotate([90, 0, 0])
            cylinder(h = clip_d + 1, d = 2.7, $fn = 16);
    }
}

// ─── Full Motor Bracket Assembly ──────────────────────────────────────────────

module motor_bracket_assembly() {
    union() {
        color("lightgreen", 0.8) motor_mount_plate();
        color("lightgreen", 0.8) standoff_pillars();
        color("lightgreen", 0.6) coupler_guard();
        color("lightgreen", 0.8) cable_clip();
    }
}

// ─── Shaft Coupler (jaw type, for 5mm motor shaft to 5mm worm input) ─────────

module jaw_coupler_half(shaft_bore, set_screw_depth = 8) {
    // One half of a jaw flexible coupler
    // Two halves + elastomer spider = complete coupler
    outer_d = 20;
    hub_len = 15;
    flange_len = 5;
    jaw_h = 8;
    n_jaws = 3;

    difference() {
        union() {
            // Hub
            cylinder(h = hub_len, d = outer_d, $fn = 32);
            // Jaw flange
            translate([0, 0, hub_len])
            for (i = [0 : n_jaws - 1]) {
                rotate([0, 0, i * 360/n_jaws])
                translate([outer_d/4, 0, 0])
                    cylinder(h = jaw_h, d = outer_d/3, $fn = 16);
            }
        }
        // Bore
        translate([0, 0, -0.5])
            cylinder(h = hub_len + jaw_h + 1, d = shaft_bore + 0.2, $fn = 32);
        // Set screw hole (M3)
        translate([outer_d/2 - 1, 0, set_screw_depth])
        rotate([90, 0, 0])
            cylinder(h = outer_d, d = 2.5, $fn = 16, center = true);
        // Key flat
        translate([shaft_bore/2 - 0.5, -1, -0.5])
            cube([1.0, 2.0, hub_len + jaw_h + 1]);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 64;

// Full bracket
motor_bracket_assembly();

// Print coupler halves separately (one per motor + gear input)
translate([70, 0, 0])  jaw_coupler_half(5.0);       // Motor side (5mm shaft)
translate([95, 0, 0])  jaw_coupler_half(5.0);       // Worm input side (5mm)

// ─── Notes ────────────────────────────────────────────────────────────────────
// 1. M3×8 SHCS bolts secure motor to plate (through adjustment slots)
// 2. M3×(standoff_h + plate_thick) bolts through standoffs to gear housing
// 3. Flexible jaw coupler: 20mm OD, 5mm/5mm bores, 95A polyurethane spider
//    (e.g., Lovejoy L-050 or printed jaw halves with commercial spider)
// 4. Apply thread-lock to set screws after alignment verified
// 5. Check shaft alignment: <0.1mm offset, <0.5° angle for long gear life
// 6. Motor cable: route through cable clip, provide 50mm service loop
// 7. Shield motor cable (twisted pair recommended for each coil winding)
