// ============================================================
//  cap_frame_qrp.scad — VC-QRP Variable Capacitor Frame
//  TM-VCAP-ENC-001 Rev A
//  VC-QRP: 10-345 pF, 200V peak, 16R+17S plates, r=20mm, d=0.5mm
//  Fits 200×200×200mm print bed; print in PETG
//  Plate stack: 40mm OD, 33 plates, 22.6mm total depth
//  Frame O.D.: 50×50×45mm
// ============================================================

$fn = 64;

// ---- Parameters ----
PLATE_OD        = 40.0;    // plate diameter mm
PLATE_R         = 20.0;    // plate radius mm
SHAFT_DIA       = 4.0;     // rotor shaft diameter mm
SHAFT_CLEAR     = 0.15;    // clearance for stator center hole
STATOR_ROD_DIA  = 2.5;     // PTFE stator mounting rod Ø
STATOR_ROD_OFS  = 32.0;    // stator rod offset from shaft centerline
N_RODS          = 3;        // stator mounting rods
PLATE_DEPTH     = 22.6;    // total plate stack depth mm
BEARING_L       = 5.0;     // bearing boss length
FRAME_W         = 50.0;    // frame width
FRAME_H         = 50.0;    // frame height
FRAME_D         = 45.0;    // frame depth (shaft axis)
WALL            = 2.5;     // minimum wall thickness
TERMINAL_H      = 8.0;     // terminal post height
MOTOR_BOSS_OD   = 12.0;    // motor coupler boss OD (optional)
MOTOR_BOSS_H    = 6.0;     // motor coupler boss height
LABEL_DEPTH     = 0.4;     // emboss depth

// ---- Derived ----
INNER_W         = FRAME_W - 2*WALL;
INNER_D         = PLATE_DEPTH + 2*BEARING_L + 2*WALL;
BEARING_OD      = SHAFT_DIA + 8.0;     // bearing boss OD
STATOR_HOLE_D   = SHAFT_DIA + 2*SHAFT_CLEAR + 6.0;  // stator center clearance

module bearingBoss(side=1) {
    // side: 1 = front, -1 = back; bearing boss centered on shaft
    translate([0, 0, side*(FRAME_D/2 - WALL)])
    rotate([0, 0, 0])
    difference() {
        cylinder(h=BEARING_L, d=BEARING_OD, center=false);
        cylinder(h=BEARING_L+1, d=SHAFT_DIA+0.2, center=false);
    }
}

module statorRodHole(dia=STATOR_ROD_DIA) {
    // Three stator rod holes at 120° spacing at STATOR_ROD_OFS radius
    for (i = [0:N_RODS-1]) {
        angle = i * (360/N_RODS);
        rotate([0, 0, angle])
        translate([STATOR_ROD_OFS/2, 0, 0])
        cylinder(h=FRAME_D+2, d=dia+0.2, center=true);
    }
}

module endPlate() {
    // Square end plate with shaft hole and stator rod holes
    difference() {
        // Outer square
        translate([-FRAME_W/2, -FRAME_H/2, 0])
        cube([FRAME_W, FRAME_H, WALL]);
        // Shaft hole
        translate([0, 0, -1])
        cylinder(h=WALL+2, d=SHAFT_DIA+0.25);
        // Stator rod holes
        translate([0, 0, -1])
        statorRodHole();
        // Corner lightening holes
        for (s = [[1,1],[-1,1],[1,-1],[-1,-1]])
        translate([s[0]*(FRAME_W/2-5), s[1]*(FRAME_H/2-5), -1])
        cylinder(h=WALL+2, d=4.0);
    }
}

module terminalPost(x_offset, label) {
    // Threaded insert post for wire terminal (M2.5 × 5mm insert)
    translate([x_offset, FRAME_H/2 - WALL, WALL])
    difference() {
        cylinder(h=TERMINAL_H, d=6.0);
        cylinder(h=TERMINAL_H+1, d=2.7);  // M2.5 tap hole
    }
}

module label() {
    translate([0, 0, FRAME_W/2 - WALL + 0.1])
    rotate([90, 0, 0])
    linear_extrude(LABEL_DEPTH)
    text("VC-QRP TM-VCAP-ENC-001", size=3, halign="center", valign="center");
}

module frame() {
    difference() {
        // Main body
        translate([-FRAME_W/2, -FRAME_H/2, -FRAME_D/2])
        cube([FRAME_W, FRAME_H, FRAME_D]);

        // Hollow interior (plate stack clearance)
        // Cylindrical bore for plate stack
        translate([0, 0, 0])
        cylinder(h=PLATE_DEPTH+2*BEARING_L, d=PLATE_OD+2, center=true);

        // Front shaft through-hole
        translate([0, 0, -FRAME_D/2-1])
        cylinder(h=FRAME_D/2+2, d=SHAFT_DIA+0.25);

        // Back shaft through-hole
        translate([0, 0, 0])
        cylinder(h=FRAME_D/2+2, d=SHAFT_DIA+0.25);

        // Stator rod through-holes
        statorRodHole();

        // Inspection window (side, shows plate mesh)
        translate([FRAME_W/2-WALL-1, -8, -4])
        cube([WALL+2, 16, 8]);
    }

    // Front bearing boss
    translate([0, 0, -FRAME_D/2 - BEARING_L])
    cylinder(h=BEARING_L, d=BEARING_OD);

    // Back bearing boss + motor coupling boss
    translate([0, 0, FRAME_D/2])
    cylinder(h=BEARING_L, d=BEARING_OD);
    translate([0, 0, FRAME_D/2 + BEARING_L])
    cylinder(h=MOTOR_BOSS_H, d=MOTOR_BOSS_OD);

    // Stator terminal post (top face)
    terminalPost(-8, "S");
    // Rotor terminal post (top face)
    terminalPost(+8, "R");
}

// ---- Assembly ----
union() {
    frame();
    label();
}

// ---- BUILD NOTES ----
// Print: PETG, 0.2mm layer, 30% gyroid infill, 3 walls
// Orientation: stand upright (shaft axis vertical) for best dimensional accuracy
// After print: clean stator rod holes with 2.5mm drill; ream bearing holes to shaft fit
// Mount PTFE stator rods with M2 × 6mm set screws into frame side walls
// Shaft bearings: 2× PTFE bushing ID=4mm OD=8mm L=5mm; press into bearing bosses
// Max plate OD before sides contact frame interior: 44mm (4mm clearance each side)
