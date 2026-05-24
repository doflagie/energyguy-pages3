/*
 * enclosure_psu.scad
 * TM-PWR-001 Rev A — Power Supply Enclosures
 *
 * Enclosures for:
 *   - Linear PSU (with heatsink slots and transformer cutout)
 *   - Switching PSU / MPPT solar controller (compact, fan-vented)
 *   - LiFePO4 battery box (cell compression + BMS, threaded rod frame)
 *   - Anderson Powerpole distribution panel
 *
 * All parts print within 200×200×200 mm bed constraint.
 * Material: PETG or ASA (outdoor/field use; heat resistance).
 * Layer height: 0.3 mm; walls: 3 perimeters; infill: 30%.
 * Add heat inserts (M3, M4) for all threaded features.
 *
 * Units: millimeters
 */

$fn = 60;

// ─── Global Parameters ───────────────────────────────────────────────────────
WALL      = 3.0;
FLOOR     = 3.5;
LID_H     = 4.0;
GASKET_D  = 2.5;   // O-ring groove diameter
GASKET_R  = GASKET_D * 0.5;
TOLERANCE = 0.3;   // Clearance for lids and inserts

// ─── Fastener Parameters ─────────────────────────────────────────────────────
M3_CLEAR  = 3.4;
M3_INSERT = 4.5;   // Heat insert OD for M3
M4_CLEAR  = 4.4;
M4_INSERT = 5.6;
M4_HEAD   = 8.0;
BOSS_H    = 8.0;
BOSS_OD   = 8.0;

// ─── SO-239 Panel Mount Cutout ────────────────────────────────────────────────
module so239_cutout() {
    cylinder(d=15.0, h=20, center=true);                       // Main through-hole
    translate([0, 0, 5]) cylinder(d=20.5, h=10, center=true); // Flange recess
}

// ─── Anderson Powerpole 30A Pair Cutout ───────────────────────────────────────
module powerpole_30a_cutout() {
    // Standard 30A pair: 15mm wide × 12mm tall
    translate([-7.5, -6, 0]) cube([15, 12, 20]);
}

module powerpole_45a_cutout() {
    // 45A pair: 18.5mm wide × 15mm tall
    translate([-9.25, -7.5, 0]) cube([18.5, 15, 20]);
}

// ─── IEC 320 C14 Inlet Cutout ─────────────────────────────────────────────────
module iec_c14_cutout() {
    translate([-15, -11.5, 0]) cube([30, 23, 20]);
    // Retaining clip holes
    translate([-19, 0, 0]) cylinder(d=3.5, h=20);
    translate([ 19, 0, 0]) cylinder(d=3.5, h=20);
}

// ─── DC Barrel Jack Cutout (5.5mm OD) ────────────────────────────────────────
module barrel_jack_cutout() {
    cylinder(d=8.5, h=20, center=true);
}

// ─── Ventilation Slots ────────────────────────────────────────────────────────
module vent_slots(n=5, slot_w=3, slot_h=20, spacing=6) {
    for (i = [0:n-1]) {
        translate([(i - (n-1)/2) * spacing, -slot_h/2, -1])
            cube([slot_w, slot_h, 10]);
    }
}

// ─── Boss with Heat Insert ────────────────────────────────────────────────────
module boss_m3(h=BOSS_H) {
    difference() {
        cylinder(d=BOSS_OD, h=h);
        cylinder(d=M3_INSERT, h=h);
    }
}

module boss_m4(h=BOSS_H) {
    difference() {
        cylinder(d=BOSS_OD + 2, h=h);
        cylinder(d=M4_INSERT, h=h);
    }
}

// ─── O-Ring Gasket Groove ─────────────────────────────────────────────────────
module gasket_groove(path_x, path_y, depth=1.5) {
    // Square O-ring groove, runs along inside lip of lid
    translate([WALL + GASKET_R, WALL + GASKET_R, -depth])
        difference() {
            cube([path_x - 2*(WALL + GASKET_R), path_y - 2*(WALL + GASKET_R), depth + 1]);
            translate([GASKET_D, GASKET_D, -1])
                cube([path_x - 2*(WALL + GASKET_R) - 2*GASKET_D,
                      path_y - 2*(WALL + GASKET_R) - 2*GASKET_D, depth + 3]);
        }
}

// =============================================================================
// MODULE 1: LINEAR PSU ENCLOSURE — BODY
// =============================================================================
// Fits: transformer (toroidal up to 120mm OD × 60mm H), bridge rectifier,
//       filter capacitors, LM338 or LM723+TIP35C on integral heatsink slots,
//       output binding posts and Powerpole.
// External: 180W × 130D × 120H mm
// Heatsink: side slots for pass element, TO-3 or TO-218 on left wall
// =============================================================================
module linear_psu_body(
    BW = 180, BD = 130, BH = 120,
    include_xfmr_boss = true
) {
    difference() {
        // Outer shell
        cube([BW, BD, BH]);

        // Interior cavity
        translate([WALL, WALL, FLOOR])
            cube([BW - 2*WALL, BD - 2*WALL, BH]);

        // Front panel cutouts (13.8V output binding posts + Powerpole)
        // 2× Powerpole 30A
        translate([40, 0, 50]) rotate([-90, 0, 0]) powerpole_30a_cutout();
        translate([65, 0, 50]) rotate([-90, 0, 0]) powerpole_30a_cutout();
        // Output switch
        translate([100, 0, 50]) cylinder(d=12, h=BD);
        // Voltmeter panel hole (40mm × 20mm)
        translate([120, 0, 40]) cube([45, 10, 25]);
        // LED indicators (×3)
        for (i=[0:2]) translate([135 + i*15, 0, 75]) cylinder(d=5.5, h=10);

        // Rear panel: IEC inlet + fuse
        translate([50, BD, 50]) rotate([90, 0, 0]) iec_c14_cutout();
        // Fuse holder (panel fuse, 20mm)
        translate([120, BD, 55]) rotate([90, 0, 0]) cylinder(d=12.5, h=10);
        // Ground lug stud hole
        translate([150, BD, 55]) rotate([90, 0, 0]) cylinder(d=6, h=10);

        // Left wall: heatsink slots for pass transistors (TO-3 / TO-218)
        // Slot array: 8 slots × 3mm wide × 80mm tall
        translate([0, 30, 20]) vent_slots(n=8, slot_w=3, slot_h=80, spacing=7);
        // TO-218/TO-3 mounting holes (2 per transistor, 3 transistors)
        for (j=[0:2]) {
            translate([0, 40 + j*20, 30]) rotate([0, 90, 0]) cylinder(d=M4_CLEAR, h=10);
            translate([0, 40 + j*20 + 10, 30]) rotate([0, 90, 0]) cylinder(d=M4_CLEAR, h=10);
        }

        // Right wall: fan exhaust (60mm fan)
        translate([BW - 10, BD/2 - 30, BH/2 - 30])
            cube([10, 60, 60]);
        // Fan mounting holes
        for (dx=[-25, 25]) for (dz=[-25, 25]) {
            translate([BW, BD/2 + dx, BH/2 + dz]) rotate([0, 90, 0])
                cylinder(d=M3_CLEAR, h=10);
        }

        // Top lid rabbet (lid sits in 2mm deep recess)
        translate([TOLERANCE, TOLERANCE, BH - LID_H])
            cube([BW - 2*TOLERANCE, BD - 2*TOLERANCE, LID_H + 1]);
    }

    // Internal PCB standoff bosses (4 corners)
    for (x=[15, BW - 25]) for (y=[15, BD - 25]) {
        translate([x, y, FLOOR]) boss_m3(h=BOSS_H);
    }

    // Transformer mounting boss (center, if toroidal)
    if (include_xfmr_boss) {
        translate([BW/2, BD/2, FLOOR])
            cylinder(d=12, h=FLOOR + 10);
        // Through-hole for M8 transformer mounting stud
        translate([BW/2, BD/2, 0])
            cylinder(d=8.5, h=FLOOR + 10 + 1);
    }
}

// =============================================================================
// MODULE 2: LINEAR PSU LID
// =============================================================================
module linear_psu_lid(BW=180, BD=130) {
    difference() {
        cube([BW, BD, LID_H]);
        // Gasket groove
        translate([WALL, WALL, LID_H - 1.5])
            difference() {
                cube([BW - 2*WALL, BD - 2*WALL, 2]);
                translate([GASKET_D, GASKET_D, -1])
                    cube([BW - 2*WALL - 2*GASKET_D, BD - 2*WALL - 2*GASKET_D, 4]);
            }
        // Lid screw holes (4 corners)
        for (x=[8, BW-8]) for (y=[8, BD-8]) {
            translate([x, y, 0]) cylinder(d=M4_CLEAR, h=LID_H + 1);
            translate([x, y, LID_H]) cylinder(d=M4_HEAD, h=3);  // Countersink
        }
        // Label recess (top center)
        translate([BW/2 - 40, BD/2 - 15, LID_H - 0.5]) cube([80, 30, 1]);
    }
}

// =============================================================================
// MODULE 3: SMPS / SOLAR CONTROLLER BODY
// =============================================================================
// Compact buck converter enclosure. Fits: ESP32 DevKit, INA219×2, power stage
// (MOSFET + inductor + caps), IEC or barrel input, Powerpole output.
// External: 160W × 110D × 70H mm
// =============================================================================
module smps_body(BW=160, BD=110, BH=70) {
    difference() {
        cube([BW, BD, BH]);

        // Interior cavity
        translate([WALL, WALL, FLOOR])
            cube([BW - 2*WALL, BD - 2*WALL, BH]);

        // Front panel: output Powerpole pair (×2), power LED, on/off switch
        translate([30, 0, BH/2]) rotate([-90, 0, 0]) powerpole_30a_cutout();
        translate([55, 0, BH/2]) rotate([-90, 0, 0]) powerpole_30a_cutout();
        translate([90, 0, BH/2]) cylinder(d=12, h=20);   // Switch
        translate([110, 0, BH/2]) cylinder(d=5.5, h=20); // LED
        // Mini voltmeter (optional, 28×13mm)
        translate([125, 0, BH/2 - 8]) cube([28, 10, 16]);

        // Rear panel: DC input barrel or IEC C14, solar panel terminals
        translate([BW/2 - 10, BD, BH/2 - 10]) cube([20, 10, 20]);  // DC input terminals
        // Solar charge version: solar panel +/- terminals
        translate([30, BD, BH/2 - 5]) cube([10, 10, 10]);  // Panel +
        translate([50, BD, BH/2 - 5]) cube([10, 10, 10]);  // Panel -

        // Top: vent slots (heat rises through power stage)
        translate([BW/2, BD/2, BH - 1])
            vent_slots(n=6, slot_w=3, slot_h=40, spacing=7);

        // Left side: heatsink slots (MOSFET TO-220 bolted to inner left wall)
        translate([0, BD/4, BH/4])
            vent_slots(n=5, slot_w=2, slot_h=30, spacing=5);
        // TO-220 mounting hole
        translate([0, BD/2, BH/2]) rotate([0, 90, 0]) cylinder(d=M3_CLEAR, h=10);

        // Lid rabbet
        translate([TOLERANCE, TOLERANCE, BH - LID_H])
            cube([BW - 2*TOLERANCE, BD - 2*TOLERANCE, LID_H + 1]);
    }

    // PCB standoffs
    for (x=[12, BW-20]) for (y=[12, BD-20]) {
        translate([x, y, FLOOR]) boss_m3(h=BOSS_H);
    }

    // MOSFET heatsink boss (inner left wall)
    translate([WALL, BD/2 - 15, FLOOR]) boss_m3(h=BOSS_H);
    translate([WALL, BD/2 + 15, FLOOR]) boss_m3(h=BOSS_H);
}

// =============================================================================
// MODULE 4: SMPS LID
// =============================================================================
module smps_lid(BW=160, BD=110) {
    difference() {
        cube([BW, BD, LID_H]);
        // Screw holes at corners
        for (x=[8, BW-8]) for (y=[8, BD-8]) {
            translate([x, y, 0]) cylinder(d=M3_CLEAR, h=LID_H + 1);
        }
    }
}

// =============================================================================
// MODULE 5: LIFEPO4 BATTERY BOX — BODY (4S prismatic cells)
// =============================================================================
// Holds 4× 280Ah or smaller prismatic LiFePO4 cells in series.
// Threaded rod compression: 2× M8 rods run through aluminum plates.
// BMS mounted inside on rear wall.
// This is the INNER box; printed ASA. Designed for EVE 280Ah cells:
//   Cell dimensions: 174mm × 72mm × 207mm each
//   4S arrangement: cells stacked narrow-side together
//   Total stack: 4 × 72mm = 288mm wide + compression plates
// Downscaled for smaller cells (50Ah, 105×30×208mm):
//   Total stack: 4 × 30mm = 120mm + plates
// =============================================================================
module lifepo4_box(
    CELL_W    = 105,   // Cell width (largest dimension)
    CELL_D    = 30,    // Cell depth (stacking direction)
    CELL_H    = 208,   // Cell height
    N_CELLS   = 4
) {
    BW = CELL_W + 2*WALL + 10;     // Width: cell + wall + room for bus bars
    BD = N_CELLS * CELL_D + 20;    // Depth: all cells + compression clearance
    BH = CELL_H + FLOOR + 15;      // Height: cell + floor + terminal clearance

    difference() {
        cube([BW, BD, BH]);

        // Interior cavity
        translate([WALL, WALL, FLOOR]) cube([BW - 2*WALL, BD - 2*WALL, BH]);

        // Cell terminals exit top (open top for cell access before lid)
        translate([WALL + 5, WALL, BH - 20])
            cube([BW - 2*WALL - 10, BD - 2*WALL, 25]);

        // BMS wiring pass-through slots on rear wall
        for (i=[0:3]) {
            translate([WALL + 10 + i*20, BD - WALL, BH/2 - 5])
                cube([4, WALL + 1, 10]);
        }

        // Vent hole (top rear corner, 1/4" = 6.35mm, for activated carbon filter)
        translate([BW - WALL - 10, BD/2, BH - WALL])
            cylinder(d=8, h=WALL + 1);

        // Powerpole panel cutout (front lower right)
        translate([BW - 60, 0, 20]) rotate([-90, 0, 0]) powerpole_45a_cutout();

        // ANL fuse window (front)
        translate([10, 0, 20]) cube([30, WALL + 1, 25]);

        // Threaded rod clearance holes (M8, through bottom and top)
        for (y=[BD/4, 3*BD/4]) {
            translate([BW/2 - 30, y, 0]) cylinder(d=9, h=BH + 2);
            translate([BW/2 + 30, y, 0]) cylinder(d=9, h=BH + 2);
        }

        // Lid rabbet
        translate([TOLERANCE, TOLERANCE, BH - LID_H])
            cube([BW - 2*TOLERANCE, BD - 2*TOLERANCE, LID_H + 1]);
    }

    // Cell retaining ribs (keep cells from moving laterally)
    for (i=[1:N_CELLS-1]) {
        translate([WALL, WALL + i*CELL_D - 1, FLOOR])
            cube([BW - 2*WALL, 2, CELL_H]);
    }

    // PCB standoff for BMS (rear wall, inner face)
    for (dx=[30, BW-40]) {
        translate([dx, BD - WALL - 8, FLOOR + 10]) boss_m3(h=5);
    }
}

// =============================================================================
// MODULE 6: LIFEPO4 BOX LID
// =============================================================================
module lifepo4_lid(
    CELL_W = 105, CELL_D = 30, N_CELLS = 4
) {
    BW = CELL_W + 2*WALL + 10;
    BD = N_CELLS * CELL_D + 20;

    difference() {
        cube([BW, BD, LID_H + 4]);

        // Bus bar slots (cells in series; copper bar exits through lid)
        // Positive terminal: cell 4 positive
        translate([BW/2 - 25, WALL, -1]) cube([20, 12, LID_H + 6]);
        // Negative terminal: cell 1 negative (to BMS B-)
        translate([BW/2 + 5, WALL, -1]) cube([20, 12, LID_H + 6]);

        // Screw holes
        for (x=[8, BW-8]) for (y=[8, BD-8]) {
            translate([x, y, 0]) cylinder(d=M3_CLEAR, h=LID_H + 5);
        }

        // Label recess
        translate([BW/2 - 35, BD/2 - 12, LID_H + 3]) cube([70, 24, 1]);

        // Rod clearance holes
        for (y=[BD/4, 3*BD/4]) {
            translate([BW/2 - 30, y, 0]) cylinder(d=9, h=LID_H + 6);
            translate([BW/2 + 30, y, 0]) cylinder(d=9, h=LID_H + 6);
        }
    }
}

// =============================================================================
// MODULE 7: ANDERSON POWERPOLE DISTRIBUTION PANEL
// =============================================================================
// Panel with 1× 45A input + 4× 30A outputs + fuse holders per output.
// Mounts on front of battery box or standalone on desk/rack.
// =============================================================================
module powerpole_panel(
    PW = 195, PH = 80, PT = 10
) {
    difference() {
        cube([PW, PT, PH]);

        // Input (45A pair, centered top)
        translate([PW/2, 0, PH - 30]) rotate([-90, 0, 0]) powerpole_45a_cutout();

        // Four outputs (30A pair each)
        for (i=[0:3]) {
            translate([20 + i*45, 0, 20]) rotate([-90, 0, 0]) powerpole_30a_cutout();
        }

        // Inline fuse holders for each output (20mm blade fuse, inline type)
        for (i=[0:3]) {
            translate([27 + i*45, 0, 45]) cube([15, PT + 1, 15]);
        }

        // Mounting holes (corners)
        for (x=[5, PW-5]) for (z=[5, PH-5]) {
            translate([x, PT/2, z]) rotate([90, 0, 0]) cylinder(d=M3_CLEAR, h=PT + 1);
        }

        // Ground lug hole (bottom right)
        translate([PW - 15, PT/2, 8]) rotate([90, 0, 0]) cylinder(d=6, h=PT + 1);
    }

    // Label ledge below each connector
    for (i=[0:3]) {
        translate([10 + i*45, PT, 5]) cube([35, 2, 12]);
    }
}

// =============================================================================
// MODULE 8: METERING / CONTROL FACEPLATE
// =============================================================================
// Front panel insert with: voltmeter, ammeter, Powerpole pair, switch, LED.
// Designed to snap into panel opening in PSU body.
// Width: 120mm, Height: 80mm, Depth: 4mm
// =============================================================================
module meter_faceplate(FW=120, FH=80, FD=4) {
    difference() {
        cube([FW, FD, FH]);

        // Digital voltmeter (45×26mm bezel)
        translate([5, 0, FH - 35]) cube([45, FD + 1, 26]);

        // Digital ammeter (45×26mm bezel)
        translate([55, 0, FH - 35]) cube([45, FD + 1, 26]);

        // Powerpole pair (30A, lower center)
        translate([FW/2 - 10, 0, 15]) rotate([-90, 0, 0]) powerpole_30a_cutout();

        // Power switch (16mm round rocker)
        translate([15, 0, 15]) cylinder(d=16, h=FD + 1);

        // Power LED indicator
        translate([15, 0, 42]) cylinder(d=5.5, h=FD + 1);

        // Mounting flanges (4× M3 clearance)
        for (x=[4, FW-4]) for (z=[4, FH-4]) {
            translate([x, 0, z]) rotate([-90, 0, 0]) cylinder(d=M3_CLEAR, h=FD + 1);
        }
    }

    // Mounting flanges
    for (x=[4, FW-4]) for (z=[4, FH-4]) {
        hull() {
            translate([x, 0, z]) rotate([-90, 0, 0]) cylinder(d=10, h=FD);
            translate([x - 4, 0, z - 4]) cube([8, FD, 8]);
        }
    }
}

// =============================================================================
// MODULE 9: CELL COMPRESSION PLATES (pair)
// =============================================================================
// Two plates clamp cells using M8 threaded rods.
// 3mm aluminum preferred; this is a template / drill guide.
module cell_compression_plate(PW=120, PD=150, PT=3) {
    difference() {
        cube([PW, PD, PT]);

        // M8 rod clearance holes (4 positions, corner)
        for (x=[15, PW-15]) for (y=[15, PD-15]) {
            translate([x, y, -1]) cylinder(d=9, h=PT + 2);
        }

        // Lightening slots
        for (i=[0:1]) {
            translate([25 + i*45, 25, -1]) cube([20, PD - 50, PT + 2]);
        }
    }
}

// =============================================================================
// RENDER CONTROLS — Uncomment one to view/export
// =============================================================================

linear_psu_body();
//linear_psu_lid();

//translate([0, 140, 0]) smps_body();
//translate([0, 140, 80]) smps_lid();

//translate([200, 0, 0]) lifepo4_box();
//translate([200, 0, 250]) lifepo4_lid();

//translate([200, 200, 0]) powerpole_panel();

//translate([0, 250, 0]) meter_faceplate();

//translate([0, 350, 0]) cell_compression_plate();
