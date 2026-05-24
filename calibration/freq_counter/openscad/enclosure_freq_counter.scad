/*
 * enclosure_freq_counter.scad
 * Frequency Counter Enclosure — TM-FC-001 Rev A
 * CYD (ESP32-2432S028) display, SMA I/O, rotary controls, GPS SMA
 * Material: ASA (UV/temp resistant)
 * Print bed: 200 × 200 mm (parts split to fit)
 *
 * Overall box: 170 × 100 × 52 mm (L × W × H)
 * Front panel:  SMA input J1, SMA prescaler J2, attenuator rotary
 * Rear panel:   SMA ref output J4, SMA GPS J5, USB-C power, power switch
 * Lid:          CYD window, gate LED, gate-time rotary, mode select rotary
 * Battery bay:  Removable tray (2 × 9V or 1 × 18650 × 2S)
 */

$fn = 48;

// ── Dimensions ────────────────────────────────────────────────────────────────
BOX_L   = 170;
BOX_W   = 100;
BOX_H   = 52;
WALL    = 2.4;
CORNER  = 4;
LID_H   = 8;    // lid engagement depth
FILLET  = 1.5;

// ── Hole sizes ─────────────────────────────────────────────────────────────────
D_SMA   = 10.0;   // SMA panel-mount hole
D_BNC   = 12.5;   // BNC (not used here but kept for reference)
D_ROT   = 10.5;   // rotary pot shaft
D_DFLAT = 6.8;    // D-flat rotary shaft flat-to-flat
D_USB   = 9.5;    // USB-C cutout width
D_USB_H = 4.0;    // USB-C cutout height
D_LED   = 5.2;    // 5mm LED bezel
D_SWPWR = 16.0;   // power rocker width
D_SWPWH = 10.0;   // power rocker height
D_M3    = 3.4;    // M3 screw clearance
D_M3_HS = 4.2;    // M3 heat-set insert OD (for Ruthex MK3-4)
D_M3_DP = 6.0;    // M3 insert depth
CYD_W   = 75.0;   // CYD visible window width
CYD_H   = 50.0;   // CYD visible window height

// ── Utilities ─────────────────────────────────────────────────────────────────
module rounded_box(l, w, h, r) {
    hull() {
        for (x = [r, l-r]) for (y = [r, w-r]) {
            translate([x, y, 0]) cylinder(r=r, h=h);
        }
    }
}

module d_shaft_hole(d, flat) {
    // Circular hole with D-flat cutout
    cylinder(d=d, h=20, center=true);
    translate([flat/2, 0, 0]) cube([d, d, 20], center=true);
}

// ── Box body (bottom half) ────────────────────────────────────────────────────
module box_body() {
    difference() {
        rounded_box(BOX_L, BOX_W, BOX_H - LID_H, CORNER);
        // Interior cavity
        translate([WALL, WALL, WALL])
            rounded_box(BOX_L - 2*WALL, BOX_W - 2*WALL,
                        BOX_H - LID_H - WALL + 0.1, CORNER - WALL);
        // Front panel cutouts
        front_holes();
        // Rear panel cutouts
        rear_holes();
        // Lid alignment groove
        translate([WALL, WALL, BOX_H - LID_H - 1])
            rounded_box(BOX_L - 2*WALL, BOX_W - 2*WALL, LID_H + 1.1, CORNER - WALL + 0.5);
    }
    // Internal pillars for PCB mounting (M3 standoffs, 4 corners)
    pcb_pillars();
    // Battery bay wall
    battery_divider();
    // Lid screw bosses
    lid_bosses(is_body=true);
}

module front_holes() {
    // Front face: y=0 plane
    // J1: SMA main input — left side
    translate([28, -1, BOX_H/2 - LID_H/2])
        rotate([-90, 0, 0]) cylinder(d=D_SMA, h=WALL+2);
    // J2: SMA prescaler input — center-left
    translate([60, -1, BOX_H/2 - LID_H/2])
        rotate([-90, 0, 0]) cylinder(d=D_SMA, h=WALL+2);
    // S1: Attenuator rotary — center
    translate([100, -1, BOX_H/2 - LID_H/2]) {
        rotate([-90, 0, 0]) d_shaft_hole(D_ROT, D_DFLAT);
    }
    // S2: LP filter rotary — right center
    translate([135, -1, BOX_H/2 - LID_H/2]) {
        rotate([-90, 0, 0]) d_shaft_hole(D_ROT, D_DFLAT);
    }
    // S3: ERA amp bypass — far right
    translate([158, -1, BOX_H/2 - LID_H/2])
        rotate([-90, 0, 0]) cylinder(d=D_ROT, h=WALL+2);
}

module rear_holes() {
    // Rear face: y=BOX_W plane
    // J4: SMA 10 MHz ref output
    translate([28, BOX_W - WALL - 1, BOX_H/2 - LID_H/2])
        rotate([90, 0, 0]) cylinder(d=D_SMA, h=WALL+2);
    // J5: GPS antenna SMA
    translate([60, BOX_W - WALL - 1, BOX_H/2 - LID_H/2])
        rotate([90, 0, 0]) cylinder(d=D_SMA, h=WALL+2);
    // USB-C power input
    translate([100, BOX_W - WALL - 1, BOX_H/2 - LID_H/2])
        rotate([90, 0, 0]) cube([D_USB, D_USB_H, WALL+2], center=true);
    // Power rocker switch
    translate([140, BOX_W - WALL - 1, BOX_H/2 - LID_H/2])
        rotate([90, 0, 0]) cube([D_SWPWR, D_SWPWH, WALL+2], center=true);
}

module pcb_pillars() {
    // 4 × M3 pillars, 10 mm tall, 6 mm OD
    for (pos = [[12, 12], [12, BOX_W-20], [BOX_L-20, 12], [BOX_L-20, BOX_W-20]]) {
        translate([pos[0], pos[1], WALL]) {
            difference() {
                cylinder(d=6, h=10);
                cylinder(d=D_M3_HS, h=D_M3_DP);
            }
        }
    }
}

module battery_divider() {
    // Internal wall separating battery bay (rear 30 mm)
    translate([BOX_L - 35, WALL, WALL])
        cube([WALL, BOX_W - 2*WALL, BOX_H - LID_H - WALL]);
    // Cable pass-through slot
    translate([BOX_L - 35 - 0.1, BOX_W/2 - 8, WALL])
        cube([WALL + 0.2, 16, 12]);
}

module lid_bosses(is_body) {
    for (pos = [[10, 10], [10, BOX_W-10], [BOX_L-10, 10], [BOX_L-10, BOX_W-10]]) {
        translate([pos[0], pos[1], BOX_H - LID_H - 1]) {
            if (is_body) {
                difference() {
                    cylinder(d=8, h=LID_H + 1);
                    translate([0, 0, -0.1]) cylinder(d=D_M3_HS, h=D_M3_DP + 0.1);
                }
            }
        }
    }
}

// ── Lid (top half with CYD window) ───────────────────────────────────────────
module lid() {
    difference() {
        union() {
            // Outer lid shell
            rounded_box(BOX_L, BOX_W, LID_H, CORNER);
            // Lip that drops into box body
            translate([WALL + 0.6, WALL + 0.6, -LID_H + 0.5])
                rounded_box(BOX_L - 2*WALL - 1.2, BOX_W - 2*WALL - 1.2,
                            LID_H - 0.5, CORNER - WALL);
        }
        // CYD display window — centered in lid, shifted left
        translate([BOX_L/2 - 40, BOX_W/2 - CYD_H/2, -0.1])
            rounded_box(CYD_W, CYD_H, LID_H + 0.2, 3);
        // Gate LED hole
        translate([BOX_L/2 + 30, BOX_W/2, -0.1])
            cylinder(d=D_LED, h=LID_H + 0.2);
        // Gate-time rotary
        translate([BOX_L - 28, 20, -0.1])
            d_shaft_hole(D_ROT, D_DFLAT);
        // Mode select rotary
        translate([BOX_L - 28, BOX_W - 20, -0.1])
            d_shaft_hole(D_ROT, D_DFLAT);
        // M3 lid screw holes
        for (pos = [[10, 10], [10, BOX_W-10], [BOX_L-10, 10], [BOX_L-10, BOX_W-10]]) {
            translate([pos[0], pos[1], -0.1])
                cylinder(d=D_M3, h=LID_H + 0.2);
        }
    }
    // CYD retention clips (snap tabs inside window edges)
    cyd_clips();
}

module cyd_clips() {
    // 4 small L-shaped tabs at CYD window corners to hold CYD PCB
    for (xo = [0, CYD_W - 4], yo = [0, CYD_H - 4]) {
        translate([BOX_L/2 - 40 + xo, BOX_W/2 - CYD_H/2 + yo, 1]) {
            difference() {
                cube([4, 4, 3]);
                translate([0.8, 0.8, -0.1]) cube([2.4, 2.4, 3.2]);
            }
        }
    }
}

// ── Battery tray ──────────────────────────────────────────────────────────────
// Slides in from rear; holds 2 × PP3 (9V) batteries side by side
// Tray dims: 29 × 90 × 28 mm
module battery_tray() {
    TRAY_L = 29;
    TRAY_W = 90;
    TRAY_H = 28;
    TRAY_W_INT = 2;
    difference() {
        rounded_box(TRAY_L, TRAY_W, TRAY_H, 2);
        translate([TRAY_W_INT, TRAY_W_INT, TRAY_W_INT])
            rounded_box(TRAY_L - 2*TRAY_W_INT, TRAY_W - 2*TRAY_W_INT,
                        TRAY_H - TRAY_W_INT + 0.1, 1);
        // Battery contact holes (solder-through)
        for (y = [22, 67]) translate([TRAY_L/2, y, -0.1]) cylinder(d=8, h=3);
    }
    // Finger pull tab
    translate([TRAY_L, TRAY_W/2 - 8, 0]) cube([6, 16, TRAY_H]);
    // Battery separator
    translate([TRAY_W_INT, TRAY_W/2 - 0.6, TRAY_W_INT])
        cube([TRAY_L - 2*TRAY_W_INT, 1.2, TRAY_H - TRAY_W_INT]);
}

// ── Feet ──────────────────────────────────────────────────────────────────────
module feet() {
    foot_h = 3;
    foot_d = 10;
    for (pos = [[10, 10], [10, BOX_W-10], [BOX_L-10, 10], [BOX_L-10, BOX_W-10]]) {
        translate([pos[0], pos[1], -foot_h])
            cylinder(d=foot_d, h=foot_h);
    }
}

// ── SMA label plate ───────────────────────────────────────────────────────────
// Thin label plate adhered to front panel over SMA connectors
module front_label_plate() {
    difference() {
        translate([10, -WALL - 2, BOX_H/2 - LID_H - 10])
            cube([BOX_L - 20, 2, 20]);
        // Holes matching SMA positions
        for (x = [28, 60]) {
            translate([x, -WALL - 3, BOX_H/2 - LID_H/2])
                rotate([-90,0,0]) cylinder(d=D_SMA + 0.5, h=5);
        }
    }
}

// ── Strap lug (rear top) ─────────────────────────────────────────────────────
module strap_lug() {
    translate([BOX_L/2 - 15, BOX_W - WALL - 0.1, BOX_H - LID_H - 3])
    difference() {
        cube([30, 6, 8]);
        translate([7, -0.1, 2]) cube([6, 7, 6]);
        translate([17, -0.1, 2]) cube([6, 7, 6]);
    }
}

// ── Assembly ──────────────────────────────────────────────────────────────────
// Uncomment sections to print individually.

// Bottom box:
box_body();

// Lid (print separately, flip upside-down):
// translate([0, BOX_W + 10, LID_H]) rotate([180, 0, 0]) lid();

// Battery tray:
// translate([BOX_L + 10, 0, 0]) battery_tray();

// Feet (print 4 separately in TPU/rubber filament):
// translate([0, -20, 3]) rotate([180, 0, 0]) feet();
