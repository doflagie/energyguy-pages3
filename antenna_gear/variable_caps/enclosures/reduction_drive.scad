/**
 * reduction_drive.scad — TM-VCAP-001 Rev A
 * 10:1 Planetary Gearbox for Variable Capacitor Drive
 *
 * Three-stage single-planet reduction with:
 *   Sun gear:    10T, module 1.5
 *   Planet gear: 15T, module 1.5 (3 planets)
 *   Ring gear:   40T internal, module 1.5
 *   Stage ratio: (ring/sun) + 1 = (40/10) + 1 = 5:1
 *   Two stages:  5 × 2 = 10:1 (practical approx — use worm gear for exact 10:1)
 *
 * PRACTICAL NOTE: For 10:1 precision, use a commercial worm gear set
 * (SDP/SI, McMaster, or surplus) rather than printed gears.
 * This file provides printed housing + coupling, commercial gear inserts.
 *
 * Alternatively: Two-pulley belt drive (simpler, lower backlash):
 *   Motor pulley:  20T GT2 belt pulley on NEMA 17 shaft
 *   Output pulley: 200T GT2 (200mm circumference, 63.7mm OD)
 *   Belt:          GT2 6mm × 500mm loop
 *   Ratio: 200/20 = 10:1
 *   THIS IS THE RECOMMENDED APPROACH for home construction.
 *
 * Print settings:
 *   Material: PETG
 *   Infill: 60%
 *   Wall: 4 perimeters
 *   Layer: 0.15mm (gear tooth accuracy)
 *   Supports: Yes for bearing seats
 *
 * Print envelope: 110mm × 110mm × 60mm (housing halves)
 */

// ─── Parameters (Belt Drive Approach) ─────────────────────────────────────────

// Motor pulley (20T GT2 — commercial, not printed)
motor_pulley_od = 13.0;     // 20T GT2 OD ≈ 13mm
motor_shaft_dia = 5.0;      // NEMA 17 shaft 5mm

// Output pulley (large, drives capacitor shaft — CAN be printed)
out_pulley_teeth = 60;      // 60T for 3:1 (combine with 2nd stage for 6:1 or 10:1)
                             // For 10:1 single stage: use 200T (too large to print)
                             // Use 60T + 10T second stage belt for 6:1
                             // OR: use 40T + worm gear 4:1 inline for 16:1
out_pulley_od   = out_pulley_teeth * 2.0 / M_PI;  // GT2 pitch = 2mm
out_shaft_dia   = 6.35;     // 1/4 inch capacitor shaft

// Housing
housing_od      = out_pulley_od + 30;
housing_h       = 40;

// Belt tensioner slot
slot_len        = 12;
slot_w          = 5.2;

// ─── Belt Pulley (Printed, Large) ─────────────────────────────────────────────

module gt2_tooth(h) {
    // GT2 belt tooth profile (simplified)
    translate([0, 0, 0])
    linear_extrude(h)
    polygon(points=[
        [-0.75,  0],
        [-0.50,  0.75],
        [ 0,     1.00],
        [ 0.50,  0.75],
        [ 0.75,  0],
    ]);
}

module gt2_pulley(teeth, bore_dia, width = 10) {
    pitch = 2.0;  // GT2 pitch = 2mm
    r_pitch = teeth * pitch / (2 * M_PI);
    r_root  = r_pitch - 0.764;  // GT2 root radius offset
    flange_h = 1.5;
    flange_od = r_pitch * 2 + 4;

    difference() {
        union() {
            // Pulley body
            cylinder(h=width, r=r_root, $fn=teeth*2, center=true);
            // Bottom flange
            translate([0,0,-width/2-flange_h])
                cylinder(h=flange_h, d=flange_od, $fn=64);
            // Top flange
            translate([0,0,width/2])
                cylinder(h=flange_h, d=flange_od, $fn=64);
        }
        // Tooth gaps (material removed between teeth)
        for (i=[0:teeth-1]) {
            rotate([0,0,i*(360/teeth)])
            translate([r_root-0.3, 0, 0])
            rotate([0,90,0])
                gt2_tooth(width+1);
        }
        // Shaft bore
        translate([0,0,-width/2-flange_h-0.5])
            cylinder(h=width+2*flange_h+1, d=bore_dia+0.3, $fn=32);
        // Set screw hole (M3)
        translate([bore_dia/2+1.5, 0, 0])
        rotate([90,0,0])
            cylinder(h=r_root+2, d=2.5, $fn=16, center=true);
    }
}

// ─── Motor Mount Plate with Tensioner Slot ────────────────────────────────────

module motor_plate() {
    // NEMA 17 face plate with tensioner slot for belt adjustment
    plate_w = housing_od + 10;
    plate_h = housing_od + 10;
    thick   = 5;

    difference() {
        hull() {
            for (x=[-1,1]) for (y=[-1,1])
                translate([x*(plate_w/2-6), y*(plate_h/2-6), 0])
                    cylinder(h=thick, r=6, $fn=32);
        }
        // Output shaft bore
        translate([0, 0, -0.5])
            cylinder(h=thick+1, d=out_shaft_dia+1, $fn=32);

        // Motor mounting holes + tensioner slots (4× M3)
        nema17_bolt = 15.5;
        for (x=[-1,1]) for (y=[-1,1]) {
            translate([x*nema17_bolt, y*nema17_bolt, -0.5])
            hull() {
                cylinder(h=thick+1, d=3.4, $fn=16);
                translate([x*slot_len/2, 0, 0])
                    cylinder(h=thick+1, d=3.4, $fn=16);
            }
        }
        // NEMA 17 pilot bore
        translate([0, 0, -0.5])
            cylinder(h=thick+1, d=22.2, $fn=64);

        // Housing mounting holes
        for (a=[45,135,225,315])
            rotate([0,0,a])
            translate([housing_od/2 - 5, 0, -0.5])
                cylinder(h=thick+1, d=3.3, $fn=16);
    }
}

// ─── Housing Cover ────────────────────────────────────────────────────────────

module housing_cover() {
    // Protective cover over belt drive, open front for inspection
    difference() {
        union() {
            difference() {
                cylinder(h=housing_h, d=housing_od+6, $fn=64);
                // Main cavity
                translate([0,0,3])
                    cylinder(h=housing_h, d=housing_od, $fn=64);
                // Inspection window (half cutout)
                translate([housing_od/4, -housing_od, 3])
                    cube([housing_od, housing_od*2, housing_h]);
            }
            // Bottom flange
            difference() {
                cylinder(h=3, d=housing_od+10, $fn=64);
                cylinder(h=4, d=housing_od-4, $fn=64);
            }
        }
        // Shaft exit
        translate([0,0,-0.5])
            cylinder(h=housing_h+1, d=out_shaft_dia+3, $fn=32);
        // M3 mounting holes in flange
        for (a=[45,135,225,315])
            rotate([0,0,a])
            translate([(housing_od+10)/2-5, 0, -0.5])
                cylinder(h=6, d=3.3, $fn=16);
    }
}

// ─── Output Shaft Bearing Block ───────────────────────────────────────────────

module shaft_bearing_block() {
    // Holds 624ZZ bearing (4mm bore, 13mm OD, 5mm width) for output shaft
    block_w = 20;
    block_h = 20;
    block_d = 15;
    bearing_od = 13.2;
    bearing_w  = 5.0;

    difference() {
        hull() {
            cube([block_w, block_d, block_h], center=true);
        }
        // Bearing press-fit bore
        rotate([90,0,0])
            cylinder(h=block_d+1, d=bearing_od, $fn=32, center=true);
        // Shaft through-bore
        rotate([90,0,0])
            cylinder(h=block_d+1, d=out_shaft_dia+0.3, $fn=32, center=true);
        // M3 mounting holes
        for (x=[-1,1])
            translate([x*7, 0, -block_h/2-0.5])
                cylinder(h=block_h+1, d=3.3, $fn=16);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 64;

// Print the large output pulley
color("lightblue")
gt2_pulley(out_pulley_teeth, out_shaft_dia, width=10);

// Motor plate (flat on bed, separately)
color("lightgreen")
translate([housing_od + 20, 0, 0])
    motor_plate();

// Housing cover
color("gray", 0.5)
translate([0, housing_od + 20, 0])
    housing_cover();

// Shaft bearing block
color("orange")
translate([-(housing_od + 20), 0, 0])
    shaft_bearing_block();

// ─── Notes ────────────────────────────────────────────────────────────────────
// Belt drive assembly:
// 1. Print large pulley (60T, 63.7mm OD) for output shaft
// 2. Purchase NEMA 17 20T GT2 pulley (motor shaft)
// 3. Purchase GT2 6mm belt, ~400mm loop (measure center-to-center × 2 + pulleys)
// 4. Install tensioner: loosen 4× motor bolts, shift motor back 5mm, retighten
//
// For 10:1 exact: Add second 2:1 stage (20T + 40T pulley) OR use worm gear from
// gear_mechanism.scad in roller_inductors project (identical mechanism, different coupling)
//
// Coupling to capacitor shaft: use shaft_coupler.scad
// CRITICAL: coupling must be electrically insulating — see shaft_coupler.scad
