/*
 * enclosure_hybrid_vhf.scad
 * Weatherproof VHF/UHF Hybrid Coupler Enclosure
 * TM-HYB-001 Rev A
 * Material: ASA; watertight O-ring seal
 * Print bed: 200 × 200 mm
 *
 * Fits: VHF/UHF branch-line or rat-race hybrid PCB (50 × 50 mm)
 * or coaxial rat-race assembly for 2M/70cm/33cm.
 * SMA connectors (4 ports); optional N-connector variant.
 *
 * Box: 80 × 70 × 28 mm (compact; all SMA flush-mount)
 * Designed for mast-head mounting; sealed for rain.
 */

$fn = 48;

BOX_L = 80; BOX_W = 70; BOX_H = 28;
WALL  = 2.5;
CORN  = 4.0;
LID_H = 8;
ORING_GROOVE_D = 2.2; // O-ring groove depth

D_SMA   = 10.0;
D_N     = 20.5;  // N-connector panel hole
D_ROT   = 10.5;
D_M3    = 3.4;
D_M3_HS = 4.2;

module rounded_box(l, w, h, r) {
    hull() for (x=[r,l-r]) for (y=[r,w-r])
        translate([x,y,0]) cylinder(r=r, h=h);
}

// ── O-ring groove (rectangular section, 2mm depth, 2.5mm wide) ───────────────
module oring_path() {
    ox = WALL + 2.5; oy = WALL + 2.5;
    difference() {
        translate([ox, oy, 0])
            rounded_box(BOX_L-2*ox, BOX_W-2*oy, 2.0, 2.5);
        translate([ox+2.5, oy+2.5, -0.1])
            rounded_box(BOX_L-2*ox-5, BOX_W-2*oy-5, 2.5, 1);
    }
}

// ── Box body ──────────────────────────────────────────────────────────────────
module box_body() {
    difference() {
        rounded_box(BOX_L, BOX_W, BOX_H-LID_H, CORN);
        // Interior
        translate([WALL, WALL, WALL])
            rounded_box(BOX_L-2*WALL, BOX_W-2*WALL, BOX_H-LID_H-WALL+0.1, CORN-WALL);
        // SMA connectors: 2 on each side for 4-port hybrid
        // Left face (Port 1, Port 2)
        for (y=[BOX_W/2-10, BOX_W/2+10])
            translate([-0.1, y, BOX_H/2-LID_H/2])
                rotate([0,90,0]) cylinder(d=D_SMA, h=WALL+0.2);
        // Right face (Port 3, Port 4)
        for (y=[BOX_W/2-10, BOX_W/2+10])
            translate([BOX_L-WALL-0.1, y, BOX_H/2-LID_H/2])
                rotate([0,90,0]) cylinder(d=D_SMA, h=WALL+0.2);
        // Lid lip recess
        translate([WALL+0.4, WALL+0.4, BOX_H-LID_H-0.5])
            rounded_box(BOX_L-2*WALL-0.8, BOX_W-2*WALL-0.8, LID_H+0.6, CORN-WALL);
    }
    // PCB standoffs (4×)
    for (pos=[[10,10],[10,BOX_W-14],[BOX_L-14,10],[BOX_L-14,BOX_W-14]])
        translate([pos[0],pos[1],WALL]) difference() {
            cylinder(d=5, h=5);
            cylinder(d=3.0, h=5);
        }
    // Lid screw bosses M3
    for (pos=[[6,6],[6,BOX_W-6],[BOX_L-6,6],[BOX_L-6,BOX_W-6]])
        translate([pos[0],pos[1], BOX_H-LID_H-1]) difference() {
            cylinder(d=8, h=LID_H+1);
            translate([0,0,-0.1]) cylinder(d=D_M3_HS, h=5);
        }
}

// ── Lid ───────────────────────────────────────────────────────────────────────
module lid() {
    difference() {
        union() {
            rounded_box(BOX_L, BOX_W, LID_H, CORN);
            // Engagement lip
            translate([WALL+0.6, WALL+0.6, -(LID_H-1)])
                rounded_box(BOX_L-2*WALL-1.2, BOX_W-2*WALL-1.2, LID_H-0.8, CORN-WALL);
        }
        // O-ring groove
        translate([0, 0, -(ORING_GROOVE_D+0.3)]) oring_path();
        // M3 screw holes
        for (pos=[[6,6],[6,BOX_W-6],[BOX_L-6,6],[BOX_L-6,BOX_W-6]])
            translate([pos[0],pos[1],-0.1]) cylinder(d=D_M3, h=LID_H+0.2);
        // Optional label recess on top
        translate([10, 10, LID_H-1.5]) cube([BOX_L-20, BOX_W-20, 2]);
    }
}

// ── N-connector variant lid ───────────────────────────────────────────────────
// For lower bands (6M, 2M) where N-connectors are preferred for weatherproofing
module lid_n_connector() {
    difference() {
        lid();
        // Replace two SMA holes on left with N-connector holes
        // (cut from lid bottom face)
        for (y=[BOX_W/2-10, BOX_W/2+10])
            translate([-0.1, y, -(LID_H/2)])
                rotate([0,90,0]) cylinder(d=D_N, h=WALL+0.2);
    }
}

// ── Mast clamp (for mast-head mounting, 1.25" = 32mm to 2" = 51mm) ───────────
module mast_clamp(mast_d=38) {
    CLAMP_T = 5;
    difference() {
        union() {
            // Clamp body
            translate([0, 0, 0]) cube([BOX_L, mast_d + 2*CLAMP_T, CLAMP_T]);
            // Half-circle for mast
            translate([BOX_L/2, mast_d/2 + CLAMP_T, 0])
                rotate([0,0,0]) difference() {
                    cylinder(d=mast_d+2*CLAMP_T, h=CLAMP_T);
                    translate([0,0,-0.1]) cylinder(d=mast_d+0.5, h=CLAMP_T+0.2);
                    translate([-(mast_d+2*CLAMP_T)/2, 0, -0.1])
                        cube([mast_d+2*CLAMP_T, mast_d+2*CLAMP_T, CLAMP_T+0.2]);
                }
        }
        // Bolt holes for clamp (M5)
        for (x=[BOX_L/2-12, BOX_L/2+12])
            translate([x, mast_d/2+CLAMP_T, -0.1]) cylinder(d=5.5, h=CLAMP_T+0.2);
        // Attachment to box bottom (M3 × 4)
        for (px=[10, BOX_L-10]) for (py=[6, mast_d+2*CLAMP_T-6])
            translate([px, py, -0.1]) cylinder(d=3.4, h=CLAMP_T+0.2);
    }
}

// ── Coaxial rat-race holder ───────────────────────────────────────────────────
// Holds RG-59 coaxial ring at correct diameter inside the VHF box
module coax_ring_holder(ring_od) {
    // Ring formed from coax; this cradle secures it at 4 points
    HOLD_H = 8;
    for (a=[0, 90, 180, 270]) {
        rotate([0,0,a]) translate([ring_od/2, 0, 0])
        difference() {
            cylinder(d=8, h=HOLD_H);
            translate([0, 0, 2]) cylinder(d=5.5, h=HOLD_H);  // coax channel
        }
    }
    // Central support ring
    difference() {
        cylinder(d=ring_od+4, h=2);
        translate([0,0,-0.1]) cylinder(d=ring_od-4, h=3);
    }
}

// ── Weatherproof vent plug ────────────────────────────────────────────────────
// Press-fit vent for pressure equalization without water ingress
module vent_plug() {
    difference() {
        cylinder(d=9, h=8);
        translate([0,0,2]) cylinder(d=5, h=6.5);
        // Tortuous vent path
        for (a=[0,90,180,270]) rotate([0,0,a]) translate([2,0,3]) cylinder(d=1, h=5);
    }
}

// ── SMA label ring ─────────────────────────────────────────────────────────────
// Thin ring that snaps over SMA panel nut, with engraved port label
module sma_label_ring(label) {
    difference() {
        cylinder(d=16, h=3);
        translate([0,0,-0.1]) cylinder(d=10.2, h=3.2);
    }
    // Embossed label would be added in slicer with text tool
}

// ── Assembly ──────────────────────────────────────────────────────────────────
box_body();
// translate([0, BOX_W+10, LID_H]) rotate([180,0,0]) lid();
// translate([BOX_L+10, 0, 0]) mast_clamp(38);
// For rat-race holder: translate([BOX_L+10, BOX_W+10, 0]) coax_ring_holder(90);
