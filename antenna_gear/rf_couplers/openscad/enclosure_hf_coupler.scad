/*
 * enclosure_hf_coupler.scad
 * TM-COUP-001 Rev A — HF Directional Coupler Enclosures
 *
 * Enclosures for:
 *   Module 1: Toroidal Bruene bridge coupler (SO-239 main line, BNC coupled)
 *   Module 2: Dual-detector board (FWD + REF, BNC in/out)
 *   Module 3: Panel-mount power meter faceplate (CYD window + SO-239)
 *   Module 4: Compact in-line SWR bridge (SO-239 × 3: IN, OUT, FWD, REF)
 *   Module 5: SO-239 strain relief plate (holds connector flush to enclosure)
 *
 * Material: PETG or ABS
 * Print: 3 perimeters, 30% infill, 0.2mm layer
 * Units: mm
 *
 * SO-239 chassis flange: 29.2mm hole; 4× M3 on 34mm BCD
 * BNC chassis: 12.4mm hole; 2× M3 on 20mm centers (BNC-75 style)
 * SMA chassis: 6.5mm hole; 2× M2.5 on 10.5mm centers (standard SMA flange)
 */

$fn = 60;

WALL  = 3.0;
LID_H = 4.0;
TOL   = 0.3;
M3_C  = 3.4;
M3_I  = 4.5;   // Heat insert OD
M2P5  = 2.7;   // M2.5 clearance
M4_C  = 4.4;
BOSS  = 8.0;   // PCB standoff boss OD
BOSS_H= 8.0;   // PCB standoff boss height

// ─── Connector cutout modules ────────────────────────────────────────────────

// SO-239 chassis hole (29.2mm) + 4× M3 on 34mm BCD
module so239_cutout() {
    cylinder(d=29.2, h=20);                  // Main aperture
    for (i=[0:3]) rotate([0,0,i*90+45])
        translate([17, 0, 0]) cylinder(d=M3_C, h=20);  // Mounting holes
}

// BNC female chassis mount: 12.4mm bore + 2× M3 on 20mm centers
module bnc_cutout() {
    cylinder(d=12.4, h=20);
    for (x=[-10, 10]) translate([x, 0, 0]) cylinder(d=M3_C, h=20);
}

// SMA female chassis: 6.5mm bore + 2× M2.5 on 10.5mm centers (landscape)
module sma_cutout() {
    cylinder(d=6.5, h=20);
    for (x=[-5.25, 5.25]) translate([x, 0, 0]) cylinder(d=M2P5, h=20);
}

// N-type chassis: 16.0mm bore + 4× M3 on 25mm BCD
module n_cutout() {
    cylinder(d=16.0, h=20);
    for (i=[0:3]) rotate([0,0,i*90+45])
        translate([12.5, 0, 0]) cylinder(d=M3_C, h=20);
}

// PCB boss (standoff for PCB screw mounting)
module pcb_boss(h=BOSS_H) {
    difference() {
        cylinder(d=BOSS, h=h);
        cylinder(d=M3_I, h=h);  // Heat insert bore
    }
}

// =============================================================================
// MODULE 1: TOROIDAL BRUENE COUPLER ENCLOSURE
// =============================================================================
// Main coupler body: 120W × 80D × 45H (external)
// Front: 2× SO-239 (IN, OUT) — main line through
// Rear:  2× BNC (FWD, REF) — coupled detector outputs
// Lid: flat, 4× M3 corners
// =============================================================================
module bruene_coupler_body(BW=120, BD=80, BH=45) {
    difference() {
        cube([BW, BD, BH]);

        // Interior cavity
        translate([WALL, WALL, WALL + 3]) cube([BW-2*WALL, BD-2*WALL, BH]);

        // Front wall: 2× SO-239, centered, 30mm apart
        for (x=[BW/2-20, BW/2+20]) {
            translate([x, 0, BH/2]) rotate([-90,0,0]) so239_cutout();
        }

        // Rear wall: 2× BNC (FWD, REF), 20mm apart
        for (x=[BW/2-15, BW/2+15]) {
            translate([x, BD, BH/2]) rotate([90,0,0]) bnc_cutout();
        }

        // Cable pass-through (main line, 12mm diameter, front to rear)
        translate([BW/2, -1, BH/2]) rotate([-90,0,0])
            cylinder(d=12, h=BD+2);

        // Lid rabbet
        translate([TOL, TOL, BH-LID_H]) cube([BW-2*TOL, BD-2*TOL, LID_H+1]);

        // Lid screw holes (4× corners, M3)
        for (x=[8, BW-8]) for (y=[8, BD-8]) {
            translate([x, y, BH-LID_H-1]) cylinder(d=M3_C, h=LID_H+2);
        }
    }

    // PCB standoffs (4× for coupler PCB)
    for (x=[12, BW-20]) for (y=[12, BD-20]) {
        translate([x, y, WALL+3]) pcb_boss(BOSS_H);
    }

    // Main line support block (clamps conductor between SO-239s)
    // Center block: holds 14 AWG main line conductor in correct position
    translate([BW/2-8, BD/2-4, WALL+3]) difference() {
        cube([16, 8, 12]);
        translate([8, 4, -1]) cylinder(d=3.5, h=14);  // Wire bore
    }
}

module bruene_coupler_lid(BW=120, BD=80) {
    difference() {
        cube([BW, BD, LID_H]);
        for (x=[8, BW-8]) for (y=[8, BD-8]) {
            translate([x, y, 0]) cylinder(d=M3_C, h=LID_H+1);
        }
        // Label recess
        translate([BW/2-25, BD/2-8, LID_H-0.4]) cube([50, 16, 0.5]);
    }
}

// =============================================================================
// MODULE 2: DUAL DETECTOR BOARD ENCLOSURE
// =============================================================================
// 80mm × 50mm × 30mm
// Front: 2× SMA (FWD in, REF in — from coupler)
// Rear: 2× 3.5mm jack or 3-pin header (DC out to ESP32)
// =============================================================================
module detector_enclosure(BW=80, BD=50, BH=30) {
    difference() {
        cube([BW, BD, BH]);
        translate([WALL, WALL, WALL+2]) cube([BW-2*WALL, BD-2*WALL, BH]);

        // Front: 2× SMA
        for (x=[BW/2-12, BW/2+12]) {
            translate([x, 0, BH/2]) rotate([-90,0,0]) sma_cutout();
        }

        // Rear: 2× 3.5mm jack holes
        for (x=[BW/2-12, BW/2+12]) {
            translate([x, BD, BH/2]) rotate([90,0,0]) cylinder(d=6, h=WALL+2);
        }

        // Lid rabbet
        translate([TOL, TOL, BH-LID_H]) cube([BW-2*TOL, BD-2*TOL, LID_H+1]);
        for (x=[6, BW-6]) for (y=[6, BD-6]) {
            translate([x, y, BH-LID_H-1]) cylinder(d=M3_C, h=LID_H+2);
        }
    }
    // PCB bosses
    for (x=[8, BW-16]) for (y=[8, BD-16]) {
        translate([x, y, WALL+2]) pcb_boss(6);
    }
}

module detector_lid(BW=80, BD=50) {
    difference() {
        cube([BW, BD, LID_H]);
        for (x=[6, BW-6]) for (y=[6, BD-6]) {
            translate([x, y, 0]) cylinder(d=M3_C, h=LID_H+1);
        }
    }
}

// =============================================================================
// MODULE 3: PANEL METER FACEPLATE (CYD + SO-239)
// =============================================================================
// Panel cutout: 86mm × 58mm (CYD display window)
// Overall: 200mm × 100mm × 6mm
// SO-239 cutouts: 4× (IN, OUT, FWD, REF) below display
// =============================================================================
module panel_meter_faceplate(PW=200, PH=100, PT=5) {
    DW = 82; DH = 52;  // CYD display active area

    difference() {
        cube([PW, PT, PH]);

        // CYD display window (upper center)
        translate([(PW-DW)/2, -1, PH-DH-12]) cube([DW, PT+2, DH]);

        // 4× SO-239 below display (evenly spaced)
        for (i=[0:3]) {
            translate([20 + i*52, PT/2, 22]) rotate([90,0,0]) so239_cutout();
        }

        // Label slots beside each SO-239
        for (i=[0:3]) {
            translate([8 + i*52, -1, 8]) cube([24, PT+2, 6]);
        }

        // Mounting holes (panel mount, 4× M4 countersunk)
        for (x=[8, PW-8]) for (z=[8, PH-8]) {
            translate([x, 0, z]) rotate([-90,0,0]) cylinder(d=M4_C, h=PT+1);
            translate([x, -1, z]) rotate([-90,0,0]) cylinder(d=8, h=3);
        }
    }

    // Alignment tabs (rear, engage panel cutout rim)
    for (z=[PH-DH-16, PH-12]) {
        translate([(PW-DW)/2 - 3, PT, z]) cube([3, 5, DH + 4]);
        translate([PW - (PW-DW)/2, PT, z]) cube([3, 5, DH + 4]);
    }
}

// =============================================================================
// MODULE 4: IN-LINE SWR BRIDGE (COMPACT)
// =============================================================================
// All-in-one: Bruene coupler + detector + CYD display
// Inline SO-239 connectors front and rear; small CYD window on top
// Size: 150mm × 80mm × 60mm
// =============================================================================
module inline_swr_bridge(BW=150, BD=80, BH=60) {
    CYD_W = 82; CYD_H = 52;  // CYD display cutout

    difference() {
        cube([BW, BD, BH]);

        // Interior
        translate([WALL, WALL, WALL+2]) cube([BW-2*WALL, BD-2*WALL, BH]);

        // Front SO-239 (RF IN)
        translate([BW/2, 0, BH*0.45]) rotate([-90,0,0]) so239_cutout();

        // Rear SO-239 (RF OUT — to antenna)
        translate([BW/2, BD, BH*0.45]) rotate([90,0,0]) so239_cutout();

        // Top: CYD display window
        translate([(BW-CYD_W)/2, (BD-50)/2, BH-WALL-1]) cube([CYD_W, 50, WALL+2]);

        // Right side: USB programming port (ESP32)
        translate([BW-WALL-1, BD/2-8, 12]) cube([WALL+2, 16, 10]);

        // Left side: Power input (micro-USB or 5.5/2.5mm barrel)
        translate([-1, BD/2-6, 12]) cube([WALL+2, 12, 9]);

        // Lid screws (though top; attaches to CYD bracket)
        for (x=[8, BW-8]) for (y=[8, BD-8]) {
            translate([x, y, BH-LID_H-1]) cylinder(d=M3_C, h=LID_H+3);
        }
    }

    // PCB standoffs (6 total — coupler PCB + detector PCB + ESP32/CYD)
    for (x=[12, BW/2, BW-20]) for (y=[12, BD-20]) {
        translate([x, y, WALL+2]) pcb_boss(BOSS_H);
    }

    // CYD mounting tabs (recessed pocket in top face)
    translate([(BW-CYD_W)/2 - 4, (BD-56)/2, BH-WALL-3]) {
        difference() {
            cube([CYD_W + 8, 56, WALL]);
            translate([4, 3, -1]) cube([CYD_W, 50, WALL+2]);
        }
    }
}

module inline_swr_lid(BW=150, BD=80) {
    difference() {
        cube([BW, BD, LID_H]);
        for (x=[8, BW-8]) for (y=[8, BD-8]) {
            translate([x, y, 0]) cylinder(d=M3_C, h=LID_H+1);
        }
        // CYD window in lid
        translate([(BW-82)/2, (BD-50)/2, LID_H-0.4]) cube([82, 50, 0.5]);
    }
}

// =============================================================================
// MODULE 5: SO-239 STRAIN RELIEF PLATE
// =============================================================================
// Outer retaining plate that holds SO-239 flush to enclosure wall.
// Prevents connector from pulling inward under cable load.
// =============================================================================
module so239_strain_plate() {
    PT = 4;
    difference() {
        cube([50, 50, PT]);
        // SO-239 aperture
        translate([25, 25, -1]) cylinder(d=29.2, h=PT+2);
        // 4× M3 flanges
        for (i=[0:3]) rotate([0,0,i*90+45]) {
            translate([17, 0, 0]) rotate([0,0,0])
                translate([25, 25, -1]) rotate([0,0,0]) {
                    // Undo translate for each flange
                }
        }
        // Direct hole approach:
        translate([25-17*0.707, 25-17*0.707, -1]) cylinder(d=M3_C, h=PT+2);
        translate([25+17*0.707, 25-17*0.707, -1]) cylinder(d=M3_C, h=PT+2);
        translate([25+17*0.707, 25+17*0.707, -1]) cylinder(d=M3_C, h=PT+2);
        translate([25-17*0.707, 25+17*0.707, -1]) cylinder(d=M3_C, h=PT+2);
        // Panel mounting holes
        for (x=[5, 45]) for (y=[5, 45]) {
            translate([x, y, -1]) cylinder(d=M4_C, h=PT+2);
        }
    }
}

// =============================================================================
// RENDER CONTROLS
// =============================================================================

bruene_coupler_body();
// translate([130, 0, 0]) bruene_coupler_lid();
// translate([0, 90, 0]) detector_enclosure();
// translate([90, 90, 0]) detector_lid();
// translate([0, 150, 0]) panel_meter_faceplate();
// translate([0, 260, 0]) inline_swr_bridge();
// translate([160, 260, 0]) inline_swr_lid();
// translate([0, 350, 0]) { for(i=[0:3]) translate([i*55,0,0]) so239_strain_plate(); }
