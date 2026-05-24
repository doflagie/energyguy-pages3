/**
 * control_panel_enclosure.scad — TM-CWND-001 Rev A
 * Controller Electronics Enclosure for Coil Winder
 *
 * Houses:
 *   - ESP32 DevKit V1 (or DevKit C)
 *   - 2× TMC2208 (or A4988) stepper driver boards
 *   - SSD1306 OLED 128×64 (front panel)
 *   - Rotary encoder with pushbutton (front panel)
 *   - Emergency stop mushroom cap (front panel, red)
 *   - Power LED (front panel)
 *   - Speed pot (front panel, panel-mount 10kΩ)
 *   - Power input (rear: 5.5/2.1mm barrel or screw terminals)
 *   - Motor output cables (rear: 4× 2-conductor, or 2× 4-conductor)
 *   - SD card access slot (side)
 *   - USB programming port (side or rear)
 *   - BT status LED (front)
 *
 * Two-piece design: body + lid
 * Lid secured with 4× M3 screws
 * DIN rail mounting option (35mm DIN rail clips on bottom)
 *
 * Print settings:
 *   Material: PETG or ABS (ABS better for high-heat environments near motor drivers)
 *   Infill: 30% gyroid
 *   Wall: 3 perimeters
 *   Layer: 0.2mm
 *   Supports: Yes for rear panel cutouts
 *
 * Print envelope:
 *   Body: 195×120×70mm — fits in 200mm cube
 *   Lid: 195×120×15mm — fits in 200mm cube
 */

// ─── Parameters ───────────────────────────────────────────────────────────────

// Outer box
box_w       = 195.0;
box_d       = 120.0;
box_h       = 70.0;
lid_h       = 14.0;
wall        = 3.5;
floor_h     = 4.0;
corner_r    = 7.0;

// Lid overlap
lid_overlap = 3.5;

// Front panel cutouts
// OLED window
oled_w      = 30.0;
oled_h      = 16.0;
oled_x      = box_w/2 - 40;
oled_z      = box_h * 0.6;

// Rotary encoder (KY-040 or EC11 with nut, 7mm thread dia)
enc_dia     = 7.5;
enc_x       = box_w/2;
enc_z       = oled_z;

// Emergency stop (22mm panel hole standard)
estop_dia   = 22.3;
estop_x     = box_w * 0.8;
estop_z     = box_h * 0.55;

// Speed pot (panel-mount 7mm shaft, 10kΩ)
pot_dia     = 7.5;
pot_x       = box_w/2 + 40;
pot_z       = oled_z;

// Status LEDs (3mm)
bt_led_dia  = 3.2;
pwr_led_dia = 3.2;
bt_led_x    = box_w/2 - 70;
pwr_led_x   = box_w/2 - 80;
led_z       = box_h * 0.35;

// Rear panel
// DC barrel jack or screw terminal
dc_jack_d   = 12.0;
dc_jack_x   = box_w/2 + 70;
dc_jack_z   = 20;

// USB port (USB-B or USB-C)
usb_w       = 12.0;
usb_h       = 6.0;
usb_x       = box_w/2 - 60;
usb_z       = 15;

// Motor cable glands (4× PG7, 12mm hole)
gland_dia   = 12.0;
gland_z     = 20;
gland_y_sp  = box_d - 30;

// Side panel
// SD card slot
sd_w        = 14.0;
sd_h        = 4.0;
sd_z        = floor_h + 8;

// PCB standoffs (ESP32 DevKit: 56×28mm, M2.5 corner holes at 47×18mm)
esp32_w     = 56.5;
esp32_d     = 28.5;
esp32_bolt_sp_x = 47.0;
esp32_bolt_sp_y = 18.0;
esp32_x     = box_w/2 - 10;
esp32_y     = box_d/2 + 10;

// Stepper driver board standoffs (generic: 42×15mm, 2× M3)
drv_w       = 42.0;
drv_d       = 15.5;
drv_x_a     = 30;
drv_x_b     = 85;
drv_y       = 25;

// Standoff dimensions
so_h        = 5.0;
so_od       = 6.0;
so_id       = 2.7;  // M3 or M2.5

// Lid fasteners
lid_bolt_dia = 3.3;
lid_bolt_sp_x = box_w - 20;
lid_bolt_sp_y = box_d - 20;

// ─── Modules ──────────────────────────────────────────────────────────────────

module box_outline(w, d, h, r) {
    hull() {
        for (x=[r, w-r]) for (y=[r, d-r])
            translate([x, y, 0])
                cylinder(h=h, r=r, $fn=32);
    }
}

module standoff(x, y, h=so_h, od=so_od, id=so_id) {
    translate([x, y, floor_h]) {
        difference() {
            cylinder(h=h, d=od, $fn=16);
            translate([0,0,-0.5])
                cylinder(h=h+1, d=id, $fn=16);
        }
    }
}

module din_rail_clip() {
    // 35mm DIN rail snap clip (bottom of enclosure)
    clip_w = 60;
    clip_h = 20;
    clip_d = 8;
    rail_w = 35.0;
    rail_h = 7.5;
    difference() {
        cube([clip_w, clip_d, clip_h]);
        // Rail groove
        translate([(clip_w-rail_w)/2, -0.5, clip_h-rail_h])
            cube([rail_w, clip_d/2+1, rail_h+0.5]);
        // M3 mounting holes
        for (x=[8, clip_w-8])
            translate([x, clip_d/2, clip_h/2])
            rotate([90,0,0])
                cylinder(h=clip_d+1, d=3.3, $fn=16, center=true);
    }
}

module control_box_body() {
    difference() {
        box_outline(box_w, box_d, box_h, corner_r);

        // Inner cavity
        translate([wall, wall, floor_h])
            box_outline(box_w-2*wall, box_d-2*wall, box_h, corner_r-wall);

        // ── FRONT PANEL CUTOUTS ──────────────────────────────────────
        // OLED window
        translate([oled_x - oled_w/2, -0.5, oled_z - oled_h/2])
        rotate([-90,0,0])
            cube([oled_w, oled_h, wall+1]);

        // Rotary encoder hole
        translate([enc_x, -0.5, enc_z])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=enc_dia, $fn=16);

        // Emergency stop
        translate([estop_x, -0.5, estop_z])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=estop_dia, $fn=32);

        // Speed pot
        translate([pot_x, -0.5, pot_z])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=pot_dia, $fn=16);

        // LEDs
        translate([bt_led_x, -0.5, led_z])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=bt_led_dia, $fn=16);
        translate([pwr_led_x, -0.5, led_z])
        rotate([-90,0,0])
            cylinder(h=wall+1, d=pwr_led_dia, $fn=16);

        // Label area (embossed recess)
        translate([box_w/2-60, -0.5, 5])
        rotate([-90,0,0])
            cube([120, 10, 1.5]);

        // ── REAR PANEL CUTOUTS ───────────────────────────────────────
        // DC power jack
        translate([dc_jack_x, box_d+0.5, dc_jack_z])
        rotate([90,0,0])
            cylinder(h=wall+1, d=dc_jack_d, $fn=32);

        // USB access slot
        translate([usb_x - usb_w/2, box_d+0.5, usb_z - usb_h/2])
        rotate([90,0,0])
            cube([usb_w, usb_h, wall+1]);

        // Motor cable glands (4 total)
        for (i=[0:3]) {
            translate([20 + i*40, box_d+0.5, gland_z])
            rotate([90,0,0])
                cylinder(h=wall+1, d=gland_dia, $fn=16);
        }

        // ── SIDE PANEL (left) — SD card slot ────────────────────────
        translate([-0.5, box_d/2 - sd_w/2, sd_z])
        rotate([0,90,0])
            cube([sd_w, sd_h, wall+1]);

        // ── LID BOLT HOLES ───────────────────────────────────────────
        for (x=[-1,1]) for (y=[-1,1])
            translate([box_w/2 + x*lid_bolt_sp_x/2,
                       box_d/2 + y*lid_bolt_sp_y/2,
                       box_h-4]) {
                cylinder(h=5, d=lid_bolt_dia, $fn=16);
                cylinder(h=3, d=7, $fn=16);  // head recess
            }
    }

    // PCB standoffs — ESP32
    for (xi=[-1,1]) for (yi=[-1,1])
        standoff(esp32_x + xi*esp32_bolt_sp_x/2,
                 esp32_y + yi*esp32_bolt_sp_y/2);

    // PCB standoffs — Stepper driver A
    for (xi=[-1,1])
        standoff(drv_x_a + xi*drv_w/2 - 5, drv_y);
    // Stepper driver B
    for (xi=[-1,1])
        standoff(drv_x_b + xi*drv_w/2 - 5, drv_y);
}

module control_box_lid() {
    difference() {
        union() {
            box_outline(box_w, box_d, lid_h - lid_overlap, corner_r);
            translate([0,0,lid_h-lid_overlap])
                box_outline(box_w-2*wall-0.5, box_d-2*wall-0.5, lid_overlap, corner_r-wall);
        }
        // Lid bolt holes
        for (x=[-1,1]) for (y=[-1,1])
            translate([box_w/2 + x*lid_bolt_sp_x/2,
                       box_d/2 + y*lid_bolt_sp_y/2, -0.5])
                cylinder(h=lid_h+1, d=lid_bolt_dia, $fn=16);
        // Ventilation slots (top of lid for driver heat)
        for (x=[50, 80, 110])
            translate([x, box_d*0.3, -0.5])
                cube([6, box_d*0.4, lid_h+1]);
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────

$fn = 64;
print_part = "body";  // "body" | "lid" | "din" | "all"

if (print_part == "body") {
    color("darkgray") control_box_body();
} else if (print_part == "lid") {
    color("gray")
    translate([0, 0, box_h + 10])
        control_box_lid();
} else if (print_part == "din") {
    color("lightgray")
    translate([box_w/2 - 30, -din_rail_clip()[1]/2, -20])
        din_rail_clip();
} else {
    color("darkgray", 0.8) control_box_body();
    color("gray", 0.5)
    translate([0, 0, box_h + 5])
        control_box_lid();
}

// ─── Notes ────────────────────────────────────────────────────────────────────
// FRONT PANEL LAYOUT (facing user, left to right):
//   [PWR LED] [BT LED] [OLED DISPLAY] [ENCODER] [SPEED POT] [E-STOP]
//
// REAR PANEL (left to right):
//   [MOTOR 1 GLAND] [MOTOR 2 GLAND] [MOTOR 3 GLAND] [MOTOR 4 GLAND] [USB] [DC IN]
//
// ASSEMBLY:
// 1. Install all M3 threaded inserts in standoffs using soldering iron
// 2. Mount ESP32 DevKit on rear standoffs with M3×6 screws
// 3. Mount stepper drivers on front standoffs
// 4. Connect OLED: press-fit into front panel window, secure with hot glue
// 5. Install encoder, pot, E-stop through front panel holes
// 6. Route motor cable bundles through PG7 glands, tighten to seal
// 7. Install SD card module adjacent to slot hole
// 8. Close lid, secure 4× M3×8 screws
//
// THERMAL:
// TMC2208 at 1.5A generates ~1.5W per driver = 3W total
// Ventilation slots in lid provide passive cooling
// Add small heatsinks to driver ICs if ambient > 35°C
// If fanless chassis: derate motor current 20% (set Vref lower)
