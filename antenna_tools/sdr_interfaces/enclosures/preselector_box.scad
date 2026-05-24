/**
 * preselector_box.scad — TM-SDR-ENC-002 Rev A
 * RF-Shielded Enclosure for 16-Band Switchable Preselector Filter Board
 *
 * Features:
 *   - Houses 160×100 mm filter PCB
 *   - SMA IN and SMA OUT on opposite ends
 *   - 8-pin and 6-pin IDC connectors for control board
 *   - Alignment dividers between filter sections (internal RF isolation)
 *   - Copper-tape shielding cavity
 *   - M3 screw-together (no snap-fit, more rigid for RF)
 *
 * Envelope: 180×120×50 mm
 * Material: PETG + copper tape or aluminum foil lining
 */

// ── Parameters ─────────────────────────────────────────────────────────────
box_w   = 180.0;
box_d   = 120.0;
box_h   = 50.0;
wall    = 3.0;
floor_h = 3.5;
lid_h   = 12.0;
body_h  = box_h - lid_h;

// PCB
pcb_w   = 160.0;
pcb_d   = 100.0;
pcb_so_h = 5.0;
pcb_so_od = 7.0;
pcb_so_id = 2.7;

// Connectors
sma_hole_d   = 6.5;
idc8_w       = 24.0;
idc8_h        = 9.0;
idc6_w       = 18.0;
idc6_h        = 9.0;

// Internal dividers (RF isolation between filter groups)
div_count = 3;   // creates 4 sections

// Corner screw posts
screw_d  = 3.3;   // M3 clearance
boss_d   = 8.0;
boss_h   = body_h;

r = 5.0;
$fn = 32;

module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

module pcb_standoff(x, y) {
    translate([x, y, floor_h])
        difference() {
            cylinder(h=pcb_so_h, d=pcb_so_od, $fn=16);
            translate([0,0,-0.5]) cylinder(h=pcb_so_h+1, d=pcb_so_id, $fn=16);
        }
}

module body() {
    difference() {
        rounded_box(box_w, box_d, body_h, r);

        // Inner cavity
        translate([wall, wall, floor_h])
            rounded_box(box_w-2*wall, box_d-2*wall, body_h, r-wall+0.5);

        // SMA IN: left end face, vertically centered
        translate([-0.5, box_d/2, floor_h + (body_h - floor_h)/2])
        rotate([0, 90, 0])
            cylinder(h=wall+1, d=sma_hole_d, $fn=16);

        // SMA OUT: right end face
        translate([box_w+0.5, box_d/2, floor_h + (body_h - floor_h)/2])
        rotate([0, -90, 0])
            cylinder(h=wall+1, d=sma_hole_d, $fn=16);

        // IDC 8-pin control connector: rear face, left half
        translate([30, -0.5, floor_h + 4])
        rotate([-90,0,0])
            cube([idc8_w, idc8_h, wall+1]);

        // IDC 6-pin: rear face, right half
        translate([box_w - 30 - idc6_w, -0.5, floor_h + 4])
        rotate([-90,0,0])
            cube([idc6_w, idc6_h, wall+1]);

        // Screw holes at four corners (M3)
        for (x=[8, box_w-8]) for (y=[8, box_d-8])
            translate([x, y, body_h - 6]) {
                cylinder(h=7, d=screw_d, $fn=16);
                cylinder(h=3, d=boss_d,  $fn=16);  // counterbore
            }
    }

    // PCB standoffs (4 corners + 2 mid span)
    ox = (box_w - pcb_w) / 2;
    oy = (box_d - pcb_d) / 2;
    pcb_standoff(ox + 4,        oy + 4);
    pcb_standoff(ox + pcb_w-4,  oy + 4);
    pcb_standoff(ox + 4,        oy + pcb_d-4);
    pcb_standoff(ox + pcb_w-4,  oy + pcb_d-4);
    pcb_standoff(ox + pcb_w/2,  oy + 4);         // center front
    pcb_standoff(ox + pcb_w/2,  oy + pcb_d-4);   // center rear

    // Internal RF isolation dividers
    // Three dividers split the filter board into 4 sections (HF-lo, HF-hi, VHF, UHF)
    div_positions = [box_w * 0.25, box_w * 0.5, box_w * 0.75];
    for (xd = div_positions) {
        translate([xd, wall, floor_h])
            cube([1.5, box_d-2*wall, body_h - floor_h - 2]);
    }

    // Boss posts at corners for screw thread engagement
    for (x=[8, box_w-8]) for (y=[8, box_d-8])
        translate([x, y, 0])
            cylinder(h=boss_h, d=boss_d, $fn=16);
}

module lid() {
    flange_w = box_w - 2*(wall + 1.0);
    flange_d = box_d - 2*(wall + 1.0);
    fl_depth = 4.0;
    lid_plate_h = lid_h - fl_depth;

    difference() {
        union() {
            rounded_box(box_w, box_d, lid_plate_h, r);
            translate([wall+1, wall+1, lid_plate_h])
                rounded_box(flange_w, flange_d, fl_depth, r-wall);
        }
        // Screw through holes
        for (x=[8, box_w-8]) for (y=[8, box_d-8])
            translate([x, y, -0.5])
                cylinder(h=lid_h+1, d=screw_d, $fn=16);

        // Label recess (optional, for band labels)
        translate([20, box_d/2-8, -0.5])
            cube([box_w-40, 16, 1.5]);
    }
}

// ── Assembly notes ─────────────────────────────────────────────────────────
// 1. Print body and lid in PETG.
// 2. Line inside with copper tape (walls, floor). Solder seams.
// 3. Install PCB on standoffs with M2.5 or M3 screws (which ground the PCB to shield).
// 4. Route SMA pigtails through the SMA holes; mount SMA connectors.
// 5. Route IDC ribbon cables to ESP32 controller box.
// 6. Apply copper tape to lid interior. Lid copper must contact body copper rim.
//    Use copper tape strip around lid flange to ensure contact when lid is closed.

print_part = "body";  // "body" | "lid"

if (print_part == "body") {
    color("silver", 0.9) body();
} else {
    color("gray", 0.9) lid();
}
