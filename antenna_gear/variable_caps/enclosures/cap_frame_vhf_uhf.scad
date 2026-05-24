// ============================================================
//  cap_frame_vhf_uhf.scad — VC-VHF and VC-UHF Frames
//  TM-VCAP-ENC-005 Rev A
//  VC-VHF: 2-47 pF, 500V peak, 8R+9S plates, r=15mm, d=1.0mm
//    Frame: 40×40×35mm
//  VC-UHF: 0.5-12 pF, 200V peak, 4R+5S plates, r=8mm, d=0.5mm
//    Frame: 25×25×20mm  (PCB-mount version also included)
//  Print: PETG; minimize frame dimensions for VHF/UHF stray inductance
// ============================================================

$fn = 64;

// ---- Select design ----
DESIGN          = "VC-VHF";   // "VC-VHF" or "VC-UHF"

// VC-VHF parameters
VHF_PLATE_OD    = 30.0;
VHF_SHAFT_DIA   = 4.0;
VHF_PLATE_DEPTH = 16.25; // 17 plates × 1.5mm pitch = 24mm; actual ~16.25mm
VHF_FRAME_W     = 40.0;
VHF_FRAME_H     = 40.0;
VHF_FRAME_D     = 35.0;
VHF_BEARING_L   = 4.5;
VHF_WALL        = 2.5;
VHF_N_RODS      = 3;
VHF_ROD_OFS     = 24.0;

// VC-UHF parameters
UHF_PLATE_OD    = 16.0;
UHF_SHAFT_DIA   = 3.0;
UHF_PLATE_DEPTH = 5.35;  // 9 plates × 0.8mm pitch
UHF_FRAME_W     = 25.0;
UHF_FRAME_H     = 25.0;
UHF_FRAME_D     = 20.0;
UHF_BEARING_L   = 3.0;
UHF_WALL        = 2.0;
UHF_N_RODS      = 3;
UHF_ROD_OFS     = 12.0;

// Active parameters
PLATE_OD    = (DESIGN == "VC-VHF") ? VHF_PLATE_OD    : UHF_PLATE_OD;
SHAFT_DIA   = (DESIGN == "VC-VHF") ? VHF_SHAFT_DIA   : UHF_SHAFT_DIA;
PLATE_DEPTH = (DESIGN == "VC-VHF") ? VHF_PLATE_DEPTH : UHF_PLATE_DEPTH;
FRAME_W     = (DESIGN == "VC-VHF") ? VHF_FRAME_W     : UHF_FRAME_W;
FRAME_H     = (DESIGN == "VC-VHF") ? VHF_FRAME_H     : UHF_FRAME_H;
FRAME_D     = (DESIGN == "VC-VHF") ? VHF_FRAME_D     : UHF_FRAME_D;
BEARING_L   = (DESIGN == "VC-VHF") ? VHF_BEARING_L   : UHF_BEARING_L;
WALL        = (DESIGN == "VC-VHF") ? VHF_WALL        : UHF_WALL;
N_RODS      = (DESIGN == "VC-VHF") ? VHF_N_RODS      : UHF_N_RODS;
ROD_OFS     = (DESIGN == "VC-VHF") ? VHF_ROD_OFS     : UHF_ROD_OFS;
ROD_DIA     = (DESIGN == "VC-VHF") ? 2.0             : 1.5;

BEARING_OD  = SHAFT_DIA + 6.0;

module statorRods() {
    for (i = [0:N_RODS-1]) {
        rotate([0, 0, i * (360/N_RODS)])
        translate([ROD_OFS/2, 0, 0])
        cylinder(h=FRAME_D+2, d=ROD_DIA+0.25, center=true);
    }
}

module mainFrame() {
    difference() {
        translate([-FRAME_W/2, -FRAME_H/2, -FRAME_D/2])
        cube([FRAME_W, FRAME_H, FRAME_D]);

        // Plate bore
        cylinder(h=PLATE_DEPTH+4, d=PLATE_OD+1.5, center=true);

        // Shaft bore
        cylinder(h=FRAME_D+2, d=SHAFT_DIA+0.25, center=true);

        // Stator rod holes
        statorRods();

        // PCB mounting slots (UHF version: 4× mounting holes for PCB mount)
        if (DESIGN == "VC-UHF") {
            for (sx = [-1,1], sy = [-1,1]) {
                translate([sx*(FRAME_W/2-4), sy*(FRAME_H/2-4), -FRAME_D/2-1])
                cylinder(h=FRAME_D+2, d=2.0);  // M2 clearance
            }
        }
        // VHF: corner M3 holes
        if (DESIGN == "VC-VHF") {
            for (sx = [-1,1], sy = [-1,1]) {
                translate([sx*(FRAME_W/2-5), sy*(FRAME_H/2-5), -FRAME_D/2-1])
                cylinder(h=FRAME_D+2, d=3.2);
            }
        }
    }

    // Bearing bosses (integral with frame for compact size)
    translate([0, 0, -FRAME_D/2 - BEARING_L])
    difference() {
        cylinder(h=BEARING_L, d=BEARING_OD);
        translate([0, 0, -1])
        cylinder(h=BEARING_L+2, d=SHAFT_DIA+0.25);
    }
    translate([0, 0, FRAME_D/2])
    difference() {
        cylinder(h=BEARING_L + 4, d=BEARING_OD);  // extra for motor coupling
        translate([0, 0, -1])
        cylinder(h=BEARING_L+5, d=SHAFT_DIA+0.25);
    }
}

// VHF terminal posts (separate stub for soldering short leads)
module vhfTerminals() {
    // Stator terminal stub (right side)
    translate([FRAME_W/2 - 3, 0, 0])
    difference() {
        cube([6, 6, FRAME_D], center=true);
        cylinder(h=FRAME_D+1, d=2.0, center=true);  // wire hole
    }
    // Rotor terminal stub (left side)
    translate([-FRAME_W/2 + 3, 0, 0])
    difference() {
        cube([6, 6, FRAME_D], center=true);
        cylinder(h=FRAME_D+1, d=2.0, center=true);
    }
}

// UHF PCB footprint pad (shows correct PCB pad geometry for board mounting)
module uhfPcbMount() {
    // Bottom face has flat mounting pads — print as labels, route PCB to match
    translate([0, 0, -FRAME_D/2 - 0.5])
    linear_extrude(0.3)
    text("UHF-PCB", size=2.0, halign="center", valign="center");
}

// ---- Render ----
mainFrame();

if (DESIGN == "VC-VHF") {
    vhfTerminals();
    translate([0, FRAME_H/2 + 0.1, 0])
    rotate([90, 0, 0])
    linear_extrude(0.4)
    text("VC-VHF 500V TM-VCAP-ENC-005", size=2.5, halign="center", valign="center");
}

if (DESIGN == "VC-UHF") {
    uhfPcbMount();
    translate([0, UHF_FRAME_H/2 + 0.1, 0])
    rotate([90, 0, 0])
    linear_extrude(0.4)
    text("VC-UHF 200V TM-VCAP-ENC-005", size=2.0, halign="center", valign="center");
}

// ---- BUILD NOTES ----
// VHF: Print PETG, 0.2mm layer, 4 walls; minimize ALL wire lead lengths (<5mm)
//      Use semi-rigid coax stubs as terminals soldered directly to stator/rotor
//      Bearing: 2× PTFE bushing 4mm ID × 7mm OD × 4.5mm L
//      PCB mount preferred; chassis mount acceptable with SMA or BNC connector
//
// UHF: Print PETG, 0.15mm layer for dimensional accuracy (plates are small)
//      Bearing: 2× PTFE bushing 3mm ID × 6mm OD × 3mm L
//      CRITICAL: total lead length stator-to-circuit MUST be <3mm at 1296 MHz
//        (lead inductance L = 1nH/mm; at 3mm = 3nH → X_L at 1296 MHz = 24Ω,
//         comparable to 1/X_C — completely swamps the capacitor!)
//      PCB-mount directly: solder stator plates to PCB trace, rotor to shaft spring
//      After printing, verify plate spacing with 0.5mm feeler gauge
//      Clean all surfaces of PETG dust with IPA — contamination increases loss tangent
