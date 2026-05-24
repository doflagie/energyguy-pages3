/**
 * freq_counter_box.scad — TM-CAL-ENC-004 Rev A
 * Standalone Frequency Counter Enclosure
 *
 * Standalone counter for field use when full cal station isn't needed.
 * Accepts GPSDO 1PPS reference via SMA (rear), or runs free-running
 * (lower accuracy) if GPS not connected.
 *
 * Contents:
 *   - ESP32-WROOM-32 devboard
 *   - 74ACT14 Schmitt trigger + 74HC390 prescaler PCB
 *   - SSD1306 OLED 128×64 (front panel)
 *
 * Front panel: BNC FREQ IN, OLED display, range switch, gate switch
 * Rear panel:  SMA 1PPS IN (GPS gate), USB power, SMA HOLD (future)
 *
 * Envelope: 100×65×38 mm
 */

// ── Parameters ──────────────────────────────────────────────────────────────
box_w  = 100.0;
box_d  = 65.0;
box_h  = 38.0;
wall   = 2.8;
floor_h = 3.0;
lid_h  = 10.0;
body_h = box_h - lid_h;
r      = 4.0;
$fn    = 32;

// Component cutouts
bnc_d      = 9.5;
sma_d      = 6.5;
oled_w     = 28.0;   // SSD1306 0.96" OLED active area
oled_h     = 14.0;
usb_w      = 9.0;
usb_h      = 4.0;
sw_d       = 6.0;    // toggle switch shaft hole
screw_d    = 3.3;

// OLED position: centered horizontally, upper third of front panel
oled_x     = (box_w - oled_w) / 2;
oled_z     = floor_h + 10;

module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

module standoff(x, y) {
    translate([x, y, floor_h])
        difference() {
            cylinder(h=5, d=5.5, $fn=16);
            translate([0,0,-0.5]) cylinder(h=6, d=2.7, $fn=16);
        }
}

module body() {
    difference() {
        rounded_box(box_w, box_d, body_h, r);

        // Inner cavity
        translate([wall, wall, floor_h])
            rounded_box(box_w-2*wall, box_d-2*wall, body_h, r-wall+0.5);

        // ── FRONT PANEL (y=box_d) ──────────────────────────────────────
        // BNC FREQ IN (left side of front panel)
        translate([18, box_d+0.5, floor_h + (body_h-floor_h)/2])
        rotate([90,0,0]) cylinder(h=wall+1, d=bnc_d, $fn=16);

        // OLED window (center/right)
        translate([oled_x, box_d+0.5, oled_z])
            cube([oled_w, wall+1, oled_h]);

        // Range toggle switch (÷1/÷10)
        translate([70, box_d+0.5, floor_h + (body_h-floor_h)/2 - 6])
        rotate([90,0,0]) cylinder(h=wall+1, d=sw_d, $fn=16);

        // Gate toggle switch (1s/10s)
        translate([85, box_d+0.5, floor_h + (body_h-floor_h)/2 - 6])
        rotate([90,0,0]) cylinder(h=wall+1, d=sw_d, $fn=16);

        // ── REAR PANEL (y=0) ──────────────────────────────────────────
        // SMA: 1PPS IN from GPSDO
        translate([20, -0.5, floor_h + (body_h-floor_h)/2])
        rotate([-90,0,0]) cylinder(h=wall+1, d=sma_d, $fn=16);

        // USB Micro-B power
        translate([50-usb_w/2, -0.5, floor_h+4])
            cube([usb_w, wall+1, usb_h]);

        // ── Lid screw holes ───────────────────────────────────────────
        for (x=[8, box_w-8]) for (y=[8, box_d-8])
            translate([x, y, body_h-5])
                cylinder(h=6, d=screw_d, $fn=16);
    }

    // PCB standoffs for ESP32 devboard
    ex = (box_w - 52) / 2 - 4;
    ey = (box_d - 28) / 2;
    standoff(ex + 2,       ey + 2);
    standoff(ex + 52-2,    ey + 2);
    standoff(ex + 2,       ey + 28-2);
    standoff(ex + 52-2,    ey + 28-2);

    // Corner bosses
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
    }
    // Label on lid top face
    translate([(box_w-60)/2, (box_d-10)/2, plate_h - 0.6])
        linear_extrude(0.8)
            text("GPS FREQ COUNTER", size=4.5, font="Liberation Sans:style=Bold");
}

print_part = "body";   // "body" | "lid"

if (print_part == "body")
    color("darkcyan", 0.9) body();
else
    color("cadetblue", 0.9) lid();

// ── Assembly Notes ──────────────────────────────────────────────────────────
// 1. Print in PETG. Connect BNC panel connector to 74ACT14 input board
//    via short coax stub (keep trace <20mm to minimize pickup).
// 2. SSD1306 OLED: glue into front panel opening from inside with silicone.
//    I2C wires: SDA → GPIO21, SCL → GPIO22.
// 3. Range switch (SPDT): connects 74HC390 ÷10 output to GPIO33 when in
//    DIV10 position; wires direct input GPIO32 otherwise.
// 4. Gate switch (SPDT): connects to GPIO39 (ADC-only, used as digital in);
//    LOW = 1s gate, HIGH = 10s gate.
// 5. 1PPS SMA: wire to GPIO34 via 4.7kΩ pull-up to 3V3.
// 6. Counter operates autonomously; provides BT packet if connected to CYD.
