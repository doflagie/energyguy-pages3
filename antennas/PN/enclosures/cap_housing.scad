// ============================================================
//  cap_housing.scad — Tuning Capacitor Housing with Motor Drive
//  TM-PNL-ENC-001 Rev A
//  Pool Noodle Magnetic Loop Antenna Project
//  Print: PLA or PETG, 0.2mm layer, 3 perimeters, 25% infill
//  Bed footprint: 120 x 95 x 70mm — fits 200x200 bed
//  WARNING: High-voltage capacitor inside. Use insulating PLA.
// ============================================================

// ---- Parameters ----
CAP_W        = 100;   // capacitor body width (mm)
CAP_D        = 60;    // capacitor body depth
CAP_H        = 40;    // capacitor body height
CAP_SHAFT_D  = 6;     // capacitor shaft diameter
MOTOR_FLANGE = 28;    // 28BYJ-48 flange OD (mm)
MOTOR_BOLT_C = 25.8;  // motor bolt circle diameter
MOTOR_DEPTH  = 19.6;  // motor body depth from flange face
GEAR_M       = 1;     // gear module (mm)
N_MOTOR      = 10;    // motor pinion teeth
N_LARGE      = 40;    // driven gear teeth (4:1 reduction)
WALL         = 2.5;   // wall thickness
LID_H        = 8;     // lid height
CABLE_W      = 16;    // cable exit slot width
BLEEDER_D    = 3.0;   // bleeder resistor pocket diameter
$fn          = 64;

// ---- Derived ----
BODY_W = CAP_W + 2*WALL + 10;
BODY_D = CAP_D + 2*WALL + 6;
BODY_H = CAP_H + 2*WALL + 12;
MOTOR_GEAR_R   = (N_MOTOR * GEAR_M) / 2;
LARGE_GEAR_R   = (N_LARGE * GEAR_M) / 2;
GEAR_DIST      = MOTOR_GEAR_R + LARGE_GEAR_R;  // center-to-center
MOTOR_SIDE_W   = MOTOR_DEPTH + WALL + 6;

// ---- Main enclosure body ----
module body() {
    difference() {
        cube([BODY_W, BODY_D, BODY_H]);
        // Interior pocket for capacitor
        translate([WALL, WALL, WALL])
            cube([CAP_W + 4, CAP_D + 2, CAP_H + 6]);
        // Shaft hole at top center
        translate([BODY_W/2, BODY_D/2, BODY_H - WALL - 0.1])
            cylinder(h=WALL + 0.2, d=CAP_SHAFT_D + 1.5);
        // Cable exit slot — rear face, center-low
        translate([BODY_W/2 - CABLE_W/2, -0.1, BODY_H * 0.2])
            cube([CABLE_W, WALL + 0.2, 18]);
        // Bleeder resistor pockets — front face, 2x
        for (bx = [BODY_W * 0.28, BODY_W * 0.68])
            translate([bx - BLEEDER_D/2, -0.1, BODY_H * 0.65])
                rotate([-90, 0, 0])
                    cylinder(h=WALL + 4, d=BLEEDER_D);
        // Lid rabbet groove (inside perimeter at top)
        translate([1.2, 1.2, BODY_H - LID_H])
            cube([BODY_W - 2.4, BODY_D - 2.4, LID_H + 0.1]);
        // M3 corner screw holes for lid
        for (cx = [5, BODY_W - 5]) for (cy = [5, BODY_D - 5])
            translate([cx, cy, BODY_H - LID_H - 0.1])
                cylinder(h=LID_H + 0.2, d=3.4);
    }
}

// ---- Side plate: motor mount ----
module motor_mount() {
    translate([BODY_W, 0, 0])
    difference() {
        cube([MOTOR_SIDE_W, BODY_D, BODY_H * 0.65]);
        // Motor circular recess
        translate([WALL + 1, BODY_D/2, BODY_H * 0.32])
            rotate([0, 90, 0])
                cylinder(h=MOTOR_DEPTH + 2, d=MOTOR_FLANGE + 1);
        // Motor M2 bolt holes (4x on 25.8mm circle)
        for (a = [0, 90, 180, 270])
            translate([WALL + 1, BODY_D/2, BODY_H * 0.32])
                rotate([0, 90, 0])
                    rotate([0, 0, a])
                        translate([MOTOR_BOLT_C/2, 0, 0])
                            cylinder(h=MOTOR_DEPTH + 3, d=2.2);
        // Gear access window between motor and main body
        translate([-0.1, BODY_D * 0.35, BODY_H * 0.2])
            cube([WALL + 2, BODY_D * 0.3, BODY_H * 0.35]);
    }
}

// ---- Gear housing cover ----
module gear_cover() {
    translate([BODY_W, BODY_D * 0.15, BODY_H * 0.42])
    difference() {
        cube([MOTOR_SIDE_W - 2, BODY_D * 0.7, BODY_H * 0.45]);
        translate([1.5, 3, 1.5])
            cube([MOTOR_SIDE_W - 5, BODY_D * 0.7 - 6, BODY_H * 0.45 - 3]);
    }
}

// ---- Large driven gear (40T, module=1) ----
module large_gear_profile() {
    r_pitch = LARGE_GEAR_R;
    r_root  = r_pitch - 1.25 * GEAR_M;
    r_tip   = r_pitch + GEAR_M;
    n       = N_LARGE;
    union() {
        circle(r=r_root + 0.5);
        for (i = [0:n-1]) {
            rotate([0, 0, i * 360/n])
            translate([r_pitch, 0, 0])
            rotate([0, 0, 15])
            square([GEAR_M * 1.8, GEAR_M * 1.4], center=true);
        }
    }
}

module large_gear() {
    // Position on cap shaft axis; offset from main body top
    translate([BODY_W/2, BODY_D/2, BODY_H + 2])
    difference() {
        linear_extrude(8)
            large_gear_profile();
        // Center bore for cap shaft
        cylinder(h=9, d=CAP_SHAFT_D + 0.3, center=false);
        // Set screw M3 hole (radial)
        translate([LARGE_GEAR_R * 0.6, 0, 4])
            rotate([0, 90, 0])
                cylinder(h=LARGE_GEAR_R, d=2.5);
    }
}

// ---- Snap-fit lid ----
module lid() {
    difference() {
        cube([BODY_W, BODY_D, LID_H]);
        // Inner pocket (fits over body rabbet)
        translate([1.2, 1.2, -0.1])
            cube([BODY_W - 2.4, BODY_D - 2.4, LID_H - 2.5]);
        // SO-239 cable hole (16mm)
        translate([BODY_W/2, BODY_D/2, -0.1])
            cylinder(h=LID_H + 0.2, d=16);
        // M3 corner screws
        for (cx = [5, BODY_W - 5]) for (cy = [5, BODY_D - 5])
            translate([cx, cy, -0.1])
                cylinder(h=LID_H + 0.2, d=3.4);
    }
    // Snap tabs (2x, on long sides)
    for (side = [0, 1])
        translate([BODY_W * 0.5 - 6, side * (BODY_D - 0.8), LID_H - 5])
            cube([12, 0.8, 4]);
}

// ---- Assembly output ----
// Main body
color("SteelBlue", 0.85) body();

// Motor mount side plate
color("DarkSlateGray", 0.85) motor_mount();

// Gear cover
color("DimGray", 0.7) gear_cover();

// Driven gear (shown on shaft)
color("Gold", 0.9) large_gear();

// Lid shown offset above (exploded view)
translate([0, 0, BODY_H + 20])
    color("SteelBlue", 0.6) lid();

// ---- Notes (in comments) ----
// Assembly:
// 1. Insert air-variable cap into body pocket through top shaft hole.
// 2. Secure motor to side plate with 4x M2 screws.
// 3. Install motor pinion (10T) on motor shaft.
// 4. Install large gear (40T) on capacitor shaft; secure with M3 set screw.
// 5. Verify gear mesh; adjust motor plate position if needed.
// 6. Route coax cable through rear slot; seal with silicone if outdoor use.
// 7. Install 100kΩ 1W bleeder resistor in front pockets across cap terminals.
// 8. Snap lid down and secure with 4x M3 screws.
