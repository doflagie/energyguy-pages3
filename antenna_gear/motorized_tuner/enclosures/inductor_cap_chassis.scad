// ============================================================
// INDUCTOR/CAP CHASSIS BRACKET
// Document: TM-HFTN-CHS-001 Rev A
// Project:  Motorized HF Antenna Tuner
// Author:   Merv Martin
// Date:     2026-05-02
// ============================================================
// Mounts roller inductor motor (NEMA 17) + variable cap motor
// side-by-side with connecting center shelf/bridge.
// Overall: ~190×100×80mm — fits 200×200×200mm print envelope.
// ============================================================
// PRINT SETTINGS: PETG, 40% gyroid infill, 4 perimeters,
//                 0.20mm layer height, no supports needed.
//                 Brim recommended for motor mount plates.
// ============================================================

// --- PRINT PART SELECTOR ---
// "chassis" | "left_mount" | "right_mount" | "all"
print_part = "all";

// --- GLOBAL DIMENSIONS ---
chassis_x = 190;   // total length
chassis_y = 100;   // total depth
chassis_z = 80;    // total height
wall      = 3.5;
floor_t   = 4.0;

// NEMA 17 face geometry
nema17_side    = 42.3;    // body square side
nema17_bolt_r  = 47.5/2; // bolt circle radius (centre-to-centre across diagonal)
nema17_shaft_d = 22.0;   // shaft clearance hole diameter (5mm shaft + bearing)
nema17_pilot_d = 22.0;   // pilot boss clearance hole

// Motor mount plate
mount_plate = 50.0;  // square side
mount_t     = 5.0;   // plate thickness

// Horizontal rail slot geometry
rail_slot_l = 30.0;  // slot length (±15mm travel)
rail_slot_w = 4.5;   // M4 clearance slot width
rail_count  = 4;

// ============================================================
// UTILITIES
// ============================================================

module m3_hole(h=10) { cylinder(h=h, d=3.3, $fn=16, center=false); }
module m4_hole(h=10) { cylinder(h=h, d=4.3, $fn=16, center=false); }
module m4_slot(l=30, w=4.5, h=5) {
    hull() {
        translate([-l/2 + w/2, 0, 0]) cylinder(h=h, d=w, $fn=16, center=false);
        translate([ l/2 - w/2, 0, 0]) cylinder(h=h, d=w, $fn=16, center=false);
    }
}

// ============================================================
// MOTOR MOUNT FACE — NEMA 17, 50×50mm plate
// ============================================================
// Bolt pattern: 47.5mm spacing (centre to centre, not diagonal)
// i.e. corners at ±23.75mm in both axes from plate centre.

module motor_mount_face(with_rail_slots=true) {
    bolt_half = 47.5/2;

    difference() {
        // Plate
        cube([mount_plate, mount_plate, mount_t], center=true);

        // Shaft clearance
        cylinder(h=mount_t + 0.2, d=nema17_shaft_d, $fn=32, center=true);

        // 4× M3 mounting holes at ±23.75mm
        for (sx=[-bolt_half/sqrt(2), bolt_half/sqrt(2)])
        for (sy=[-bolt_half/sqrt(2), bolt_half/sqrt(2)])
            translate([sx, sy, 0])
                m3_hole(h=mount_t + 0.2);

        // Horizontal rail adjustment slots (M4) if requested
        if (with_rail_slots) {
            for (ry=[-10, 10])
                translate([0, ry, 0])
                    m4_slot(l=rail_slot_l, w=rail_slot_w, h=mount_t + 0.2);
        }
    }
}

// Stand-alone left mount (for separate printing)
module left_motor_mount_standalone() {
    motor_mount_face(with_rail_slots=true);
}

// Stand-alone right mount
module right_motor_mount_standalone() {
    motor_mount_face(with_rail_slots=true);
}

// ============================================================
// CABLE GUIDE POSTS — pairs of posts with slot for zip tie
// ============================================================

module cable_guide_post(h=20) {
    post_d = 8;
    gap_w  = 4;   // zip tie slot width
    gap_d  = 3;   // depth into post
    difference() {
        cylinder(h=h, d=post_d, $fn=20);
        translate([0, -post_d/2, h*0.4])
            cube([gap_d, post_d, gap_w], center=true);
    }
}

// ============================================================
// DUST SHIELD — folds over variable cap vanes
// ============================================================

module dust_shield() {
    shield_x = 60;
    shield_y = 30;
    shield_t = 2.5;
    slot_x   = 30;  // cap vane clearance slot
    slot_y   = 20;

    difference() {
        cube([shield_x, shield_y, shield_t], center=true);
        cube([slot_x, slot_y, shield_t + 0.2], center=true);
    }
    // Fold tabs (two side flanges 10×10mm with M3 holes)
    for (fx=[-shield_x/2 - 10/2, shield_x/2 + 10/2])
        translate([fx, 0, 0])
        difference() {
            cube([10, shield_y, shield_t], center=true);
            cylinder(h=shield_t + 0.2, d=3.3, $fn=16, center=true);
        }
}

// ============================================================
// CHASSIS FRAME — main structural body
// ============================================================

module chassis_frame() {
    mount_inset_x = 25;   // distance from each end to motor plate centre
    bridge_z      = 50;   // height of connecting bridge shelf

    difference() {
        union() {
            // ── FLOOR PLATE ──
            translate([0, 0, floor_t/2])
                cube([chassis_x, chassis_y, floor_t], center=true);

            // ── LEFT VERTICAL TOWER (inductor side) ──
            translate([-(chassis_x/2 - mount_inset_x), 0, chassis_z/2])
                cube([mount_plate + wall*2, mount_plate + wall*2, chassis_z], center=true);

            // ── RIGHT VERTICAL TOWER (cap side) ──
            translate([(chassis_x/2 - mount_inset_x), 0, chassis_z/2])
                cube([mount_plate + wall*2, mount_plate + wall*2, chassis_z], center=true);

            // ── CENTER BRIDGE SHELF ──
            translate([0, 0, bridge_z/2])
                cube([chassis_x - mount_plate*2 - wall*4, chassis_y, wall], center=true);
            // Shelf support gussets
            for (gx=[-chassis_x/4, chassis_x/4])
                translate([gx, 0, bridge_z/4])
                    cube([wall, chassis_y, bridge_z/2], center=true);

            // ── CABLE GUIDE POSTS (4 pairs) ──
            for (px=[-chassis_x/2 + 15, -chassis_x/4, chassis_x/4, chassis_x/2 - 15])
                translate([px, chassis_y/2 - 8, floor_t])
                    cable_guide_post(h=20);

            // ── GROUNDING LUG BOSS ──
            translate([0, -chassis_y/2 + 10, floor_t])
                cylinder(h=6, d=10, $fn=20);
        }

        // ── LEFT MOTOR FACE CUTOUT ──
        translate([-(chassis_x/2 - mount_inset_x), 0, chassis_z/2]) {
            // Shaft clearance
            rotate([0, 90, 0])
                cylinder(h=mount_plate + wall*4, d=nema17_shaft_d, $fn=32, center=true);
            // Bolt holes M3
            bolt_half = 47.5/2 / sqrt(2);
            for (sy=[-bolt_half, bolt_half], sz=[-bolt_half, bolt_half])
                rotate([0, 90, 0])
                translate([sy, sz, 0])
                    cylinder(h=mount_plate + wall*4, d=3.3, $fn=16, center=true);
            // Rail adjustment slots
            for (ry=[-10, 10])
                rotate([0, 90, 0])
                translate([0, ry, 0])
                    hull() {
                        translate([-rail_slot_l/2 + rail_slot_w/2, 0, 0])
                            cylinder(h=mount_plate + wall*4, d=rail_slot_w, $fn=16, center=true);
                        translate([ rail_slot_l/2 - rail_slot_w/2, 0, 0])
                            cylinder(h=mount_plate + wall*4, d=rail_slot_w, $fn=16, center=true);
                    }
        }

        // ── RIGHT MOTOR FACE CUTOUT ──
        translate([(chassis_x/2 - mount_inset_x), 0, chassis_z/2]) {
            rotate([0, 90, 0])
                cylinder(h=mount_plate + wall*4, d=nema17_shaft_d, $fn=32, center=true);
            bolt_half = 47.5/2 / sqrt(2);
            for (sy=[-bolt_half, bolt_half], sz=[-bolt_half, bolt_half])
                rotate([0, 90, 0])
                translate([sy, sz, 0])
                    cylinder(h=mount_plate + wall*4, d=3.3, $fn=16, center=true);
            for (ry=[-10, 10])
                rotate([0, 90, 0])
                translate([0, ry, 0])
                    hull() {
                        translate([-rail_slot_l/2 + rail_slot_w/2, 0, 0])
                            cylinder(h=mount_plate + wall*4, d=rail_slot_w, $fn=16, center=true);
                        translate([ rail_slot_l/2 - rail_slot_w/2, 0, 0])
                            cylinder(h=mount_plate + wall*4, d=rail_slot_w, $fn=16, center=true);
                    }
        }

        // ── BOTTOM MOUNTING HOLES (4× M4 for enclosure floor) ──
        for (bx=[-(chassis_x/2 - 15), (chassis_x/2 - 15)])
        for (by=[-(chassis_y/2 - 12), (chassis_y/2 - 12)])
            translate([bx, by, -0.1])
                m4_hole(h=floor_t + 0.2);

        // ── VIBRATION ISOLATION FEET (4× M4 shoulder bolt holes) ──
        for (bx=[-(chassis_x/2 - 10), (chassis_x/2 - 10)])
        for (by=[-(chassis_y/2 - 8), (chassis_y/2 - 8)])
            translate([bx, by, floor_t/2])
                cylinder(h=floor_t + 0.2, d=4.3, $fn=16, center=true);

        // ── GROUNDING LUG HOLE (M4) ──
        translate([0, -chassis_y/2 + 10, -0.1])
            m4_hole(h=floor_t + 8);
    }

    // ── DUST SHIELD (attached at right tower top) ──
    translate([(chassis_x/2 - mount_inset_x), 0, chassis_z + 2])
        dust_shield();
}

// ============================================================
// RENDER SELECTOR
// ============================================================

if (print_part == "chassis") {
    chassis_frame();
} else if (print_part == "left_mount") {
    left_motor_mount_standalone();
} else if (print_part == "right_mount") {
    right_motor_mount_standalone();
} else if (print_part == "all") {
    chassis_frame();
    translate([0, chassis_y + 20, 0]) left_motor_mount_standalone();
    translate([60, chassis_y + 20, 0]) right_motor_mount_standalone();
}

// ============================================================
// ASSEMBLY NOTES
// ============================================================
// 1. Print chassis in PETG: strong, slightly flexible, heat resistant.
//    40% gyroid infill gives excellent mechanical strength-to-weight.
//    4 perimeters ensure solid wall around motor bore and bolt holes.
//
// 2. Tap all M3 holes (motor plate corners) with M3 taper tap.
//    M4 holes in floor and isolation feet: M4 tap or heat inserts.
//
// 3. NEMA 17 bolt circle: 47.5mm spacing = 23.75mm from shaft centre.
//    Shaft pilot (22mm hole) clears 5mm shaft plus bearing flange.
//
// 4. Rail slots (M4, 30mm travel): loosen M4 bolts, slide motor
//    left/right ±15mm to set shaft-to-component coupling alignment,
//    then re-torque.
//
// 5. Cable guide posts: route motor phase wires (2 pairs per motor)
//    through pairs of posts; secure with 2.5mm cable ties.
//
// 6. Grounding lug (M4): crimp lug to chassis braid; connect to
//    enclosure ground post and RF ground point.
//
// 7. Vibration isolation: use M4 shoulder bolts with 10mm silicone
//    grommets through floor holes; mount chassis on standoffs.
//    Reduces motor vibration coupling to SWR bridge PCB.
//
// 8. Dust shield: snap onto right tower; vane clearance slot (30×20mm)
//    allows cap shaft rotation. Remove for initial alignment.
//
// DIMENSIONAL NOTES
// - Chassis is 190×100×80mm — fits 200mm cube print envelope.
// - Left tower (inductor): shaft axis at x = -70mm from centre.
// - Right tower (cap): shaft axis at x = +70mm from centre.
// - Bridge shelf at z=50mm provides PCB mounting surface if needed.
// ============================================================
