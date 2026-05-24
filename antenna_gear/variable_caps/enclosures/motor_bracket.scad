// ============================================================
//  motor_bracket.scad — NEMA 14/17 Stepper Motor Mount
//  TM-VCAP-ENC-003 Rev A
//  Mounts stepper motor co-axially to capacitor shaft
//  Fits NEMA 14 (35.2mm face) and NEMA 17 (42.3mm face) steppers
//  Shaft coupler: 5mm motor shaft to 4/6/8mm cap shaft (configure below)
//  Print: PETG, supports on motor pocket only
// ============================================================

$fn = 64;

// ---- Configuration ----
MOTOR_TYPE      = "NEMA14";  // "NEMA14" or "NEMA17"

// NEMA 14 params
NEMA14_FACE     = 35.2;
NEMA14_HOLE_SPACING = 26.0;  // 4× M3 holes on 26mm square pattern
NEMA14_BOSS_DIA = 22.0;     // pilot boss diameter
NEMA14_BOSS_H   = 2.0;      // pilot boss protrusion
NEMA14_SHAFT    = 5.0;      // motor shaft diameter

// NEMA 17 params
NEMA17_FACE     = 42.3;
NEMA17_HOLE_SPACING = 31.0;
NEMA17_BOSS_DIA = 22.0;
NEMA17_BOSS_H   = 2.0;
NEMA17_SHAFT    = 5.0;

// Cap shaft params (match cap frame design)
CAP_SHAFT_DIA   = 6.0;      // 4 for VC-QRP/VHF, 6 for VC-HF100, 8 for VC-TX
CAP_BOSS_DIA    = 12.0;     // matches cap frame motor boss OD

// Bracket dimensions
PLATE_T         = 5.0;      // mounting plate thickness
PLATE_W         = 70.0;     // for NEMA17; adjust to NEMA14_FACE+20 for NEMA14
PLATE_H         = 70.0;
SPACER_H        = 20.0;     // distance motor face to cap frame; room for coupler

COUPLER_OD      = 19.0;     // rigid shaft coupler OD (off-shelf, 5mm→6mm)
COUPLER_L       = 25.0;     // coupler length
COUPLER_BORE_MOTOR = NEMA14_SHAFT + 0.1;  // motor shaft bore
COUPLER_BORE_CAP   = CAP_SHAFT_DIA + 0.1; // cap shaft bore

// Motor face params (active)
FACE    = (MOTOR_TYPE == "NEMA14") ? NEMA14_FACE    : NEMA17_FACE;
HOLES   = (MOTOR_TYPE == "NEMA14") ? NEMA14_HOLE_SPACING : NEMA17_HOLE_SPACING;
BOSS_D  = (MOTOR_TYPE == "NEMA14") ? NEMA14_BOSS_DIA : NEMA17_BOSS_DIA;
BOSS_H  = (MOTOR_TYPE == "NEMA14") ? NEMA14_BOSS_H  : NEMA17_BOSS_H;
M_SHAFT = (MOTOR_TYPE == "NEMA14") ? NEMA14_SHAFT   : NEMA17_SHAFT;

module motorFacePlate() {
    difference() {
        // Main plate
        translate([-PLATE_W/2, -PLATE_H/2, 0])
        cube([PLATE_W, PLATE_H, PLATE_T]);

        // Motor pilot boss clearance
        translate([0, 0, -1])
        cylinder(h=PLATE_T+2, d=BOSS_D+0.5);

        // Motor shaft clearance
        translate([0, 0, -1])
        cylinder(h=PLATE_T+2, d=M_SHAFT+2);

        // Motor mounting holes: 4× M3
        for (sx = [-1,1], sy = [-1,1]) {
            translate([sx*HOLES/2, sy*HOLES/2, -1])
            cylinder(h=PLATE_T+2, d=3.2);
        }

        // Corner mounting holes (bracket to chassis): 4× M3
        for (mx = [-1,1], my = [-1,1]) {
            translate([mx*(PLATE_W/2-6), my*(PLATE_H/2-6), -1])
            cylinder(h=PLATE_T+2, d=3.2);
        }

        // Lightening pockets
        for (sx = [-1,1]) {
            translate([sx*(FACE/4 + 5), 0, 1])
            cylinder(h=PLATE_T-2, d=10);
        }
    }

    // Pilot boss recess (negative: fits over motor boss)
    // This is a ring that locates the motor; motor boss goes into this
    difference() {
        translate([0, 0, 0])
        cylinder(h=BOSS_H+PLATE_T, d=BOSS_D+0.5+2*3);  // ring OD
        translate([0, 0, -1])
        cylinder(h=BOSS_H+PLATE_T+2, d=BOSS_D+0.5);    // ring ID = boss clear
    }
}

module standoffs() {
    // 4× standoff posts from motor plate to cap frame mount
    for (sx = [-1,1], sy = [-1,1]) {
        translate([sx*(PLATE_W/2-6), sy*(PLATE_H/2-6), PLATE_T])
        difference() {
            cylinder(h=SPACER_H, d=8);
            translate([0, 0, -1])
            cylinder(h=SPACER_H+2, d=3.4);  // M3 through
        }
    }
}

module capMountPlate() {
    // Mating plate that attaches to capacitor frame motor boss
    translate([0, 0, PLATE_T + SPACER_H])
    difference() {
        translate([-PLATE_W/2, -PLATE_H/2, 0])
        cube([PLATE_W, PLATE_H, PLATE_T]);

        // Cap motor boss socket (press-fit or loose)
        translate([0, 0, -1])
        cylinder(h=PLATE_T+2, d=CAP_BOSS_DIA+0.3);

        // Cap shaft clearance
        translate([0, 0, -1])
        cylinder(h=PLATE_T+2, d=CAP_SHAFT_DIA+1);

        // Mounting holes
        for (mx = [-1,1], my = [-1,1]) {
            translate([mx*(PLATE_W/2-6), my*(PLATE_H/2-6), -1])
            cylinder(h=PLATE_T+2, d=3.2);
        }
    }
}

module rigidCoupler() {
    // One-piece 3D-printed shaft coupler (for non-critical alignment applications)
    // For precision use: aluminum jaw coupler (off-shelf)
    translate([0, 0, PLATE_T + PLATE_T + SPACER_H - COUPLER_L/2 + 2])
    difference() {
        cylinder(h=COUPLER_L, d=COUPLER_OD);
        // Motor shaft bore + set-screw groove
        translate([0, 0, -1])
        cylinder(h=COUPLER_L/2 + 1, d=COUPLER_BORE_MOTOR);
        // Cap shaft bore
        translate([0, 0, COUPLER_L/2 - 1])
        cylinder(h=COUPLER_L/2 + 2, d=COUPLER_BORE_CAP);
        // Set screw holes (M3): motor side and cap side
        translate([COUPLER_OD/2, 0, COUPLER_L/4])
        rotate([0, 90, 0])
        cylinder(h=COUPLER_OD, d=2.5);
        translate([COUPLER_OD/2, 0, COUPLER_L*3/4])
        rotate([0, 90, 0])
        cylinder(h=COUPLER_OD, d=2.5);
        // Alignment slit (makes it slightly flexible for minor misalignment)
        translate([-0.5, -COUPLER_OD/2-1, COUPLER_L/2-5])
        cube([1.0, COUPLER_OD+2, 10]);
    }
}

// ---- Render all parts ----
motorFacePlate();
standoffs();
capMountPlate();
rigidCoupler();

// Label
translate([0, PLATE_H/2 + 0.1, PLATE_T/2])
rotate([90, 0, 0])
linear_extrude(0.4)
text(str("MOTOR BRACKET ", MOTOR_TYPE, " TM-VCAP-ENC-003"), size=2.5,
     halign="center", valign="center");

// ---- BUILD NOTES ----
// Print orientation: motor face plate flat on bed; standoffs print vertical
// For NEMA14 on VC-QRP: coupler 5mm motor → 4mm cap shaft
// For NEMA17 on VC-HF100: coupler 5mm motor → 6mm cap shaft
// For VC-TX: use NEMA17 with coupler 5mm → 8mm; add flex jaw coupler for alignment
// After print: drill M3 through holes to clean; tap coupler set-screw holes M3
// Align motor shaft and cap shaft concentrically before tightening standoff screws
// Misalignment >0.5mm will cause shaft wobble and plate-to-plate contact — CHECK!
