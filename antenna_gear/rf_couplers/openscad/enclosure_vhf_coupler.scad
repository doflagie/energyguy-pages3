/*
 * enclosure_vhf_coupler.scad
 * TM-COUP-001 Rev A — VHF/UHF Directional Coupler Enclosures
 *
 * Enclosures for:
 *   Module 1: SMA in-line coupler body (PCB microstrip, 50–500 MHz)
 *   Module 2: Coaxial sleeve coupler housing (50–1300 MHz, field-built)
 *   Module 3: UHF/SHF microstrip coupler (23cm / 1296 MHz)
 *   Module 4: Quad-band coupler stack (6m+2m+70cm+23cm in one enclosure)
 *   Module 5: SMA bulkhead adapter plate (panel/rack mount for SMA connectors)
 *   Module 6: PCB alignment jig (holds PCB during soldering)
 *
 * Connector notes:
 *   SMA female chassis: 6.5mm bore, 2× M2.5 on 10.5mm centers
 *   SMA edge-launch PCB: no holes needed (PCB edge mount)
 *   N-type chassis: 16mm bore, 4× M3 on 25mm BCD
 *
 * Material: PETG (up to 500 MHz); ABS or Nylon12 above 500 MHz
 *   (PETG εr ≈ 3.0; minimal effect on external connectors/air-gap path)
 * Units: mm
 */

$fn = 60;

WALL  = 2.5;     // Thinner walls for VHF (reduced dielectric loading)
LID_H = 3.5;
TOL   = 0.25;
M2P5  = 2.7;
M3_C  = 3.4;
M3_I  = 4.5;
M4_C  = 4.4;
BOSS  = 6.0;    // Smaller PCB standoffs for VHF boards

// ─── Connector cutouts ────────────────────────────────────────────────────────

// SMA female chassis: 6.5mm bore + 2× M2.5 on ±5.25mm
module sma_cutout(horiz=true) {
    cylinder(d=6.5, h=20);
    if (horiz) {
        for (x=[-5.25, 5.25]) translate([x,0,0]) cylinder(d=M2P5, h=20);
    } else {
        for (y=[-5.25, 5.25]) translate([0,y,0]) cylinder(d=M2P5, h=20);
    }
}

// N-type: 16mm bore + 4× M3 on 25mm BCD
module n_cutout() {
    cylinder(d=16.0, h=20);
    for (i=[0:3]) rotate([0,0,i*90+45])
        translate([12.5, 0, 0]) cylinder(d=M3_C, h=20);
}

// BNC: 12.4mm bore + 2× M3 on 20mm centers
module bnc_cutout() {
    cylinder(d=12.4, h=20);
    for (x=[-10, 10]) translate([x,0,0]) cylinder(d=M3_C, h=20);
}

// PCB mounting boss (no heat insert for VHF — use M3 self-tapping)
module pcb_boss_vhf(h=5) {
    difference() {
        cylinder(d=BOSS, h=h);
        cylinder(d=2.8, h=h);    // M3 self-tapping pilot hole
    }
}

// ─── Ground plane stitch post (electrically bonds lid to base ground plane) ──
module gnd_stitch(h=4) {
    cylinder(d=3, h=h);    // Print solid; lid applies force contact to PCB GND
}

// =============================================================================
// MODULE 1: SMA IN-LINE PCB COUPLER (50–500 MHz)
// =============================================================================
// Holds a 50mm × 30mm PCB with SMA edge-launch connectors.
// Four SMA chassis connectors: IN, THROUGH, COUPLED, ISOLATED
// Lid uses four M3 screws + ground-plane stitching posts around PCB.
// Internal dimensions: 54mm × 34mm × 22mm (1mm clearance each side of PCB)
// =============================================================================
module sma_pcb_coupler(BW=60, BD=40, BH=30) {
    PCB_W = 50; PCB_D = 30;
    PCB_T = 1.6;  // FR4 thickness

    difference() {
        cube([BW, BD, BH]);

        // Interior (PCB sits on bosses, 5mm above floor)
        translate([WALL, WALL, WALL+2]) cube([BW-2*WALL, BD-2*WALL, BH]);

        // SMA connectors on all 4 sides (1 each)
        // Front (IN)
        translate([BW/2, 0, BH*0.4]) rotate([-90,0,0]) sma_cutout();
        // Rear (THROUGH)
        translate([BW/2, BD, BH*0.4]) rotate([90,0,0]) sma_cutout();
        // Right (COUPLED)
        translate([BW, BD/2, BH*0.4]) rotate([0,90,0]) sma_cutout(horiz=false);
        // Left (ISOLATED)
        translate([0, BD/2, BH*0.4]) rotate([0,-90,0]) sma_cutout(horiz=false);

        // Lid rabbet
        translate([TOL, TOL, BH-LID_H]) cube([BW-2*TOL, BD-2*TOL, LID_H+1]);

        // Lid M3 screws
        for (x=[5, BW-5]) for (y=[5, BD-5]) {
            translate([x, y, BH-LID_H-1]) cylinder(d=M3_C, h=LID_H+3);
        }

        // Cable routing channels (allow RG-316 exit if using coax launchers)
        translate([BW/2-3, -1, 8]) cube([6, WALL+2, 6]);
        translate([BW/2-3, BD-WALL-1, 8]) cube([6, WALL+2, 6]);
    }

    // PCB bosses (4×, 1mm inset from PCB edge)
    for (x=[WALL+2, BW-WALL-8]) for (y=[WALL+2, BD-WALL-8]) {
        translate([x, y, WALL+2]) pcb_boss_vhf(5);
    }

    // Ground stitching posts (contact PCB ground plane around perimeter)
    for (x=[BW*0.25, BW*0.5, BW*0.75]) {
        translate([x, WALL+1, WALL+2+5+PCB_T]) gnd_stitch(3);
        translate([x, BD-WALL-4, WALL+2+5+PCB_T]) gnd_stitch(3);
    }
    for (y=[BD*0.33, BD*0.66]) {
        translate([WALL+1, y, WALL+2+5+PCB_T]) gnd_stitch(3);
        translate([BW-WALL-4, y, WALL+2+5+PCB_T]) gnd_stitch(3);
    }
}

module sma_pcb_lid(BW=60, BD=40) {
    difference() {
        cube([BW, BD, LID_H]);
        for (x=[5, BW-5]) for (y=[5, BD-5]) {
            translate([x, y, 0]) cylinder(d=M3_C, h=LID_H+1);
        }
        // Label recess
        translate([BW/2-12, BD/2-5, LID_H-0.4]) cube([24, 10, 0.5]);
    }

    // Ground stitching contact pads (match base posts; solid blocks)
    for (x=[BW*0.25, BW*0.5, BW*0.75]) {
        translate([x-1.5, WALL+1-1.5, LID_H]) cube([3, 3, 2]);
        translate([x-1.5, BD-WALL-4-1.5, LID_H]) cube([3, 3, 2]);
    }
}

// =============================================================================
// MODULE 2: COAXIAL SLEEVE COUPLER HOUSING (50–1300 MHz)
// =============================================================================
// Cylindrical housing around coaxial-construction coupler.
// Inner bore: 12mm (fits 10mm OD outer conductor + clearance).
// Four SMA chassis ports: 1× IN/OUT inline, 2× FWD/REF radial.
// =============================================================================
module coax_sleeve_housing(
    body_l  = 60,    // Total housing length
    bore_d  = 12,    // Inner bore (fits coaxial coupler assembly)
    body_od = 32     // Outer diameter of housing
) {
    difference() {
        union() {
            // Main cylindrical body
            cylinder(d=body_od, h=body_l);

            // End flanges (for SMA connector mounting)
            translate([0, 0, 0])         cylinder(d=body_od+6, h=8);
            translate([0, 0, body_l-8])  cylinder(d=body_od+6, h=8);
        }

        // Through bore (coaxial assembly inserts here)
        cylinder(d=bore_d, h=body_l+2);

        // End SMA connections (IN and THROUGH, axial)
        // These connect to inner conductor via short SMA jack at each end
        translate([0, 0, -1]) cylinder(d=bore_d-1, h=8+2);
        translate([0, 0, body_l-8-1]) cylinder(d=bore_d-1, h=8+2);

        // Radial SMA ports (FWD and REF), 90° apart
        rotate([0, 90, 0]) translate([0, 0, -body_od/2-5])
            sma_cutout(horiz=false);  // FWD port (radial)
        rotate([0, 90, 90]) translate([0, 0, -body_od/2-5])
            sma_cutout(horiz=false);  // REF port (radial, 90° from FWD)

        // Longitudinal slot for coupling wire routing (top of bore)
        translate([-1, bore_d/2, body_l*0.1])
            cube([2, bore_d, body_l*0.8]);

        // Alignment flat (prevents rotation in outer enclosure)
        translate([body_od/2-2, -(bore_d), body_l*0.1])
            cube([4, bore_d*2, body_l*0.8]);

        // End-cap retention M3 holes (3× per end, 120° spacing)
        for (end=[2, body_l-2]) {
            for (i=[0:2]) rotate([0,0,i*120]) {
                translate([body_od/2+1, 0, end])
                    rotate([0,90,0]) cylinder(d=M3_C, h=5);
            }
        }
    }
}

// End cap for coaxial sleeve housing
module coax_end_cap(bore_d=12, body_od=32) {
    difference() {
        cylinder(d=body_od+6, h=8);
        translate([0,0,-1]) cylinder(d=bore_d, h=10);  // RF through-bore
        // M3 retention holes
        for (i=[0:2]) rotate([0,0,i*120]) {
            translate([body_od/2+1, 0, 4]) rotate([0,90,0]) cylinder(d=M3_C, h=6);
        }
    }
}

// =============================================================================
// MODULE 3: 23CM / 1296 MHz MICROSTRIP COUPLER (SMALL PCB)
// =============================================================================
// Very compact: 35mm × 25mm × 22mm
// PCB size: 30mm × 20mm
// SMA edge-launch connectors on two opposite walls
// Full wraparound ground contact (critical at 1296 MHz)
// =============================================================================
module uhf_1296_coupler(BW=38, BD=28, BH=22) {
    PCB_H = 10;   // PCB sits at this height

    difference() {
        cube([BW, BD, BH]);

        // Interior (keep walls thin for minimal dielectric)
        translate([WALL, WALL, WALL]) cube([BW-2*WALL, BD-2*WALL, BH]);

        // SMA ports: IN (front), THROUGH (rear), COUPLED (right), ISOLATED (left)
        translate([BW/2, 0, PCB_H]) rotate([-90,0,0]) sma_cutout();
        translate([BW/2, BD, PCB_H]) rotate([90,0,0]) sma_cutout();
        translate([BW, BD/2, PCB_H]) rotate([0,90,0]) sma_cutout(horiz=false);
        translate([0, BD/2, PCB_H]) rotate([0,-90,0]) sma_cutout(horiz=false);

        // Lid slot
        translate([TOL, TOL, BH-LID_H]) cube([BW-2*TOL, BD-2*TOL, LID_H+1]);

        // Lid M3 screws (small enclosure: 2× only)
        for (x=[5, BW-5]) {
            translate([x, BD/2, BH-LID_H-1]) cylinder(d=M3_C, h=LID_H+3);
        }
    }

    // PCB ledge (PCB rests on this shelf)
    translate([WALL, WALL, PCB_H-PCB_H]) {
        // Ledge 1.0mm wide, 1.6mm tall (PCB sits on top)
        difference() {
            cube([BW-2*WALL, BD-2*WALL, PCB_H]);
            translate([1, 1, -1]) cube([BW-2*WALL-2, BD-2*WALL-2, PCB_H+2]);
        }
    }
}

module uhf_1296_lid(BW=38, BD=28) {
    difference() {
        cube([BW, BD, LID_H]);
        for (x=[5, BW-5]) translate([x, BD/2, 0]) cylinder(d=M3_C, h=LID_H+1);
        // Vent slot (not needed electrically but aids temperature at 1296 MHz PA)
        translate([BW/2-8, BD/2-10, LID_H-0.5]) cube([16, 20, 1]);
    }
}

// =============================================================================
// MODULE 4: QUAD-BAND COUPLER STACK ENCLOSURE
// =============================================================================
// Stacks four PCB coupler boards (6m, 2m, 70cm, 23cm) in one housing.
// Front: 4× SMA IN, 4× SMA THROUGH (two columns)
// Rear:  4× SMA FWD, 4× SMA REF
// Selector switch / jumpers choose which PCB is active.
// =============================================================================
module quad_band_stack(BW=80, BD=70, BH=120) {
    N_BANDS = 4;
    BAND_PITCH = (BH - 2*WALL) / N_BANDS;

    difference() {
        cube([BW, BD, BH]);
        translate([WALL, WALL, WALL]) cube([BW-2*WALL, BD-2*WALL, BH]);

        // Front: 8× SMA (IN + THROUGH × 4 bands)
        for (i=[0:N_BANDS-1]) {
            z = WALL + BAND_PITCH/2 + i*BAND_PITCH;
            translate([BW/2-12, 0, z]) rotate([-90,0,0]) sma_cutout();  // IN
            translate([BW/2+12, 0, z]) rotate([-90,0,0]) sma_cutout();  // THROUGH
        }

        // Rear: 8× SMA (FWD + REF × 4 bands)
        for (i=[0:N_BANDS-1]) {
            z = WALL + BAND_PITCH/2 + i*BAND_PITCH;
            translate([BW/2-12, BD, z]) rotate([90,0,0]) sma_cutout();  // FWD
            translate([BW/2+12, BD, z]) rotate([90,0,0]) sma_cutout();  // REF
        }

        // Top: USB-C port (ESP32 programming)
        translate([BW-20, BD/2-5, BH-WALL-1]) cube([12, 10, WALL+2]);

        // Band label windows (left side)
        for (i=[0:N_BANDS-1]) {
            z = WALL + BAND_PITCH/2 + i*BAND_PITCH - 6;
            translate([-1, BD/2-8, z]) cube([WALL+2, 16, 12]);
        }

        // Lid slot (top)
        translate([TOL, TOL, BH-LID_H]) cube([BW-2*TOL, BD-2*TOL, LID_H+1]);
        for (x=[6, BW-6]) for (y=[6, BD-6]) {
            translate([x, y, BH-LID_H-1]) cylinder(d=M3_C, h=LID_H+3);
        }
    }

    // PCB shelves (each band PCB rests on ledge)
    for (i=[0:N_BANDS-1]) {
        z = WALL + i*BAND_PITCH;
        translate([WALL, WALL, z]) {
            difference() {
                cube([BW-2*WALL, BD-2*WALL, 2]);
                translate([3, 3, -1]) cube([BW-2*WALL-6, BD-2*WALL-6, 4]);
            }
        }
    }

    // PCB standoffs (2 per PCB × 4 = 8 total)
    for (i=[0:N_BANDS-1]) {
        z = WALL + i*BAND_PITCH + 2;
        for (x=[BW*0.25, BW*0.75]) for (y=[BD*0.25, BD*0.75]) {
            translate([x, y, z]) pcb_boss_vhf(5);
        }
    }
}

// =============================================================================
// MODULE 5: SMA BULKHEAD ADAPTER PLATE
// =============================================================================
// Panel plate for mounting SMA connectors on enclosure wall or rack panel.
// 2× 4× 6× configurations.
// =============================================================================
module sma_bulkhead_plate(n_conn=4, cols=2, plate_t=3) {
    rows = ceil(n_conn / cols);
    PW = cols * 20 + 10;
    PH = rows * 20 + 10;

    difference() {
        cube([PW, plate_t, PH]);

        // SMA holes
        for (c=[0:cols-1]) for (r=[0:rows-1]) {
            if (c*rows + r < n_conn) {
                translate([10 + c*20, -1, 10 + r*20])
                    rotate([-90,0,0]) sma_cutout();
            }
        }

        // Mounting holes (M3, corners)
        for (x=[4, PW-4]) for (z=[4, PH-4]) {
            translate([x, -1, z]) rotate([-90,0,0]) cylinder(d=M3_C, h=plate_t+2);
        }
    }
}

// =============================================================================
// MODULE 6: PCB ALIGNMENT JIG FOR SOLDERING
// =============================================================================
// Holds a PCB in correct orientation for soldering SMA edge-launch connectors.
// Ensures connectors are flush and co-planar before reflow/hand soldering.
// =============================================================================
module pcb_solder_jig(pcb_w=50, pcb_d=30, pcb_t=1.6) {
    jig_h = 10;
    RAIL  = 2.0;    // PCB retention rail width

    difference() {
        cube([pcb_w + 2*RAIL + 6, pcb_d + 2*RAIL + 6, jig_h]);

        // PCB pocket (exact fit + 0.2mm clearance)
        translate([RAIL+3, RAIL+3, jig_h-pcb_t-TOL])
            cube([pcb_w + TOL, pcb_d + TOL, pcb_t + TOL + 2]);

        // Vacuum pick clearance (allows suction cup access to PCB top)
        translate([pcb_w/2 + RAIL + 3 - 8, pcb_d/2 + RAIL + 3 - 8, jig_h-pcb_t-8])
            cube([16, 16, 10]);

        // Connector alignment slots (front and rear edges)
        for (x=[pcb_w*0.3, pcb_w*0.7]) {
            translate([RAIL+3+x-3, -1, jig_h-pcb_t-4])
                cube([6, RAIL+4, pcb_t+5]);
            translate([RAIL+3+x-3, pcb_d+RAIL+3-1, jig_h-pcb_t-4])
                cube([6, RAIL+4, pcb_t+5]);
        }
    }
}

// =============================================================================
// RENDER CONTROLS
// =============================================================================

sma_pcb_coupler();
// translate([70, 0, 0]) sma_pcb_lid();
// translate([0, 60, 0]) coax_sleeve_housing();
// translate([0, 130, 0]) { coax_end_cap(); translate([40,0,0]) coax_end_cap(); }
// translate([100, 60, 0]) uhf_1296_coupler();
// translate([150, 60, 0]) uhf_1296_lid();
// translate([0, 170, 0]) quad_band_stack();
// translate([0, 300, 0]) sma_bulkhead_plate(n_conn=4, cols=2);
// translate([0, 340, 0]) pcb_solder_jig(pcb_w=50, pcb_d=30);
