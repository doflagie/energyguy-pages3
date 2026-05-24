/**
 * chuck_mechanism.scad — TM-CWND-001 Rev A
 * Adjustable Chuck for Coil Winder Spindle
 *
 * Three modules (print separately):
 *   1. chuck_body()     — outer ring + jaw tracks, mounts to spindle
 *   2. chuck_jaw()      — print 3×, adjustable, M5 grub screw clamp
 *   3. chuck_backplate()— secures to spindle shaft, flanged
 *
 * Range: 6.35mm (1/4") to 102mm (4") coil former diameter
 *
 * Design: sliding T-slot jaw, not scroll chuck (simpler 3D print)
 * Jaw extension: 0 (fully retracted) to 40mm (fully extended)
 * At zero extension: grips 6mm forms (using center bore rubber insert)
 * At max extension: grips 102mm forms (jaws at outer track limit)
 *
 * Print settings:
 *   Material: PETG (toughness, dimensional stability)
 *   Infill: 80% for chuck body, 100% for jaws
 *   Wall: 4 perimeters
 *   Layer: 0.15mm (jaw track accuracy)
 *
 * Print envelope:
 *   chuck_body: 160mm × 160mm × 35mm — fits in 200mm cube
 *   chuck_jaw:  60mm × 25mm × 20mm — print 3 at once
 *   backplate:  80mm × 80mm × 20mm
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

chuck_od         = 150.0;  // Outer diameter of chuck body
chuck_h          = 32.0;   // Total height
chuck_id_bore    = 20.0;   // Center through-bore (spindle shaft pass-through)

// Jaw T-slot track geometry
jaw_count        = 3;
jaw_track_w      = 14.0;   // T-slot width at base
jaw_track_h      = 10.0;   // T-slot depth
jaw_track_len    = 50.0;   // Radial travel length
jaw_t_w          = 18.0;   // T-slot cap width (wider than base)
jaw_t_h          = 5.0;    // T-slot cap height
jaw_start_r      = 10.0;   // Track center radius at minimum position (jaw retracted)

// Jaw body
jaw_body_w       = 12.0;   // Jaw body width (fits in T-slot)
jaw_body_h       = jaw_track_h - 0.4;  // Height (sliding fit)
jaw_body_l       = 55.0;   // Jaw body length (extends inward + outward)
jaw_grip_w       = 20.0;   // Jaw face width (contacts former)
jaw_grip_h       = 20.0;   // Jaw face height
jaw_grip_step    = 5.0;    // Step from jaw body to grip face
jaw_grip_groove  = 2.0;    // Grip groove depth (texture)
jaw_setscrew_dia = 5.0;    // M5 set screw through jaw body

// Backplate
bp_od            = 80.0;
bp_h             = 18.0;
bp_bore          = 14.2;   // 14mm shaft + clearance
bp_bolt_sp       = 60.0;   // Bolt pattern (4× M4 to chuck body)
bp_key_w         = 4.0;    // Key flat width
bp_key_d         = 2.0;    // Key flat depth

// Chuck-to-backplate bolt pattern
c2bp_bolt_sp     = 70.0;
c2bp_bolt_dia    = 4.3;    // M4 clearance

// ─── Chuck Body ───────────────────────────────────────────────────────────────

module jaw_track_slot(angle) {
    // Single T-slot track at given angle
    rotate([0,0,angle])
    translate([chuck_od/2 - jaw_track_len - jaw_start_r, 0, 0])
    rotate([0,90,0]) {
        // T-slot cross-section extruded radially
        linear_extrude(jaw_track_len + chuck_od/2)
        polygon([
            [-jaw_t_w/2, 0],
            [-jaw_t_w/2, jaw_t_h],
            [-jaw_track_w/2, jaw_t_h],
            [-jaw_track_w/2, jaw_track_h],
            [ jaw_track_w/2, jaw_track_h],
            [ jaw_track_w/2, jaw_t_h],
            [ jaw_t_w/2, jaw_t_h],
            [ jaw_t_w/2, 0],
        ]);
    }
}

module chuck_body() {
    difference() {
        // Outer cylinder
        cylinder(h=chuck_h, d=chuck_od, $fn=128);

        // Center bore
        translate([0,0,-0.5])
            cylinder(h=chuck_h+1, d=chuck_id_bore, $fn=32);

        // Three jaw tracks at 120° intervals
        for (a=[0, 120, 240])
            translate([0, 0, chuck_h - jaw_track_h - 2])
                jaw_track_slot(a);

        // Chuck-to-backplate bolts (from front face)
        for (a=[60, 180, 300])
            rotate([0,0,a])
            translate([c2bp_bolt_sp/2, 0, -0.5])
                cylinder(h=chuck_h+1, d=c2bp_bolt_dia, $fn=16);

        // Weight relief pockets (back face)
        for (a=[0, 60, 120, 180, 240, 300])
            rotate([0,0,a])
            translate([chuck_od/2 - 20, 0, 0])
            rotate([90,0,0])
                cylinder(h=8, r=12, $fn=32, center=true);

        // Front face label recess
        translate([0,0,chuck_h-1.5])
            cylinder(h=2, d=30, $fn=64);
    }
}

// ─── Chuck Jaw ────────────────────────────────────────────────────────────────

module chuck_jaw() {
    difference() {
        union() {
            // T-slider body (fits in chuck T-slot)
            translate([0, -jaw_body_w/2, 0])
                cube([jaw_body_l - jaw_grip_step, jaw_body_w, jaw_body_h]);

            // T-cap (wider, engages T-slot)
            translate([0, -jaw_t_w/2 + 0.2, 0])
                cube([jaw_body_l - jaw_grip_step - 5, jaw_t_w - 0.4, jaw_t_h - 0.2]);

            // Grip face (extends inward toward chuck center)
            translate([jaw_body_l - jaw_grip_step, -jaw_grip_w/2, 0])
                cube([jaw_grip_step, jaw_grip_w, jaw_grip_h]);
        }
        // M5 clamp set screw (from side, locks jaw in track)
        translate([jaw_body_l/2, -jaw_body_w/2 - 1, jaw_body_h/2])
        rotate([-90,0,0])
            cylinder(h=jaw_body_w+2, d=jaw_setscrew_dia, $fn=16);

        // Grip face grooves (horizontal, texture for grip)
        for (z=[3, 8, 13])
            translate([jaw_body_l - jaw_grip_step - 0.5, -jaw_grip_w/2 - 0.5, z])
                cube([jaw_grip_step + 1, jaw_grip_w + 1, jaw_grip_groove]);
    }
}

// ─── Backplate ────────────────────────────────────────────────────────────────

module chuck_backplate() {
    // Mounts to spindle shaft, chuck body bolts to front face
    difference() {
        union() {
            cylinder(h=bp_h, d=bp_od, $fn=64);
            // Spindle shaft collar (extends behind main disc)
            cylinder(h=bp_h + 15, d=30, $fn=32);
        }
        // Shaft bore
        translate([0,0,-0.5])
            cylinder(h=bp_h+16, d=bp_bore, $fn=32);
        // Key flat (D-flat on shaft)
        translate([bp_bore/2 - bp_key_d, -bp_bore, -0.5])
            cube([bp_key_d+1, bp_bore*2, bp_h+16]);
        // Shaft set screw (M5 from side)
        translate([30/2+1, 0, bp_h+7])
        rotate([0,-90,0])
            cylinder(h=30, d=5.0, $fn=16);
        // Chuck bolt through holes
        for (a=[60,180,300])
            rotate([0,0,a])
            translate([c2bp_bolt_sp/2, 0, -0.5])
                cylinder(h=bp_h+1, d=c2bp_bolt_dia, $fn=16);
        // Weight relief
        for (a=[0,90,180,270])
            rotate([0,0,a])
            translate([bp_od/2-12, 0, bp_h/2])
            rotate([90,0,0])
                cylinder(h=8, r=7, $fn=16, center=true);
    }
}

// ─── Small Form Insert (rubber sleeve for <25mm forms) ────────────────────────

module small_form_insert() {
    // Press-fit rubber or foam insert for gripping small coil formers
    // Design: cylindrical, tapered ID
    insert_od = chuck_id_bore - 0.4;
    insert_id_top = 6.0;
    insert_id_bot = 20.0;
    insert_h  = 25.0;
    difference() {
        cylinder(h=insert_h, d=insert_od, $fn=32);
        cylinder(h=insert_h+1, d1=insert_id_bot, d2=insert_id_top, $fn=32);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 64;
print_part = "body";  // "body" | "jaw" | "backplate" | "insert" | "all"

if (print_part == "body") {
    color("lightgray") chuck_body();
} else if (print_part == "jaw") {
    // Print 3 at once
    for (i=[0:2])
        translate([i*65, 0, 0])
            color("orange") chuck_jaw();
} else if (print_part == "backplate") {
    color("lightblue") chuck_backplate();
} else if (print_part == "insert") {
    color("gray", 0.5) small_form_insert();
} else {
    // Assembly view
    color("lightgray", 0.8) chuck_body();
    color("lightblue", 0.8)
    translate([0, 0, -bp_h])
        chuck_backplate();
    for (a=[0, 120, 240])
        rotate([0,0,a])
        translate([chuck_od/2 - jaw_track_len/2 - jaw_start_r, 0,
                   chuck_h - jaw_track_h - 2])
            color("orange") chuck_jaw();
}

// ─── Notes ────────────────────────────────────────────────────────────────────
// ASSEMBLY:
// 1. Press backplate onto spindle shaft (key flat aligned), tighten M5 set screw
// 2. Insert jaws into T-slots from outer edge, slide inward until finger-tight
// 3. Tighten jaw M5 grub screws to lock at current position
// 4. For coil formers < 25mm OD: insert small_form_insert into center bore
//    and pinch former in place
// 5. For formers 25–102mm: extend jaws to contact, tighten grub screws
//
// BALANCE: Heavy-gauge wire wound at high RPM may unbalance the chuck.
// Maximum safe RPM with chuck: ~150 RPM (limited by bearing vibration)
// For >100 RPM: use lighter chuck or dynamically balance jaws.
