/**
 * bird_body.scad — TM-PWR-ENCL-BIRD-001 Rev A
 * Bird Model 43-style Thruline Wattmeter Body
 * SO-239 connectors, slug slot, analog meter window
 * Print volume: 200×200×200mm — prints as two halves
 */

// ─── Parameters ──────────────────────────────────────────────────────────────
body_l   = 155;    // Total body length (mm)
body_od  = 50;     // Body outer diameter
body_id  = 35;     // Body inner diameter (coax region)
wall     = 7.5;    // Wall thickness floor (body_od - body_id)/2

// Coax center conductor bore
center_d = 5.0;    // Center conductor bore diameter (SO-239 pin is 4.6mm)

// Slug slot
slug_od   = 25.4;  // Slug outer diameter (1.000")
slug_l    = 25.4;  // Slug slot depth
slug_x    = body_l / 2;  // Centered along body length

// SO-239 connectors (both ends)
so239_od      = 22.5;  // Body OD for press fit
so239_pin_d   = 4.8;   // Center pin hole
so239_thread_d= 21.0;  // Thread bore ID (5/8-24 UNS)
so239_depth   = 18;    // Depth of connector counterbore

// Meter window (top face)
meter_w  = 72;    // Meter face width
meter_h  = 32;    // Meter face height
meter_r  = 4;     // Corner radius
meter_x  = body_l / 2;  // Centered (over slug)
meter_z  = body_od / 2; // On top surface

// Meter lens recess (2mm deep)
lens_depth   = 2;
lens_w       = meter_w + 2;
lens_h       = meter_h + 2;

// Banana jack hole (slug output, bottom)
banana_d = 4.2;   // 4mm banana hole
banana_x = body_l / 2;
banana_y = 0;     // Bottom center

// Internal spring contact for slug pin
spring_d = 3.0;   // Spring pin bore
spring_h = 8;     // Spring well depth

// Split line (body halves split at center horizontal plane)
split_y = 0;  // Y=0 is center; body half at +Y, bottom half at -Y

// Mounting feet (bottom)
foot_w   = 20;
foot_h   = 5;
foot_d   = 10;
foot_screw_d = 3.3;  // M3 clearance

// ─── Modules ──────────────────────────────────────────────────────────────────

module so239_counterbore(depth) {
    // Outer thread bore
    cylinder(d=so239_thread_d, h=depth, $fn=40);
    // Center pin clearance
    cylinder(d=so239_pin_d, h=depth + 5, $fn=20);
}

module meter_window() {
    // Rectangular aperture with rounded corners
    linear_extrude(body_od)
        offset(r=meter_r)
            square([meter_w - 2*meter_r, meter_h - 2*meter_r], center=true);
}

module slug_slot() {
    // Cylindrical bore for slug, transverse to RF axis
    rotate([0, 90, 0])
        cylinder(d=slug_od + 0.4, h=slug_l, center=true, $fn=60);
    // O-ring groove
    rotate([0, 90, 0])
        difference() {
            cylinder(d=slug_od + 0.4 + 3, h=1.5, center=true, $fn=60);
            cylinder(d=slug_od + 0.4 - 0.1, h=2, center=true, $fn=60);
        }
}

module center_conductor_bore() {
    cylinder(d=center_d, h=body_l + 4, center=true, $fn=20);
}

module banana_jack_hole() {
    cylinder(d=banana_d, h=body_od + 4, center=true, $fn=20);
}

module spring_well() {
    translate([0, 0, -body_od/2 + 0])
        cylinder(d=spring_d, h=spring_h, $fn=16);
}

module body_cylinder() {
    // Main body: cylinder with flat top (D-cross section would be complex;
    // use cylinder with flat top face for meter window)
    difference() {
        union() {
            cylinder(d=body_od, h=body_l, $fn=80);
        }
        // Hollow interior (coax channel is body_id)
        translate([0, 0, so239_depth])
            cylinder(d=body_id, h=body_l - 2*so239_depth, $fn=60);
    }
}

// ─── TOP HALF OF BODY ─────────────────────────────────────────────────────────
module body_top() {
    difference() {
        // Top half of cylinder
        intersection() {
            body_cylinder();
            translate([0, 0, body_l/2])
                cube([body_od + 2, body_od + 2, body_l + 2], center=true);
        }

        // Center conductor bore (RF through-line)
        translate([0, 0, body_l/2])
            rotate([90, 0, 90])
                center_conductor_bore();

        // Left SO-239 bore
        translate([so239_depth/2, 0, body_l/2])
            rotate([90, 0, 90])
                so239_counterbore(so239_depth);

        // Right SO-239 bore
        translate([body_l - so239_depth/2, 0, body_l/2])
            rotate([90, 0, -90])
                so239_counterbore(so239_depth);

        // Slug slot (transverse, centered)
        translate([slug_x, 0, body_l/2])
            slug_slot();

        // Meter window (top face, over slug slot)
        translate([slug_x, 0, body_l/2 + body_od/2 - lens_depth])
            linear_extrude(lens_depth + 1)
                offset(r=meter_r + 0.5)
                    square([lens_w - 2*(meter_r+0.5), lens_h - 2*(meter_r+0.5)], center=true);
        translate([slug_x, 0, body_l/2 + body_od/2 - body_od])
            meter_window();

        // Banana jack hole (bottom of slug slot, connects to slug pin)
        translate([slug_x, -body_od/2 - 0.1, body_l/2])
            rotate([-90, 0, 0])
                banana_jack_hole();

        // Spring well (inside, receives slug pin)
        translate([slug_x, -body_od/2 + spring_h/2, body_l/2])
            rotate([90, 0, 0])
                spring_well();

        // Split line flat (mate surface)
        translate([body_od/2, 0, body_l/2])
            cube([body_od + 2, 1, body_l + 2], center=true);
    }

    // Alignment pins (2× on split face)
    for (px = [30, body_l - 30])
        translate([px, 0, body_l/2])
            cylinder(d=2.9, h=3, $fn=16);  // 3mm alignment pin, male
}

// ─── BOTTOM HALF ──────────────────────────────────────────────────────────────
module body_bottom() {
    difference() {
        // Bottom half of cylinder
        intersection() {
            body_cylinder();
            translate([0, -(body_od + 2)/2, body_l/2])
                cube([body_od + 2, body_od + 2, body_l + 2], center=true);
        }

        // Same bores as top
        translate([0, 0, body_l/2])
            rotate([90, 0, 90])
                center_conductor_bore();
        translate([so239_depth/2, 0, body_l/2])
            rotate([90, 0, 90])
                so239_counterbore(so239_depth);
        translate([body_l - so239_depth/2, 0, body_l/2])
            rotate([90, 0, -90])
                so239_counterbore(so239_depth);
        translate([slug_x, 0, body_l/2])
            slug_slot();
        translate([slug_x, -body_od/2 - 0.1, body_l/2])
            rotate([-90, 0, 0])
                banana_jack_hole();
        translate([slug_x, -body_od/2 + spring_h/2, body_l/2])
            rotate([90, 0, 0])
                spring_well();

        // Alignment pin holes (female, 0.3mm clearance)
        for (px = [30, body_l - 30])
            translate([px, -0.1, body_l/2])
                cylinder(d=3.2, h=3.5, $fn=16);

        // M4 screw holes for body halves (4 screws)
        for (sx = [20, body_l/2, body_l - 20])
            translate([sx, -body_od/4, body_l/2])
                cylinder(d=4.3, h=body_od, center=true, $fn=16);
    }

    // Mounting feet (bottom)
    translate([body_l * 0.25, -body_od/2 - foot_h/2, body_l/2])
        difference() {
            cube([foot_w, foot_h, foot_d], center=true);
            cylinder(d=foot_screw_d, h=foot_h + 2, center=true, $fn=16);
        }
    translate([body_l * 0.75, -body_od/2 - foot_h/2, body_l/2])
        difference() {
            cube([foot_w, foot_h, foot_d], center=true);
            cylinder(d=foot_screw_d, h=foot_h + 2, center=true, $fn=16);
        }
}

// ─── METER BEZEL ──────────────────────────────────────────────────────────────
module meter_bezel() {
    // Decorative bezel that holds meter face / lens
    difference() {
        hull() {
            for (dx = [-(meter_w+6)/2, (meter_w+6)/2])
                for (dy = [-(meter_h+6)/2, (meter_h+6)/2])
                    translate([dx, dy, 0]) cylinder(r=3, h=3, $fn=20);
        }
        // Aperture
        translate([0, 0, -0.1])
            linear_extrude(4)
                offset(r=meter_r)
                    square([meter_w - 2*meter_r, meter_h - 2*meter_r], center=true);
        // Glass recess (1.5mm deep for 2mm acrylic lens)
        translate([0, 0, 1.5])
            linear_extrude(3)
                offset(r=meter_r + 0.5)
                    square([meter_w + 1 - 2*(meter_r+0.5), meter_h + 1 - 2*(meter_r+0.5)], center=true);
    }
}

// ─── RENDER ───────────────────────────────────────────────────────────────────
body_top();
translate([0, body_od + 10, 0]) body_bottom();
translate([slug_x, body_od + 60, body_l/2]) meter_bezel();

// ─── NOTES ────────────────────────────────────────────────────────────────────
// Print settings:
//   Top/bottom halves: PLA+ or PETG, 3 walls, 25% gyroid infill
//   Layer: 0.2mm
//   Supports: required for SO-239 counterbores (bottom-half connectors)
//   No supports needed for top half
//
// Assembly:
//   (1) Install SO-239 connectors in both ends (press or thread-fit)
//   (2) Install spring contact pin in spring well (repurpose SO-239 pin or
//       use Pogo pin, 2.8mm OD, spring force 0.5N min)
//   (3) Solder banana jack to spring contact and body outer GND contact
//   (4) Join top/bottom halves with 4× M4×20 hex cap screws
//   (5) Insert slug element — rotate to select FWD or REF
//   (6) Glue 2mm clear acrylic lens into meter bezel recess
//   (7) Install panel meter (50µA FSD) behind bezel
