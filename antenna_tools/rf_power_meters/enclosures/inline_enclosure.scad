/**
 * inline_enclosure.scad — TM-PWR-ENCL-INLINE-001 Rev A
 * Inline Directional Power Meter Enclosure
 * 130×90×50mm, SO-239 both ends, OLED front, 4 buttons, SD slot
 * For digital (ESP32) version with ADS1115, peak + average display
 */

// ─── Parameters ──────────────────────────────────────────────────────────────
box_w    = 130;   // Width (X) — RF axis
box_d    = 90;    // Depth (Y) — front-to-back
box_h    = 50;    // Height (Z)
wall     = 3.0;   // Wall thickness
floor_t  = 3.0;
lid_h    = 10;
fillet   = 4.0;

// SO-239 connectors (left/right walls, centered)
so239_d      = 16.0;   // Panel drill diameter
so239_z      = box_h / 2;
so239_bcd    = 25.4;   // Bolt circle diameter (for 4 × M3 flange screws)

// OLED window (front face)
oled_w   = 28;
oled_h   = 16;
oled_cx  = box_w / 2;
oled_cz  = box_h - 20;

// Bargraph LED strip window (below OLED)
bar_w    = 90;
bar_h    = 5;
bar_cz   = oled_cz - oled_h/2 - bar_h/2 - 4;

// Buttons (front, 4× 6mm tact)
btn_d    = 6.5;
btn_z    = 12;
btn_xs   = [30, 50, 80, 100];

// SD card slot (rear face, bottom)
sd_w     = 15;
sd_h     = 3;
sd_z     = 8;
sd_x     = box_w - 25;

// USB-C power (rear face)
usb_w    = 9;
usb_h    = 3.5;
usb_z    = 10;
usb_x    = 20;

// Antenna label area (front face, top)
label_w  = 50;
label_h  = 7;
label_z  = box_h - lid_h - 8;

// PCB standoffs (M3)
so_h     = 5;
so_od    = 6;
so_id    = 2.8;
pcb_ins  = 8;

// RF line bore (center conductor through enclosure)
rf_bore_d = 8;  // clearance for RF PCB trace/connector

// Range selector switch (front, right side) — 3-position rotary
sw_d    = 8.5;
sw_x    = box_w - 20;
sw_z    = box_h / 2;

// ─── Modules ──────────────────────────────────────────────────────────────────

module rounded_box_profile(w, d, r) {
    offset(r=r) square([w - 2*r, d - 2*r], center=false);
}

module so239_hole() {
    cylinder(d=so239_d, h=wall*3, center=true, $fn=40);
    // 4× M3 flange screws
    for (a = [45, 135, 225, 315])
        translate([cos(a)*so239_bcd/2/1.5, sin(a)*so239_bcd/2/1.5, 0])
            cylinder(d=3.3, h=wall*3, center=true, $fn=16);
}

module standoff(h) {
    difference() {
        cylinder(d=so_od, h=h, $fn=20);
        cylinder(d=so_id, h=h+0.1, $fn=20);
    }
}

// ─── Body ─────────────────────────────────────────────────────────────────────
module body() {
    difference() {
        // Shell
        linear_extrude(box_h - lid_h)
            rounded_box_profile(box_w, box_d, fillet);
        // Cavity
        translate([wall, wall, floor_t])
            linear_extrude(box_h)
                rounded_box_profile(box_w - 2*wall, box_d - 2*wall, max(fillet-wall, 1));

        // Left SO-239
        translate([-0.1, box_d/2, so239_z]) rotate([0, 90, 0])
            so239_hole();
        // Right SO-239
        translate([box_w + 0.1, box_d/2, so239_z]) rotate([0, -90, 0])
            so239_hole();

        // RF bore (PCB side, connects J1 to J2 via PCB trace)
        translate([-0.1, box_d/2, so239_z]) rotate([0, 90, 0])
            cylinder(d=rf_bore_d, h=box_w + 0.2, $fn=20);

        // Front: OLED window
        translate([oled_cx, -0.1, oled_cz]) rotate([-90, 0, 0])
            linear_extrude(wall*3)
                square([oled_w, oled_h], center=true);

        // Front: bargraph window
        translate([box_w/2, -0.1, bar_cz]) rotate([-90, 0, 0])
            linear_extrude(wall*3)
                square([bar_w, bar_h], center=true);

        // Front: 4 buttons
        for (bx = btn_xs)
            translate([bx, -0.1, btn_z]) rotate([-90, 0, 0])
                cylinder(d=btn_d, h=wall*3, $fn=20);

        // Front: range switch
        translate([sw_x, -0.1, sw_z]) rotate([-90, 0, 0])
            cylinder(d=sw_d, h=wall*3, $fn=20);

        // Front: label recess
        translate([box_w/2, -0.1, label_z]) rotate([-90, 0, 0])
            linear_extrude(0.5)
                square([label_w, label_h], center=true);

        // Rear: SD card slot
        translate([sd_x, box_d + 0.1, sd_z]) rotate([90, 0, 0])
            hull() {
                for (dx = [-sd_w/2 + sd_h/2, sd_w/2 - sd_h/2])
                    translate([dx, 0, 0])
                        cylinder(d=sd_h, h=wall*3, $fn=16);
            }

        // Rear: USB-C
        translate([usb_x, box_d + 0.1, usb_z]) rotate([90, 0, 0])
            hull() {
                for (dx = [-usb_w/2 + usb_h/2, usb_w/2 - usb_h/2])
                    translate([dx, 0, 0])
                        cylinder(d=usb_h, h=wall*3, $fn=16);
            }

        // Bottom vent slots
        for (i = [-1:1])
            translate([box_w/2 + i*30, box_d/2, 0])
                cube([20, 3, floor_t*2], center=true);

        // Lid guide channel
        translate([wall - 0.3, wall - 0.3, box_h - lid_h - floor_t])
            linear_extrude(lid_h + 0.5)
                difference() {
                    rounded_box_profile(box_w - 2*(wall-0.3), box_d - 2*(wall-0.3), fillet);
                    rounded_box_profile(box_w - 2*(wall+0.5), box_d - 2*(wall+0.5), max(fillet-wall, 1));
                }
    }

    // PCB standoffs (4 corners)
    for (sx = [pcb_ins, box_w - pcb_ins])
        for (sy = [pcb_ins, box_d - pcb_ins - wall])
            translate([sx, sy, floor_t])
                standoff(so_h);

    // PCB support rail (center, along RF axis)
    translate([wall + 2, box_d/2 - 1.5, floor_t])
        cube([box_w - 2*wall - 4, 3, so_h - 1]);
}

// ─── Lid ─────────────────────────────────────────────────────────────────────
module lid() {
    difference() {
        linear_extrude(lid_h)
            rounded_box_profile(box_w, box_d, fillet);
        // Inner recess that fits over body top
        translate([wall - 0.2, wall - 0.2, floor_t])
            linear_extrude(lid_h)
                rounded_box_profile(box_w - 2*(wall-0.2), box_d - 2*(wall-0.2), max(fillet-wall, 1));
        // Vent holes
        for (i = [-1:1]) for (j = [-1:1])
            translate([box_w/2 + i*25, box_d/2 + j*20, -0.1])
                cylinder(d=4, h=lid_h + 0.2, $fn=20);
        // M3 screw holes (4 corners)
        for (sx = [15, box_w - 15]) for (sy = [15, box_d - 15])
            translate([sx, sy, -0.1])
                cylinder(d=3.3, h=lid_h + 0.2, $fn=16);
    }
}

// ─── Rubber feet template (cut from 3mm sheet) ────────────────────────────────
module foot_template() {
    difference() {
        cylinder(d=15, h=3, $fn=30);
        cylinder(d=3.5, h=3.1, $fn=16);
    }
}

body();
translate([0, box_d + 15, 0]) lid();
translate([box_w + 15, 0, 0]) {
    foot_template();
    translate([20, 0, 0]) foot_template();
    translate([40, 0, 0]) foot_template();
    translate([60, 0, 0]) foot_template();
}
