// ============================================================
//  cap_frame_hf100.scad — VC-HF100 Variable Capacitor Frame
//  TM-VCAP-ENC-002 Rev A
//  VC-HF100: 10-225 pF, 1500V peak, 14R+15S plates, r=30mm, d=1.5mm
//  Fits 200×200×200mm print bed; print in PETG or Delrin (preferred)
//  Plate stack: 60mm OD, 29 plates, 42.25mm total depth
//  Frame: 80×80×70mm
//  HV NOTE: 1500V peak requires clean insulation; no conductive contamination
// ============================================================

$fn = 64;

// ---- Parameters ----
PLATE_OD        = 60.0;
PLATE_R         = 30.0;
SHAFT_DIA       = 6.0;
SHAFT_CLEAR     = 0.15;
STATOR_ROD_DIA  = 3.0;     // PTFE rod Ø (3mm for HV; larger insulation path)
STATOR_ROD_OFS  = 50.0;    // at 25mm offset from center
N_RODS          = 4;        // 4-rod stator support for rigidity
PLATE_DEPTH     = 42.25;   // 29 plates × (0.5t + 1.5d) = 28× + 0.5mm outer pair
BEARING_L       = 7.0;     // longer bearing for shaft rigidity
FRAME_W         = 80.0;
FRAME_H         = 80.0;
FRAME_D         = 70.0;
WALL            = 3.0;     // thicker walls for HV creepage distance
TERMINAL_H      = 12.0;    // taller terminal posts; M3 inserts
MOTOR_BOSS_OD   = 16.0;
MOTOR_BOSS_H    = 8.0;
HV_CLEARANCE    = 12.0;    // minimum air clearance between live parts at 1500V
                            // Rule: 10mm/kV + safety margin → 15mm for 1.5kV
LABEL_DEPTH     = 0.5;

// ---- Derived ----
BEARING_OD      = SHAFT_DIA + 10.0;
STATOR_HOLE_D   = SHAFT_DIA + 2*SHAFT_CLEAR + 10.0;

module bearingBoss(z_pos, length) {
    translate([0, 0, z_pos])
    difference() {
        cylinder(h=length, d=BEARING_OD);
        cylinder(h=length+1, d=SHAFT_DIA+0.25);
    }
}

module statorRodHoles() {
    // 4 rods at 90° spacing; offset at (PLATE_OD/2 + WALL + 2) from center
    for (i = [0:N_RODS-1]) {
        angle = 45 + i * 90;
        r_rod = PLATE_OD/2 + 3;
        rotate([0, 0, angle])
        translate([r_rod, 0, 0])
        cylinder(h=FRAME_D+2, d=STATOR_ROD_DIA+0.3, center=true);
    }
}

module hvBarrierRibs() {
    // Radial ribs inside frame to increase creepage distance between stator and rotor
    // Located between plate stack OD and frame wall
    for (i = [0, 90, 180, 270]) {
        rotate([0, 0, i])
        translate([PLATE_OD/2 + WALL/2, 0, 0])
        cube([WALL, 2.0, FRAME_D-2*WALL], center=true);
    }
}

module terminalPost(x_offset, y_offset) {
    translate([x_offset, y_offset, -FRAME_D/2])
    difference() {
        cylinder(h=TERMINAL_H + FRAME_D, d=8.0);
        cylinder(h=TERMINAL_H + FRAME_D + 1, d=3.4);  // M3 through hole
    }
}

module frame() {
    difference() {
        // Outer body
        translate([-FRAME_W/2, -FRAME_H/2, -FRAME_D/2])
        cube([FRAME_W, FRAME_H, FRAME_D]);

        // Plate stack bore
        cylinder(h=FRAME_D - 2*WALL, d=PLATE_OD+2.0, center=true);

        // Shaft bore (full length)
        cylinder(h=FRAME_D+2, d=SHAFT_DIA+0.3, center=true);

        // Stator rod holes
        statorRodHoles();

        // Inspection slots (front and rear halves, sides)
        for (s = [1,-1]) {
            translate([s*(FRAME_W/2-WALL-0.5), -12, -6])
            cube([WALL+1, 24, 12]);
        }

        // Mounting holes: 4× M3 corner holes
        for (mx = [-1,1], my = [-1,1]) {
            translate([mx*(FRAME_W/2-8), my*(FRAME_H/2-8), -FRAME_D/2-1])
            cylinder(h=FRAME_D+2, d=3.4);
        }
    }

    // HV barrier ribs
    hvBarrierRibs();

    // Bearing bosses
    bearingBoss(-FRAME_D/2 - BEARING_L, BEARING_L);
    bearingBoss(FRAME_D/2, BEARING_L);

    // Motor boss
    translate([0, 0, FRAME_D/2 + BEARING_L])
    cylinder(h=MOTOR_BOSS_H, d=MOTOR_BOSS_OD);

    // Stator terminal (right side top)
    terminalPost(FRAME_W/2 - 8, FRAME_H/2 - 8);

    // Rotor terminal (left side top, connects to shaft via spring contact)
    terminalPost(-FRAME_W/2 + 8, FRAME_H/2 - 8);
}

module engravings() {
    // Voltage rating label on front face
    translate([0, FRAME_H/2 - 5, -FRAME_D/2 - 0.1])
    rotate([0, 0, 0])
    linear_extrude(LABEL_DEPTH+0.1)
    text("VC-HF100  1500V  TM-VCAP-ENC-002", size=2.8, halign="center", valign="center");

    // HV warning triangle on each end face
    translate([-15, -FRAME_H/2 + 5, -FRAME_D/2 - 0.1])
    linear_extrude(LABEL_DEPTH+0.1)
    text("⚠ HV", size=3.5, halign="center", valign="center");
}

union() {
    frame();
    // Note: engravings subtracted in final diff — add here as positive to show text emboss
    translate([0, FRAME_H/2 + 0.1, -FRAME_D/2])
    rotate([90, 0, 0])
    linear_extrude(LABEL_DEPTH)
    text("VC-HF100  MAX 1500V  TM-VCAP-ENC-002", size=3, halign="center", valign="center");
}

// ---- BUILD NOTES ----
// Material: PETG minimum; Delrin (POM) preferred for better creepage resistance
// Orientation: shaft axis vertical for print; re-orient for use
// After print: clean all holes with appropriate drill; chase M3 holes with tap
// PTFE stator rods (3mm): coat with silicone grease before insertion
// Bearing: 2× PTFE bushing ID=6mm OD=12mm L=7mm
// CRITICAL: Remove all conductive print supports; clean with IPA before HV operation
// Stator and rotor terminals must be separated by at least 15mm in air (HV clearance)
// For operation above 1000V: apply Glyptol insulating varnish (MIL-I-24092) to frame
