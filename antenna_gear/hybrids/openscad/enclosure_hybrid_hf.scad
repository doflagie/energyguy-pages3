/*
 * enclosure_hybrid_hf.scad
 * Weatherproof HF Hybrid/Power Divider Enclosure
 * TM-HYB-001 Rev A
 * Material: ASA (UV/weather resistant)
 * Print bed: 200 × 200 mm
 *
 * Fits: Band-switched Wilkinson or quadrature hybrid PCB (100 × 80 mm)
 * with up to 3 BNC connectors (front) and 2 BNC connectors (rear),
 * rotary switches on lid, and O-ring gasket seal.
 *
 * Box: 130 × 95 × 50 mm (L × W × H)
 * Lid: Sliding or bolted, with O-ring groove
 */

$fn = 48;

BOX_L = 130; BOX_W = 95; BOX_H = 50;
WALL  = 3.0;   // thicker wall for weatherproofing
CORN  = 5.0;
LID_H = 10;    // lid depth (engagement)
ORING_D = 2.0; // O-ring cross-section diameter

D_BNC   = 12.5;
D_SMA   = 10.0;
D_ROT   = 10.5;
D_DFLAT = 6.8;
D_M4    = 4.3;
D_M4_HS = 5.6;
D_CBLE  = 10.0; // cable pass-through

module rounded_box(l, w, h, r) {
    hull() for (x=[r,l-r]) for (y=[r,w-r])
        translate([x,y,0]) cylinder(r=r, h=h);
}

module oring_groove() {
    // O-ring channel: rectangular cross-section, depth=1.5mm, width=2.5mm
    // Sits in lid mating surface
    offset_x = WALL + 2;
    offset_y = WALL + 2;
    path_l = BOX_L - 2*offset_x;
    path_w = BOX_W - 2*offset_y;
    r = 3;
    // Approximate with rounded rectangle path
    translate([offset_x, offset_y, 0])
    difference() {
        rounded_box(path_l, path_w, 1.6, r);
        translate([2.5, 2.5, -0.1]) rounded_box(path_l-5, path_w-5, 2.0, r-2);
    }
}

// ── Box body ──────────────────────────────────────────────────────────────────
module box_body() {
    difference() {
        rounded_box(BOX_L, BOX_W, BOX_H - LID_H, CORN);
        // Interior
        translate([WALL, WALL, WALL])
            rounded_box(BOX_L-2*WALL, BOX_W-2*WALL, BOX_H-LID_H-WALL+0.1, CORN-WALL);
        // Front BNC holes (3×)
        for (x = [22, BOX_L/2, BOX_L-22]) {
            translate([x, -0.1, BOX_H/2-LID_H/2])
                rotate([-90,0,0]) cylinder(d=D_BNC, h=WALL+0.2);
        }
        // Rear BNC holes (2×)
        for (x = [35, BOX_L-35]) {
            translate([x, BOX_W-WALL-0.1, BOX_H/2-LID_H/2])
                rotate([90,0,0]) cylinder(d=D_BNC, h=WALL+0.2);
        }
        // Cable entry (bottom, rear): gland hole
        translate([BOX_L/2, BOX_W-15, -0.1]) cylinder(d=D_CBLE, h=WALL+0.2);
        // Lid lip recess
        translate([WALL+0.5, WALL+0.5, BOX_H-LID_H-1])
            rounded_box(BOX_L-2*WALL-1, BOX_W-2*WALL-1, LID_H+1.1, CORN-WALL);
    }
    // PCB standoffs (4×, M3 inserts)
    for (pos=[[12,12],[12,BOX_W-20],[BOX_L-20,12],[BOX_L-20,BOX_W-20]])
        translate([pos[0],pos[1],WALL]) difference() {
            cylinder(d=6, h=8);
            cylinder(d=3.4, h=8);
        }
    // Lid screw bosses (4×, M4 inserts)
    lid_bosses(body=true);
    // Cable gland boss (raised on bottom)
    translate([BOX_L/2, BOX_W-15, -4])
        difference() {
            cylinder(d=18, h=4);
            translate([0,0,-0.1]) cylinder(d=D_CBLE, h=5);
        }
}

module lid_bosses(body=true) {
    for (pos=[[10,10],[10,BOX_W-10],[BOX_L-10,10],[BOX_L-10,BOX_W-10]])
        translate([pos[0],pos[1], BOX_H-LID_H-1]) {
            if (body) difference() {
                cylinder(d=10, h=LID_H+1);
                translate([0,0,-0.1]) cylinder(d=D_M4_HS, h=7);
            }
        }
}

// ── Lid ───────────────────────────────────────────────────────────────────────
module lid() {
    difference() {
        union() {
            rounded_box(BOX_L, BOX_W, LID_H, CORN);
            // Engagement lip (drops into box)
            translate([WALL+0.7, WALL+0.7, -(LID_H-1)])
                rounded_box(BOX_L-2*WALL-1.4, BOX_W-2*WALL-1.4, LID_H-0.5, CORN-WALL);
        }
        // O-ring groove in lid mating face
        translate([0, 0, -(ORING_D+0.4)]) oring_groove();
        // Rotary switch holes (×2)
        translate([BOX_L/2-20, 20, -0.1]) rotate([0,0,0]) {
            cylinder(d=D_ROT, h=LID_H+0.2);
        }
        translate([BOX_L/2+20, 20, -0.1]) {
            cylinder(d=D_ROT, h=LID_H+0.2);
        }
        // Band label window (engraved)
        translate([10, 40, LID_H-1.5]) cube([BOX_L-20, 30, 2]);
        // M4 screw clearance holes
        for (pos=[[10,10],[10,BOX_W-10],[BOX_L-10,10],[BOX_L-10,BOX_W-10]])
            translate([pos[0],pos[1],-0.1]) cylinder(d=D_M4, h=LID_H+0.2);
    }
}

// ── BNC panel label plate ─────────────────────────────────────────────────────
// Engraveable label; glue or snap onto front face
module front_label() {
    difference() {
        translate([8, -WALL-2, BOX_H/2-LID_H-12]) cube([BOX_L-16, 2, 24]);
        for (x=[22, BOX_L/2, BOX_L-22])
            translate([x, -WALL-3, BOX_H/2-LID_H/2])
                rotate([-90,0,0]) cylinder(d=D_BNC+0.5, h=5);
    }
}

// ── Cable entry gland (M20 compatible) ───────────────────────────────────────
module cable_gland_plug() {
    // Solid plug for unused entries
    difference() {
        cylinder(d=19, h=10);
        translate([0,0,-0.1]) cylinder(d=9.5, h=3); // partial depression for grip
    }
}

// ── Mounting bracket (U-bracket, mast or pole mount) ─────────────────────────
module mast_bracket() {
    MAST_D = 40; // fits 1.5" (38mm) or 40mm mast
    difference() {
        union() {
            // Base plate
            cube([BOX_L, 20, 8]);
            // U-clamp arms
            translate([BOX_L/2-20, 0, 0]) cube([40, 20, 8]);
            translate([BOX_L/2-20, -MAST_D/2-8, 0]) cube([40, MAST_D/2+8, 8]);
            // U-clamp arch
            translate([BOX_L/2, -MAST_D/2, 0]) difference() {
                cylinder(d=MAST_D+16, h=8);
                translate([0,0,-0.1]) cylinder(d=MAST_D+0.5, h=9);
                translate([-MAST_D, -MAST_D, -0.1]) cube([MAST_D*2, MAST_D, 9]);
            }
        }
        // Bolt holes for U-clamp (M6)
        for (x=[BOX_L/2-10, BOX_L/2+10]) {
            translate([x, -MAST_D/2-4, -0.1]) cylinder(d=6.5, h=9);
        }
        // Attachment holes to box bottom (M4)
        for (x=[15, BOX_L-15]) translate([x, 10, -0.1]) cylinder(d=4.3, h=9);
    }
}

// ── Drip cap (mounts on top of box to shed rain) ──────────────────────────────
module drip_cap() {
    translate([-5, -5, 0])
    difference() {
        rounded_box(BOX_L+10, BOX_W+10, 4, 6);
        // Drip edge groove
        translate([3, 3, 2]) rounded_box(BOX_L+4, BOX_W+4, 3, 4);
        // Center opening (over lid)
        translate([5, 5, -0.1]) rounded_box(BOX_L, BOX_W, 5, 5);
    }
}

// ── Assembly ──────────────────────────────────────────────────────────────────
box_body();
// translate([0, BOX_W+10, LID_H]) rotate([180,0,0]) lid();
// translate([BOX_L+10, 0, 0]) mast_bracket();
// translate([0, -30, 0]) drip_cap();
