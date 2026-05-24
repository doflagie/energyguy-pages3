// ============================================================
//  position_indicator_dial.scad — Capacitance / Angle Dial
//  TM-VCAP-ENC-004 Rev A
//  Mechanical dial indicator: 0–180° scale with pF markings
//  Pointer needle + dial face + shaft coupling
//  Configurable for all five VC designs
//  Print: PETG (dial face), any for needle
// ============================================================

$fn = 64;

// ---- Configuration: Select cap design ----
CAP_DESIGN      = "VC-HF100";  // "VC-QRP", "VC-HF100", "VC-TX", "VC-VHF", "VC-UHF"
SHAFT_DIA       = 6.0;         // must match cap design shaft dia

// ---- Per-design data ----
// C_min, C_max, step_pF for tick marks
C_DATA = (CAP_DESIGN == "VC-QRP")   ? [8, 345, 50]   :
         (CAP_DESIGN == "VC-HF100") ? [10, 225, 25]  :
         (CAP_DESIGN == "VC-TX")    ? [5, 84, 10]    :
         (CAP_DESIGN == "VC-VHF")   ? [2, 47, 5]     :
                                      [0.5, 12, 2];

C_MIN   = C_DATA[0];
C_MAX   = C_DATA[1];
C_STEP  = C_DATA[2];

// ---- Dial dimensions ----
DIAL_OD         = 80.0;    // outer diameter of dial face
DIAL_T          = 3.0;     // dial face thickness
POINTER_L       = 35.0;    // pointer needle length from center
POINTER_W       = 2.0;     // pointer width
POINTER_T       = 1.5;     // pointer thickness
BOSS_H          = 8.0;     // hub height (shaft coupling)
BOSS_OD         = 16.0;    // hub OD
TICK_MAJOR      = 6.0;     // major tick mark length
TICK_MINOR      = 3.0;     // minor tick mark length
TICK_W          = 1.0;     // tick width
TEXT_SIZE_MAJOR = 3.0;     // angle text
TEXT_SIZE_PF    = 2.5;     // pF label
SCALE_RADIUS    = DIAL_OD/2 - TICK_MAJOR - 2; // tick mark inner radius

// ---- Band frequency markers ----
// (angle corresponding to nominal cap for each band in VC-HF100 example)
BAND_MARKS = (CAP_DESIGN == "VC-HF100") ?
    [["160m",160], ["80m",130], ["40m",85], ["30m",64], ["20m",42],
     ["17m",35], ["15m",29], ["12m",23], ["10m",18]] :
    (CAP_DESIGN == "VC-QRP") ?
    [["10m",20], ["12m",22], ["15m",26], ["17m",30], ["20m",36],
     ["30m",53], ["40m",63]] :
    [];

function cap_to_angle(c) = (c - C_MIN) / (C_MAX - C_MIN) * 180;
function angle_to_dial(a) = 180 - a;  // 0°=left, 180°=right; flip so 0=left=C_min

module dialFace() {
    difference() {
        // Main disc
        cylinder(h=DIAL_T, d=DIAL_OD);

        // Shaft clearance
        translate([0, 0, -1])
        cylinder(h=DIAL_T+2, d=SHAFT_DIA+0.3);

        // Hub pocket (pointer attaches here, not dial face)
        translate([0, 0, DIAL_T-2])
        cylinder(h=3, d=BOSS_OD+0.5);
    }

    // Tick marks: every 10°, major at 0,30,60,90,120,150,180
    for (a = [0 : 10 : 180]) {
        is_major = (a % 30 == 0);
        tick_l = is_major ? TICK_MAJOR : TICK_MINOR;
        r_outer = DIAL_OD/2 - 1;
        r_inner = r_outer - tick_l;
        rotate([0, 0, angle_to_dial(a)])
        translate([r_inner, -TICK_W/2, DIAL_T])
        cube([tick_l, TICK_W, 0.8]);
    }

    // Degree labels at every 30°
    for (a = [0, 30, 60, 90, 120, 150, 180]) {
        r_label = DIAL_OD/2 - TICK_MAJOR - 7;
        ang_on_dial = angle_to_dial(a);
        label_x = r_label * cos(ang_on_dial);
        label_y = r_label * sin(ang_on_dial);
        translate([label_x, label_y, DIAL_T])
        rotate([0, 0, ang_on_dial + 90])
        linear_extrude(0.6)
        text(str(a), size=TEXT_SIZE_MAJOR, halign="center", valign="center");
    }

    // Capacitance labels (pF) at key angles
    for (c = [C_MIN : C_STEP : C_MAX]) {
        a = cap_to_angle(c);
        r_label = DIAL_OD/2 - TICK_MAJOR - 14;
        ang_on_dial = angle_to_dial(a);
        label_x = r_label * cos(ang_on_dial);
        label_y = r_label * sin(ang_on_dial);
        // Only label if not too crowded (skip labels near 0 if step is small)
        translate([label_x, label_y, DIAL_T])
        rotate([0, 0, ang_on_dial + 90])
        linear_extrude(0.5)
        text(str(round(c)), size=TEXT_SIZE_PF, halign="center", valign="center");
    }

    // Band labels (triangle markers)
    for (i = [0:len(BAND_MARKS)-1]) {
        band_name = BAND_MARKS[i][0];
        band_ang  = BAND_MARKS[i][1];
        r_band = DIAL_OD/2 - TICK_MAJOR - 22;
        ang_on_dial = angle_to_dial(band_ang);
        bx = r_band * cos(ang_on_dial);
        by = r_band * sin(ang_on_dial);
        translate([bx, by, DIAL_T])
        rotate([0, 0, ang_on_dial + 90])
        linear_extrude(0.5)
        text(band_name, size=2.0, halign="center", valign="center");
    }

    // Design label
    translate([0, -DIAL_OD/2 + 10, DIAL_T])
    linear_extrude(0.6)
    text(str(CAP_DESIGN, " TM-VCAP-ENC-004"), size=2.5, halign="center", valign="center");

    // pF unit label at center
    translate([0, 8, DIAL_T])
    linear_extrude(0.5)
    text("pF", size=2.0, halign="center", valign="center");
    translate([0, -8, DIAL_T])
    linear_extrude(0.5)
    text("deg", size=2.0, halign="center", valign="center");
}

module pointerHub() {
    // Hub: press-fits onto shaft; pointer attaches to hub
    translate([0, 0, 0])
    difference() {
        union() {
            // Hub cylinder
            cylinder(h=BOSS_H, d=BOSS_OD);
            // Pointer arm
            translate([-POINTER_W/2, 0, BOSS_H - POINTER_T])
            cube([POINTER_W, POINTER_L, POINTER_T]);
            // Counterweight arm (balance)
            translate([-POINTER_W/2, -POINTER_L/4, BOSS_H - POINTER_T])
            cube([POINTER_W, POINTER_L/4, POINTER_T]);
        }
        // Shaft press-fit bore
        translate([0, 0, -1])
        cylinder(h=BOSS_H+2, d=SHAFT_DIA-0.1);  // -0.1 = press fit
        // Set screw hole (M2.5)
        translate([BOSS_OD/2, 0, BOSS_H/2])
        rotate([0, 90, 0])
        cylinder(h=BOSS_OD, d=2.2);
    }
    // Pointer tip (arrow)
    translate([0, POINTER_L, BOSS_H - POINTER_T])
    linear_extrude(POINTER_T)
    polygon([[0, 0], [-2, -5], [2, -5]]);
}

// ---- Render ----
// Print separately: dial face flat on bed, pointer hub flat on bed
translate([0, 0, 0])
dialFace();

translate([DIAL_OD/2 + 20, 0, 0])
pointerHub();

// ---- BUILD NOTES ----
// Dial face: print flat, 0.15mm layer height for text clarity; 2 walls, 20% infill
// Pointer hub: print flat (boss face down); 4 walls for shaft bore strength
// Press pointer hub onto cap shaft (tight press fit); secure with M2.5 set screw
// Dial face sits behind hub; attach to frame face with M2 screws through outer holes
// After assembly: rotate cap to C_min position (θ=0); align pointer to 0° mark
// Use indelible ink to mark band positions if laser engraving not available
// For best readability: spray dial face with matte white primer; engravings fill with black ink
