/**
 * wire_guide_carriage.scad — TM-CWND-001 Rev A
 * Wire Guide Traverse Carriage
 *
 * Slides on two 12mm smooth rods (60mm center spacing).
 * Driven by T8 leadscrew (8mm, 2mm pitch).
 * Carries the wire eye that guides wire onto the coil former.
 *
 * Features:
 *   - Anti-backlash T8 nut pocket (spring-loaded split nut or commercial)
 *   - LM12UU linear bearing pockets (or printed PTFE bushings)
 *   - Adjustable wire eye height (±10mm from nominal)
 *   - V-pointer for visual turn position
 *   - Wire optical sensor bracket (for wire break sensor mounting)
 *   - Magnetic position indicator strip slot
 *
 * Print settings:
 *   Material: PETG (good bearing surface against steel rods)
 *   Infill: 60% gyroid
 *   Wall: 4 perimeters
 *   Layer: 0.15mm (bearing bore accuracy)
 *   Supports: Yes under bearing bores
 *
 * Print envelope: 120×100×80mm — fits in 200mm cube
 *
 * Hardware:
 *   LM12UU bearings (4×) — 12mm bore, 21mm OD, 30mm length
 *   OR: printed PTFE bushing inserts (self-lubricated)
 *   T8 anti-backlash nut (commercial) — Tr8×2 or T8×2
 *   Wire guide: 2.5mm bore ceramic eyelet, or print PTFE insert
 *   M3 set screw × 4 (bearing retention)
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// Rail rod geometry (must match base_plate.scad)
rail_dia        = 12.0;
rail_spacing    = 60.0;  // Center-to-center Y
lm12uu_od       = 21.3;  // LM12UU OD + 0.1mm press clearance
lm12uu_len      = 30.0;  // LM12UU length

// Leadscrew (T8, 2mm pitch)
lead_dia        = 8.4;   // Through bore (anti-backlash nut threads into this)
lead_nut_od     = 22.0;  // Anti-backlash nut OD flange
lead_nut_h      = 20.0;  // Nut height (with spring)
lead_nut_bolt_sp= 12.0;  // Nut flange bolt hole spacing

// Carriage body
body_w          = 90.0;  // Width (Y direction, spans rail centers + margin)
body_d          = 50.0;  // Depth (X direction, along travel)
body_h          = 70.0;  // Height (Z direction)
body_corner     = 4.0;

// Wire guide arm (extends toward coil former)
arm_len         = 35.0;  // Extension toward spindle
arm_w           = 15.0;
arm_h           = 8.0;
arm_z           = 50.0;  // Height of wire guide above base (nominal)

// Wire eye
eye_dia         = 2.6;   // Wire passage (for AWG10 = 2.6mm dia wire)
eye_boss_od     = 10.0;
eye_slot_h      = 20.0;  // Vertical slot for height adjustment

// V-pointer
ptr_len         = 12.0;
ptr_w           = 2.0;
ptr_h           = 15.0;

// Optical sensor bracket (for wire break sensor)
sens_bracket_w  = 20.0;
sens_bracket_h  = 25.0;
sens_gap        = 6.0;   // Gap in bracket for wire to pass (optical slot)
sens_z          = arm_z - 10;  // Height of optical sensor

// Position indicator strip slot
strip_w         = 12.0;  // Magnetic strip width
strip_d         = 2.0;   // Strip thickness
strip_z         = 5.0;   // Height from bottom

// M3 retention screw holes (for bearing retention)
ret_screw_dia   = 3.3;

// Printed bushing (alternative to LM12UU)
bushing_od      = lm12uu_od;
bushing_len     = 20.0;  // Shorter than LM12UU
bushing_wall    = 2.0;

// ─── Modules ──────────────────────────────────────────────────────────────────

module carriage_body() {
    difference() {
        union() {
            // Main body
            hull() {
                for (x=[body_corner, body_d-body_corner])
                for (y=[body_corner, body_w-body_corner])
                    translate([x, y, 0])
                        cylinder(h=body_h, r=body_corner, $fn=16);
            }

            // Wire guide arm (extends in X direction toward spindle)
            translate([body_d, body_w/2 - arm_w/2, arm_z - arm_h/2])
                cube([arm_len, arm_w, arm_h]);

            // Eye boss at end of arm
            translate([body_d + arm_len, body_w/2, arm_z])
                cylinder(h=arm_h, d=eye_boss_od, $fn=32, center=true);

            // V-pointer (visual indicator on front face)
            translate([body_d + 2, body_w/2 - ptr_w/2, 2])
                cube([ptr_len, ptr_w, ptr_h]);
            // V-tip
            translate([body_d + ptr_len + 1, body_w/2, 2])
                cylinder(h=ptr_h, r=1.5, $fn=8);

            // Optical sensor bracket (back of body, extends down)
            translate([-sens_bracket_w - 2, body_w/2 - sens_bracket_w/2, sens_z])
                cube([sens_bracket_w, sens_bracket_w, sens_bracket_h]);
        }

        // Front rail bore (LM12UU or printed bushing)
        translate([body_d/2, rail_spacing/2 + (body_w/2 - rail_spacing/2)/2, -0.5])
            cylinder(h=body_h+1, d=lm12uu_od, $fn=32);
        // Set screw for front bearing
        translate([body_d/2, rail_spacing/2 + (body_w/2 - rail_spacing/2)/2 + lm12uu_od/2 + 1, body_h/2])
        rotate([90,0,0])
            cylinder(h=lm12uu_od+2, d=ret_screw_dia, $fn=16);

        // Rear rail bore
        translate([body_d/2, body_w - (rail_spacing/2 + (body_w/2 - rail_spacing/2)/2), -0.5])
            cylinder(h=body_h+1, d=lm12uu_od, $fn=32);
        translate([body_d/2, body_w - (rail_spacing/2 + (body_w/2 - rail_spacing/2)/2) - lm12uu_od/2 - 1, body_h/2])
        rotate([90,0,0])
            cylinder(h=lm12uu_od+2, d=ret_screw_dia, $fn=16);

        // Leadscrew nut pocket (center bottom)
        translate([body_d/2, body_w/2, -0.5])
            cylinder(h=lead_nut_h+0.5, d=lead_nut_od+0.3, $fn=32);
        // Leadscrew through bore (full height)
        translate([body_d/2, body_w/2, -0.5])
            cylinder(h=body_h+1, d=lead_dia, $fn=32);
        // Anti-backlash nut bolt holes
        for (a=[0,180])
            rotate([0,0,a])
            translate([body_d/2 + lead_nut_bolt_sp/2, body_w/2, -0.5])
                cylinder(h=lead_nut_h+0.5, d=3.3, $fn=16);

        // Wire eye bore (with adjustment slot)
        translate([body_d + arm_len, body_w/2, arm_z - eye_slot_h/2 - 0.5])
            cylinder(h=eye_slot_h+1, d=eye_dia, $fn=16);
        // Height adjustment slot (M3 clamping bolt through arm)
        translate([body_d + arm_len/2, body_w/2, arm_z])
        rotate([0,90,0])
            cylinder(h=arm_len+1, d=3.3, $fn=16);

        // Position strip slot
        translate([body_d/2 - strip_w/2, -0.5, strip_z])
            cube([strip_w, strip_d+0.5, body_h/2]);

        // Optical sensor slot (gap in sensor bracket)
        translate([-sens_bracket_w-3, body_w/2 - sens_gap/2, sens_z + sens_bracket_h/2])
            cube([sens_bracket_w+2, sens_gap, sens_bracket_h/2+1]);

        // Weight relief pockets
        translate([10, 15, 20])
            cube([body_d-20, body_w-30, body_h-30]);
    }
}

module printed_bushing() {
    // PTFE or printed bushing (alternative to LM12UU)
    // Self-lubricating with PTFE filament; print in 0.1mm layers
    difference() {
        cylinder(h=bushing_len, d=bushing_od, $fn=32);
        translate([0,0,-0.5])
            cylinder(h=bushing_len+1, d=rail_dia+0.15, $fn=32);
        // Oil groove (retain small amount of lubricant)
        translate([0, 0, bushing_len/2])
            rotate_extrude($fn=64)
            translate([rail_dia/2 + bushing_wall/2, 0, 0])
                circle(r=0.5, $fn=8);
    }
}

module wire_eye_insert() {
    // Drop-in ceramic-style eye insert
    // Print in PETG, 100% infill, 0.1mm layers for smooth bore
    h = arm_h + 4;
    difference() {
        cylinder(h=h, d=eye_boss_od-0.4, $fn=32);
        // Bore with chamfers
        translate([0,0,-0.5]) cylinder(h=h+1, d=eye_dia, $fn=16);
        translate([0,0,h-1.5]) cylinder(h=2, d1=eye_dia, d2=eye_dia+2, $fn=16);
        translate([0,0,-0.5]) cylinder(h=2, d1=eye_dia+2, d2=eye_dia, $fn=16);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 32;
print_part = "carriage";  // "carriage" | "bushing" | "eye" | "all"

if (print_part == "carriage") {
    color("lightgray") carriage_body();
} else if (print_part == "bushing") {
    for (i=[0:3])
        translate([i * 25, 0, 0])
            color("lightblue") printed_bushing();
} else if (print_part == "eye") {
    color("white") wire_eye_insert();
} else {
    color("lightgray", 0.8) carriage_body();
    // Show bearings in position
    for (y=[rail_spacing/2 + (90/2 - rail_spacing/2)/2,
            90 - (rail_spacing/2 + (90/2 - rail_spacing/2)/2)])
        color("silver", 0.5)
        translate([45, y, 0])
            cylinder(h=lm12uu_len, d=lm12uu_od-0.3, $fn=32);
}
