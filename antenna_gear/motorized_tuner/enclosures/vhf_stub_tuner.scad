// ============================================================
// VHF/UHF COAXIAL STUB TUNER HOUSING
// Document: TM-VHFST-ENC-001 Rev A
// Project:  Motorized VHF/UHF Stub Tuner
// Author:   Merv Martin
// Date:     2026-05-02
// ============================================================
// Sliding short-circuit plunger in air-dielectric coaxial stub.
// Bands: 2M (144 MHz), 1.25M (222 MHz), 70cm (432 MHz),
//        33cm (902 MHz), 20cm (1296 MHz)
// Housing: 180×60×60mm rectangular outer body
// Internal coax bore: 22mm dia (outer conductor of stub line)
// Inner conductor: 6mm dia rod (passes through housing centre)
// Plunger driven by NEMA 17 via M8×1.25 threaded rod
// ============================================================
// PRINT SETTINGS: PETG, 50% gyroid infill, 4 perimeters,
//                 0.15mm layer height for plunger bore accuracy.
//                 Print plunger with 0.10mm layers if possible.
//                 Brim on housing ends. Support-free design.
// ============================================================
// λ/4 STUB LENGTHS (electrical, in air):
//   2M    (144.2 MHz): λ/4 = 520mm — too long for this housing.
//                       Use λ/8 = 260mm + series cap for 2M.
//                       Or use a 1.5λ/4 = 1560mm coaxial resonator.
//   1.25M (222.0 MHz): λ/4 = 338mm — marginal; use λ/8 = 169mm.
//   70cm  (432.0 MHz): λ/4 = 173mm — fits housing (0–150mm range).
//   33cm  (902.0 MHz): λ/4 =  83mm — fits housing.
//   20cm  (1296 MHz):  λ/4 =  58mm — fits housing.
// Velocity factor of air line ≈ 0.97 (PTFE centre support).
// Effective electrical lengths = physical × 0.97.
// ============================================================

// --- PRINT PART SELECTOR ---
// "housing" | "plunger" | "motor_plate" | "all"
print_part = "all";

// --- GLOBAL DIMENSIONS ---
housing_x  = 180;   // total length along coax axis
housing_y  = 60;    // outer width
housing_z  = 60;    // outer height
wall       = 5.0;   // outer wall (thicker for RF shielding mass)
coax_bore  = 22.0;  // outer-conductor bore diameter
inner_rod  = 6.0;   // centre conductor rod diameter
inner_clear= 6.3;   // centre rod clearance in plunger

// Plunger geometry
plunger_d  = 21.5;  // sliding disc diameter (0.5mm clearance in bore)
plunger_l  = 15.0;  // plunger length
m8_nut_w   = 13.0;  // M8 hex nut width-across-flats
m8_nut_h   = 6.5;   // M8 hex nut height
m8_rod_d   = 8.0;   // M8 threaded rod diameter

// N-type panel mount hole
n_type_d   = 18.0;

// NEMA 17 motor plate
nema17_bolt_sp = 47.5/2 / sqrt(2);  // = 16.77mm per axis from centre
nema17_shaft_d = 5.1;   // motor shaft bore
nema17_pilot_d = 22.2;  // pilot boss clearance

motor_plate_x  = 60.0;
motor_plate_y  = 60.0;
motor_plate_t  = 6.0;

// Scale tick marks
tick_start = 0;
tick_end   = 150;
tick_step  = 10;
tick_depth = 0.8;
tick_l_maj = 5.0;   // major tick (every 10mm) length
tick_l_min = 3.0;   // minor tick (every 5mm) length

// ============================================================
// UTILITIES
// ============================================================

module m3_hole(h=8) { cylinder(h=h, d=3.3, $fn=16, center=false); }
module m4_hole(h=8) { cylinder(h=h, d=4.3, $fn=16, center=false); }
module m8_hole(h=8) { cylinder(h=h, d=8.3, $fn=20, center=false); }

module hex_pocket(waf, h) {
    // Hexagonal pocket for nut capture, waf = width-across-flats
    cylinder(h=h, d=waf * 2/sqrt(3), $fn=6);
}

// ============================================================
// SCALE TICK MARKS — engraved into housing top face
// ============================================================

module position_scale() {
    for (pos=[tick_start:5:tick_end]) {
        is_major = (pos % 10 == 0);
        tl = is_major ? tick_l_maj : tick_l_min;
        // Position along x axis; scale starts 15mm from left end face
        translate([-housing_x/2 + 15 + pos, housing_y/2 - tl/2, housing_z/2 - tick_depth])
            cube([0.8, tl, tick_depth + 0.1], center=true);
    }
    // Band calibration notches (wider mark at λ/4 positions)
    // 70cm: 173mm physical from feed end — outside scale (>150mm), mark at λ/8=86mm
    // 33cm:  83mm
    // 20cm:  58mm
    band_marks = [[86, "70cm λ/8"], [83, "33cm λ/4"], [58, "20cm λ/4"]];
    for (bm = band_marks) {
        pos = bm[0];
        if (pos <= tick_end)
            translate([-housing_x/2 + 15 + pos, housing_y/2 - tick_l_maj - 1, housing_z/2 - tick_depth*2])
                cube([2.5, tick_l_maj + 2, tick_depth*2 + 0.1], center=true);
    }
}

// ============================================================
// STUB HOUSING — main rectangular body
// ============================================================

module stub_housing() {
    difference() {
        union() {
            // Outer rectangular shell
            cube([housing_x, housing_y, housing_z], center=true);
            // End flanges for motor plate and connector end
            // Left end (connector end)
            translate([-housing_x/2 - 4, 0, 0])
                cube([8, housing_y + 10, housing_z + 10], center=true);
            // Right end (motor end)
            translate([housing_x/2 + 4, 0, 0])
                cube([8, housing_y + 10, housing_z + 10], center=true);
        }

        // ── COAX BORE (outer conductor channel) ──
        rotate([0, 90, 0])
            cylinder(h=housing_x + 20, d=coax_bore, $fn=48, center=true);

        // ── CENTRE ROD SUPPORT (PTFE bushing pockets at each end) ──
        // Rod runs full length; bushings hold it centred
        // Bushing pocket: 10mm dia × 10mm deep at each end
        for (ex=[-1, 1])
            translate([ex * (housing_x/2 + 6), 0, 0])
            rotate([0, 90, 0])
                cylinder(h=12, d=10, $fn=20, center=true);

        // ── N-TYPE CONNECTORS both ends ──
        // Left connector
        translate([-(housing_x/2 + 8), 0, 0]) rotate([0, 90, 0])
            cylinder(h=20, d=n_type_d, $fn=32, center=true);
        // Right connector (can be used as alternative feed or terminated)
        translate([(housing_x/2 + 8), 0, 0]) rotate([0, 90, 0])
            cylinder(h=20, d=n_type_d, $fn=32, center=true);

        // ── M8 THREADED ROD BORE (below coax bore) ──
        // Rod drives plunger; offset -18mm from coax axis in Z
        translate([0, 0, -18])
        rotate([0, 90, 0])
            cylinder(h=housing_x + 20, d=m8_rod_d + 0.4, $fn=20, center=true);

        // ── PLUNGER ACCESS SLOT (top of housing, full length minus ends) ──
        // Allows visual inspection and manual plunger positioning
        translate([0, 0, housing_z/2 - 1])
            cube([housing_x - 30, 8, 4], center=true);

        // ── POSITION SCALE ENGRAVING ──
        position_scale();

        // ── FLANGE MOUNTING HOLES (M4, on left and right flanges) ──
        for (fx=[-(housing_x/2 + 4), (housing_x/2 + 4)])
        for (fy=[-15, 15])
            translate([fx, fy, 0])
            rotate([0, 90, 0])
                m4_hole(h=10);
    }
}

// ============================================================
// SLIDING PLUNGER
// ============================================================
// Disc that slides in the coax bore, shorting outer to inner conductor.
// M8 nut pocket for threaded rod drive.
// 2× M3 set-screw holes to lock position on inner rod.

module sliding_plunger() {
    difference() {
        union() {
            // Main disc
            cylinder(h=plunger_l, d=plunger_d, $fn=48);
            // M8 drive arm (extends radially to connect to drive rod)
            translate([0, -(plunger_d/2 + 6), plunger_l/2 - 4])
                cube([10, 12, 8], center=true);
        }

        // Centre rod bore (sliding clearance)
        translate([0, 0, -0.1])
            cylinder(h=plunger_l + 0.2, d=inner_clear, $fn=24);

        // M8 hex nut pocket in drive arm
        translate([0, -(plunger_d/2 + 6), plunger_l/2 - 4])
        rotate([0, 0, 30])
            hex_pocket(m8_nut_w, m8_nut_h + 0.2);

        // M8 rod bore through drive arm
        translate([0, -(plunger_d/2 + 6), -0.1])
            cylinder(h=plunger_l + 0.2, d=m8_rod_d + 0.4, $fn=20);

        // 2× M3 set screws through plunger body into centre rod
        // Offset 90° apart for secure locking
        for (angle=[0, 90])
            rotate([0, 0, angle])
            translate([inner_clear/2 + 1.5, 0, plunger_l/2])
            rotate([0, 90, 0])
                m3_hole(h=plunger_d);

        // Perimeter contact slots (4× radial slots to ensure
        // good electrical contact with outer bore wall)
        for (a=[0, 90, 180, 270])
            rotate([0, 0, a])
            translate([plunger_d/2 - 1.5, 0, plunger_l - 3])
                cube([3, 1.5, 3.1], center=true);
    }
}

// ============================================================
// MOTOR END PLATE — NEMA 17 mount for M8 rod drive
// ============================================================

module motor_end_plate() {
    difference() {
        union() {
            // Main plate
            cube([motor_plate_x, motor_plate_y, motor_plate_t], center=true);
            // Flange tabs for bolting to housing end
            for (ty=[-motor_plate_y/2 - 5, motor_plate_y/2 + 5])
                translate([0, ty, 0])
                    cube([motor_plate_x, 10, motor_plate_t], center=true);
        }

        // NEMA 17 shaft bore
        cylinder(h=motor_plate_t + 0.2, d=nema17_pilot_d, $fn=32, center=true);

        // NEMA 17 bolt holes (M3, ±23.75mm × ±23.75mm pattern)
        // Using 47.5mm bolt spacing: half-diagonal = 23.75/sqrt(2)...
        // Actually NEMA 17 = 31mm bolt circle: holes at ±21.95mm from centre
        // Standard NEMA17: 4 holes on 31mm bolt circle (centre-to-centre 43.84mm diagonal)
        // Or equivalently: holes at (±15.5mm, ±15.5mm) from shaft centre
        nema_half = 15.5;
        for (nx=[-nema_half, nema_half])
        for (ny=[-nema_half, nema_half])
            translate([nx, ny, 0])
                m3_hole(h=motor_plate_t + 0.2);

        // M8 threaded rod bore (offset for drive rod, same as housing)
        translate([0, -18, 0])
            m8_hole(h=motor_plate_t + 0.2);

        // Flange mounting holes (M4, attach plate to housing end flange)
        for (ty=[-motor_plate_y/2 - 5, motor_plate_y/2 + 5])
        for (tx=[-12, 12])
            translate([tx, ty, 0])
                m4_hole(h=motor_plate_t + 0.2);

        // M8 anti-rotation slot (prevents rod from spinning freely)
        translate([-15, -18, 0])
            cube([6, 4, motor_plate_t + 0.2], center=true);
    }
}

// ============================================================
// RENDER SELECTOR
// ============================================================

if (print_part == "housing") {
    stub_housing();
} else if (print_part == "plunger") {
    sliding_plunger();
} else if (print_part == "motor_plate") {
    motor_end_plate();
} else if (print_part == "all") {
    stub_housing();
    translate([0, housing_y + 25, 0]) {
        // Show plunger in open position
        rotate([90, 0, 0]) sliding_plunger();
    }
    translate([housing_x/2 + 50, 0, 0])
        motor_end_plate();
}

// ============================================================
// OPERATIONAL NOTES
// ============================================================
// STUB TUNER THEORY:
// A shorted coaxial stub presents a reactive impedance at its
// input that varies with electrical length. For a short stub
// (l < λ/4), it appears inductive. At l = λ/4, Zin → ∞ (open).
// Above λ/4, it appears capacitive. Used for:
//   - Harmonic suppression (place stub λ/4 from TX port)
//   - Impedance matching (shunt tuning element)
//   - Band-pass/stop filtering
//
// BAND CALIBRATION (velocity factor = 0.97 for air line):
//   70cm (432 MHz): λ/4 physical = 173mm × 0.97 = 168mm effective
//                   λ/8 physical =  86mm (inductive matching)
//   33cm (902 MHz): λ/4 physical =  83mm × 0.97 =  81mm effective
//   20cm (1296 MHz):λ/4 physical =  58mm × 0.97 =  56mm effective
//   2M  (144 MHz):  λ/4 = 520mm — use an external longer housing
//                   or use λ/8 = 260mm with series capacitor.
//
// ASSEMBLY:
// 1. Inner rod: 6mm brass or copper-plated steel, full housing length.
//    Mount in PTFE bushings at each end for centre alignment.
// 2. Plunger: slide onto rod; M8 nut press-fit into hex pocket.
//    Thread M8 all-thread rod (1.25mm pitch) through nut and plate.
// 3. NEMA 17 couples to M8 rod via 5mm-to-8mm shaft coupler.
// 4. 1.25mm pitch × 200 steps/rev (with 1/16 microstepping = 3200
//    steps/rev): 1.25/3200 = 0.00039mm per microstep resolution.
// 5. N-type connectors: solder or crimp inner rod to N-type pin.
//    Outer conductor of stub bonds to N-type body/shell.
//
// PLUNGER CONTACT:
// For good RF performance, the plunger must make low-resistance
// contact with the bore wall. Options:
//   a) Conductive graphite-loaded PLA/PETG (resistive — avoid)
//   b) Metal disc insert (6061 Al or brass) turned to 21.5mm dia
//   c) Spring-finger contact ring from SMA terminator gasket
// Best practice: print plunger for structure, press-fit a
// turned 21.5mm × 14mm aluminium disc with centre bore.
// ============================================================
