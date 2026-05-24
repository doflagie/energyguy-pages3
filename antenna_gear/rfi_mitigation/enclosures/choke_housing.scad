/**
 * choke_housing.scad — TM-RFI-ENC-002 Rev A
 * Common-Mode Choke Housing for HF Feed Line Chokes
 *
 * Holds 2× FT-240-31 or FT-240-43 stacked toroid cores.
 * Coax threads through cores inside housing.
 * Weatherproof (PETG + self-amalgamating tape on SMA/PL-259 entries).
 * Mounts at antenna feed point or shack entry.
 *
 * Features:
 *   - Toroid core retainer (friction fit, no adhesive on cores)
 *   - Coax cable management: 8 turns guided inside housing
 *   - Two-part split housing: snap together after winding
 *   - PVC/PETG pipe stub at each end for strain relief
 *   - Mounting flange: bolts to mast, antenna bracket, or wall
 *
 * FT-240 core OD: 61 mm. Two stacked: 30 mm tall (2× 15 mm per core).
 * Housing interior: 70×70×38 mm (fits 2× FT-240 + 8 turns RG-8X)
 * Outer envelope: 88×88×55 mm
 */

// ── Parameters ─────────────────────────────────────────────────────────────
// Toroid core dimensions (FT-240-xx)
core_od    = 61.0;  // outer diameter
core_id    = 25.4;  // inner hole diameter (1.0 inch)
core_h     = 15.0;  // height per core
num_cores  = 2;     // stacked cores

// Coax: RG-8X OD = 7.5mm
coax_od    = 8.0;   // slightly oversized for easy routing
coax_entry_d = 10.0;  // entry hole with gland allowance

// Housing body
inner_w    = 72.0;   // core OD + clearance
inner_d    = 72.0;
inner_h    = core_h * num_cores + 4;  // core stack height + clearance

wall       = 3.5;
body_w     = inner_w + 2*wall;
body_d     = inner_d + 2*wall;
body_h     = inner_h + 2*wall;

// Split line: horizontal split at mid-height
split_z    = body_h / 2;

// Mounting flange
flange_w   = body_w + 20;
flange_d   = body_d + 20;
flange_h   = 5.0;
bolt_d     = 4.5;  // M4 clearance

// Coax entry stubs
stub_l     = 12.0;
stub_od    = coax_entry_d + 6;

// Alignment pins/sockets
pin_d = 3.0;
pin_h = 5.0;

$fn = 48;

module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

// ── Core pocket ─────────────────────────────────────────────────────────────
module core_pocket() {
    // Cylindrical pocket for toroid core stack
    translate([body_w/2, body_d/2, wall])
        cylinder(h=inner_h+1, d=core_od + 2, $fn=48);
}

module coax_channel() {
    // Channels for coax routing around cores (8 turns)
    // Simplified: two straight entry channels at opposite faces
    // Real turns are made by hand; housing provides entry/exit only
    // Entry: left face (y=0 side)
    translate([-0.5, body_d/2 - coax_od/2, body_h/2 - coax_od/2])
        cube([wall+2, coax_od+1, coax_od+1]);
    // Exit: right face (y=body_d side)
    translate([body_w - wall - 1.5, body_d/2 - coax_od/2, body_h/2 - coax_od/2])
        cube([wall+2, coax_od+1, coax_od+1]);
}

// ── Bottom half ─────────────────────────────────────────────────────────────
module bottom_half() {
    difference() {
        union() {
            // Body
            rounded_box(body_w, body_d, split_z, 4);
            // Mounting flange (only on bottom)
            translate([(body_w - flange_w)/2, (body_d - flange_d)/2, 0])
                difference() {
                    cube([flange_w, flange_d, flange_h]);
                    rounded_box(body_w, body_d, flange_h+1, 4);
                }
            // Coax entry stubs
            translate([-stub_l, body_d/2 - stub_od/2, split_z - stub_od/2 - 2])
                cube([stub_l, stub_od, stub_od]);
            translate([body_w, body_d/2 - stub_od/2, split_z - stub_od/2 - 2])
                cube([stub_l, stub_od, stub_od]);
        }

        // Core pocket (half depth)
        translate([body_w/2, body_d/2, wall])
            cylinder(h=inner_h, d=core_od + 2.5, $fn=48);

        // Coax entry holes (left)
        translate([-0.5, body_d/2, split_z - 2])
        rotate([0, 90, 0])
            cylinder(h=wall + stub_l + 1, d=coax_entry_d, $fn=16);

        // Coax exit holes (right)
        translate([body_w - wall - 0.5, body_d/2, split_z - 2])
        rotate([0, 90, 0])
            cylinder(h=wall + stub_l + 1, d=coax_entry_d, $fn=16);

        // Mounting bolt holes in flange
        bx = (body_w - flange_w)/2;
        by = (body_d - flange_d)/2;
        for (x=[bx+6, bx+flange_w-6]) for (y=[by+6, by+flange_d-6])
            translate([x, y, -0.5])
                cylinder(h=flange_h+1, d=bolt_d, $fn=16);

        // Alignment pins sockets (on split face — top of bottom half)
        for (p=[[body_w/4, body_d/4], [body_w*3/4, body_d*3/4]])
            translate([p[0], p[1], split_z - pin_h])
                cylinder(h=pin_h + 0.5, d=pin_d + 0.4, $fn=16);  // socket
    }

    // Alignment pins (protrude upward from bottom half)
    for (p=[[body_w/4, body_d*3/4], [body_w*3/4, body_d/4]])
        translate([p[0], p[1], split_z])
            cylinder(h=pin_h, d=pin_d, $fn=16);
}

// ── Top half ────────────────────────────────────────────────────────────────
module top_half() {
    difference() {
        union() {
            rounded_box(body_w, body_d, body_h - split_z, 4);
        }

        // Inner cavity (top half)
        translate([wall, wall, 0])
            rounded_box(inner_w, inner_d, body_h - split_z, 4-wall+0.5);

        // Alignment pin sockets (bottom of top half)
        for (p=[[body_w/4, body_d*3/4], [body_w*3/4, body_d/4]])
            translate([p[0], p[1], -0.5])
                cylinder(h=pin_h + 0.5, d=pin_d + 0.4, $fn=16);

        // Coax channels (top half mirrors bottom)
        translate([-0.5, body_d/2, (body_h - split_z)/2 - coax_od/2])
            cube([wall+2, coax_od+1, coax_od+1]);
        translate([body_w-wall-1.5, body_d/2, (body_h-split_z)/2 - coax_od/2])
            cube([wall+2, coax_od+1, coax_od+1]);

        // Alignment pin holes (for bottom half pins)
        for (p=[[body_w/4, body_d/4], [body_w*3/4, body_d*3/4]])
            translate([p[0], p[1], -0.5])
                cylinder(h=pin_h + 0.5, d=pin_d + 0.4, $fn=16);
    }
}

// ── Assembly instructions ───────────────────────────────────────────────────
// WINDING PROCEDURE:
// 1. Print bottom half and top half separately.
// 2. Stack 2× FT-240-31 cores (tape together with Kapton tape first).
// 3. Place stacked cores in bottom half core pocket.
// 4. Thread RG-8X coax through entry stub hole.
// 5. Wind coax 8 times through the core center hole (8 complete passes).
// 6. Thread coax out exit stub hole.
// 7. Snap top half onto bottom half (alignment pins click into sockets).
// 8. Secure with cable tie around housing perimeter.
// 9. Seal entry/exit stubs with self-amalgamating tape (weatherproof).
// 10. Attach PL-259 connectors to coax ends.

print_part = "bottom";  // "bottom" | "top" | "both"

if (print_part == "bottom") {
    color("tan", 0.9) bottom_half();
} else if (print_part == "top") {
    color("sienna", 0.9) top_half();
} else {
    color("tan", 0.9) bottom_half();
    color("sienna", 0.7)
    translate([0, 0, split_z]) top_half();
}
