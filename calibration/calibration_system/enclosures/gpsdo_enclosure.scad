/**
 * gpsdo_enclosure.scad — TM-CAL-ENC-001 Rev A
 * GPS-Disciplined Oscillator Enclosure
 *
 * Contents:
 *   - ESP32-WROOM-32 devboard (38mm × 52mm)
 *   - Si5351A module or PCB (25mm × 15mm)
 *   - u-blox NEO-M8N module (16mm × 12mm)
 *   - Small perfboard/PCB for AD8307, buffers
 *
 * Front panel: 3× SMA (10M REF, 1M REF, PROG OUT), 1× SMA (CAL LVL), LEDs
 * Rear panel:  SMA (GPS ANT), USB Micro-B, barrel jack 5V, BNC (FREQ IN)
 *
 * Envelope: 120×80×40 mm (well within 200×200×200 print bed)
 */

// ── Parameters ─────────────────────────────────────────────────────────────
box_w  = 120.0;
box_d  = 80.0;
box_h  = 40.0;
wall   = 3.0;
floor_h = 3.0;
lid_h  = 10.0;
body_h = box_h - lid_h;
r      = 4.0;
$fn    = 32;

// SMA panel mount hole
sma_d  = 6.5;   // SMA(F) panel mount threaded barrel
led_d  = 3.2;   // 3mm LED bezel
usb_w  = 9.0;   // USB Micro-B footprint width
usb_h  = 4.0;   // USB Micro-B footprint height
barrel_d = 6.5; // 5V barrel jack
bnc_d  = 9.5;   // BNC panel mount hole

// PCB standoffs (matches ESP32 devboard + aux PCB)
so_h   = 4.0;   // standoff height
so_od  = 5.5;
so_id  = 2.7;   // M3 clearance
screw_d = 3.3;  // M3 lid screw

module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

module standoff(x, y) {
    translate([x, y, floor_h])
        difference() {
            cylinder(h=so_h, d=so_od, $fn=16);
            translate([0, 0, -0.5]) cylinder(h=so_h+1, d=so_id, $fn=16);
        }
}

module body() {
    difference() {
        rounded_box(box_w, box_d, body_h, r);

        // Inner cavity
        translate([wall, wall, floor_h])
            rounded_box(box_w-2*wall, box_d-2*wall, body_h, r-wall+0.5);

        // ── FRONT PANEL (y=box_d): SMA outputs + LEDs ──────────────────
        // SMA: 10M REF
        translate([18, box_d+0.5, floor_h + (body_h-floor_h)/2])
        rotate([90,0,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // SMA: 1M REF
        translate([36, box_d+0.5, floor_h + (body_h-floor_h)/2])
        rotate([90,0,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // SMA: PROG OUT
        translate([54, box_d+0.5, floor_h + (body_h-floor_h)/2])
        rotate([90,0,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // SMA: CAL LVL (PROG through 20dB pad)
        translate([72, box_d+0.5, floor_h + (body_h-floor_h)/2])
        rotate([90,0,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // LED: GPS (green)
        translate([92, box_d+0.5, floor_h + (body_h-floor_h)/2 + 4])
        rotate([90,0,0]) cylinder(h=wall+1, d=led_d, $fn=16);
        // LED: LOCK (blue)
        translate([104, box_d+0.5, floor_h + (body_h-floor_h)/2 + 4])
        rotate([90,0,0]) cylinder(h=wall+1, d=led_d, $fn=16);

        // ── REAR PANEL (y=0): GPS ANT, USB, power, FREQ IN ─────────────
        // SMA: GPS ANTENNA
        translate([18, -0.5, floor_h + (body_h-floor_h)/2])
        rotate([-90,0,0]) cylinder(h=wall+1, d=sma_d, $fn=16);
        // USB Micro-B (ESP32 programming + power)
        translate([42 - usb_w/2, -0.5, floor_h + 5])
            cube([usb_w, wall+1, usb_h]);
        // Barrel jack 5V
        translate([65, -0.5, floor_h + (body_h-floor_h)/2])
        rotate([-90,0,0]) cylinder(h=wall+1, d=barrel_d, $fn=16);
        // BNC: FREQ IN (counter)
        translate([90, -0.5, floor_h + (body_h-floor_h)/2])
        rotate([-90,0,0]) cylinder(h=wall+1, d=bnc_d, $fn=16);

        // ── Lid screw holes (M3) ───────────────────────────────────────
        for (x=[8, box_w-8]) for (y=[8, box_d-8])
            translate([x, y, body_h-5])
                cylinder(h=6, d=screw_d, $fn=16);
    }

    // PCB standoffs — ESP32 devboard footprint approx 38×52mm
    // Offset to center in box
    ex = (box_w - 52) / 2;
    ey = (box_d - 38) / 2;
    standoff(ex + 2,       ey + 2);
    standoff(ex + 52 - 2,  ey + 2);
    standoff(ex + 2,       ey + 38 - 2);
    standoff(ex + 52 - 2,  ey + 38 - 2);

    // Corner screw bosses
    for (x=[8, box_w-8]) for (y=[8, box_d-8])
        translate([x, y, 0]) cylinder(h=body_h, d=7, $fn=16);
}

module lid() {
    fl_d    = 3.5;
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
        // Label recess on top
        translate([(box_w-60)/2, (box_d-12)/2, -0.5])
            cube([60, 12, 1.0]);
    }
    // Raised label text
    translate([(box_w-54)/2, (box_d-8)/2, plate_h-0.8])
        linear_extrude(1.0)
            text("GPSDO CAL REF", size=4.5, font="Liberation Sans:style=Bold");
}

// ── Render ─────────────────────────────────────────────────────────────────
print_part = "body";   // "body" | "lid"

if (print_part == "body")
    color("darkslategray", 0.9) body();
else
    color("slategray", 0.9) lid();

// ── Panel Labels (for reference — print separate decal or engrave) ──────────
// FRONT (y=box_d face):  10M | 1M | PROG | CAL-LVL | GPS | LOCK
// REAR  (y=0 face):      GPS-ANT | USB | 5V-IN | FREQ-IN
