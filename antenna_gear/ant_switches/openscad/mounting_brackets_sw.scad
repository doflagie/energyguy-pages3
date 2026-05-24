// ============================================================================
// TM-ANT-SW-001  Rev A — Antenna Switch Mounting Hardware
// Mast, wall, tower, desk, and rack mounting brackets
// Print bed: 200 × 200 × 200 mm  |  Material: ASA
// ============================================================================
//
// MODULES:
//   1. mast_clamp_sw(mast_d)        — round mast U-bolt saddle
//   2. wall_mount_sw()              — exterior wall lag-bolt plate
//   3. tower_leg_clamp_sw(leg_w)    — square tower leg wrap bracket
//   4. desk_bracket_sw()            — under-desk or shelf mount, tilt adjustable
//   5. rack_panel_1u()              — 1U rack panel with switch cutout
//   6. cable_dress_bar(n)           — SO-239 strain relief + cable routing bar
//   7. label_strip(n_ports)         — snap-on port label strip (front panel)
//   8. knob_pointer_6mm()           — replacement pointer knob, 6mm D-shaft
//   9. led_bezel_5mm()              — 5mm LED snap-in bezel (panel mount)
//  10. remote_panel_faceplate()     — remote head faceplate for CYD touchscreen
// ============================================================================

$fn = 64;

WALL    = 3.0;
TOL     = 0.25;
BOLT_M3 = 3.3;
BOLT_M4 = 4.2;
BOLT_M6 = 6.4;
BOLT_M8 = 8.4;
NUT_M4_W = 7.7;  NUT_M4_H = 3.2;
NUT_M6_W = 10.0; NUT_M6_H = 5.0;

// ── Render control ──────────────────────────────────────────────────────────
// SHOW: 0=layout, 1=mast, 2=wall, 3=tower, 4=desk, 5=rack, 6=cable bar,
//       7=label, 8=knob, 9=led bezel, 10=remote faceplate
SHOW = 0;

if (SHOW == 0) {
    mast_clamp_sw(mast_d=50);
    translate([130, 0, 0]) wall_mount_sw();
    translate([0, 130, 0]) desk_bracket_sw();
    translate([130, 130, 0]) cable_dress_bar(4);
}
if (SHOW == 1) mast_clamp_sw(mast_d=50);
if (SHOW == 2) wall_mount_sw();
if (SHOW == 3) tower_leg_clamp_sw(leg_w=40);
if (SHOW == 4) desk_bracket_sw();
if (SHOW == 5) rack_panel_1u();
if (SHOW == 6) cable_dress_bar(4);
if (SHOW == 7) label_strip(4);
if (SHOW == 8) knob_pointer_6mm();
if (SHOW == 9) led_bezel_5mm();
if (SHOW == 10) remote_panel_faceplate();


// ============================================================================
// 1. MAST CLAMP BRACKET
//    Mounts the switch enclosure to a round mast (antenna mast, pipe stand).
//    Back plate with 4× M6 switch mount holes.
//    Two M8 U-bolt slots; accepts 25–75mm masts.
// ============================================================================
module mast_clamp_sw(mast_d=50) {
    r_out = mast_d/2 + WALL + 8;
    back_w = 130;
    back_h = 110;
    back_t = WALL + 2;
    ubolt_sep = mast_d + 14;

    difference() {
        union() {
            cube([back_w, back_t, back_h]);
            // Saddle arc on front face
            translate([back_w/2, back_t, back_h/2])
            rotate([90, 0, 0])
            linear_extrude(height=back_t)
            difference() {
                hull() {
                    circle(r=r_out);
                    translate([-back_w/2, -r_out]) square([back_w, 1]);
                }
                circle(r=mast_d/2 + TOL);
                translate([-back_w, 0]) square([2*back_w, r_out + 1]);
            }
        }

        // U-bolt holes (M8)
        for (dx = [-ubolt_sep/2, ubolt_sep/2])
            translate([back_w/2 + dx, -1, back_h/2])
            rotate([-90, 0, 0]) cylinder(d=BOLT_M8, h=back_t + r_out + 2);

        // 4× M6 switch-mount holes on back face
        for (dx = [-40, -15, 15, 40])
            translate([back_w/2 + dx, -1, back_h/2])
            rotate([-90, 0, 0]) cylinder(d=BOLT_M6, h=back_t + 2);

        // M6 countersinks (back face)
        for (dx = [-40, -15, 15, 40])
            translate([back_w/2 + dx, back_t - 3.5, back_h/2])
            rotate([-90, 0, 0]) cylinder(d=12, h=5);

        // Mast bore in saddle
        translate([back_w/2, back_t - 0.1, back_h/2])
        rotate([90, 0, 0]) cylinder(d=mast_d + TOL*2, h=r_out + 0.2);

        // Weight relief
        translate([10, back_t*0.25, 15]) cube([back_w-20, back_t+0.1, back_h-30]);
    }
}


// ============================================================================
// 2. WALL MOUNT BRACKET
//    Mounts switch to exterior wall or wooden post.
//    Adjustable tilt: two slots allow 0–15° forward tilt for visibility.
//    4× lag bolt holes for studs (400mm OC); 4× switch mount holes.
// ============================================================================
module wall_mount_sw() {
    W = 130;  H = 120;  T = WALL + 2;  EAR = 20;

    difference() {
        union() {
            cube([W, T, H]);
            // Stiffener flanges top and bottom
            for (z = [0, H - EAR]) translate([0, 0, z]) cube([W, T + 18, EAR]);
        }

        // 4× wall lag-bolt holes (6mm) in flanges
        for (dx = [15, W-15]) for (dz = [EAR/2, H - EAR/2])
            translate([dx, -1, dz]) rotate([-90, 0, 0]) cylinder(d=BOLT_M6, h=T+20);

        // 4× switch mount holes (centre field)
        for (dx = [-35, 35]) for (dz = [-30, 30])
            translate([W/2 + dx, -1, H/2 + dz])
            rotate([-90, 0, 0]) cylinder(d=BOLT_M6, h=T+2);

        // Tilt slots (2× arcs — 15° forward tilt option)
        // Simple: elongated slot version
        for (dx = [-45, 45])
            translate([W/2 + dx, -1, H/2])
            rotate([-90, 0, 0]) {
                cylinder(d=BOLT_M6, h=T+2);
                translate([0, -8, 0]) cylinder(d=BOLT_M6, h=T+2);
                translate([0, -4, -BOLT_M6/2]) cube([BOLT_M6, 8, BOLT_M6]);
            }

        // Weight relief
        translate([15, T*0.3, EAR + 5]) cube([W-30, T, H - 2*EAR - 10]);

        // Drain slot
        translate([W/2 - 5, 0, 2]) cube([10, T+20, 8]);
    }
}


// ============================================================================
// 3. TOWER LEG CLAMP
//    C-channel wraps around a square tower leg (Rohn 25G: ~38mm flat).
//    M8 clamping bolt pulls two ears together.
//    4× M6 on back face for switch enclosure attachment.
// ============================================================================
module tower_leg_clamp_sw(leg_w=40) {
    depth  = leg_w + WALL*2 + 10;
    body_w = 130;
    body_h = 100;
    ear_w  = 22;

    difference() {
        union() {
            cube([body_w, depth, body_h]);
            // Clamping ears
            for (dx = [0, body_w])
                translate([dx - (dx > 0 ? ear_w : 0), 0, 0])
                cube([ear_w, depth + 18, body_h]);
        }

        // Tower leg slot (open front)
        translate([-1, depth - leg_w - TOL*2, (body_h - leg_w*1.6)/2])
        cube([body_w + 2, leg_w + TOL*2 + 1, leg_w*1.6]);

        // M8 clamping bolt holes
        for (dz = [body_h*0.3, body_h*0.7])
            translate([-ear_w - 2, depth + 8, dz])
            rotate([0, 90, 0]) cylinder(d=BOLT_M8, h=body_w + 2*ear_w + 4);

        // Nut traps (hex, one side)
        for (dz = [body_h*0.3, body_h*0.7])
            translate([body_w + ear_w - 12, depth + 8, dz])
            rotate([0, 90, 0]) {
                cylinder(d=NUT_M6_W + 2, h=10, $fn=6);
                translate([0, 0, -1]) cylinder(d=BOLT_M8, h=15);
            }

        // 4× M6 switch mount (front face of bracket)
        for (dx = [-35, 35]) for (dz = [-28, 28])
            translate([body_w/2 + dx, -1, body_h/2 + dz])
            rotate([-90, 0, 0]) cylinder(d=BOLT_M6, h=WALL + 2);
    }
}


// ============================================================================
// 4. DESK BRACKET
//    Tilt-angle mount for operating desk / shelf.
//    Rubber foot pads (4× holes, M4 bumper feet), tilt angle 15°.
//    Two configurations: flat base, or side-mount clamp to desk lip.
// ============================================================================
module desk_bracket_sw() {
    BASE_W = 150;  BASE_D = 80;  BASE_H = WALL + 2;
    RISE   = 40;   // height of the switch face from desk surface
    TILT   = 15;   // tilt angle toward operator (degrees)

    difference() {
        union() {
            // Flat base
            cube([BASE_W, BASE_D, BASE_H]);
            // Two rise columns with tilt
            for (dx = [20, BASE_W - 20])
                translate([dx, BASE_D/2, BASE_H])
                rotate([-TILT, 0, 0]) {
                    cube([8, WALL, RISE]);
                    // Top ear (switch mount)
                    translate([0, WALL, RISE]) cube([8, 18, WALL+2]);
                }
        }

        // 4× rubber foot holes in base
        for (px = [15, BASE_W-15]) for (py = [15, BASE_D-15])
            translate([px, py, -1]) cylinder(d=BOLT_M4, h=BASE_H+2);

        // 4× switch mount holes in top ears
        for (dx = [20, BASE_W - 20])
            translate([dx + 4, BASE_D/2 + WALL, BASE_H + RISE - TILT*0.3])
            cylinder(d=BOLT_M6, h=BASE_H+2);
    }
}


// ============================================================================
// 5. 1U RACK PANEL
//    Standard 19" rack panel (482.6mm wide, 44.45mm tall = 1U)
//    Switch enclosure cutout centered; 4× M6 rack ear holes.
//    Split for print bed: prints in two halves, joined with M4 screws.
// ============================================================================
module rack_panel_1u() {
    RACK_W = 482.6;
    RACK_H = 44.45;
    RACK_T = 3.0;

    // Prints as LEFT HALF (241mm; just fits 200mm bed if oriented diagonally)
    // Set HALF = "LEFT" or "RIGHT"
    HALF = "LEFT";  // change to "RIGHT" for other half

    CUTOUT_W = 150;  // switch enclosure cutout width
    CUTOUT_H = 38;   // switch enclosure cutout height

    half_w = RACK_W / 2;

    difference() {
        cube([half_w, RACK_T, RACK_H]);

        // Rack ear holes (2× M6 per half, 15.875mm from edge, standard)
        for (x = [7.94, 25.40])
            translate([x, -1, RACK_H/2]) rotate([-90,0,0])
            cylinder(d=BOLT_M6, h=RACK_T+2);

        // Switch cutout (centred, left half portion)
        cx = (RACK_W - CUTOUT_W) / 2;
        if (HALF == "LEFT")
            translate([cx, -1, (RACK_H - CUTOUT_H)/2])
            cube([half_w - cx + 5, RACK_T+2, CUTOUT_H]);

        // Join screws (M4, 3× along centre joint)
        for (z = [RACK_H*0.2, RACK_H*0.5, RACK_H*0.8])
            translate([half_w - 1, -1, z]) rotate([-90,0,0])
            cylinder(d=BOLT_M4, h=RACK_T+2);
    }
}


// ============================================================================
// 6. CABLE DRESS BAR
//    Holds N coax cables in parallel at the rear of the switch.
//    Snap-fit onto switch enclosure; strain relief to prevent connector stress.
//    n = number of ports (4 or 6).
// ============================================================================
module cable_dress_bar(n=4, coax_od=11) {
    r     = coax_od/2 + TOL;
    pitch = coax_od + 5;
    total_w = n * pitch + 2*WALL;
    body_h  = coax_od + 2*WALL;
    body_d  = coax_od * 1.3 + WALL;
    snap_w  = 6;  snap_h = 4;

    // Bottom half
    difference() {
        cube([total_w, body_d, body_h/2 + WALL]);
        // Cable grooves
        for (i = [0:n-1])
            translate([WALL + pitch/2 + i*pitch, -1, WALL + r])
            rotate([-90,0,0]) cylinder(r=r, h=body_d+2);
        // M4 clamping bolt holes
        for (bx = [total_w*0.2, total_w*0.8])
            translate([bx, -1, body_h/4]) rotate([-90,0,0])
            cylinder(d=BOLT_M4, h=body_d+2);
        // M4 wall mount hole
        translate([total_w/2, body_d+1, body_h/4]) rotate([90,0,0])
        cylinder(d=BOLT_M4, h=body_d+2);
    }

    // Top clamp half (offset for printing flat)
    translate([0, body_d + 12, 0])
    difference() {
        cube([total_w, body_d, body_h/2 + WALL]);
        for (i = [0:n-1])
            translate([WALL + pitch/2 + i*pitch, -1, -r + 0.1])
            rotate([-90,0,0]) cylinder(r=r, h=body_d+2);
        for (bx = [total_w*0.2, total_w*0.8])
            translate([bx, -1, body_h/4]) rotate([-90,0,0])
            cylinder(d=BOLT_M4, h=body_d+2);
        // Nut traps
        for (bx = [total_w*0.2, total_w*0.8])
            translate([bx, body_d - NUT_M4_H, body_h/4])
            rotate([-90,0,0]) cylinder(d=NUT_M4_W, h=NUT_M4_H+1, $fn=6);
    }
}


// ============================================================================
// 7. LABEL STRIP
//    Snap-on strip that fits below the SO-239 row on the switch front panel.
//    n_ports raised label areas (emboss text after printing, or add sticker).
// ============================================================================
module label_strip(n_ports=4) {
    pitch = 38;
    strip_w = 20 + n_ports * pitch;
    strip_h = 18;
    strip_t = 2.0;
    snap_h  = 3;

    difference() {
        union() {
            // Main strip
            cube([strip_w, strip_t + snap_h, strip_h]);
            // Snap tab (hooks over panel edge)
            translate([0, strip_t, 0]) cube([strip_w, snap_h, 3]);
        }
        // Label recesses per port
        for (i = [0:n_ports-1]) {
            x = 10 + i * pitch;
            translate([x, -1, 4]) cube([28, strip_t + 2, 10]);
        }
        // Mounting screw holes at ends
        for (px = [8, strip_w - 8])
            translate([px, -1, strip_h/2]) rotate([-90,0,0])
            cylinder(d=BOLT_M3, h=strip_t + snap_h + 2);
    }
}


// ============================================================================
// 8. POINTER KNOB — 6mm D-SHAFT
//    Replaces the stock knob on a manual rotary switch.
//    8 grip ridges; pointer line embossed on top.
//    D-shaft bore with set screw.
// ============================================================================
module knob_pointer_6mm() {
    KD = 32;   KH = 20;   SHAFT_D = 6.2;
    difference() {
        union() {
            // Main cylinder with grip ridges
            difference() {
                cylinder(d=KD, h=KH);
                // 8 grip flutes
                for (a = [0:45:315])
                    rotate([0, 0, a])
                    translate([KD/2 - 1.2, 0, -1])
                    cylinder(d=2.5, h=KH+2);
            }
            // Pointer rib on top
            translate([-1, -KD/2, KH])
            cube([2, KD, 1.5]);
        }
        // D-shaft bore
        difference() {
            translate([0, 0, -1]) cylinder(d=SHAFT_D + TOL, h=KH + 2);
            // D flat
            translate([SHAFT_D/2 - 0.5, -(SHAFT_D/2+1), -1])
            cube([3, SHAFT_D + 2, KH + 2]);
        }
        // M3 set screw hole (from side)
        translate([KD/2 + 1, 0, KH * 0.35]) rotate([0, 90, 0])
        cylinder(d=BOLT_M3, h=KD/2 + 2);
        // Set screw nut trap
        translate([KD/2 - 3.5, 0, KH * 0.35]) rotate([0, 90, 0])
        cylinder(d=6.0, h=3.5, $fn=6);
        // Top face indicator dot
        translate([KD/2 - 5, 0, KH]) cylinder(d=2, h=2);
    }
}


// ============================================================================
// 9. LED BEZEL — 5mm
//    Snap-in panel bezel for 5mm LEDs in the relay switch front panel.
//    Frosted lens effect from thin-wall translucent ASA.
//    (Print in translucent/natural ASA; or clear PETG for better light)
// ============================================================================
module led_bezel_5mm() {
    OD_OUTER = 8.5;    // outer diameter of flange
    OD_SNAP  = 5.0 + TOL; // diameter of snap barrel (fits 5mm panel hole)
    FLANGE_T = 1.5;    // flange thickness
    BARREL_H = 4.0;    // depth into panel

    difference() {
        union() {
            // Flange
            cylinder(d=OD_OUTER, h=FLANGE_T);
            // Snap barrel
            translate([0, 0, FLANGE_T])
            cylinder(d=OD_SNAP + 0.8, h=BARREL_H);
        }
        // LED bore (5mm)
        translate([0, 0, -1]) cylinder(d=5.2, h=BARREL_H + FLANGE_T + 2);
        // 2× snap clips on barrel (allow compression for insertion)
        for (a = [0, 180])
            rotate([0, 0, a])
            translate([OD_SNAP/2 - 0.5, -0.6, FLANGE_T + 1])
            cube([2.5, 1.2, BARREL_H]);
    }
}


// ============================================================================
// 10. REMOTE PANEL FACEPLATE
//     Houses a CYD (ESP32-2432S028) 2.8" TFT as a remote head.
//     Mounts on desk; HDMI cable carries data back to switch unit (not HDMI —
//     actually a Cat5 cable via I2C/SPI extension).
//     Display window + 4× button holes + 4× LED holes.
// ============================================================================
module remote_panel_faceplate() {
    PW = 120;  PH = 90;  PT = WALL + 2;
    // CYD TFT window: active area ~58mm × 44mm
    CYD_W = 66;  CYD_H = 52;

    difference() {
        union() {
            cube([PW, PT, PH]);
            // Desktop stand legs (angled 20° backward)
            for (dx = [8, PW - 8 - 8])
                translate([dx, PT, 0]) rotate([-20, 0, 0])
                cube([8, WALL, 40]);
        }

        // CYD display window (centered, upper 2/3)
        translate([(PW - CYD_W)/2, -1, PH - CYD_H - 10])
        cube([CYD_W, PT + 2, CYD_H]);

        // 4× push-button holes (bottom row)
        for (i = [0:3])
            translate([18 + i*22, -1, 14]) rotate([-90,0,0])
            cylinder(d=6.8, h=PT+2);

        // 4× LED holes (above buttons)
        for (i = [0:3])
            translate([18 + i*22, -1, 26]) rotate([-90,0,0])
            cylinder(d=5.2, h=PT+2);

        // RJ45 / Cat5 data cable hole (right side)
        translate([PW-1, PT/2, PH/2]) rotate([0, 90, 0])
        cylinder(d=12, h=PT+2);   // RJ45 body clearance

        // 4× M3 mount holes (for mounting to wall/arm)
        for (px = [6, PW-6]) for (pz = [6, PH-6])
            translate([px, -1, pz]) rotate([-90,0,0])
            cylinder(d=BOLT_M3, h=PT+2);

        // Label recess
        translate([10, PT-0.8, 5]) cube([PW-20, 1, 8]);
    }
}

// ============================================================================
// END OF FILE
// ============================================================================
