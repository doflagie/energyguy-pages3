// ============================================================
// HF TUNER ENCLOSURE — Weatherproof Two-Piece Body + Lid
// Document: TM-HFTN-ENC-001 Rev A
// Project:  Motorized HF Antenna Tuner
// Author:   Merv Martin
// Date:     2026-05-02
// ============================================================
// Houses: ESP32 DevKit, 2× TMC2208 stepper drivers,
//         3× ULN2003 relay modules, SWR bridge PCB (~60×40mm)
// Outer:  ~180×140×90mm body + 6mm lid
// IP54 gasket groove, M3 lid screws, DIN rail option
// ============================================================

// --- PRINT PART SELECTOR ---
// "body" | "lid" | "din" | "all"
print_part = "all";

// --- GLOBAL DIMENSIONS ---
outer_x   = 180;   // length (front-to-back)
outer_y   = 140;   // width (left-right)
outer_z   = 90;    // body height (without lid)
wall      = 3.5;   // wall thickness
floor_t   = 4.0;   // floor thickness
lid_t     = 6.0;   // lid thickness
corner_r  = 8.0;   // outer corner radius

// Inner cavity
inner_x   = outer_x - 2*wall;
inner_y   = outer_y - 2*wall;
inner_z   = outer_z - floor_t;

// Lid / gasket
gasket_w  = 2.0;   // gasket groove width
gasket_d  = 1.5;   // gasket groove depth
lid_lip   = 4.0;   // lid inner lip depth (fits into body opening)

// Standoff heights above floor
standoff_h = 8.0;

// ============================================================
// UTILITIES
// ============================================================

module rounded_box(x, y, z, r) {
    // Minkowski-rounded rectangular box, origin at bottom-center
    translate([0, 0, z/2])
    minkowski() {
        cube([x - 2*r, y - 2*r, z - 2*r], center=true);
        sphere(r=r, $fn=32);
    }
}

module standoff(h, od=6, id=2.7, fn=20) {
    // Hollow cylindrical standoff; id = tap drill for M2.5/M3
    difference() {
        cylinder(h=h, d=od, $fn=fn);
        cylinder(h=h+0.1, d=id, $fn=fn);
    }
}

module m3_boss(h=8) {
    standoff(h=h, od=7, id=2.7, fn=20);
}

module m25_boss(h=8) {
    standoff(h=h, od=6, id=2.1, fn=20);
}

// ============================================================
// BOX OUTLINE — hollow shell with rounded exterior
// ============================================================

module box_outline() {
    difference() {
        // Outer shell
        rounded_box(outer_x, outer_y, outer_z, corner_r);
        // Inner cavity (remove top face — open top)
        translate([0, 0, floor_t])
            cube([inner_x, inner_y, outer_z], center=true);
        // Open top
        translate([0, 0, outer_z])
            cube([inner_x + wall, inner_y + wall, 20], center=true);
    }
}

// ============================================================
// GASKET GROOVE — runs around top perimeter of body wall
// ============================================================

module gasket_groove() {
    // Grooved into the top face of the body walls
    // Offset inward by wall/2 from outer edge
    gx = outer_x - 2*(wall/2 + gasket_w/2 + 1);
    gy = outer_y - 2*(wall/2 + gasket_w/2 + 1);
    translate([0, 0, outer_z - gasket_d])
    difference() {
        // Outer boundary of groove path
        translate([0,0,gasket_d/2])
        minkowski() {
            cube([gx, gy, 0.01], center=true);
            cylinder(r=gasket_w/2 + 1, h=0.01, $fn=20);
        }
        // Inner removal (the groove itself)
        translate([0,0,-0.05])
        minkowski() {
            cube([gx - gasket_w*2, gy - gasket_w*2, 0.01], center=true);
            cylinder(r=gasket_w/2, h=0.1, $fn=20);
        }
    }
    // Actual groove cut (negative) applied in body module
}

module gasket_groove_cut() {
    gx = outer_x - wall*3;
    gy = outer_y - wall*3;
    translate([0, 0, outer_z - gasket_d - 0.01])
    linear_extrude(height=gasket_d + 0.1)
    difference() {
        offset(r=gasket_w/2 + gasket_w)
            square([gx, gy], center=true);
        offset(r=gasket_w/2)
            square([gx, gy], center=true);
    }
}

// ============================================================
// DIN RAIL CLIP — snaps to 35mm DIN rail, mounts on body bottom
// ============================================================

module din_rail_clip() {
    clip_l  = 80;
    clip_w  = 12;
    clip_h  = 18;
    rail_w  = 35;
    rail_h  = 7.5;
    t       = 2.5;

    difference() {
        union() {
            // Base plate
            cube([clip_l, clip_w, t], center=false);
            // Back wall
            translate([0, clip_w - t, 0])
                cube([clip_l, t, clip_h], center=false);
            // Top hook
            translate([0, clip_w - t, clip_h - t])
                cube([clip_l, t + rail_h, t], center=false);
            // Bottom spring tab
            translate([0, 0, clip_h - t - rail_h - t])
                cube([clip_l, t + rail_h + 2, t], center=false);
        }
        // Mounting holes in base plate (M4)
        for (mx = [10, clip_l - 10])
            translate([mx, clip_w/2, -0.1])
                cylinder(h=t+0.2, d=4.3, $fn=16);
    }
}

// ============================================================
// FRONT PANEL CUTOUTS
// ============================================================
// Front panel = face at y = -outer_y/2

module front_panel_cutouts() {
    fp_y = outer_y/2 + 1;   // punch through from outside

    // SO-239 antenna connector (UHF/SMA equiv hole 20.5mm)
    translate([-50, -fp_y, 35])  rotate([90,0,0])
        cylinder(h=wall*3, d=20.5, $fn=32, center=true);

    // SMA input connector
    translate([-20, -fp_y, 35])  rotate([90,0,0])
        cylinder(h=wall*3, d=20.5, $fn=32, center=true);

    // OLED 128×64 window 30×16mm
    translate([20, -fp_y, 55])  rotate([90,0,0])
        cube([30, 16, wall*3], center=true);

    // Rotary encoder shaft 7.5mm
    translate([60, -fp_y, 55])  rotate([90,0,0])
        cylinder(h=wall*3, d=7.5, $fn=20, center=true);

    // Power LED 3.2mm
    translate([70, -fp_y, 42])  rotate([90,0,0])
        cylinder(h=wall*3, d=3.2, $fn=16, center=true);

    // Tuning status LED 3.2mm
    translate([78, -fp_y, 42])  rotate([90,0,0])
        cylinder(h=wall*3, d=3.2, $fn=16, center=true);
}

// ============================================================
// REAR PANEL CUTOUTS
// ============================================================
// Rear panel = face at y = +outer_y/2

module rear_panel_cutouts() {
    rp_y = outer_y/2 + 1;

    // DC barrel jack 12mm hole
    translate([-60, rp_y, 35])  rotate([90,0,0])
        cylinder(h=wall*3, d=12.0, $fn=24, center=true);

    // Cable gland PG7 left (12mm hole)
    translate([-20, rp_y, 35])  rotate([90,0,0])
        cylinder(h=wall*3, d=12.0, $fn=24, center=true);

    // Cable gland PG7 right (12mm hole)
    translate([20, rp_y, 35])   rotate([90,0,0])
        cylinder(h=wall*3, d=12.0, $fn=24, center=true);

    // USB slot 12×6mm
    translate([60, rp_y, 35])  rotate([90,0,0])
        cube([12, 6, wall*3], center=true);
}

// ============================================================
// SIDE PANEL CUTOUT — RJ45 slot
// ============================================================
// Right side = face at x = +outer_x/2

module side_panel_cutouts() {
    sp_x = outer_x/2 + 1;

    // RJ45 header slot 16×14mm
    translate([sp_x, 10, 35])  rotate([0,90,0])
        cube([14, 16, wall*3], center=true);
}

// ============================================================
// INTERNAL STANDOFFS & MOUNTING
// ============================================================

module internal_hardware() {
    fz = floor_t;   // floor offset

    // --- ESP32 DevKit standoffs ---
    // 47×18mm spacing, 4 corners, M2.5
    esp_x = 47/2; esp_y = 18/2;
    for (sx=[-esp_x, esp_x], sy=[-esp_y, esp_y])
        translate([sx - 30, sy + 40, fz])
            m25_boss(h=standoff_h);

    // --- SWR bridge PCB standoffs (60×40mm footprint, M3) ---
    swr_x = 60/2; swr_y = 40/2;
    for (sx=[-swr_x, swr_x], sy=[-swr_y, swr_y])
        translate([sx + 50, sy - 20, fz])
            m3_boss(h=standoff_h);

    // --- Relay board standoffs (2× M3, 40mm spacing) ---
    for (sx=[-20, 20])
        translate([sx, -40, fz])
            m3_boss(h=standoff_h);

    // --- TMC2208 driver board standoffs (approx 15×30mm, M2.5) ---
    // Driver 1
    for (sx=[-7.5, 7.5])
        translate([sx - 55, 0, fz])
            m25_boss(h=standoff_h);
    // Driver 2
    for (sx=[-7.5, 7.5])
        translate([sx - 55, 35, fz])
            m25_boss(h=standoff_h);

    // --- M3 lid screw bosses (4 corners, inside body top) ---
    boss_inset = 12;
    for (bx=[-(outer_x/2 - boss_inset), (outer_x/2 - boss_inset)])
    for (by=[-(outer_y/2 - boss_inset), (outer_y/2 - boss_inset)])
        translate([bx, by, fz])
            cylinder(h=outer_z - fz - 2, d=7, $fn=20);
}

module lid_screw_holes_in_body() {
    boss_inset = 12;
    for (bx=[-(outer_x/2 - boss_inset), (outer_x/2 - boss_inset)])
    for (by=[-(outer_y/2 - boss_inset), (outer_y/2 - boss_inset)])
        translate([bx, by, outer_z - 20])
            cylinder(h=25, d=3.2, $fn=16);  // M3 clearance
}

// ============================================================
// HF TUNER BODY
// ============================================================

module hf_tuner_body() {
    difference() {
        union() {
            box_outline();
            internal_hardware();
        }
        front_panel_cutouts();
        rear_panel_cutouts();
        side_panel_cutouts();
        gasket_groove_cut();
        lid_screw_holes_in_body();
    }
}

// ============================================================
// LID VENTILATION SLOTS (over driver area)
// ============================================================

module lid_vent_slots() {
    // 3× slots 6mm wide, 40mm long, over left side (driver area)
    slot_w  = 6;
    slot_l  = 40;
    slot_sp = 12;
    for (i=[0:2])
        translate([-60 + i*slot_sp, 0, -0.1])
            cube([slot_w, slot_l, lid_t + 0.2], center=true);
}

module lid_m3_holes() {
    boss_inset = 12;
    for (bx=[-(outer_x/2 - boss_inset), (outer_x/2 - boss_inset)])
    for (by=[-(outer_y/2 - boss_inset), (outer_y/2 - boss_inset)])
        translate([bx, by, -0.1])
            cylinder(h=lid_t + 0.2, d=3.2, $fn=16);
}

module lid_inner_lip() {
    // Raised lip on underside of lid that fits inside body opening
    lx = inner_x - 0.4;
    ly = inner_y - 0.4;
    translate([0, 0, -lid_lip])
    difference() {
        cube([lx, ly, lid_lip], center=true);
        translate([0, 0, 0.5])
            cube([lx - wall*2, ly - wall*2, lid_lip + 1], center=true);
    }
}

module hf_tuner_lid() {
    difference() {
        union() {
            // Flat lid plate
            translate([0, 0, lid_t/2])
                rounded_box(outer_x, outer_y, lid_t, corner_r);
            lid_inner_lip();
        }
        lid_vent_slots();
        lid_m3_holes();
    }
}

// ============================================================
// RENDER SELECTOR
// ============================================================

if (print_part == "body") {
    hf_tuner_body();
} else if (print_part == "lid") {
    translate([0, 0, lid_t]) rotate([180,0,0]) hf_tuner_lid();
} else if (print_part == "din") {
    din_rail_clip();
} else if (print_part == "all") {
    hf_tuner_body();
    translate([0, outer_y + 20, 0]) hf_tuner_lid();
    translate([outer_x/2 + 50, 0, 0]) din_rail_clip();
}

// ============================================================
// FRONT PANEL LAYOUT (looking at front, left-to-right)
// ============================================================
// x=-50  y=35  : SO-239 antenna connector (20.5mm hole)
// x=-20  y=35  : SMA RF input connector (20.5mm hole)
// x=+20  y=55  : OLED window 30×16mm (128×64 0.96" display)
// x=+60  y=55  : Rotary encoder knob shaft (7.5mm)
// x=+70  y=42  : Power LED 3mm green
// x=+78  y=42  : Tuning status LED 3mm red/amber
//
// REAR PANEL LAYOUT (looking at rear, left-to-right)
// x=-60  y=35  : DC barrel jack 5.5/2.1mm (12mm hole)
// x=-20  y=35  : Cable gland PG7 (motor 1 cables)
// x=+20  y=35  : Cable gland PG7 (motor 2 cables)
// x=+60  y=35  : USB-B micro/Type-C slot (12×6mm) for ESP32 prog
//
// RIGHT SIDE PANEL
// y=+10  z=35  : RJ45 header slot (16×14mm) — expansion bus
//
// ASSEMBLY NOTES
// 1. Print body and lid in PETG or ASA for weather resistance.
// 2. Tap M2.5 holes in ESP32 standoffs; M3 in relay/SWR/lid bosses.
// 3. Insert M3×12 screws through lid into body corner bosses.
// 4. Apply 2mm cross-section silicone O-cord in gasket groove
//    before closing lid (IP54 requires continuous seal).
// 5. Cable glands: PG7 rated for 3–6.5mm OD cable.
// 6. OLED bezel: glue 2mm acrylic window behind cutout recess.
// 7. DIN clip attaches to body bottom with 4× M4×10 screws.
//
// WEATHERPROOFING NOTES
// - IP54: dust-protected, splash-resistant from any direction.
// - Gasket groove: 2mm wide × 1.5mm deep — use 2mm silicone cord.
// - Lid lip seats 4mm into body to compress gasket.
// - All connector cutouts should have panel-mount gasket washers.
// - Seal cable glands with included rubber insert — torque finger-tight.
// - Vent slots are positioned over driver area; add foam vent plugs
//   (e.g., 3M 9205 adhesive foam) if IP54 is required continuously.
//   Drivers generate ~0.5W each — passive convection sufficient
//   for most HF duty cycles; remove foam for heavy CW operation.
// ============================================================
