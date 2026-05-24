/**
 * shaft_coupler.scad — TM-VCAP-001 Rev A
 * Insulating Shaft Coupler and Position Potentiometer Mount
 *
 * THREE modules in this file:
 *   1. insulating_coupler()    — Motor to capacitor shaft, HIGH VOLTAGE ISOLATION
 *   2. pot_mount()             — Pot coupled to capacitor shaft for position sensing
 *   3. shaft_collar()          — Plain set-screw collar for shaft retention
 *
 * CRITICAL SAFETY NOTE:
 *   The capacitor shaft is at RF potential during transmit.
 *   At 1 kW into a mismatched load, the capacitor shaft can be at
 *   hundreds to thousands of volts RF.
 *   The coupling between motor shaft and capacitor shaft MUST be electrically
 *   insulating. This coupler uses a Delrin (acetal) center section.
 *   Material: Delrin (POM-C): dielectric strength 20 kV/mm
 *   Coupling length: 25 mm minimum for 5 kV rating
 *   (25 mm × 20 kV/mm = 500 kV breakdown >> 5 kV operating)
 *
 * Print settings:
 *   Material:    Delrin/POM or PETG (NOT carbon-filled or conductive filament)
 *   Infill:      100% (solid — no voids for high voltage)
 *   Wall:        Maximum perimeters
 *   Layer:       0.15mm
 *
 * Print envelope: 80mm × 50mm × 50mm total
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// Shaft dimensions
motor_shaft_d   = 5.0;    // NEMA 17 motor shaft
cap_shaft_d     = 6.35;   // 1/4 inch capacitor shaft (6.35mm = 0.25 in)
pot_shaft_d     = 6.0;    // Potentiometer shaft (most pots: 6mm)

// Insulating coupler
coupler_od      = 20.0;   // Outer diameter
coupler_len     = 50.0;   // Total length (25mm per shaft half + 0mm gap)
                           // Must be ≥ 25mm for 5kV; ≥ 40mm for 10kV
hub_len         = 20.0;   // Length of each hub (motor hub + cap hub)
gap_len         = coupler_len - 2*hub_len;  // Center gap (insulating zone)

// Set screw positions
ss_dia          = 2.5;    // M3 set screw minor thread diameter
ss_recess       = 3.0;    // Set screw recess depth

// Key flat (prevents shaft rotation without full key)
key_flat_depth  = 1.0;

// Anti-static discharge groove (prevents static charge buildup on coupler)
// Leave grooves empty — air gaps interrupt any leakage path
groove_d        = 2.0;
groove_pitch    = 5.0;

// Potentiometer coupling
pot_coupler_od  = 14.0;
pot_coupler_len = 30.0;
pot_hub_len     = 12.0;

// Shaft collar
collar_od       = 14.0;
collar_len      = 12.0;

// ─── Insulating Coupler (Motor ↔ Capacitor) ───────────────────────────────────

module insulating_coupler() {
    // Motor hub (bottom): connects to NEMA 17 shaft
    // Insulating section (center): Delrin, no voids, full density
    // Capacitor hub (top): connects to capacitor shaft

    difference() {
        // Solid outer body
        cylinder(h=coupler_len, d=coupler_od, $fn=64);

        // Motor shaft bore (bottom half)
        translate([0, 0, -0.5])
            cylinder(h=hub_len+1, d=motor_shaft_d+0.3, $fn=32);

        // Capacitor shaft bore (top half)
        translate([0, 0, coupler_len-hub_len-0.5])
            cylinder(h=hub_len+1, d=cap_shaft_d+0.3, $fn=32);

        // Motor shaft set screw (from side, M3)
        translate([coupler_od/2+0.5, 0, hub_len/2])
        rotate([0,-90,0])
            cylinder(h=coupler_od/2+ss_recess, d=ss_dia, $fn=16);

        // Cap shaft set screw (from opposite side for balance)
        translate([-coupler_od/2-0.5, 0, coupler_len-hub_len/2])
        rotate([0,90,0])
            cylinder(h=coupler_od/2+ss_recess, d=ss_dia, $fn=16);

        // Motor shaft key flat
        translate([motor_shaft_d/2-key_flat_depth, -motor_shaft_d/2, -0.5])
            cube([key_flat_depth+1, motor_shaft_d, hub_len+1]);

        // Cap shaft key flat
        translate([cap_shaft_d/2-key_flat_depth, -cap_shaft_d/2, coupler_len-hub_len-0.5])
            cube([key_flat_depth+1, cap_shaft_d, hub_len+1]);

        // Anti-creepage grooves (ring grooves in center section)
        // These do NOT reduce breakdown — they extend creepage path length
        for (z=[hub_len+groove_pitch/2 : groove_pitch : coupler_len-hub_len-groove_pitch/2]) {
            translate([0, 0, z])
                difference() {
                    cylinder(h=2, d=coupler_od+0.1, $fn=64, center=true);
                    cylinder(h=3, d=coupler_od-groove_d*2, $fn=64, center=true);
                }
        }
    }

    // HV rating label impressed in surface
    // Note: engrave "5kV ISOLATION" on side for safety labeling
}

module insulating_coupler_label() {
    // Text labels indicating voltage rating (for post-print marking)
    color("red")
    translate([coupler_od/2 + 0.1, 0, coupler_len/2])
    rotate([0,90,0])
    linear_extrude(0.5)
    text("5kV ISO", size=3, halign="center", valign="center");
}

// ─── Potentiometer Coupler ────────────────────────────────────────────────────

module pot_coupler() {
    // Couples capacitor shaft to 10kΩ position potentiometer shaft
    // NOTE: This is ALSO at RF potential — must be mounted to isolated pot body
    // OR: use optical encoder to avoid electrical coupling entirely

    difference() {
        cylinder(h=pot_coupler_len, d=pot_coupler_od, $fn=64);

        // Cap shaft bore (bottom)
        translate([0, 0, -0.5])
            cylinder(h=pot_hub_len+1, d=cap_shaft_d+0.3, $fn=32);

        // Pot shaft bore (top)
        translate([0, 0, pot_coupler_len-pot_hub_len-0.5])
            cylinder(h=pot_hub_len+1, d=pot_shaft_d+0.3, $fn=32);

        // Cap shaft set screw
        translate([pot_coupler_od/2+0.5, 0, pot_hub_len/2])
        rotate([0,-90,0])
            cylinder(h=pot_coupler_od, d=ss_dia, $fn=16);

        // Pot shaft set screw
        translate([-pot_coupler_od/2-0.5, 0, pot_coupler_len-pot_hub_len/2])
        rotate([0,90,0])
            cylinder(h=pot_coupler_od, d=ss_dia, $fn=16);

        // Flat for pot shaft
        translate([pot_shaft_d/2-0.8, -pot_shaft_d, pot_coupler_len-pot_hub_len-0.5])
            cube([1.0, pot_shaft_d*2, pot_hub_len+1]);
    }
}

// ─── Pot Mounting Bracket ─────────────────────────────────────────────────────

module pot_bracket() {
    // L-bracket to hold pot body, allows shaft alignment adjustment
    // Mounts to capacitor frame or chassis
    br_w  = 30;
    br_h  = 40;
    br_d  = 6;
    pot_b_dia = 7.0;    // Pot body thread boss OD clearance
    pot_nut_h = 8.0;    // Nut hex flat to flat (10kΩ pot panel nut)

    difference() {
        union() {
            cube([br_w, br_d, br_h]);
            translate([0, br_d, 0]) cube([br_w, 12, br_d]);
        }
        // Pot shaft hole
        translate([br_w/2, br_d/2, br_h/2])
        rotate([90,0,0])
            cylinder(h=br_d+1, d=pot_b_dia, $fn=32, center=true);
        // Pot anti-rotation flat slot
        translate([br_w/2, br_d/2, br_h/2+3])
        rotate([90,0,0])
            cube([pot_nut_h, 2, br_d+1], center=true);
        // Mounting holes (on base flange)
        for (x=[-1,1])
            translate([br_w/2 + x*10, br_d+6, -0.5])
                cylinder(h=br_d+1, d=3.3, $fn=16);
    }
}

// ─── Plain Shaft Collar ───────────────────────────────────────────────────────

module shaft_collar(shaft_dia=cap_shaft_d) {
    // Retention collar (prevents axial shaft movement)
    difference() {
        cylinder(h=collar_len, d=collar_od, $fn=32);
        // Shaft bore
        translate([0,0,-0.5])
            cylinder(h=collar_len+1, d=shaft_dia+0.25, $fn=32);
        // Set screw (M3)
        translate([collar_od/2+0.5, 0, collar_len/2])
        rotate([0,-90,0])
            cylinder(h=collar_od, d=ss_dia, $fn=16);
        // Shaft flat
        translate([shaft_dia/2-0.8, -shaft_dia, -0.5])
            cube([1, shaft_dia*2, collar_len+1]);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 64;

// Main insulating coupler (print in Delrin/POM or PETG, 100% infill)
color("white", 0.9)
insulating_coupler();

// Pot coupler (adjacent, same material)
color("lightblue")
translate([30, 0, 0])
    pot_coupler();

// Pot bracket
color("lightgreen")
translate([55, 0, 0])
    pot_bracket();

// Shaft collar
color("orange")
translate([90, 0, 0])
    shaft_collar(cap_shaft_d);

// ─── Notes ────────────────────────────────────────────────────────────────────
// MATERIAL SELECTION:
//   Delrin (POM-C): Best. Machinable, 20kV/mm, available in filament (rare)
//   PETG:           Good. Printable, 20kV/mm, moisture resistant
//   PLA:            Avoid for HV. Hygroscopic, reduces surface resistivity
//   ABS:            Acceptable. Less moisture sensitive than PLA
//   Nylon:          Avoid. Highly hygroscopic, HV rating degrades with humidity
//
// ASSEMBLY:
// 1. Install motor shaft key flat first (file if needed)
// 2. Press motor shaft into coupler bottom, tighten M3 set screw firmly
// 3. Install capacitor shaft key flat
// 4. Slide coupler onto capacitor shaft, leave 2mm shaft protrusion beyond coupler
// 5. Tighten M3 set screw
// 6. Verify: no RF continuity between motor shaft and capacitor shaft
//    (ohmmeter: should read > 10MΩ)
