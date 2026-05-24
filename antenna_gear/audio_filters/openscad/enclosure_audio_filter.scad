/*
 * enclosure_audio_filter.scad
 * Portable CW/SSB Audio Filter Enclosure
 * TM-AF-001 Rev A
 * Material: ASA or PETG; print bed 200×200 mm
 *
 * Outer dimensions: 160 × 100 × 55 mm (fits 200×200 print bed)
 * Wall: 2.5 mm
 * Lid: 12 mm deep
 *
 * Front panel (y=0):
 *   Left:   IN jack (3.5mm), OUT jack (3.5mm)
 *   Center: Bypass toggle (DPDT), Power toggle (SPST), LED
 *   Right:  Mode rotary knob (encoder), BW rotary (if analog board)
 *
 * Rear panel (y=BOX_W):
 *   Battery compartment door (2× PP3 or 1× 18650 tray)
 *   USB-C charge port
 *
 * Top lid:
 *   No cutouts (clean); optional label recess
 *
 * Board mounting:
 *   M3 standoffs (×4) for PCB or DSP module
 *   PP3 battery holders: two slots cast into rear wall
 */

$fn = 48;

BOX_L = 160;
BOX_W = 100;
BOX_H = 55;
WALL  = 2.5;
CORN  = 5.0;
LID_H = 12;

D_35MM   = 6.5;    // 3.5mm audio jack hole
D_TOGGLE = 6.4;    // SPST / DPDT toggle switch
D_ENC    = 7.2;    // Rotary encoder shaft
D_LED    = 3.3;    // LED 3mm
D_USBC_W = 9.5;    // USB-C slot width
D_USBC_H = 3.5;    // USB-C slot height
D_M3     = 3.4;    // M3 clearance
D_M3_HS  = 5.5;    // M3 heat-set insert bore

// ── Utilities ─────────────────────────────────────────────────────────────────

module rounded_box(l, w, h, r) {
    hull() for (x=[r, l-r]) for (y=[r, w-r])
        translate([x, y, 0]) cylinder(r=r, h=h);
}

module jack_hole() {
    cylinder(d=D_35MM, h=WALL+1, center=true);
}

module toggle_hole() {
    cylinder(d=D_TOGGLE, h=WALL+1, center=true);
}

module enc_hole() {
    cylinder(d=D_ENC, h=WALL+1, center=true);
}

// ── Box body ──────────────────────────────────────────────────────────────────

module box_body() {
    difference() {
        rounded_box(BOX_L, BOX_W, BOX_H - LID_H, CORN);

        // Interior pocket
        translate([WALL, WALL, WALL])
            rounded_box(BOX_L - 2*WALL, BOX_W - 2*WALL,
                        BOX_H - LID_H - WALL + 0.1, CORN - WALL);

        // ── FRONT PANEL (y=0) cutouts ─────────────────────────────────────
        // z_mid: vertical center of front panel
        assign(z_mid = (BOX_H - LID_H) / 2) {

        // Audio IN jack
        translate([20, -0.1, z_mid])
            rotate([-90,0,0]) jack_hole();

        // Audio OUT jack
        translate([40, -0.1, z_mid])
            rotate([-90,0,0]) jack_hole();

        // Bypass toggle switch
        translate([68, -0.1, z_mid])
            rotate([-90,0,0]) toggle_hole();

        // Power toggle switch
        translate([86, -0.1, z_mid])
            rotate([-90,0,0]) toggle_hole();

        // Status LED
        translate([104, -0.1, z_mid])
            rotate([-90,0,0]) cylinder(d=D_LED, h=WALL+1, center=true);

        // Encoder knob (mode select)
        translate([130, -0.1, z_mid])
            rotate([-90,0,0]) enc_hole();

        // Second encoder (BW select, analog board only) — optional
        translate([148, -0.1, z_mid])
            rotate([-90,0,0]) enc_hole();

        } // end assign z_mid

        // ── REAR PANEL (y=BOX_W) ─────────────────────────────────────────
        // USB-C charge slot
        translate([BOX_L/2 - D_USBC_W/2, BOX_W-WALL-0.1, (BOX_H-LID_H)/2 - D_USBC_H/2])
            cube([D_USBC_W, WALL+1, D_USBC_H]);

        // Battery door recess (rectangular opening for PP3 door or 18650 tray)
        translate([20, BOX_W-WALL-0.1, WALL+3])
            cube([60, WALL+1, BOX_H-LID_H-WALL-6]);

        // ── LID engagement recess (top of box body) ───────────────────────
        translate([WALL+0.3, WALL+0.3, BOX_H-LID_H-0.5])
            rounded_box(BOX_L-2*WALL-0.6, BOX_W-2*WALL-0.6,
                        LID_H+0.6, CORN-WALL);
    }

    // PCB standoffs — 4× corners (M3 heat-set)
    for (pos=[[12,12],[12,BOX_W-17],[BOX_L-17,12],[BOX_L-17,BOX_W-17]])
        translate([pos[0], pos[1], WALL])
            difference() {
                cylinder(d=6, h=8);
                cylinder(d=D_M3_HS, h=8);
            }

    // Lid screw bosses M3 (×4 corners)
    for (pos=[[6,6],[6,BOX_W-6],[BOX_L-6,6],[BOX_L-6,BOX_W-6]])
        translate([pos[0], pos[1], BOX_H-LID_H-1])
            difference() {
                cylinder(d=8, h=LID_H+1);
                translate([0,0,-0.1]) cylinder(d=D_M3_HS, h=5);
            }

    // Front panel connector label ribs (raised 0.4mm for engraving guide)
    for (x=[20, 40, 68, 86, 130, 148])
        translate([x-4, 0, BOX_H-LID_H-3])
            cube([8, 0.4, 2]);
}

// ── Lid ───────────────────────────────────────────────────────────────────────

module lid() {
    difference() {
        union() {
            rounded_box(BOX_L, BOX_W, LID_H, CORN);
            // Engagement lip
            translate([WALL+0.5, WALL+0.5, -(LID_H-2)])
                rounded_box(BOX_L-2*WALL-1.0, BOX_W-2*WALL-1.0,
                            LID_H-1.5, CORN-WALL);
        }

        // Optional: OLED display window (30×13 mm)
        translate([BOX_L/2, BOX_W/2, -0.1])
            hull() for (x=[-13,13]) for (y=[-5.5,5.5])
                translate([x,y,0]) cylinder(r=1.5, h=LID_H+0.2);

        // Optional: OLED bezel recess 1 mm deep
        translate([BOX_L/2, BOX_W/2, LID_H-1.2])
            hull() for (x=[-15,15]) for (y=[-7,7])
                translate([x,y,0]) cylinder(r=2, h=1.3);

        // M3 lid screw holes ×4
        for (pos=[[6,6],[6,BOX_W-6],[BOX_L-6,6],[BOX_L-6,BOX_W-6]])
            translate([pos[0],pos[1],-0.1]) cylinder(d=D_M3, h=LID_H+0.2);

        // Label recess (silk-screen or Dymo label area)
        translate([BOX_L/2, BOX_W*0.75, LID_H-0.8])
            hull() for (x=[-35,35]) for (y=[-8,8])
                translate([x,y,0]) cylinder(r=2, h=0.9);
    }
}

// ── Battery door (PP3 × 2, slides in from rear) ───────────────────────────────

module battery_door() {
    DOOR_L = 60;
    DOOR_W = WALL;
    DOOR_H = BOX_H - LID_H - WALL - 6;
    difference() {
        cube([DOOR_L, DOOR_W+2, DOOR_H]);
        // Snap tab slot
        translate([DOOR_L/2-5, -0.1, DOOR_H-4]) cube([10, DOOR_W+3, 3]);
    }
    // Snap tab spring (cantilever)
    translate([DOOR_L/2-3, DOOR_W, DOOR_H-5])
        cube([6, 1.5, 5]);
}

// ── PP3 battery tray (internal; holds 2× PP3 side by side) ───────────────────

module pp3_tray() {
    TRAY_L = 56;
    TRAY_W = 30;
    TRAY_H = BOX_H - LID_H - WALL - 8;
    difference() {
        cube([TRAY_L, TRAY_W, TRAY_H]);
        // Battery cell cavities (2× PP3: 26.5mm × 17.5mm × 48.5mm each)
        for (y=[1, 14])
            translate([1, y, 2]) cube([TRAY_L-2, 12.5, TRAY_H]);
        // Wire pass-through slot
        translate([TRAY_L/2-5, 0, 0]) cube([10, 3, TRAY_H]);
    }
    // PP3 snap retainer tabs (×4, one per terminal end per battery)
    for (pos=[[1,2],[1,15],[TRAY_L-3,2],[TRAY_L-3,15]])
        translate([pos[0], pos[1], TRAY_H-6])
            cube([2, 6, 6]);
}

// ── 18650 tray (alternative to PP3 tray) ─────────────────────────────────────

module cell18650_tray() {
    CELL_D = 18.6;
    CELL_L = 65.5;
    TRAY_H = CELL_D + 4;
    difference() {
        cube([CELL_L+4, CELL_D+4, TRAY_H]);
        translate([2, 2, 4]) cylinder(d=CELL_D, h=CELL_L+2);
        // Spring contact slot (negative terminal end)
        translate([2, CELL_D/2+2-3, 0]) cube([6, 6, 4.5]);
    }
    // PCB standoff for TP4056 charger (M2, 3mm high)
    translate([CELL_L-8, CELL_D+6, 0])
        difference() {
            cylinder(d=5, h=3);
            cylinder(d=2.4, h=3);
        }
}

// ── Front label panel (overlay) ───────────────────────────────────────────────

module front_label() {
    difference() {
        cube([BOX_L, WALL, BOX_H-LID_H]);
        assign(z_mid = (BOX_H-LID_H)/2) {
            for (x=[20,40]) translate([x,-0.1,z_mid])
                rotate([-90,0,0]) cylinder(d=D_35MM+1, h=WALL+0.3, center=true);
            for (x=[68,86]) translate([x,-0.1,z_mid])
                rotate([-90,0,0]) cylinder(d=D_TOGGLE+1, h=WALL+0.3, center=true);
            translate([104,-0.1,z_mid])
                rotate([-90,0,0]) cylinder(d=D_LED+1, h=WALL+0.3, center=true);
            for (x=[130,148]) translate([x,-0.1,z_mid])
                rotate([-90,0,0]) cylinder(d=D_ENC+2, h=WALL+0.3, center=true);
        }
    }
}

// ── Encoder knob ─────────────────────────────────────────────────────────────

module encoder_knob() {
    KNOB_D = 24;
    KNOB_H = 18;
    SHAFT_D = 6.0;
    difference() {
        union() {
            // Main knob body (slight taper)
            cylinder(d1=KNOB_D, d2=KNOB_D-2, h=KNOB_H);
            // Grip ring at base
            translate([0,0,2]) cylinder(d=KNOB_D+1, h=2);
        }
        // D-shaft bore (flat at 270°)
        translate([0,0,-0.1]) {
            cylinder(d=SHAFT_D, h=KNOB_H+0.2);
            translate([SHAFT_D/2-0.5,-SHAFT_D/2-0.1,0]) cube([2, SHAFT_D+0.2, KNOB_H+0.2]);
        }
        // Grip flutes ×8
        for (a=[0:45:315])
            rotate([0,0,a]) translate([KNOB_D/2-0.5, 0, 4])
                cylinder(d=2.5, h=KNOB_H-5);
        // Top indicator line
        translate([-0.5, KNOB_D/2-3, KNOB_H-1.5]) cube([1, 3, 2]);
    }
}

// ── Render ────────────────────────────────────────────────────────────────────
box_body();
// translate([0, BOX_W+15, LID_H]) rotate([180,0,0]) lid();
// translate([BOX_L+15, 0, 0]) pp3_tray();
// translate([BOX_L+15, 40, 0]) cell18650_tray();
// translate([BOX_L+15, BOX_W/2, 10]) encoder_knob();
// translate([0, -(WALL+15), 0]) front_label();
