/*
 * mounting_brackets_bms.scad
 * TM-BMS-001 Rev A — BMS Mounting Hardware
 *
 * Hardware for mounting and organizing BMS equipment in field deployments:
 *   - BMS controller panel mount (rack / pelican lid)
 *   - Cell holder mounting tray (vehicle / pack box)
 *   - Mast clamp for battery box (50–60mm round mast)
 *   - DIN rail clip for BMS board
 *   - Kelvin shunt mounting block
 *   - Balance resistor ventilated bracket
 *   - Sensor bracket (NTC thermistor, cell surface mount)
 *   - Cable strain relief / terminal label clips
 *   - Pack lid with voltage window (for voltmeter display)
 *
 * Print: PETG or ASA. 3 perimeters, 30% infill, 0.3mm layer.
 * Units: mm
 */

$fn = 60;

// ─── Constants ───────────────────────────────────────────────────────────────
WALL  = 3.0;
M3_C  = 3.4;
M3_I  = 4.5;
M4_C  = 4.4;
M5_C  = 5.4;
M6_C  = 6.4;
M8_C  = 8.5;

// =============================================================================
// MODULE 1: PANEL MOUNT FACEPLATE
// =============================================================================
// Mounts BMS controller enclosure into a Pelican case lid or equipment panel.
// Faceplate is flush; enclosure sits behind the panel.
// =============================================================================
module panel_faceplate(
    PW  = 175,   // Panel cutout width
    PH  = 85,    // Panel cutout height
    FW  = 200,   // Faceplate total width (with flanges)
    FH  = 110,   // Faceplate total height
    FT  = 4.0    // Faceplate thickness
) {
    difference() {
        cube([FW, FT, FH]);

        // Central cutout for display/front panel access
        translate([(FW-PW)/2, -1, (FH-PH)/2]) cube([PW, FT+2, PH]);

        // Mounting flange holes (6× M4, two rows)
        for (x=[10, FW/2, FW-10]) for (z=[8, FH-8]) {
            translate([x, FT/2, z]) rotate([90,0,0]) cylinder(d=M4_C, h=FT+2);
        }

        // Countersink on front face
        for (x=[10, FW/2, FW-10]) for (z=[8, FH-8]) {
            translate([x, -1, z]) rotate([90,0,0]) cylinder(d=8.5, h=3);
        }
    }

    // Alignment tabs (rear face, engage enclosure rim)
    for (side=[-1, 1]) {
        translate([(FW + side*(PW-10))/2, FT, (FH-PH)/2])
            cube([4, 6, PH]);
    }
}

// =============================================================================
// MODULE 2: CELL HOLDER MOUNTING TRAY
// =============================================================================
// Flat tray with retaining lips and anti-skid bosses.
// Accepts 3S2P cell holder; bolts to vehicle console or pack box base.
// =============================================================================
module cell_tray(
    CW = 90,    // Cell holder footprint width
    CD = 80,    // Cell holder footprint depth
    TH = 5.0    // Tray height
) {
    TW = CW + 2*WALL + 6;
    TD = CD + 2*WALL + 6;

    difference() {
        cube([TW, TD, TH]);

        // Cell holder pocket (inset, cell holder drops in)
        translate([WALL+3, WALL+3, TH-2]) cube([CW, CD, TH]);

        // M3 through-holes for securing cell holder
        for (x=[WALL+8, TW-WALL-8]) for (y=[WALL+8, TD-WALL-8]) {
            translate([x, y, -1]) cylinder(d=M3_C, h=TH+2);
        }

        // Tray mounting slots (elongated, for vehicle track or rail)
        for (x=[WALL/2, TW-WALL/2]) {
            translate([x-2, TD/2-12, -1]) cube([4, 24, TH+2]);
        }
    }

    // Retaining lip (prevents cell holder from sliding up)
    for (side=[0, 1]) {
        translate([side*(TW-WALL-3), WALL+3, TH-2])
            cube([WALL+3, CD, 4]);
    }
}

// =============================================================================
// MODULE 3: MAST CLAMP FOR BATTERY BOX
// =============================================================================
// Clamps BMS/battery box assembly to a vertical mast (40–60mm dia)
// Saddle clamp style; two U-bolts or M8 clamping bolts
// =============================================================================
module mast_clamp_bms(
    mast_d  = 50,
    plate_l = 90,    // Length of mounting plate
    plate_w = 80,    // Width of mounting plate
    plate_t = 5.0    // Plate thickness
) {
    CLAMP_H = 50;
    CLAMP_T = WALL + 4;

    difference() {
        union() {
            // Curved saddle to fit mast
            difference() {
                cylinder(d=mast_d + 2*CLAMP_T, h=CLAMP_H);
                cylinder(d=mast_d + 1.0, h=CLAMP_H + 2);  // Mast bore
            }

            // Flat mounting plate on back
            translate([-(plate_l/2), -(mast_d/2 + CLAMP_T), 0])
                cube([plate_l, plate_w, plate_t]);
        }

        // Remove front half of clamp (strap/U-bolt holds it)
        translate([-(mast_d + CLAMP_T*2 + 5), 0, -1])
            cube([mast_d + CLAMP_T*2 + 5, mast_d + CLAMP_T*2 + 5, CLAMP_H + 2]);

        // U-bolt clearance slots
        for (z=[12, CLAMP_H-12]) {
            translate([-(mast_d/2 + CLAMP_T + 2), -(mast_d/2 + CLAMP_T + 2), z-3])
                cube([mast_d + 2*CLAMP_T + 4, 6, 6]);
        }

        // Battery box mounting holes on plate
        for (x=[-plate_l/2+15, plate_l/2-15]) for (y=[10, plate_w-10]) {
            translate([x, -(mast_d/2 + CLAMP_T) + y, -1])
                cylinder(d=M4_C, h=plate_t+2);
        }
    }
}

// =============================================================================
// MODULE 4: DIN RAIL CLIP — 35mm TS35
// =============================================================================
// Snaps onto 35mm DIN rail. Mounts BMS PCB or small module.
// =============================================================================
module din_clip_bms(body_w=50, body_h=40, body_d=18) {
    DIN_HOOK = 2.5;
    DIN_H    = 7.5;

    difference() {
        union() {
            cube([body_w, body_d, body_h]);
            // Fixed hook (top)
            translate([0, 0, body_h - DIN_HOOK - DIN_H])
                cube([body_w, DIN_H + DIN_HOOK, DIN_HOOK]);
            translate([0, DIN_H, body_h - DIN_HOOK - DIN_H])
                cube([body_w, DIN_HOOK, DIN_H + DIN_HOOK]);
        }
        // Device mounting holes
        for (x=[body_w/4, 3*body_w/4]) {
            translate([x, body_d/2, -1]) cylinder(d=M3_C, h=body_h+2);
        }
        // Spring slot (bottom snap)
        translate([5, -1, 5]) cube([body_w-10, DIN_H+2, DIN_HOOK+1]);
    }

    // Bottom snap tongue
    translate([6, 0, 0]) difference() {
        cube([body_w-12, DIN_H, DIN_HOOK+2]);
        translate([2, -1, -1]) cube([body_w-16, DIN_H+2, DIN_HOOK+4]);
    }
}

// =============================================================================
// MODULE 5: KELVIN SHUNT MOUNTING BLOCK
// =============================================================================
// PCB-mountable block that holds a 4-terminal current shunt resistor
// in correct orientation, strain-relieved.
// =============================================================================
module shunt_mount_block(
    SR_L = 25,   // Shunt resistor length (typical 25mm for 2W/5W type)
    SR_W = 8,    // Shunt body width
    SR_H = 4     // Shunt body height
) {
    BW = SR_L + 2*WALL;
    BD = SR_W + 2*WALL;
    BH = SR_H + WALL + 4;

    difference() {
        cube([BW, BD, BH]);
        // Resistor channel
        translate([WALL, WALL, WALL]) cube([SR_L, SR_W, SR_H + 2]);
        // Current terminal access holes (both ends)
        translate([-1, BD/2 - 3, WALL + SR_H/2 - 2]) cube([WALL+2, 6, 4]);
        translate([BW-WALL-1, BD/2 - 3, WALL + SR_H/2 - 2]) cube([WALL+2, 6, 4]);
        // Sense wire exit (bottom)
        for (x=[WALL + SR_L*0.2, WALL + SR_L*0.8]) {
            translate([x-1.5, BD/2, -1]) cube([3, 3, WALL+2]);
        }
        // PCB mounting holes
        for (x=[3, BW-3]) translate([x, BD/2, -1]) cylinder(d=M3_C, h=BH+2);
    }

    // Clamp bar (screws over top of resistor, holds it in channel)
    translate([BW + 5, 0, 0]) {
        difference() {
            cube([BW, BD, 4]);
            for (x=[3, BW-3]) translate([x, BD/2, -1]) cylinder(d=M3_C, h=6);
        }
    }
}

// =============================================================================
// MODULE 6: NTC THERMISTOR CELL SURFACE CLIP
// =============================================================================
// Spring clip holds NTC flat against prismatic cell surface.
// Retains sensor without adhesive for easy replacement.
// =============================================================================
module ntc_cell_clip(cell_w=105, clip_h=12) {
    CT = 1.5;   // Clip thickness

    // U-shaped clip body
    difference() {
        cube([20, cell_w, clip_h]);
        // NTC pocket (6mm × 6mm × 3mm deep recess, centered)
        translate([0, cell_w/2 - 3, clip_h/2 - 1.5]) cube([4, 6, 3]);
        // Lightening
        translate([CT, CT, CT]) cube([20-2*CT, cell_w-2*CT, clip_h-2*CT]);
    }

    // Spring fingers (press against cell)
    for (side=[0, 1]) {
        translate([0, side*(cell_w-CT), 0]) {
            difference() {
                cube([15, CT, clip_h]);
                translate([2, -1, 2]) cube([11, CT+2, clip_h-4]);
            }
        }
    }

    // Wire strain relief notch
    translate([20, cell_w/2-3, 0]) cube([5, 6, CT]);
}

// =============================================================================
// MODULE 7: BALANCE RESISTOR BRACKET — VENTILATED
// =============================================================================
// Open-frame bracket holding 4× 2W axial balance resistors.
// Designed for forced or convection cooling.
// =============================================================================
module balance_bracket(n=4, r_length=25, r_dia=6) {
    PITCH = r_length + 4;
    BW = n * PITCH + 2*WALL;
    BD = r_dia + 2*WALL + 4;
    BH = r_dia + 2*WALL;

    difference() {
        cube([BW, BD, BH]);
        // Resistor bores
        for (i=[0:n-1]) {
            translate([WALL + PITCH/2 + i*PITCH, -1, BH/2])
                rotate([-90,0,0]) cylinder(d=r_dia+0.5, h=BD+2);
        }
        // Airflow slots (sides)
        for (z=[BH*0.3, BH*0.6]) {
            translate([-1, BD/4, z]) cube([BW+2, BD/2, 3]);
        }
        // Terminal wire exits (front and back)
        for (i=[0:n-1]) {
            translate([WALL + PITCH/2 + i*PITCH - 2, -1, 0])
                cube([4, WALL+2, BH/3]);
            translate([WALL + PITCH/2 + i*PITCH - 2, BD-WALL-1, 0])
                cube([4, WALL+2, BH/3]);
        }
        // Mounting holes
        for (x=[5, BW-5]) translate([x, BD/2, -1]) cylinder(d=M3_C, h=BH+2);
    }
}

// =============================================================================
// MODULE 8: BATTERY TERMINAL COVER (insulating)
// =============================================================================
// Snap-on insulating cap for exposed M6 bus bar bolt heads.
// Prevents accidental short circuit from dropped tools.
// =============================================================================
module terminal_cover(bolt_head_d=12, bolt_head_h=6, cap_wall=2.5) {
    CID = bolt_head_d + 0.5;  // Inner diameter (clearance over bolt head)
    COD = CID + 2*cap_wall;
    CH  = bolt_head_h + cap_wall;

    difference() {
        cylinder(d=COD, h=CH);
        translate([0, 0, cap_wall]) cylinder(d=CID, h=CH);
    }

    // Snap ring (inner lip catches below bolt head)
    translate([0, 0, cap_wall + bolt_head_h - 1]) {
        difference() {
            cylinder(d=CID + 2, h=1.5);
            cylinder(d=CID - 2, h=2);
        }
    }

    // Tactile grip ridges (outer)
    for (i=[0:5]) rotate([0, 0, i*60]) {
        translate([COD/2, 0, CH/2]) cube([cap_wall*0.6, 2, CH*0.6], center=true);
    }
}

// =============================================================================
// MODULE 9: PACK LABEL PLATE
// =============================================================================
// Embossed information plate: chemistry, voltage, capacity, warnings.
// Bolts to battery enclosure lid.
// =============================================================================
module pack_label_plate(PW=90, PH=50, PT=2.5) {
    difference() {
        cube([PW, PT, PH]);
        // Mounting holes
        for (x=[5, PW-5]) for (z=[5, PH-5]) {
            translate([x, 0, z]) rotate([-90,0,0]) cylinder(d=M3_C, h=PT+1);
        }
        // Text field recess (emboss from this face)
        translate([8, PT-0.5, 12]) cube([PW-16, 0.6, PH-24]);
    }

    // Embossed warning text (printed as raised letters)
    // "LITHIUM BATTERY" line
    translate([PW/2 - 20, PT, PH-10]) {
        cube([40, 1, 2]);  // Underline bar (placeholder; use inkscape for text)
    }

    // Border ridge
    translate([3, PT, 3]) difference() {
        cube([PW-6, 1, PH-6]);
        translate([2, -1, 2]) cube([PW-10, 3, PH-10]);
    }
}

// =============================================================================
// MODULE 10: PACK CARRY HANDLE
// =============================================================================
// Sturdy top handle for portable battery pack.
// Bolts through lid with M4 × 20mm bolts and fender washers inside.
// =============================================================================
module carry_handle(
    span = 120,   // Distance between mounting posts
    h    = 35,    // Handle loop height above mounting surface
    grip_d = 18   // Grip tube diameter
) {
    GT = grip_d/2;  // Grip tube radius

    // Two mounting posts
    for (x=[0, span]) {
        translate([x, 0, 0]) {
            difference() {
                cylinder(d=grip_d + 4, h=h*0.6);
                translate([0, 0, -1]) cylinder(d=M4_C, h=h*0.6+2);
            }
        }
    }

    // Arch connecting the posts (half-torus)
    translate([span/2, 0, h*0.6]) rotate([90, 0, 0]) {
        rotate_extrude(angle=180) {
            translate([span/2, 0, 0]) circle(d=grip_d);
        }
    }
}

// =============================================================================
// RENDER CONTROLS
// =============================================================================

panel_faceplate();
// translate([0, 120, 0]) cell_tray();
// translate([0, 220, 0]) mast_clamp_bms(mast_d=50);
// translate([0, 340, 0]) din_clip_bms();
// translate([200, 0, 0]) shunt_mount_block();
// translate([200, 80, 0]) ntc_cell_clip();
// translate([200, 120, 0]) balance_bracket();
// translate([200, 200, 0]) { for (i=[0:3]) translate([i*20, 0, 0]) terminal_cover(); }
// translate([200, 300, 0]) pack_label_plate();
// translate([200, 380, 0]) carry_handle();
