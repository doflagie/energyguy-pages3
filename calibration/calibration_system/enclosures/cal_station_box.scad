/**
 * cal_station_box.scad — TM-CAL-ENC-002 Rev A
 * Calibration Station Combined Enclosure
 *
 * Larger integration box for full bench/field cal station:
 *   - GPSDO PCB with ESP32 + Si5351 + GPS
 *   - Power reference PCB (AD8307 + attenuator chain)
 *   - CYD touchscreen display (optional, front panel cutout)
 *   - Internal RF dividers to isolate signal paths
 *
 * This enclosure is for bench/station use.
 * The gpsdo_enclosure.scad (smaller) is for pure portable use.
 *
 * Envelope: 180×120×55 mm
 */

// ── Parameters ──────────────────────────────────────────────────────────────
box_w  = 180.0;
box_d  = 120.0;
box_h  = 55.0;
wall   = 3.0;
floor_h = 3.5;
lid_h  = 12.0;
body_h = box_h - lid_h;
r      = 5.0;
$fn    = 32;

sma_d     = 6.5;
bnc_d     = 9.5;
led_d     = 3.2;
usb_w     = 9.0;
usb_h     = 4.0;
barrel_d  = 6.5;
screw_d   = 3.3;

// CYD touchscreen window (2.8" TFT: 75mm × 58mm active area)
cyd_w     = 78.0;
cyd_h     = 62.0;

// Internal RF divider wall (vertical partition between signal ref and counter)
div_x     = 100.0;

module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

module standoff(x, y, h_=5.0) {
    translate([x, y, floor_h])
        difference() {
            cylinder(h=h_, d=6.0, $fn=16);
            translate([0,0,-0.5]) cylinder(h=h_+1, d=2.7, $fn=16);
        }
}

module body() {
    difference() {
        rounded_box(box_w, box_d, body_h, r);

        // Inner cavity
        translate([wall, wall, floor_h])
            rounded_box(box_w-2*wall, box_d-2*wall, body_h, r-wall+0.5);

        // ── FRONT PANEL (y=box_d) ─────────────────────────────────────
        // CYD display window (if CYD mounted in front panel)
        translate([(box_w-cyd_w)/2, box_d+0.5, floor_h+2])
            cube([cyd_w, wall+1, cyd_h]);

        // ── LEFT PANEL (x=0): SMA outputs ─────────────────────────────
        // SMA: 10M REF
        translate([-0.5, 20, floor_h + (body_h-floor_h)/2])
        rotate([0,90,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // SMA: 1M REF
        translate([-0.5, 34, floor_h + (body_h-floor_h)/2])
        rotate([0,90,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // SMA: PROG OUT
        translate([-0.5, 48, floor_h + (body_h-floor_h)/2])
        rotate([0,90,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // SMA: CAL LVL (−20 dBm)
        translate([-0.5, 62, floor_h + (body_h-floor_h)/2])
        rotate([0,90,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // LEDs
        translate([-0.5, 85, floor_h + (body_h-floor_h)/2 + 4])
        rotate([0,90,0]) cylinder(h=wall+1, d=led_d, $fn=16);  // GPS
        translate([-0.5, 98, floor_h + (body_h-floor_h)/2 + 4])
        rotate([0,90,0]) cylinder(h=wall+1, d=led_d, $fn=16);  // LOCK

        // ── RIGHT PANEL (x=box_w): FREQ IN, GPS ANT, power ────────────
        // BNC: FREQ IN
        translate([box_w+0.5, 20, floor_h + (body_h-floor_h)/2])
        rotate([0,-90,0]) cylinder(h=wall+1, d=bnc_d, $fn=16);
        // SMA: GPS ANT
        translate([box_w+0.5, 40, floor_h + (body_h-floor_h)/2])
        rotate([0,-90,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // USB Micro-B
        translate([box_w+0.5, 60-usb_w/2, floor_h + 5])
            cube([wall+1, usb_w, usb_h]);
        // Barrel jack 5V
        translate([box_w+0.5, 80, floor_h + (body_h-floor_h)/2])
        rotate([0,-90,0]) cylinder(h=wall+1, d=barrel_d, $fn=16);

        // ── Lid screw holes ─────────────────────────────────────────────
        for (x=[10, box_w-10]) for (y=[10, box_d-10])
            translate([x, y, body_h-5])
                cylinder(h=6, d=screw_d, $fn=16);
    }

    // Internal RF divider wall (separates signal ref from counter section)
    translate([div_x, wall, floor_h])
        cube([2.5, box_d-2*wall, body_h - floor_h - 1]);

    // PCB standoffs — left section (GPSDO PCB ~80×90mm)
    for (pos=[[15, 12], [90, 12], [15, 105], [90, 105]])
        standoff(pos[0], pos[1]);

    // PCB standoffs — right section (power ref PCB ~60×90mm)
    for (pos=[[110, 12], [170, 12], [110, 105], [170, 105]])
        standoff(pos[0], pos[1]);

    // Corner screw bosses
    for (x=[10, box_w-10]) for (y=[10, box_d-10])
        translate([x, y, 0]) cylinder(h=body_h, d=8, $fn=16);
}

module lid() {
    fl_d    = 4.0;
    plate_h = lid_h - fl_d;
    difference() {
        union() {
            rounded_box(box_w, box_d, plate_h, r);
            translate([wall+1, wall+1, plate_h])
                rounded_box(box_w-2*(wall+1), box_d-2*(wall+1), fl_d, r-wall);
        }
        for (x=[10, box_w-10]) for (y=[10, box_d-10])
            translate([x, y, -0.5])
                cylinder(h=lid_h+1, d=screw_d, $fn=16);
        // Vent slots (RF-safe: ≤10mm length for f < 15 GHz)
        for (sx=[20:15:160])
            translate([sx, 40, -0.5])
                cube([8, 2, plate_h+1]);
    }
}

print_part = "body";   // "body" | "lid"

if (print_part == "body")
    color("steelblue", 0.9) body();
else
    color("lightsteelblue", 0.9) lid();

// ── Assembly Notes ──────────────────────────────────────────────────────────
// 1. Print body and lid in PETG (temperature-stable, adequate RF shielding base).
// 2. Line interior walls, floor, lid inner face with copper adhesive tape.
// 3. Mount GPSDO PCB in left section (x=0 to 100); power ref PCB in right section.
// 4. Connect RF divider wall to copper tape ground plane with copper tape.
// 5. If using CYD: mount CYD in front panel opening; secure with M2 screws.
//    Run ribbon cable from CYD to ESP32 GPIO through internal grommet.
// 6. GPS patch antenna mounts outside on top of enclosure or remote via SMA coax.
