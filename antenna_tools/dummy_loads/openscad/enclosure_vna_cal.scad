// ============================================================
// VNA CALIBRATION LOAD ENCLOSURE — SMA VERSION
// Precision 50-ohm load, DC to 6 GHz
// Machined brass body (CNC) or printed carrier
//
// This design produces:
//   1. PCB carrier plate (holds Rogers PCB, mounts to SMA)
//   2. Outer shell (snap-fit or threaded, provides protection)
//   3. Strain relief cap for handling
//
// Print: Resin (SLA/MSLA) for best dimensional accuracy
//        OR PETG with 0.1mm layer height
// Post:  Brass powder coat optional for EMI shielding
//
// Dimensions designed around Southwest Microwave 1092-01A-6
// end-launch SMA connector (PCB thickness 0.508mm, 1.27mm pin)
// ============================================================

/* [PCB Parameters] */
pcb_w    = 15.0;    // PCB width
pcb_l    = 15.0;    // PCB length
pcb_t    = 0.508;   // Rogers RO4003C thickness

/* [SMA Parameters] */
sma_body_d   = 8.0;   // SMA hex body OD (~hex flat to flat = 7.92mm)
sma_mating_d = 9.5;   // SMA mating interface OD
sma_thread_d = 7.92;  // SMA thread OD (3/8-32 UNEF)
sma_l        = 18.0;  // SMA end-launch body length

/* [Housing Parameters] */
shell_w   = pcb_w + 4.0;
shell_l   = pcb_l + sma_l + 8.0;
shell_h   = 12.0;
wall      = 2.0;
sma_face  = 6.0;    // face thickness around SMA

$fn = 64;

// ============================================================
// PCB carrier (mounts Rogers PCB, end-launch SMA attaches)
// ============================================================
module pcb_carrier() {
    difference() {
        union() {
            // Main body
            cube([shell_w, shell_l, shell_h], center=true);
            // SMA mating boss
            translate([0, shell_l/2, 0])
                cylinder(d=sma_mating_d + 4, h=shell_h, center=true);
        }
        // PCB slot
        cube([pcb_w + 0.1, pcb_l + 2, pcb_t + 0.2], center=true);
        // SMA body clearance
        translate([0, shell_l/2 + wall, 0])
            rotate([90, 0, 0])
                cylinder(d=sma_body_d + 0.5, h=sma_l + 4, center=true);
        // SMA mating face hole
        translate([0, shell_l/2, 0])
            rotate([90, 0, 0])
                cylinder(d=sma_mating_d, h=8, center=true);
        // Interior pocket (air around PCB)
        translate([0, -5, 0])
            cube([pcb_w + 0.5, pcb_l + 4, pcb_t + 2], center=true);
        // Label window on top
        translate([0, 0, shell_h/2 - wall/2])
            cube([shell_w - wall*2, 10, wall + 1], center=true);
        // Side grip texture slots
        for (x=[-1,1]) {
            for (i=[-1,0,1]) {
                translate([x*(shell_w/2), i*5, 0])
                    cube([wall + 1, 2, shell_h + 1], center=true);
            }
        }
    }
}

// ============================================================
// Protective outer cap (slides over carrier)
// ============================================================
module outer_cap() {
    clearance = 0.4;
    cap_w  = shell_w + wall*2 + clearance;
    cap_l  = shell_l / 2 + 5;
    cap_h  = shell_h + wall*2 + clearance;

    translate([0, -(shell_l/2 + 30), 0])
    difference() {
        cube([cap_w, cap_l, cap_h], center=true);
        // Interior
        translate([0, -wall/2, 0])
            cube([shell_w + clearance, cap_l - wall + 0.01, shell_h + clearance],
                 center=true);
        // Retention click (snap ridge inside)
        translate([0, -cap_l/4, 0])
        for (x = [-1, 1]) {
            translate([x*(shell_w/2 + clearance/2), 0, 0])
                cube([1.5, 3, shell_h + clearance], center=true);
        }
        // Label window
        translate([0, cap_l/2 - wall/2, 0])
            cube([cap_w - wall*2, wall + 1, 12], center=true);
    }
}

// ============================================================
// Anti-torque grip ring (for handling with VNA)
// ============================================================
module grip_ring() {
    translate([0, shell_l/2 + 25, 0])
    difference() {
        cylinder(d=18, h=8, center=true);
        translate([0, 0, -0.01]) cylinder(d=sma_mating_d, h=9, center=true);
        // Knurling (12 flats)
        for (a=[0:30:150]) {
            rotate([0, 0, a])
                translate([8.5, 0, 0])
                    cube([2.5, 2, 9], center=true);
        }
    }
}

// ============================================================
// N-TYPE CAL LOAD version (22mm diameter disc)
// ============================================================
module n_cal_board_holder() {
    // Holds 22mm diameter Rogers disc PCB for N-type load
    translate([shell_w + 20, 0, 0])
    difference() {
        cylinder(d=30, h=6, center=true);
        // PCB slot (22mm disc, 0.508mm thick)
        cylinder(d=22.2, h=pcb_t + 0.3, center=true);
        // N-connector bore
        cylinder(d=6.0, h=7, center=true);
        // N bolt pattern (4x #4-40)
        for (a=[45,135,225,315]) {
            translate([12.7*cos(a), 12.7*sin(a), 0])
                cylinder(d=2.9, h=7, center=true);
        }
    }
}

// ============================================================
// Render all parts
// ============================================================
pcb_carrier();
outer_cap();
grip_ring();
n_cal_board_holder();

echo("=== VNA CAL LOAD ENCLOSURES ===");
echo("SMA version: pcb_carrier + outer_cap");
echo("N-type version: n_cal_board_holder");
echo("PCB: Rogers RO4003C, 0.508mm, 15x15mm (SMA) or 22mm dia (N)");
echo("Resistors: 2x 100R 0402 thin film (SMA) or 4x 200R 0603 (N)");
echo("Print: SLA resin for best accuracy, or PETG 0.1mm layers");
echo(str("SMA housing: ", shell_w, " x ", shell_l, " x ", shell_h, " mm"));
