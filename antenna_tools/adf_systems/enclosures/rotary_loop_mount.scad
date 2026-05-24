/**
 * rotary_loop_mount.scad — TM-ADF-ENC-002 Rev A
 * Motorized Rotary Loop RDF Antenna Mount
 *
 * Provides: vertical rotation shaft for 400mm copper loop,
 *           NEMA 17 motor mount (bottom), GT2 belt drive (3:1),
 *           Faraday shield gap alignment bracket,
 *           coax feed-through at rotation center.
 *
 * Print in two parts: base_mount + shaft_pulley
 * Material: PETG, 50% gyroid, 4 perimeters, 0.15mm layers (bearing bore)
 * Envelope: base_mount 110×110×80mm, shaft_pulley 65×65×30mm
 * Document: TM-ADF-ENC-002 Rev A
 */

// ─── Parameters ───────────────────────────────────────────────

// Base plate
base_w    = 110.0;
base_d    = 110.0;
base_h    = 12.0;
base_r    = 8.0;

// Vertical post / bearing housing
post_h    = 70.0;
post_od   = 30.0;    // outer diameter of bearing housing
post_id   = 22.0;    // bearing bore (608ZZ = 22mm OD, or 22mm for custom bearing)
bearing_h = 7.0;     // 608ZZ height

// Rotation shaft (center)
shaft_od  = 8.0;     // shaft diameter
shaft_h   = 100.0;   // total shaft height (extends above post)
coax_bore = 5.5;     // coax through center of shaft (for RG-316)

// GT2 driven pulley (60T) on shaft
gt2_60t_od = 39.2;   // GT2 60-tooth OD
gt2_60t_h  = 13.0;
gt2_pitch_dia = 38.2;
belt_depth = 1.4;    // GT2 tooth depth

// NEMA 17 motor mount (below base)
nema17_face = 42.3;  // mounting face OD (actually square)
nema17_bolt = 31.0;  // bolt circle (center to center, 31mm)
nema17_shaft= 22.0;  // shaft protrusion
motor_offset_z = -45.0;  // motor hangs below base plate

// GT2 driver pulley (20T) on motor shaft
gt2_20t_od = 13.0;
gt2_20t_h  = 12.0;

// Belt tensioner slot
belt_slot_len = 15.0;
belt_slot_w   = 6.0;

// Loop arm bracket (mounts at top of shaft)
arm_tab_w = 20.0;
arm_tab_d = 10.0;
arm_tab_h = 15.0;

// Mounting holes for base (4× M5 to tripod / camera mount / mast)
mount_bolt_d = 5.3;
mount_bolt_sp = base_w - 20;

// ─── Modules ──────────────────────────────────────────────────

module rounded_base(w, d, h, r) {
    hull()
        for (x=[r, w-r]) for (y=[r, d-r])
            translate([x, y, 0]) cylinder(h=h, r=r, $fn=24);
}

module bearing_housing() {
    // Vertical post with bearing pocket top and bottom
    difference() {
        cylinder(h=post_h, d=post_od, $fn=32);

        // Top bearing pocket
        translate([0, 0, post_h - bearing_h - 0.5])
            cylinder(h=bearing_h+1, d=post_id+0.2, $fn=32);

        // Bottom bearing pocket
        translate([0, 0, -0.5])
            cylinder(h=bearing_h+1, d=post_id+0.2, $fn=32);

        // Shaft clearance through center
        translate([0, 0, -0.5])
            cylinder(h=post_h+1, d=shaft_od+0.4, $fn=24);

        // Coax bore (eccentric by 3mm for coax routing)
        translate([3, 0, -0.5])
            cylinder(h=post_h+1, d=coax_bore, $fn=16);
    }
}

module gt2_pulley_60t() {
    // 60-tooth GT2 pulley with set screw
    difference() {
        union() {
            // Belt groove section
            cylinder(h=gt2_60t_h, d=gt2_60t_od, $fn=64);
            // Flanges
            translate([0, 0, -2]) cylinder(h=2, d=gt2_60t_od+3, $fn=48);
            translate([0, 0, gt2_60t_h]) cylinder(h=2, d=gt2_60t_od+3, $fn=48);
        }
        // Shaft bore
        translate([0, 0, -2.5])
            cylinder(h=gt2_60t_h+5, d=shaft_od+0.15, $fn=24);
        // Set screw M3 (2×, 90° offset)
        for (a=[0, 90])
            rotate([0, 0, a])
            translate([gt2_60t_od/2+1, 0, gt2_60t_h/2])
            rotate([90, 0, 0])
                cylinder(h=gt2_60t_od, d=3.2, $fn=16, center=true);
        // Belt teeth (simplified — 60 rectangular notches)
        for (i=[0:59]) {
            rotate([0, 0, i * 6])
            translate([gt2_60t_od/2 - belt_depth, 0, 2])
                cube([belt_depth+0.5, 1.5, gt2_60t_h-4], center=true);
        }
    }
}

module nema17_mount_plate() {
    // Motor mounting plate that attaches below base
    plate_w = 60.0; plate_d = 60.0; plate_h = 6.0;
    difference() {
        translate([base_w/2 - plate_w/2, base_d/2 - plate_d/2, motor_offset_z])
            rounded_base(plate_w, plate_d, plate_h, 5);

        // NEMA 17 face opening (28mm pilot hole for shaft + boss)
        translate([base_w/2, base_d/2, motor_offset_z - 0.5])
            cylinder(h=plate_h+1, d=22.5, $fn=32);

        // NEMA 17 bolt holes (4× M3 at 31mm bolt circle)
        for (a=[45, 135, 225, 315])
            translate([base_w/2 + cos(a)*nema17_bolt/2,
                       base_d/2 + sin(a)*nema17_bolt/2,
                       motor_offset_z - 0.5])
                cylinder(h=plate_h+1, d=3.3, $fn=16);

        // Belt slot (for belt access and tension adjustment)
        translate([base_w/2 - belt_slot_w/2, base_d/2 + 10,
                   motor_offset_z - 0.5])
            cube([belt_slot_w, belt_slot_len, plate_h+1]);

        // Attach to base via M4 screws (4× corners)
        for (x=[-1,1]) for (y=[-1,1])
            translate([base_w/2 + x*20, base_d/2 + y*20, motor_offset_z-0.5])
                cylinder(h=plate_h+1, d=4.3, $fn=16);
    }
}

module base_mount() {
    difference() {
        union() {
            rounded_base(base_w, base_d, base_h, base_r);
            // Bearing housing post at center
            translate([base_w/2, base_d/2, base_h])
                bearing_housing();
        }

        // Central shaft hole through base
        translate([base_w/2, base_d/2, -0.5])
            cylinder(h=base_h+1, d=shaft_od+0.5, $fn=24);

        // Coax routing hole
        translate([base_w/2 + 3, base_d/2, -0.5])
            cylinder(h=base_h+1, d=coax_bore, $fn=16);

        // 4× mounting holes (M5 for tripod head or clamp)
        for (x=[-1,1]) for (y=[-1,1])
            translate([base_w/2 + x*mount_bolt_sp/2,
                       base_d/2 + y*mount_bolt_sp/2, -0.5])
                cylinder(h=base_h+1, d=mount_bolt_d, $fn=16);

        // Weight relief pockets
        for (xi=[-1,1]) for (yi=[-1,1])
            translate([base_w/2 + xi*22, base_d/2 + yi*22, 3])
                cylinder(h=base_h+1, d=18, $fn=24);
    }

    // NEMA 17 mount plate
    nema17_mount_plate();
}

module loop_arm_bracket() {
    // Clamp bracket that attaches to top of shaft, holds loop frame
    difference() {
        union() {
            cylinder(h=arm_tab_h, d=shaft_od+8, $fn=24);
            translate([-arm_tab_w/2, 0, 0])
                cube([arm_tab_w, arm_tab_d, arm_tab_h]);
        }
        // Shaft bore
        translate([0, 0, -0.5])
            cylinder(h=arm_tab_h+1, d=shaft_od+0.15, $fn=24);
        // Set screw M3
        translate([shaft_od/2+5, 0, arm_tab_h/2])
        rotate([90, 0, 0])
            cylinder(h=shaft_od+10, d=3.2, $fn=16, center=true);
        // Loop attachment holes (2× M4)
        for (x=[-1,1])
            translate([x*6, arm_tab_d/2, arm_tab_h/2])
            rotate([90, 0, 0])
                cylinder(h=arm_tab_d+1, d=4.3, $fn=16, center=true);
    }
}

// ─── Render ───────────────────────────────────────────────────
$fn = 48;
print_part = "base";  // "base" | "pulley" | "bracket" | "all"

if (print_part == "base") {
    color("darkgray") base_mount();
} else if (print_part == "pulley") {
    color("silver") gt2_pulley_60t();
} else if (print_part == "bracket") {
    color("lightgray") loop_arm_bracket();
} else {
    color("darkgray", 0.85) base_mount();
    color("silver", 0.7)
    translate([base_w/2, base_d/2, base_h + post_h + 5])
        gt2_pulley_60t();
}
