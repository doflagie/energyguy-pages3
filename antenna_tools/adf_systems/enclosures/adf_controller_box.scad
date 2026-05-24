/**
 * adf_controller_box.scad — TM-ADF-ENC-001 Rev A
 * ADF System Controller Enclosure
 *
 * Houses ESP32 DevKit + SSD1306 OLED + QMC5883L + GPS module
 * Compatible with Watson-Watt HF, Doppler VHF, and Loop RDF firmware.
 *
 * Front panel: OLED window, status LED (×2), rotary encoder
 * Rear panel: DC power, USB programming, 4× BNC/SMA connectors
 * Side: GPS antenna SMA, external antenna port
 *
 * Print settings: PETG, 30% gyroid, 3 perimeters, 0.2mm
 * Envelope: body 160×100×60mm, lid 160×100×12mm (both fit 200mm cube)
 * Document: TM-ADF-ENC-001 Rev A
 */

// ─── Parameters ───────────────────────────────────────────────
box_w     = 160.0;
box_d     = 100.0;
box_h     = 58.0;
lid_h     = 12.0;
wall      = 3.0;
floor_h   = 3.5;
corner_r  = 6.0;
lid_overlap = 3.0;

// Front panel features
oled_w    = 30.0;   oled_h  = 16.0;
oled_x    = box_w * 0.28;
oled_z    = box_h * 0.62;

enc_dia   = 7.5;
enc_x     = box_w * 0.52;
enc_z     = oled_z;

pwr_led_dia = 3.3;
bt_led_dia  = 3.3;
pwr_led_x   = 18;
bt_led_x    = 28;
led_z       = box_h * 0.4;

// Rear panel
dc_jack_d = 12.0;
dc_jack_x = box_w - 20;
dc_jack_z = 20;

usb_w = 12.0;  usb_h = 6.0;
usb_x = 30;    usb_z = 15;

// RF connectors (SMA panel-mount, 6.5mm hole, M3 bolts at 11.4mm spacing)
sma_dia     = 6.5;
sma_spacing = 20.0;
sma_z       = 22;
// 4 SMA ports: for N/S/EW/SENSE or array elements
sma_x_start = 55;

// GPS antenna SMA on left side
gps_sma_z = 20;

// Lid bolts
lid_bolt_d    = 3.3;
lid_bolt_sp_x = box_w - 18;
lid_bolt_sp_y = box_d - 18;

// Standoffs for ESP32 (47×18mm bolt pattern, M2.5)
esp_x = box_w / 2 + 5;
esp_y = box_d / 2 + 5;
esp_bx = 23.5;
esp_by = 9.0;
so_h = 5.0; so_od = 6.0; so_id = 2.7;

// Standoff for GPS module (small, 20×20mm)
gps_x = 20;  gps_y = box_d - 25;

// ─── Modules ──────────────────────────────────────────────────

module rounded_box(w, d, h, r) {
    hull()
        for (x=[r, w-r]) for (y=[r, d-r])
            translate([x, y, 0]) cylinder(h=h, r=r, $fn=24);
}

module standoff(x, y, h=so_h, od=so_od, id=so_id) {
    translate([x, y, floor_h])
        difference() {
            cylinder(h=h, d=od, $fn=16);
            translate([0,0,-0.5]) cylinder(h=h+1, d=id, $fn=16);
        }
}

module sma_cutout(x, z, d=box_d) {
    translate([x, d+0.5, z])
    rotate([90,0,0])
        cylinder(h=wall+1, d=sma_dia, $fn=16);
}

module adf_body() {
    difference() {
        rounded_box(box_w, box_d, box_h, corner_r);

        // Inner cavity
        translate([wall, wall, floor_h])
            rounded_box(box_w-2*wall, box_d-2*wall, box_h, corner_r-wall+0.5);

        // ── FRONT PANEL ─────────────────────────────────────
        // OLED window
        translate([oled_x - oled_w/2, -0.5, oled_z - oled_h/2])
        rotate([-90,0,0])
            cube([oled_w, oled_h, wall+1]);

        // Encoder
        translate([enc_x, -0.5, enc_z])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=enc_dia, $fn=16);

        // LEDs
        translate([pwr_led_x, -0.5, led_z])
        rotate([-90,0,0]) cylinder(h=wall+1, d=pwr_led_dia, $fn=16);
        translate([bt_led_x,  -0.5, led_z])
        rotate([-90,0,0]) cylinder(h=wall+1, d=bt_led_dia,  $fn=16);

        // Label recess
        translate([box_w/2 - 50, -0.5, 4])
        rotate([-90,0,0]) cube([100, 8, 1.5]);

        // ── REAR PANEL ──────────────────────────────────────
        // DC barrel jack
        translate([dc_jack_x, box_d+0.5, dc_jack_z])
        rotate([90,0,0]) cylinder(h=wall+1, d=dc_jack_d, $fn=32);

        // USB slot
        translate([usb_x - usb_w/2, box_d+0.5, usb_z - usb_h/2])
        rotate([90,0,0]) cube([usb_w, usb_h, wall+1]);

        // 4× SMA connectors
        for (i=[0:3])
            sma_cutout(sma_x_start + i*sma_spacing, sma_z);

        // ── LEFT SIDE PANEL ─────────────────────────────────
        // GPS antenna SMA
        translate([-0.5, box_d/2, gps_sma_z])
        rotate([0,90,0]) cylinder(h=wall+1, d=sma_dia, $fn=16);

        // ── LID BOLT HOLES ───────────────────────────────────
        for (x=[-1,1]) for (y=[-1,1])
            translate([box_w/2 + x*lid_bolt_sp_x/2,
                       box_d/2 + y*lid_bolt_sp_y/2,
                       box_h-4]) {
                cylinder(h=6, d=lid_bolt_d, $fn=16);
                cylinder(h=3, d=7, $fn=16);
            }
    }

    // PCB standoffs — ESP32
    for (xi=[-1,1]) for (yi=[-1,1])
        standoff(esp_x + xi*esp_bx, esp_y + yi*esp_by);

    // GPS module standoffs
    standoff(gps_x,       gps_y);
    standoff(gps_x + 20,  gps_y);
}

module adf_lid() {
    difference() {
        union() {
            rounded_box(box_w, box_d, lid_h - lid_overlap, corner_r);
            translate([0, 0, lid_h - lid_overlap])
                rounded_box(box_w-2*wall-0.4, box_d-2*wall-0.4, lid_overlap, corner_r-wall);
        }
        // Bolt holes
        for (x=[-1,1]) for (y=[-1,1])
            translate([box_w/2 + x*lid_bolt_sp_x/2,
                       box_d/2 + y*lid_bolt_sp_y/2, -0.5])
                cylinder(h=lid_h+1, d=lid_bolt_d, $fn=16);
        // Vent slots (3×)
        for (x=[50, 80, 110])
            translate([x, box_d*0.25, -0.5])
                cube([6, box_d*0.5, lid_h+1]);
    }
}

// ─── Render ───────────────────────────────────────────────────
$fn = 48;
print_part = "body";  // "body" | "lid" | "all"

if (print_part == "body") {
    color("darkgray") adf_body();
} else if (print_part == "lid") {
    color("gray")
    translate([0, 0, box_h + 10]) adf_lid();
} else {
    color("darkgray", 0.85) adf_body();
    color("gray", 0.5)
    translate([0, 0, box_h + 5]) adf_lid();
}

// ─── Notes ────────────────────────────────────────────────────
// FRONT PANEL (L to R): [PWR LED][BT LED] [OLED 30×16mm] [ENCODER]
// REAR PANEL (L to R): [USB] [SMA×4: E1/E2/E3/SENSE] [DC 12V]
// LEFT SIDE: [GPS SMA]
// ASSEMBLY:
//   1. M3 heat-set inserts in all standoffs
//   2. Mount ESP32 on standoffs with M2.5×6
//   3. Mount GPS module on GPS standoffs with M2.5×6
//   4. Press OLED into front panel window, hot-glue to secure
//   5. Install encoder and LEDs through front panel
//   6. SMA connectors thread through rear holes and nut from inside
//   7. Close lid, 4× M3×8 screws
