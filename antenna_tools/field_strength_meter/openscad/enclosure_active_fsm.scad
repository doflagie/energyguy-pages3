// ============================================================
// ACTIVE / DIGITAL FIELD STRENGTH METER ENCLOSURE
// TM-FSM-001 Rev A — CYD + ERA-3SM + AD8307 Version
//
// Dimensions: 160 x 100 x 48 mm body
// Fits: CYD 2.8" display (76x57 mm board), main PCB,
//       9V battery compartment, SMA input, BNC aux,
//       headphone jack, speaker, attenuator rotary switch
//
// Print settings:
//   Material: ASA (UV/temp resistant for field use)
//   Layer: 0.2mm, Walls: 4, Infill: 25% gyroid
//   Supports: lid overhang only
// ============================================================

/* [Box Dimensions] */
box_w  = 160.0;
box_d  = 100.0;
box_h  = 48.0;
wall   = 3.0;
lid_h  = 12.0;
lid_wall = 3.0;

/* [CYD Display Window — lid top face] */
cyd_win_w = 65.0;
cyd_win_h = 48.0;
cyd_cx    = box_w / 2;
cyd_cy    = box_d * 0.38;

/* [Front Panel Connectors — Y=0 face] */
sma_x   = 18.0;    // SMA probe input
sma_z   = box_h / 2;
bnc_x   = 50.0;    // BNC aux input
bnc_z   = box_h / 2;
hp_x    = 80.0;    // 3.5mm headphone/audio out
hp_z    = box_h / 2;

/* [Lid Controls] */
att_x   = 130.0;  // attenuator rotary switch
att_y   = box_d * 0.65;
pwr_x   = 130.0;  // power rocker
pwr_y   = box_d * 0.82;
spkr_cx = 130.0;  // speaker grille center
spkr_cy = box_d * 0.35;
spkr_r  = 15.0;   // speaker grille radius

/* [Battery compartment — 9V PP3] */
batt_w = 32.0;
batt_d = 52.0;
batt_h = 20.0;

$fn = 64;

// ============================================================
module rnd_hole(d, depth = wall*3) {
    cylinder(d = d, h = depth, center = true);
}

module rect_hole(w, h, r = 1.5, depth = wall*3) {
    hull() {
        for (dx = [-w/2+r, w/2-r]) for (dy = [-h/2+r, h/2-r])
            translate([dx, dy, 0]) cylinder(r = r, h = depth, center = true);
    }
}

module pcb_boss(x, y, bh = 5.0) {
    translate([x, y, 0]) {
        cylinder(d = 7.0, h = wall + bh);
        translate([0, 0, wall + bh - 0.01]) cylinder(d = 3.0, h = 12);
    }
}

// ============================================================
// MAIN BOX BODY
// ============================================================
module box_body() {
    difference() {
        union() {
            cube([box_w, box_d, box_h]);
            // Lid engagement rail
            translate([wall, wall, box_h - 1])
                cube([box_w - 2*wall, box_d - 2*wall, 2]);
        }

        // Interior
        translate([wall, wall, wall])
            cube([box_w - 2*wall, box_d - 2*wall, box_h]);

        // Battery pocket (rear, bottom-accessible)
        translate([box_w - batt_w - wall*2, box_d - batt_d - wall, -0.01])
            cube([batt_w, batt_d, batt_h + wall + 1]);

        // Front face connectors
        translate([sma_x, -0.01, sma_z])
            rotate([-90,0,0]) rnd_hole(10.0);    // SMA: 9.5mm flange hole

        translate([bnc_x, -0.01, bnc_z])
            rotate([-90,0,0]) rnd_hole(12.5);    // BNC

        translate([hp_x, -0.01, hp_z])
            rotate([-90,0,0]) rnd_hole(7.0);     // 3.5mm jack

        // Lid bolts: 6x M3
        for (x = [12, box_w/2, box_w-12]) {
            for (y = [wall/2, box_d - wall/2]) {
                translate([x, y, box_h + 0.5])
                    cylinder(d = 3.4, h = 10, center = true);
            }
        }

        // Bottom vent slots
        for (i = [0:4]) {
            translate([18 + i*26, box_d/2, -0.01])
                cube([10, 4, wall + 0.02], center = true);
        }

        // Drain hole (rear bottom corner)
        translate([wall + 4, box_d - wall - 4, -0.01])
            cylinder(d = 2, h = wall + 2);
    }

    // PCB mounting bosses
    translate([0, 0, wall]) {
        pcb_boss(12, 12);
        pcb_boss(box_w - 12, 12);
        pcb_boss(12, box_d - 12);
        pcb_boss(box_w - 12, box_d - 12);
        pcb_boss(box_w/2, box_d/2);
    }

    // Battery retainer lip
    translate([box_w - batt_w - wall*2 + 3, box_d - batt_d - wall, batt_h])
        cube([batt_w - 6, 3, 5]);
}

// ============================================================
// LID
// ============================================================
module box_lid() {
    translate([0, 0, box_h + 5])  // offset for preview
    difference() {
        union() {
            cube([box_w, box_d, lid_wall]);
            translate([lid_wall/2, lid_wall/2, -(lid_h - lid_wall)])
                difference() {
                    cube([box_w - lid_wall, box_d - lid_wall, lid_h]);
                    translate([lid_wall, lid_wall, -0.01])
                        cube([box_w - lid_wall*3, box_d - lid_wall*3, lid_h + 0.02]);
                }
        }

        // CYD display window
        translate([cyd_cx, cyd_cy, lid_wall/2])
            rect_hole(cyd_win_w, cyd_win_h, 2.0, lid_wall + 0.2);

        // CYD mounting holes (4x M3)
        for (dx = [-1,1]) for (dy = [-1,1]) {
            translate([cyd_cx + dx*(cyd_win_w/2 + 3.5),
                       cyd_cy + dy*(cyd_win_h/2 + 3.5), 0])
                cylinder(d = 3.2, h = lid_wall*3, center = true);
        }

        // Attenuator rotary switch
        translate([att_x, att_y, 0])
            cylinder(d = 10.5, h = lid_wall*3, center = true);
        translate([att_x + 5.5, att_y, 0])
            cube([3, 8, lid_wall*3], center = true);

        // Power rocker
        translate([pwr_x, pwr_y, 0])
            rect_hole(15, 10, 1.0);

        // Speaker grille: array of 2mm holes
        for (r = [4, 8, 12]) {
            steps = max(6, (int)(2 * 3.14159 * r / 4));
            for (i = [0:steps-1]) {
                a = 360.0 * i / steps;
                translate([spkr_cx + r * cos(a), spkr_cy + r * sin(a), 0])
                    cylinder(d = 2, h = lid_wall*3, center = true);
            }
        }
        // Center speaker hole
        translate([spkr_cx, spkr_cy, 0])
            cylinder(d = 2, h = lid_wall*3, center = true);

        // Lid bolt holes
        for (x = [12, box_w/2, box_w-12]) {
            for (y = [wall/2, box_d - wall/2]) {
                translate([x, y, 0])
                    cylinder(d = 3.4, h = lid_wall*3, center = true);
            }
        }
    }
}

// ============================================================
// Battery door (rear slot)
// ============================================================
module battery_door() {
    translate([box_w + 8, 0, 0])
    difference() {
        cube([batt_w + wall*2, batt_d + wall*2, wall + 2]);
        translate([wall, wall, -0.01])
            cube([batt_w, batt_d, wall + 3]);
        // Thumb notch
        translate([batt_w/2 + wall, batt_d + wall*1.5, 0])
            cylinder(d = 8, h = wall*4, center = true);
    }
}

// ============================================================
// Rubber foot recesses
// ============================================================
module feet() {
    for (x = [10, box_w-10]) for (y = [10, box_d-10])
        translate([x, y, -0.01]) cylinder(d = 12, h = 2.5);
}

// ============================================================
// Belt/strap loop (rear top)
// ============================================================
module strap_lug() {
    translate([box_w/2, box_d - wall, box_h - 12])
    difference() {
        hull() {
            cube([18, wall*2, 18], center = true);
            translate([0,-4,0]) cube([24, 1, 22], center = true);
        }
        cube([12, wall*4, 9], center = true);
        translate([0, 0, -4]) cylinder(d = 4.4, h = 24, center = true);
    }
}

// ============================================================
// Render
// ============================================================
box_body();
box_lid();
battery_door();
feet();
strap_lug();

echo("=== ACTIVE FSM ENCLOSURE ===");
echo(str("Box: ", box_w, " x ", box_d, " x ", box_h, " mm"));
echo(str("CYD window: ", cyd_win_w, " x ", cyd_win_h, " mm"));
echo(str("Fits 200x200 bed: ", max(box_w, box_d) <= 195 ? "YES" : "SPLIT REQUIRED"));
echo("Front: SMA + BNC + 3.5mm audio");
echo("Lid: CYD display + attenuator switch + power rocker + speaker grille");
echo("Rear: 9V battery compartment with removable door");
