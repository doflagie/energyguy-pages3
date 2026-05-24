/**
 * terminating_enclosure.scad — TM-PWR-ENCL-TERM-001 Rev A
 * Terminating Power Meter / Dummy Load Enclosure
 * 180×130×65mm, SO-239 input, heatsink fins, digital display
 * Rated 100W continuous with fins, 500W with external heatsink
 */

// ─── Parameters ──────────────────────────────────────────────────────────────
box_w    = 180;   // Width  (RF axis, connector on left end)
box_d    = 130;   // Depth
box_h    = 65;    // Height
wall     = 3.5;
floor_t  = 4.0;   // Thicker floor to conduct heat to heatsink fins
lid_h    = 12;
fillet   = 5.0;

// SO-239 input (left end wall)
so239_d    = 16.0;
so239_flg  = 25.4;  // flange OD
so239_z    = box_h * 0.55;  // slightly above center (center conductor height)

// Load resistor mounting (bottom of interior)
// 2× resistors 100×15mm footprint each
res_w    = 15;
res_l    = 100;
res_pad_d = 4.5;   // Through-hole for M4 mounting screw
res_count = 2;

// Heatsink fins (on outer bottom surface)
fin_h    = 18;    // Fin height
fin_t    = 2;     // Fin thickness
fin_gap  = 6;     // Gap between fins
fin_n    = 12;    // Number of fins
fin_inset= 10;    // Distance from side edges

// CYD display window (front face, upper)
// CYD: 2.8" TFT, 240×320 pixels, module body 71×52mm
cyd_w    = 73;
cyd_h    = 54;
cyd_cx   = box_w * 0.6;
cyd_cz   = box_h - lid_h - cyd_h/2 - 5;

// Status LEDs (front face)
led_d    = 5.2;
led_z    = 12;
led_xs   = [15, 25, 35];  // three LEDs: power, overload, thermal

// USB-C (front face, lower)
usb_w    = 9;
usb_h    = 3.5;
usb_z    = 8;
usb_x    = box_w * 0.6 - 35;

// DS18B20 thermal probe port (rear or side)
probe_d  = 6.5;
probe_z  = floor_t + 15;

// Fan mount (rear face, optional 40mm fan)
fan_sq   = 40;
fan_screw_bcd = 32;  // screw hole BCD
fan_screw_d   = 3.3;
fan_cx   = box_w / 2;
fan_cz   = box_h / 2;

// PCB standoffs
so_h  = 6;
so_od = 6;
so_id = 2.8;

// Power input banana jacks (for bench use, rear panel)
banana_d = 4.2;
dc_jack_d = 5.7;  // 5.5mm DC barrel jack

// ─── Modules ──────────────────────────────────────────────────────────────────

module shell_profile(w, d, r) {
    offset(r=r) square([w - 2*r, d - 2*r], center=false);
}

module so239_cutout() {
    cylinder(d=so239_d, h=wall*3, center=true, $fn=40);
    for (a = [45, 135, 225, 315])
        translate([cos(a)*12, sin(a)*12, 0])
            cylinder(d=3.3, h=wall*3, center=true, $fn=16);
}

module heatsink_fins() {
    fin_total_w = fin_n * fin_t + (fin_n - 1) * fin_gap;
    for (i = [0:fin_n-1]) {
        translate([fin_inset + i*(fin_t + fin_gap), 0, -fin_h])
            cube([fin_t, box_d - 2*fin_inset, fin_h]);
    }
}

module cyd_window() {
    // CYD module has 4-pin headers on back — need adequate depth
    // Window: 73×54mm for display glass + bezel
    translate([0, 0, -wall])
        linear_extrude(wall * 3)
            square([cyd_w, cyd_h], center=true);
    // Touch connector clearance (rear of CYD board)
    translate([0, 0, -wall - 5])
        linear_extrude(5)
            square([cyd_w + 8, cyd_h + 8], center=true);
}

module cyd_mount_bosses() {
    // 4 standoffs matching CYD mounting holes (57.6mm × 41.6mm BCD)
    for (dx = [-28.8, 28.8]) for (dy = [-20.8, 20.8])
        translate([dx, dy, wall])
            difference() {
                cylinder(d=5, h=3, $fn=16);
                cylinder(d=2.0, h=3.1, $fn=12);  // M2 self-tap
            }
}

module res_mount_pads() {
    // Mounting boss pairs for two load resistors
    for (rx = [box_w*0.3, box_w*0.6])
        for (ry = [box_d*0.3, box_d*0.7])
            translate([rx, ry, floor_t])
                difference() {
                    cylinder(d=8, h=so_h, $fn=16);
                    cylinder(d=so_id, h=so_h+0.1, $fn=12);
                }
}

// ─── Body ─────────────────────────────────────────────────────────────────────
module body() {
    difference() {
        union() {
            // Main shell
            linear_extrude(box_h - lid_h)
                shell_profile(box_w, box_d, fillet);
            // Heatsink fins (exterior bottom)
            heatsink_fins();
        }

        // Cavity
        translate([wall, wall, floor_t])
            linear_extrude(box_h)
                shell_profile(box_w - 2*wall, box_d - 2*wall, max(fillet-wall, 1));

        // Left wall: SO-239 input
        translate([-0.1, box_d/2, so239_z]) rotate([0, 90, 0])
            so239_cutout();

        // Front face: CYD window
        translate([cyd_cx, -0.1, cyd_cz]) rotate([-90, 0, 0])
            cyd_window();

        // Front face: status LEDs
        for (lx = led_xs)
            translate([lx, -0.1, led_z]) rotate([-90, 0, 0])
                cylinder(d=led_d, h=wall*3, $fn=20);

        // Front face: USB-C
        translate([usb_x, -0.1, usb_z]) rotate([-90, 0, 0])
            hull() {
                for (dx = [-usb_w/2+usb_h/2, usb_w/2-usb_h/2])
                    translate([dx, 0, 0])
                        cylinder(d=usb_h, h=wall*3, $fn=16);
            }

        // Rear face: fan cutout (optional 40mm fan)
        translate([fan_cx, box_d + 0.1, fan_cz]) rotate([90, 0, 0]) {
            cylinder(d=fan_sq - 2, h=wall*3, $fn=40);  // fan airhole
            for (a = [45, 135, 225, 315])
                translate([cos(a)*fan_screw_bcd/2, sin(a)*fan_screw_bcd/2, 0])
                    cylinder(d=fan_screw_d, h=wall*3, $fn=16);
        }

        // Rear face: DS18B20 probe port
        translate([box_w - 20, box_d + 0.1, probe_z]) rotate([90, 0, 0])
            cylinder(d=probe_d, h=wall*3, $fn=20);

        // Rear face: DC barrel jack (external power)
        translate([15, box_d + 0.1, 12]) rotate([90, 0, 0])
            cylinder(d=dc_jack_d, h=wall*3, $fn=20);

        // Bottom vent holes (large, below heatsink fins – not needed if fins used)
        // Left/right vent cutouts between fins (airflow)
        for (i = [1:3:fin_n-2]) {
            fin_x = fin_inset + i*(fin_t + fin_gap) + fin_t/2;
            translate([fin_x, box_d*0.2, 0])
                cube([fin_gap - 0.5, box_d*0.6, floor_t*2], center=false);
        }

        // Lid guide slot
        translate([wall-0.3, wall-0.3, box_h-lid_h-floor_t])
            linear_extrude(lid_h+0.5)
                difference() {
                    shell_profile(box_w-2*(wall-0.3), box_d-2*(wall-0.3), fillet);
                    shell_profile(box_w-2*(wall+0.5), box_d-2*(wall+0.5), max(fillet-wall, 1));
                }
    }

    // CYD mount bosses (inside front wall)
    translate([cyd_cx, wall, cyd_cz])
        cyd_mount_bosses();

    // Load resistor mounting pads
    res_mount_pads();

    // PCB standoffs (for digital control board)
    for (sx = [12, box_w-12]) for (sy = [12, box_d-12-wall])
        translate([sx, sy, floor_t])
            difference() {
                cylinder(d=so_od, h=so_h, $fn=20);
                cylinder(d=so_id, h=so_h+0.1, $fn=20);
            }
}

// ─── Lid ─────────────────────────────────────────────────────────────────────
module lid() {
    difference() {
        linear_extrude(lid_h)
            shell_profile(box_w, box_d, fillet);
        translate([wall-0.2, wall-0.2, floor_t])
            linear_extrude(lid_h)
                shell_profile(box_w-2*(wall-0.2), box_d-2*(wall-0.2), max(fillet-wall, 1));
        // Large vent holes (heat rises through lid)
        for (i = [0:2]) for (j = [0:2])
            translate([40 + i*50, 30 + j*35, -0.1])
                cylinder(d=10, h=lid_h+0.2, $fn=30);
        // M3 screw holes
        for (sx = [10, box_w-10]) for (sy = [10, box_d-10])
            translate([sx, sy, -0.1])
                cylinder(d=3.3, h=lid_h+0.2, $fn=16);
    }
}

// ─── CYD Bezel (mounts from outside front) ───────────────────────────────────
module cyd_bezel() {
    difference() {
        // Outer plate matching front face hole area
        translate([0, 0, 0])
            hull() {
                for (dx = [-(cyd_w+12)/2, (cyd_w+12)/2])
                    for (dy = [-(cyd_h+12)/2, (cyd_h+12)/2])
                        translate([dx, dy, 0]) cylinder(r=3, h=2, $fn=20);
            }
        // CYD glass aperture
        translate([0, 0, -0.1])
            linear_extrude(2.2)
                square([cyd_w - 4, cyd_h - 4], center=true);
        // M3 screw holes (4 corners)
        for (dx = [-(cyd_w+6)/2, (cyd_w+6)/2])
            for (dy = [-(cyd_h+6)/2, (cyd_h+6)/2])
                translate([dx, dy, -0.1])
                    cylinder(d=3.3, h=2.2, $fn=16);
    }
}

body();
translate([0, box_d + 20, 0]) lid();
translate([cyd_cx, -60, cyd_cz]) cyd_bezel();

// Print notes:
// Body: print upside-down (fins point up during print, no supports for fins)
// Lid: print flat
// Material: PETG for body (temperature resistance near RF load)
// Fin printing: fins print cleanly upside-down with 0.2mm layers, no supports
// After print: tap M3 threads in PCB standoffs (or use M3 heat inserts)
// Load resistors: mount with M4 screws + thermal compound to floor interior
