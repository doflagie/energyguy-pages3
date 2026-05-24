/**
 * noise_canceller_box.scad — TM-RFI-ENC-003 Rev A
 * Active Noise Canceller Enclosure
 *
 * Front panel: 3× control knobs (Phase1, Phase2, Gain), LEDs
 * Rear panel: 3× SMA (Main, Reference, Output)
 * Side: DC power jack
 * PCB: 100×80 mm (phase shift + gain + combiner circuits)
 * Copper tape lining for RF shielding
 *
 * Envelope: 140×100×55 mm
 */

// ── Parameters ─────────────────────────────────────────────────────────────
box_w  = 140.0;
box_d  = 100.0;
box_h  = 55.0;
wall   = 3.0;
floor_h = 3.5;
lid_h  = 12.0;
body_h = box_h - lid_h;
r = 5.0;
$fn = 32;

// Knobs: 7mm shaft (RK09 series pot shaft = 6mm D-shaft)
knob_d   = 7.2;   // panel hole
knob_sp  = 30.0;  // spacing between knob centers

// SMA
sma_d    = 6.5;

// LED holes
led_d    = 3.2;

// DC jack (2.1mm barrel)
barrel_d = 6.5;

// PCB
pcb_w    = 100.0;
pcb_d    = 80.0;
pcb_so_h = 4.0;
pcb_so_od = 6.0;
pcb_so_id = 2.7;

screw_d  = 3.3;

module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

module standoff(x, y) {
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

        // ── FRONT PANEL (y=box_d): knobs + LEDs ───────────────────────
        // 3× control knobs: Phase1, Phase2, Gain
        for (i=[0:2])
            translate([20 + i*knob_sp, box_d+0.5, body_h - 15])
            rotate([90,0,0])
                cylinder(h=wall+1, d=knob_d, $fn=16);

        // LED indicators (2×): Signal, Null
        for (i=[0:1])
            translate([105 + i*14, box_d+0.5, body_h - 14])
            rotate([90,0,0])
                cylinder(h=wall+1, d=led_d, $fn=16);

        // Knob labels recess
        for (i=[0:2])
            translate([14 + i*knob_sp, box_d - 0.5, body_h - 22])
                cube([24, 0.8, 6]);

        // ── REAR PANEL (y=0): 3× SMA ──────────────────────────────────
        // MAIN antenna input
        translate([20, -0.5, floor_h + (body_h-floor_h)/2])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=sma_d, $fn=16);
        // REFERENCE input
        translate([50, -0.5, floor_h + (body_h-floor_h)/2])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=sma_d, $fn=16);
        // OUTPUT to SDR
        translate([80, -0.5, floor_h + (body_h-floor_h)/2])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=sma_d, $fn=16);

        // ── RIGHT SIDE: DC power jack ──────────────────────────────────
        translate([box_w+0.5, box_d/2, floor_h + 10])
        rotate([0,90,0])
            cylinder(h=wall+1, d=barrel_d, $fn=16);

        // ── Corner screw holes ─────────────────────────────────────────
        for (x=[8, box_w-8]) for (y=[8, box_d-8])
            translate([x, y, body_h-5])
                cylinder(h=6, d=screw_d, $fn=16);
    }

    // PCB standoffs
    ox = (box_w - pcb_w) / 2;
    oy = (box_d - pcb_d) / 2;
    standoff(ox + 4,       oy + 4);
    standoff(ox + pcb_w-4, oy + 4);
    standoff(ox + 4,       oy + pcb_d-4);
    standoff(ox + pcb_w-4, oy + pcb_d-4);

    // Corner bosses
    for (x=[8, box_w-8]) for (y=[8, box_d-8])
        translate([x, y, 0]) cylinder(h=body_h, d=8, $fn=16);
}

module lid() {
    fl_d = 4.0;
    plate_h = lid_h - fl_d;
    difference() {
        union() {
            rounded_box(box_w, box_d, plate_h, r);
            translate([wall+1, wall+1, plate_h])
                rounded_box(box_w-2*(wall+1), box_d-2*(wall+1), fl_d, r-wall);
        }
        for (x=[8, box_w-8]) for (y=[8, box_d-8])
            translate([x, y, -0.5])
                cylinder(h=lid_h+1, d=screw_d, $fn=16);
    }
}

print_part = "body";  // "body" | "lid"

if (print_part == "body") {
    color("dimgray", 0.9) body();
} else {
    color("gray", 0.9) lid();
}
