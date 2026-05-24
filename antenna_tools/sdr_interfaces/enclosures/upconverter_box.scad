/**
 * upconverter_box.scad — TM-SDR-ENC-001 Rev A
 * RF-Shielded Enclosure for HF Upconverter + Direct Sampling Interface
 *
 * Features:
 *   - Two-piece clamshell (body + lid), snap-fit + M2.5 screws
 *   - Inner copper-foil shielding cavity (print with foil lining)
 *   - SMA IN (rear) and SMA OUT (front)
 *   - USB Micro-B power connector (side)
 *   - LED indicator holes
 *   - PCB standoffs for 70×50 mm PCB
 *
 * Envelope: 90×65×36 mm (fits in 200×200×200 build volume)
 * Material: PETG + copper tape lining inside for RF shielding
 */

// ── Parameters ─────────────────────────────────────────────────────────────
box_w   = 90.0;    // X
box_d   = 65.0;    // Y
box_h   = 36.0;    // Z total
wall    = 2.5;     // wall thickness
floor_h = 3.0;     // floor thickness
lid_h   = 10.0;    // lid height (includes overlap flange)
body_h  = box_h - lid_h;  // body height

// PCB
pcb_w  = 70.0;
pcb_d  = 50.0;
pcb_stoff_h = 4.0;   // PCB standoff height
pcb_stoff_d = 6.0;   // standoff outer dia
pcb_hole_d  = 2.7;   // M2.5 clearance

// Connectors
sma_hole_d  = 6.5;   // SMA edge connector hole
usb_w       = 9.0;   // USB Micro-B opening width
usb_h       = 4.5;   // USB Micro-B opening height
led_d       = 3.2;   // 3mm LED hole

// Corner radius
r = 4.0;

// Lid flange
flange_depth = 3.0;   // how deep lid flange goes into body
flange_wall  = 1.5;   // flange wall thickness

// Shielding cavity (slightly smaller than interior)
shield_offset = 0.6;  // gap for copper tape

// Screw holes at lid corners
screw_d    = 2.7;     // M2.5 through
screw_boss = 5.0;     // boss diameter

$fn = 32;

// ── Utility ────────────────────────────────────────────────────────────────
module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

module standoff(x, y, h, od, id) {
    translate([x, y, floor_h])
        difference() {
            cylinder(h=h, d=od, $fn=16);
            translate([0,0,-0.5]) cylinder(h=h+1, d=id, $fn=16);
        }
}

// ── Body ────────────────────────────────────────────────────────────────────
module body() {
    difference() {
        // Outer shell
        rounded_box(box_w, box_d, body_h, r);

        // Inner cavity
        translate([wall, wall, floor_h])
            rounded_box(box_w-2*wall, box_d-2*wall, body_h, r-wall+0.5);

        // ── SMA IN: rear face, center height, left of center ──────────
        translate([box_w/2 - 14, -0.5, body_h/2 + floor_h/2])
        rotate([-90, 0, 0])
            cylinder(h=wall+1, d=sma_hole_d, $fn=16);

        // ── SMA OUT: front face, center height, right of center ────────
        translate([box_w/2 + 14, box_d+0.5, body_h/2 + floor_h/2])
        rotate([90, 0, 0])
            cylinder(h=wall+1, d=sma_hole_d, $fn=16);

        // ── USB Micro-B: right side, near bottom ───────────────────────
        translate([box_w+0.5, box_d/2 - usb_w/2, floor_h + 2])
        rotate([0, 90, 0])
            cube([usb_h, usb_w, wall+1]);

        // ── LED holes: front face ──────────────────────────────────────
        for (x=[box_w/2 - 8, box_w/2 + 8])
            translate([x, box_d+0.5, body_h - 6])
            rotate([90,0,0])
                cylinder(h=wall+1, d=led_d, $fn=16);

        // ── Lid screw bosses: countersunk from top ─────────────────────
        for (x=[8, box_w-8]) for (y=[8, box_d-8])
            translate([x, y, body_h - 5])
                cylinder(h=6, d=screw_d, $fn=16);
    }

    // PCB standoffs
    // Center PCB in box: offsets = (box_w - pcb_w)/2, (box_d - pcb_d)/2
    pcb_ox = (box_w - pcb_w) / 2;
    pcb_oy = (box_d - pcb_d) / 2;
    standoff(pcb_ox + 3,        pcb_oy + 3,        pcb_stoff_h, pcb_stoff_d, pcb_hole_d);
    standoff(pcb_ox + pcb_w-3,  pcb_oy + 3,        pcb_stoff_h, pcb_stoff_d, pcb_hole_d);
    standoff(pcb_ox + 3,        pcb_oy + pcb_d-3,  pcb_stoff_h, pcb_stoff_d, pcb_hole_d);
    standoff(pcb_ox + pcb_w-3,  pcb_oy + pcb_d-3,  pcb_stoff_h, pcb_stoff_d, pcb_hole_d);

    // Screw bosses at corners (inside body, below lid flange)
    for (x=[8, box_w-8]) for (y=[8, box_d-8])
        translate([x, y, body_h - 8])
            cylinder(h=8, d=screw_boss, $fn=16);
}

// ── Lid ─────────────────────────────────────────────────────────────────────
module lid() {
    difference() {
        union() {
            // Top plate
            rounded_box(box_w, box_d, lid_h - flange_depth, r);

            // Flange (snaps inside body opening)
            translate([wall + flange_wall + shield_offset,
                       wall + flange_wall + shield_offset,
                       lid_h - flange_depth])
                rounded_box(
                    box_w - 2*(wall + flange_wall + shield_offset),
                    box_d - 2*(wall + flange_wall + shield_offset),
                    flange_depth, r - wall);
        }

        // Screw through holes at corners
        for (x=[8, box_w-8]) for (y=[8, box_d-8])
            translate([x, y, -0.5])
                cylinder(h=lid_h+1, d=screw_d, $fn=16);

        // Vent slot (center of lid) — optional
        translate([box_w/2 - 15, box_d/2 - 3, -0.5])
            cube([30, 6, lid_h - flange_depth + 1]);
    }
}

// ── Shielding note ─────────────────────────────────────────────────────────
// After printing:
// 1. Cut copper tape to fit inner walls (4 sides + floor + lid inside).
// 2. Overlap tape corners for continuous shield.
// 3. Solder or conductive-epoxy tape seams.
// 4. Verify conductivity with multimeter across seams.
// 5. PCB ground plane should contact bottom copper tape via spring fingers
//    or M2.5 standoff screws (body standoffs are grounded via PCB GND plane).

// ── Render ─────────────────────────────────────────────────────────────────
print_part = "body";  // "body" | "lid" | "both"

if (print_part == "body") {
    color("lightblue", 0.9) body();
} else if (print_part == "lid") {
    color("steelblue", 0.9) lid();
} else {
    color("lightblue", 0.9) body();
    color("steelblue", 0.7)
    translate([0, 0, body_h]) lid();
}
