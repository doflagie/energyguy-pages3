/**
 * wire_tensioner.scad — TM-CWND-001 Rev A
 * Wire Tensioner and Feed Guide Assembly
 *
 * Maintains consistent wire tension from spool to coil former.
 * Prevents wire loops, kinks, and variable turn spacing.
 *
 * Three modules:
 *   1. tensioner_base()  — Mounts to machine frame, holds spool post + tensioner
 *   2. tensioner_arm()   — Spring-loaded lever, adjustable tension
 *   3. wire_guide_eye()  — Ceramic or PTFE guide eyelet (or print with smooth bore)
 *
 * Design philosophy:
 *   - Spring-loaded arm with adjustable preload
 *   - Wire path: spool → post guide → tensioner arm → wire guide carriage
 *   - Tension controlled by spring preload bolt
 *   - Wire break detection when arm swings to zero-tension limit
 *   - Supports AWG 10 (2.6mm) to AWG 30 (0.3mm) with appropriate eyelet size
 *
 * Print settings:
 *   Material: PETG
 *   Infill: 50%
 *   Wall: 3 perimeters
 *   Layer: 0.2mm
 *   Print envelope: 180×80×60mm (all parts fit together)
 *
 * Hardware:
 *   Spring: 10mm OD, 40mm free length, ~0.5N/mm (McMaster 9657K113 or similar)
 *   Eyelet: 3mm bore (AWG 10–16), 1.5mm bore (AWG 17–30)
 *   Pivot: M4×25mm bolt + flanged bearing F624ZZ (4mm bore, 13mm OD)
 *   Tension adjustment: M4 thumbscrew × 1
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// Base plate
base_w          = 80.0;
base_d          = 40.0;
base_h          = 8.0;
base_corner_r   = 5.0;

// Vertical post (supports spool axle + guide)
post_h          = 80.0;
post_w          = 15.0;
post_d          = base_d;
post_x          = base_w - post_w/2 - 5;

// Spool post hole (accommodates standard spool 10mm axle hole)
spool_axle_dia  = 10.5;  // 10mm axle + clearance
spool_height    = 50.0;  // Height on post where spool axle mounts

// Tensioner arm pivot
pivot_h         = 60.0;  // Height above base
pivot_x         = base_w/2;
pivot_dia       = 4.0;   // M4 pivot bolt
pivot_boss_od   = 14.0;  // Bearing/boss OD
pivot_boss_h    = 8.0;

// Tensioner arm body
arm_len         = 80.0;
arm_w           = 10.0;
arm_h           = 8.0;

// Wire eye on arm
arm_eye_x       = arm_len - 10;  // Distance from pivot to eye center
arm_eye_dia     = 3.2;           // Wire passage hole (3mm + clearance)
arm_eye_boss_od = 12.0;

// Spring seat
spring_seat_od  = 10.0;
spring_seat_d   = 5.0;   // Pocket depth
spring_x        = arm_len / 2;  // Position along arm

// Zero-tension stop (wire break detection switch contact)
stop_h          = 5.0;
stop_w          = 12.0;

// Tension adjuster (thumbscrew compresses spring)
adj_hole_dia    = 4.3;   // M4 clearance hole
adj_x           = pivot_x;
adj_base_y      = base_d - 15;

// Base mounting holes (2× M4 to machine frame)
mount_hole_dia  = 4.3;
mount_hole_sp   = base_w - 20;

// Lower wire guide (fixed post)
lower_guide_h   = post_h - 20;
lower_guide_dia = 6.0;   // Round post, smooth bore groove

// ─── Modules ──────────────────────────────────────────────────────────────────

module tensioner_base() {
    difference() {
        union() {
            // Base footprint
            hull() {
                for (x=[base_corner_r, base_w-base_corner_r])
                for (y=[base_corner_r, base_d-base_corner_r])
                    translate([x, y, 0])
                        cylinder(h=base_h, r=base_corner_r, $fn=16);
            }

            // Vertical post (back of base)
            translate([post_x-post_w/2, 0, 0])
                cube([post_w, post_d, post_h + base_h]);

            // Pivot support boss
            translate([pivot_x, base_d/2, base_h + pivot_h])
                cylinder(h=pivot_boss_h, d=pivot_boss_od, $fn=32);

            // Tension adjuster tower
            translate([adj_x-6, adj_base_y, base_h])
                cube([12, 8, pivot_h - 5]);
        }

        // Spool axle slot on vertical post (slotted for easy spool loading)
        translate([post_x, base_d/2, base_h + spool_height]) {
            cylinder(h=post_w+2, d=spool_axle_dia, $fn=16, center=true);
            // Open slot from front for easy threading
            translate([0, -spool_axle_dia/2, 0])
                cube([spool_axle_dia, spool_axle_dia, post_w+2], center=true);
        }

        // Lower wire guide hole in post
        translate([post_x, base_d/2, base_h + lower_guide_h])
            rotate([90,0,0])
                cylinder(h=post_d+1, d=lower_guide_dia, $fn=16, center=true);

        // Pivot bore
        translate([pivot_x, base_d/2, base_h + pivot_h])
        rotate([90,0,0])
            cylinder(h=base_d+1, d=pivot_dia+0.3, $fn=16, center=true);

        // Tension adjuster M4 threaded hole
        translate([adj_x, adj_base_y+4, base_h + 10])
        rotate([-90,0,0])
            cylinder(h=pivot_h, d=adj_hole_dia, $fn=16);

        // Base mounting holes
        for (x=[10, base_w-10])
            translate([x, base_d/2, -0.5])
                cylinder(h=base_h+1, d=mount_hole_dia, $fn=16);

        // Weight relief
        translate([10, 5, 3])
            cube([base_w-20, base_d-10, base_h]);
    }
}

module tensioner_arm() {
    difference() {
        union() {
            // Arm body
            hull() {
                cylinder(h=arm_h, d=pivot_boss_od+2, $fn=32);
                translate([arm_len, 0, 0])
                    cylinder(h=arm_h, d=arm_eye_boss_od, $fn=32);
            }
            // Spring seat boss
            translate([spring_x, -arm_w/2, 0])
                cylinder(h=arm_h + spring_seat_d, d=spring_seat_od+4, $fn=16);
            // Zero-tension stop tab (contacts limit switch)
            translate([-arm_len*0.2, arm_w/2, 0])
                cube([stop_w, stop_h, arm_h]);
        }
        // Pivot hole (center)
        translate([0, 0, -0.5])
            cylinder(h=arm_h+1, d=pivot_dia+0.3, $fn=16);

        // Wire eye
        translate([arm_len, 0, -0.5])
            cylinder(h=arm_h+1, d=arm_eye_dia, $fn=16);

        // Spring seat pocket
        translate([spring_x, -arm_w/2, arm_h])
            cylinder(h=spring_seat_d+0.5, d=spring_seat_od+0.3, $fn=16);

        // Material relief
        translate([arm_len*0.2, -arm_w/2+2, 2])
            cube([arm_len*0.5, arm_w-4, arm_h-2]);
    }
}

module wire_guide_eye(bore_dia=3.2) {
    // Small guide eyelet, print with smooth bore or install commercial ceramic insert
    // Can be printed in 0.1mm layers for smooth wire passage
    eye_od = bore_dia + 8.0;
    eye_h  = 10.0;
    difference() {
        cylinder(h=eye_h, d=eye_od, $fn=32);
        // Wire bore with gentle entry chamfers
        translate([0,0,-0.5])
            cylinder(h=eye_h+1, d=bore_dia, $fn=16);
        // Entry chamfer top
        translate([0,0,eye_h-2])
            cylinder(h=3, d1=bore_dia, d2=bore_dia+2, $fn=16);
        // Entry chamfer bottom
        translate([0,0,-0.5])
            cylinder(h=3, d1=bore_dia+2, d2=bore_dia, $fn=16);
    }
}

module spool_axle() {
    // Simple spool axle rod with end stop
    // 10mm OD × 100mm, with 15mm OD flange on one end
    union() {
        cylinder(h=100, d=10, $fn=16);
        translate([0,0,95]) cylinder(h=5, d=18, $fn=16);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 32;
print_part = "all";  // "base" | "arm" | "eye3" | "eye15" | "axle" | "all"

if (print_part == "base") {
    color("lightgray") tensioner_base();
} else if (print_part == "arm") {
    color("orange") tensioner_arm();
} else if (print_part == "eye3") {
    color("white") wire_guide_eye(3.2);
} else if (print_part == "eye15") {
    color("white") wire_guide_eye(1.6);
} else if (print_part == "axle") {
    color("lightblue") spool_axle();
} else {
    color("lightgray", 0.8) tensioner_base();
    color("orange", 0.8)
    translate([pivot_x, base_d/2, base_h + pivot_h])
    rotate([90,0,0])
        tensioner_arm();
    color("white")
    translate([pivot_x + arm_len, base_d/2, base_h + pivot_h])
        wire_guide_eye(3.2);
}

// ─── Notes ────────────────────────────────────────────────────────────────────
// WIRE PATH (front view):
//   Spool (on post axle slot)
//     ↓ (over lower guide post)
//     ↓ (through tensioner arm eye)
//     ↓ → to wire guide carriage
//
// TENSION ADJUSTMENT:
//   Turn thumb screw CW → compresses spring → increases tension
//   Start with minimum tension (just enough to prevent wire sag)
//   Heavy wire (AWG10): ~3–5N, Fine wire (AWG24+): 0.3–0.5N
//   Too much tension: wire breaks or stretches (especially for fine magnet wire)
//   Too little tension: uneven winding, wire falls off
//
// WIRE BREAK DETECTION:
//   When wire breaks, tension disappears, spring swings arm to high position
//   Limit micro-switch at high arm position → firmware FAULT: WIRE_BREAK
//   Mount SS-5GL micro-switch with roller lever at top of swing arc
