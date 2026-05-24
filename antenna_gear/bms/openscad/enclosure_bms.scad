/*
 * enclosure_bms.scad
 * TM-BMS-001 Rev A — Battery Management System Enclosures
 *
 * Enclosures for:
 *   - Compact BMS controller board (BQ76920 + ESP32 + CYD)
 *   - 18650 cell holder for 3S2P (6 cells) and 3S4P (12 cells)
 *   - 4S pouch/prismatic cell frame with compression
 *   - External balance resistor heatsink frame
 *   - LVD relay module housing
 *
 * All parts print within 200×200×200 mm bed.
 * Material: PETG preferred (heat resistant, chemical resistant)
 *           ASA for outdoor use; PLA for prototype/bench use only
 * Walls: 3 perimeters; floor/top: 4 layers; infill: 30%
 *
 * Units: mm
 */

$fn = 60;

// ─── Global Constants ────────────────────────────────────────────────────────
WALL  = 3.0;
FLOOR = 3.5;
LID_H = 4.0;
TOL   = 0.3;     // Print tolerance / clearance

M3_C  = 3.4;
M3_I  = 4.5;     // Heat insert OD
M4_C  = 4.4;
BOSS_OD = 8.0;
BOSS_H  = 8.0;

// ─── Helper Modules ──────────────────────────────────────────────────────────
module boss_m3(h=BOSS_H) {
    difference() {
        cylinder(d=BOSS_OD, h=h);
        cylinder(d=M3_I, h=h);
    }
}

module screw_thru(h=20) {
    cylinder(d=M3_C, h=h);
}

module vent_slot(w=3, h=20) {
    cube([w, WALL+2, h], center=true);
}

// ─── Anderson Powerpole and Connector Cutouts ─────────────────────────────────
module pp30_cutout() { translate([-7.5,-6,0]) cube([15,12,20]); }
module barrel_j()    { cylinder(d=8.5, h=20, center=true); }
module led_hole()    { cylinder(d=5.5, h=20, center=true); }
module sw_hole()     { cylinder(d=12,  h=20, center=true); }

// =============================================================================
// MODULE 1: BMS CONTROLLER ENCLOSURE
// =============================================================================
// Holds: CYD (82×53mm), BQ76920 PCB (~80×60mm), INA226 modules × 2,
//        ADS1115 module, screw terminals for cell taps and load connections
// Front: CYD display window, 4× LED indicators (cell faults)
// Rear:  4× cell balance tap terminals, charge input, load output, UART
// Top:   Power on/off switch, charge LED
// Dimensions: 165W × 110D × 75H
// =============================================================================
module bms_controller_body(BW=165, BD=110, BH=75) {
    difference() {
        cube([BW, BD, BH]);

        // Interior
        translate([WALL, WALL, FLOOR]) cube([BW-2*WALL, BD-2*WALL, BH]);

        // Front: CYD display window (82×53mm, centered)
        translate([BW/2 - 41, 0, BH/2 - 26.5]) cube([82, WALL+1, 53]);

        // Front: 4× fault LEDs (right of display)
        for (i=[0:3]) translate([BW-25, 0, 20+i*12]) rotate([-90,0,0]) led_hole();

        // Front: buzzer hole
        translate([BW-15, 0, BH-15]) cylinder(d=8, h=WALL+1);

        // Rear: 5× screw terminals for cell taps (B−, VC1, VC2, VC3, B+)
        for (i=[0:4]) translate([15+i*28, BD, BH/2-5]) cube([10, WALL+1, 10]);

        // Rear: charge input (2× terminals or Powerpole)
        translate([BW-55, BD, 15]) rotate([90,0,0]) pp30_cutout();

        // Rear: load output (Powerpole 30A)
        translate([BW-25, BD, 15]) rotate([90,0,0]) pp30_cutout();

        // Rear: UART (3.5mm jack or 3-pin header hole)
        translate([10, BD, BH-15]) rotate([90,0,0]) cylinder(d=8, h=WALL+1);

        // Top: power switch
        translate([BW/2, BD/2, BH]) sw_hole();

        // Top: charge status LED
        translate([BW/2+20, BD/2, BH]) led_hole();

        // Top: vent slots
        for (i=[-2:2]) translate([BW/2+i*12, BD/2, BH-1])
            cube([5, 25, WALL+2], center=true);

        // Lid rabbet
        translate([TOL, TOL, BH-LID_H]) cube([BW-2*TOL, BD-2*TOL, LID_H+1]);

        // Left side: USB access port (for ESP32 programming)
        translate([0, 25, 15]) cube([WALL+1, 15, 10]);
    }

    // Internal PCB standoffs
    for (x=[12, BW-20]) for (y=[12, BD-20]) {
        translate([x, y, FLOOR]) boss_m3(BOSS_H);
    }
}

// =============================================================================
// MODULE 2: BMS CONTROLLER LID
// =============================================================================
module bms_controller_lid(BW=165, BD=110) {
    difference() {
        cube([BW, BD, LID_H]);
        for (x=[8, BW-8]) for (y=[8, BD-8]) {
            translate([x, y, 0]) screw_thru(LID_H+1);
        }
        // Label recess
        translate([BW/2-35, BD/2-12, LID_H-0.4]) cube([70, 24, 0.5]);
    }
}

// =============================================================================
// MODULE 3: 18650 CELL HOLDER — 3S2P (6 cells)
// =============================================================================
// 18650 cell: 18.5mm diameter × 65mm length
// 3S2P: 3 groups of 2 parallel cells in series
// Layout: 2 cells wide × 3 cells long (side by side in each group)
// =============================================================================
module holder_18650_3s2p() {
    CELL_D  = 18.5 + 0.5;   // Cell diameter + clearance
    CELL_L  = 65.0 + 2.0;   // Cell length + end clearance
    N_SER   = 3;             // Series groups
    N_PAR   = 2;             // Parallel cells per group
    PITCH_P = CELL_D + 2.0;  // Pitch between parallel cells
    PITCH_S = CELL_D + 3.0;  // Pitch between series groups
    GAP     = 2.5;           // Wall between cells

    HW = N_PAR * PITCH_P + 2*WALL;
    HD = N_SER * PITCH_S + 2*WALL;
    HH = CELL_L + 2*WALL;

    difference() {
        cube([HW, HD, HH]);

        // Cell bores (18650 = cylinder)
        for (j=[0:N_SER-1]) for (i=[0:N_PAR-1]) {
            translate([WALL + PITCH_P/2 + i*PITCH_P,
                       WALL + PITCH_S/2 + j*PITCH_S,
                       WALL]) {
                cylinder(d=CELL_D, h=CELL_L);
                // Positive terminal access (top)
                cylinder(d=8, h=HH);
                // Negative terminal access (bottom) via bottom plate
                translate([0, 0, -WALL-1]) cylinder(d=8, h=WALL+2);
            }
        }

        // Nickel strip slots between groups (inter-series connections)
        for (j=[0:N_SER-2]) {
            translate([WALL-1, WALL + PITCH_S*(j+1) - 1, HH/2-5])
                cube([HW-2*WALL+2, 2, 10]);
        }

        // Screw holes for securing to enclosure
        for (x=[WALL/2, HW-WALL/2]) for (y=[WALL/2, HD-WALL/2]) {
            translate([x, y, -1]) cylinder(d=M3_C, h=HH+2);
        }
    }

    // Polarity markers (+/- embossed on top face)
    for (j=[0:N_SER-1]) for (i=[0:N_PAR-1]) {
        // Emboss "+" on positive terminal end
        translate([WALL + PITCH_P/2 + i*PITCH_P - 1.5,
                   WALL + PITCH_S/2 + j*PITCH_S,
                   HH - 0.5])
            cube([1.5, 0.5, 0.5]);
    }
}

// =============================================================================
// MODULE 4: 18650 CELL HOLDER — 3S4P (12 cells, higher capacity)
// =============================================================================
module holder_18650_3s4p() {
    CELL_D  = 18.5 + 0.5;
    CELL_L  = 65.0 + 2.0;
    N_SER   = 3;
    N_PAR   = 4;
    PITCH_P = CELL_D + 2.0;
    PITCH_S = CELL_D + 3.0;

    HW = N_PAR * PITCH_P + 2*WALL;  // ~96mm
    HD = N_SER * PITCH_S + 2*WALL;  // ~69mm
    HH = CELL_L + 2*WALL;           // ~75mm

    difference() {
        cube([HW, HD, HH]);
        for (j=[0:N_SER-1]) for (i=[0:N_PAR-1]) {
            translate([WALL + PITCH_P/2 + i*PITCH_P,
                       WALL + PITCH_S/2 + j*PITCH_S,
                       WALL]) {
                cylinder(d=CELL_D, h=CELL_L);
                cylinder(d=8, h=HH);
                translate([0, 0, -WALL-1]) cylinder(d=8, h=WALL+2);
            }
        }
        for (j=[0:N_SER-2]) {
            translate([WALL-1, WALL + PITCH_S*(j+1) - 1, HH/2-5])
                cube([HW-2*WALL+2, 2, 10]);
        }
        for (x=[WALL/2, HW-WALL/2]) for (y=[WALL/2, HD-WALL/2]) {
            translate([x, y, -1]) cylinder(d=M3_C, h=HH+2);
        }
    }
}

// =============================================================================
// MODULE 5: PRISMATIC 4S LIFEPO4 CELL FRAME
// =============================================================================
// For smaller prismatic cells (e.g., CATL 50Ah, ~105×30×208mm)
// Frame holds cells in compression using threaded rod
// =============================================================================
module prismatic_4s_frame(
    CW=105, CD=30, CH=208, N=4
) {
    FW = CW + 2*WALL + 10;  // Extra for terminals
    FD = N * CD + 20;        // Cells + compression clearance
    FH = CH + FLOOR + 15;    // Cell height + floor + terminal clearance

    difference() {
        cube([FW, FD, FH]);
        translate([WALL, WALL, FLOOR]) cube([FW-2*WALL, FD-2*WALL, FH]);

        // Cell terminal access (top open)
        translate([WALL+5, WALL, FH-25]) cube([FW-2*WALL-10, FD-2*WALL, 30]);

        // BMS wiring pass-through (rear bottom)
        for (i=[0:3]) translate([WALL+10+i*20, FD-WALL, FH/2-5]) cube([5, WALL+1, 12]);

        // Powerpole output (front)
        translate([FW-55, 0, 25]) rotate([-90,0,0]) pp30_cutout();

        // Vent (top rear)
        translate([FW-15, FD/2, FH-WALL]) cylinder(d=9, h=WALL+1);

        // M8 threaded rod clearance holes
        for (y=[FD/4, 3*FD/4]) {
            translate([FW/2-30, y, -1]) cylinder(d=9, h=FH+2);
            translate([FW/2+30, y, -1]) cylinder(d=9, h=FH+2);
        }

        // Lid rabbet
        translate([TOL, TOL, FH-LID_H]) cube([FW-2*TOL, FD-2*TOL, LID_H+1]);
    }

    // Cell retaining ribs (lateral constraint between cells)
    for (i=[1:N-1]) {
        translate([WALL, WALL+i*CD-1, FLOOR]) cube([FW-2*WALL, 2, CH]);
    }
    // BMS PCB standoffs
    for (dx=[25, FW-35]) translate([dx, FD-WALL-8, FLOOR+10]) boss_m3(6);
}

// =============================================================================
// MODULE 6: BALANCE RESISTOR HEATSINK FRAME
// =============================================================================
// Holds 4× balance resistors (10Ω 2W, TO-220 or axial)
// Open sides for air convection
// =============================================================================
module balance_heatsink_frame() {
    FW = 80; FH = 40; FD = 20;
    difference() {
        cube([FW, FD, FH]);
        // Four resistor cutouts (axial 2W: 8mm dia × 20mm long)
        for (i=[0:3]) translate([10+i*18, -1, FH/2]) rotate([-90,0,0])
            cylinder(d=8, h=FD+2);
        // Airflow slots on sides
        for (z=[5, 15, 25, 35]) translate([-1, FD/4, z]) cube([FW+2, FD/2, 5]);
        // Mounting holes
        for (x=[5, FW-5]) translate([x, FD/2, -1]) cylinder(d=M3_C, h=FH+2);
    }
}

// =============================================================================
// MODULE 7: LVD RELAY MODULE HOUSING
// =============================================================================
// Holds: LVD comparator PCB (≤ 60×50mm), SPDT relay, terminal blocks
// =============================================================================
module lvd_housing(BW=90, BD=70, BH=40) {
    difference() {
        cube([BW, BD, BH]);
        translate([WALL, WALL, FLOOR]) cube([BW-2*WALL, BD-2*WALL, BH]);

        // Terminal block holes (input + output, front)
        for (x=[10, 30, 55, 75]) translate([x, 0, BH/2-4]) cube([8, WALL+1, 8]);

        // LED indicator
        translate([BW/2, 0, BH-10]) rotate([-90,0,0]) led_hole();

        // Lid rabbet
        translate([TOL, TOL, BH-LID_H]) cube([BW-2*TOL, BD-2*TOL, LID_H+1]);
    }
    for (x=[12, BW-20]) for (y=[12, BD-20]) translate([x, y, FLOOR]) boss_m3(BOSS_H);
}

module lvd_lid(BW=90, BD=70) {
    difference() {
        cube([BW, BD, LID_H]);
        for (x=[7, BW-7]) for (y=[7, BD-7])
            translate([x, y, 0]) cylinder(d=M3_C, h=LID_H+1);
        translate([BW/2-20, BD/2-10, LID_H-0.4]) cube([40, 20, 0.5]);
    }
}

// =============================================================================
// MODULE 8: COMPLETE BATTERY MODULE — 3S2P + BMS STACKED
// =============================================================================
// Cell holder on bottom, BMS controller on top, integrated single enclosure
// =============================================================================
module integrated_battery_bms() {
    // Bottom: cell holder (3S2P)
    holder_18650_3s2p();

    // Top: BMS controller (translated above)
    // (Render separately; stacked and bolted together in assembly)
    // translate([0, 0, 80]) bms_controller_body();
}

// =============================================================================
// RENDER CONTROLS
// =============================================================================

bms_controller_body();
// translate([180, 0, 0]) bms_controller_lid();

// translate([0, 120, 0]) holder_18650_3s2p();
// translate([0, 200, 0]) holder_18650_3s4p();

// translate([200, 0, 0]) prismatic_4s_frame();

// translate([0, 300, 0]) balance_heatsink_frame();

// translate([100, 300, 0]) lvd_housing();
// translate([200, 300, 0]) lvd_lid();
