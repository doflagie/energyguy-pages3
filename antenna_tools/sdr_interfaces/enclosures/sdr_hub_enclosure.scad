/**
 * sdr_hub_enclosure.scad — TM-SDR-ENC-003 Rev A
 * SDR Interface Hub: Master Enclosure
 * Houses: ESP32 controller + RTL-SDR dongle bay + attenuator board + bias-T
 *
 * Features:
 *   - 190×140×65 mm overall
 *   - Removable RTL-SDR dongle tray (USB-A port internal)
 *   - SMA IN × 2 (antenna A and B)
 *   - SMA OUT × 2 (to SDR dongle and upconverter)
 *   - USB-B for main power/control
 *   - 8-pin IDC to preselector box
 *   - Barrel jack 12V for relay supply
 *   - Ventilation slots (RF-shielded with offset slots)
 *   - Front panel: OLED window + rotary encoder hole + 4× LED
 *
 * Envelope: 190×140×65 mm (fits 200×200×200 build, just)
 * Material: PETG
 */

// ── Parameters ─────────────────────────────────────────────────────────────
box_w   = 190.0;
box_d   = 140.0;
box_h   = 65.0;
wall    = 3.0;
floor_h = 3.5;
lid_h   = 14.0;
body_h  = box_h - lid_h;
r = 6.0;
$fn = 32;

// OLED window
oled_w  = 30.0;
oled_h  = 12.0;

// Rotary encoder: EC11, 7mm shaft
enc_d   = 7.2;

// SMA holes
sma_d   = 6.5;

// USB A (internal for RTL-SDR dongle)
usba_w  = 15.0;
usba_h  = 7.5;

// USB B (external power/data)
usbb_w  = 12.0;
usbb_h  = 11.0;

// Barrel jack
barrel_d = 6.5;

// IDC 8-pin
idc8_w   = 24.0;
idc8_h   = 9.0;

// Screw posts
screw_d  = 3.3;   // M3
boss_d   = 8.0;

// RTL-SDR dongle tray parameters
tray_w  = 38.0;
tray_d  = 90.0;
tray_h  = 22.0;

// ESP32 footprint (DevKit 30-pin)
esp_w   = 53.0;
esp_d   = 28.0;

// Ventilation slots (staggered, RF safe — slots < λ/2 at 3 GHz = 50mm, use 10mm)
vent_l  = 10.0;
vent_w  = 2.0;

module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

module pcb_standoff(x, y, sh, od, id) {
    translate([x, y, floor_h])
        difference() {
            cylinder(h=sh, d=od, $fn=16);
            translate([0,0,-0.5]) cylinder(h=sh+1, d=id, $fn=16);
        }
}

module vent_slots(x_start, y, count, spacing) {
    for (i=[0:count-1])
        translate([x_start + i*spacing, y - 0.5, body_h - 14])
            cube([vent_w, wall+1, vent_l]);
}

// ── RTL-SDR Dongle Tray ────────────────────────────────────────────────────
module rtl_sdr_tray() {
    // Printed separately, slides into body guide rails
    difference() {
        cube([tray_w, tray_d, tray_h]);

        // RTL-SDR dongle pocket (28×85×20 mm)
        translate([2, 2, 3])
            cube([tray_w-4, tray_d-4, tray_h]);  // open top

        // USB port opening in front of tray
        translate([-0.5, (tray_d-usba_w)/2, 5])
            cube([wall+1, usba_w, usba_h]);

        // SMA hole (dongle antenna input)
        translate([tray_w/2, -0.5, tray_h/2])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=sma_d, $fn=16);
    }
    // Guide rails (on outside of tray, engage body guide grooves)
    translate([-2, 0, tray_h-3])
        cube([2, tray_d, 3]);
    translate([tray_w, 0, tray_h-3])
        cube([2, tray_d, 3]);
}

// ── Main Body ──────────────────────────────────────────────────────────────
module body() {
    difference() {
        rounded_box(box_w, box_d, body_h, r);

        // Inner cavity
        translate([wall, wall, floor_h])
            rounded_box(box_w-2*wall, box_d-2*wall, body_h, r-wall+0.5);

        // ── FRONT PANEL (y = box_d) ────────────────────────────────────

        // OLED window (centered horizontally)
        translate([box_w/2 - oled_w/2, box_d - wall - 0.5, body_h - oled_h - 8])
            cube([oled_w, wall+1, oled_h]);

        // Rotary encoder
        translate([box_w/2 + 24, box_d+0.5, body_h - 14])
        rotate([90,0,0])
            cylinder(h=wall+1, d=enc_d, $fn=16);

        // LED holes (4×)
        for (i=[0:3])
            translate([box_w/2 - 30 + i*14, box_d+0.5, body_h - 6])
            rotate([90,0,0])
                cylinder(h=wall+1, d=3.2, $fn=16);

        // ── REAR PANEL (y = 0) ─────────────────────────────────────────

        // SMA IN × 2 (antenna inputs)
        translate([20, -0.5, floor_h + (body_h-floor_h)*0.5])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=sma_d, $fn=16);
        translate([40, -0.5, floor_h + (body_h-floor_h)*0.5])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=sma_d, $fn=16);

        // SMA OUT × 2 (to upconverter and direct)
        translate([60, -0.5, floor_h + (body_h-floor_h)*0.5])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=sma_d, $fn=16);
        translate([80, -0.5, floor_h + (body_h-floor_h)*0.5])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=sma_d, $fn=16);

        // IDC 8-pin connector to preselector
        translate([110, -0.5, floor_h + 5])
        rotate([-90,0,0])
            cube([idc8_w, idc8_h, wall+1]);

        // USB-B (main control)
        translate([box_w - 30, -0.5, floor_h + 3])
        rotate([-90,0,0])
            cube([usbb_w, usbb_h, wall+1]);

        // Barrel jack 12V
        translate([box_w - 15, -0.5, floor_h + (body_h-floor_h)*0.5])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=barrel_d, $fn=16);

        // ── RIGHT SIDE: RTL-SDR dongle tray slot ─────────────────────
        // Tray slot guides (horizontal grooves)
        translate([box_w - wall - 0.5, (box_d-tray_d)/2, body_h - tray_h - 2])
        rotate([0,90,0])
            cube([tray_h + 2, tray_d, wall+1]);  // full tray opening

        // ── VENTILATION (top-side slots, RF-safe offset pattern) ──────
        vent_slots(wall + 10, box_d - wall, 8, 15);   // rear cluster
        vent_slots(wall + 10, wall,         8, 15);    // front cluster (offset)

        // ── CORNER SCREW HOLES ─────────────────────────────────────────
        for (x=[10, box_w-10]) for (y=[10, box_d-10])
            translate([x, y, body_h-6]) {
                cylinder(h=7, d=screw_d, $fn=16);
                cylinder(h=3, d=boss_d,  $fn=16);
            }
    }

    // ── PCB standoffs: ESP32 controller board ─────────────────────────
    ox = wall + 5; oy = wall + 5;
    pcb_standoff(ox,         oy,         5, 7, 2.7);
    pcb_standoff(ox+esp_w,   oy,         5, 7, 2.7);
    pcb_standoff(ox,         oy+esp_d,   5, 7, 2.7);
    pcb_standoff(ox+esp_w,   oy+esp_d,   5, 7, 2.7);

    // ── PCB standoffs: attenuator + bias-T board (right of ESP32) ─────
    atx = ox + esp_w + 12;
    pcb_standoff(atx,     oy,          5, 7, 2.7);
    pcb_standoff(atx+40,  oy,          5, 7, 2.7);
    pcb_standoff(atx,     oy+40,       5, 7, 2.7);
    pcb_standoff(atx+40,  oy+40,       5, 7, 2.7);

    // ── Tray guide rails (inside body, right wall) ─────────────────────
    translate([box_w - wall - tray_w - 4, (box_d-tray_d)/2, body_h - tray_h - 2]) {
        // Bottom guide ledge
        cube([tray_w + 4, tray_d, 2]);
        // Side guide rails
        cube([2, tray_d, tray_h]);
        translate([tray_w+2, 0, 0]) cube([2, tray_d, tray_h]);
    }

    // Corner screw bosses
    for (x=[10, box_w-10]) for (y=[10, box_d-10])
        translate([x, y, 0]) cylinder(h=body_h, d=boss_d, $fn=16);
}

// ── Lid ─────────────────────────────────────────────────────────────────────
module lid() {
    fl_d = 4.0;
    plate_h = lid_h - fl_d;
    difference() {
        union() {
            rounded_box(box_w, box_d, plate_h, r);
            translate([wall+1, wall+1, plate_h])
                rounded_box(box_w-2*(wall+1), box_d-2*(wall+1), fl_d, r-wall);
        }
        // Screw holes
        for (x=[10, box_w-10]) for (y=[10, box_d-10])
            translate([x, y, -0.5])
                cylinder(h=lid_h+1, d=screw_d, $fn=16);
        // Cable access notch
        translate([box_w/2 - 10, -0.5, -0.5])
            cube([20, wall+1, plate_h + 1]);
    }
}

// ── Render ─────────────────────────────────────────────────────────────────
print_part = "body";  // "body" | "lid" | "tray"

if (print_part == "body") {
    color("darkslategray", 0.9) body();
} else if (print_part == "lid") {
    color("gray", 0.9) lid();
} else if (print_part == "tray") {
    color("steelblue", 0.9) rtl_sdr_tray();
} else {
    color("darkslategray", 0.7) body();
    color("gray", 0.5)
    translate([0, 0, body_h]) lid();
    color("steelblue", 0.9)
    translate([box_w - wall - tray_w - 2, (box_d-tray_d)/2, body_h - tray_h])
        rtl_sdr_tray();
}

// ── Assembly notes ─────────────────────────────────────────────────────────
// INSTALLATION ORDER:
//   1. Line box interior with copper tape (walls + floor).
//   2. Mount ESP32 DevKit on 4× standoffs (front-left quadrant).
//   3. Mount ATT/bias-T PCB on 4× standoffs (center-right).
//   4. Install RTL-SDR dongle in tray; slide tray into right-side slot.
//   5. Connect USB-A (internal) from ESP32 USB hub to RTL-SDR dongle.
//   6. Connect SMA pigtails between boards and front/rear panel SMA connectors.
//   7. Connect IDC ribbon to preselector box (6×SMA pigtail + IDC cable).
//   8. Install lid; secure with 4× M3×8 cap screws.
//
// RF SHIELDING:
//   Copper tape on all interior surfaces provides >40 dB shielding effectiveness
//   at HF frequencies. For VHF/UHF, solder all seams.
//   Vent slots are 10mm long: first resonance at 15 GHz — safe below 3 GHz.
