/*
 * mounting_brackets_psu.scad
 * TM-PWR-001 Rev A — Power Supply Mounting Hardware
 *
 * Hardware for mounting and organizing power supply equipment:
 *   - Rack ears (1U, 2U) for 19" equipment rack panels
 *   - Vehicle / mobile mount (din rail, console bracket)
 *   - Mast clamp (40–60mm round mast)
 *   - Anderson Powerpole cable management
 *   - Cable bundle clamps (supply output leads)
 *   - DIN rail clip (35mm standard DIN rail)
 *   - Desktop feet (non-skid rubber foot bosses)
 *   - Heatsink clip (secures TO-220 to external heatsink fin)
 *
 * Print: PETG or ASA. 0.3mm layer, 40% infill for structural parts.
 */

$fn = 60;

// ─── Standard Dimensions ────────────────────────────────────────────────────
WALL  = 3.0;
M3_C  = 3.4;   // M3 clearance hole
M3_I  = 4.5;   // M3 heat insert OD
M4_C  = 4.4;
M4_I  = 5.6;
M5_C  = 5.4;
M6_C  = 6.4;
M8_C  = 8.5;

RACK_U        = 44.45;  // 1U height (44.45mm)
RACK_W_EAR    = 483.0;  // 19" rack panel width, outer
RACK_HOLE_SEP = 15.875; // Rack unit hole spacing (5/8")
RACK_HOLE_D   = 6.35;   // Standard rack M6 or #10-32 clearance

DIN_W         = 35.0;   // 35mm DIN rail width
DIN_H         = 7.5;    // DIN rail height (TS35)

// =============================================================================
// MODULE 1: RACK EAR (1U, single ear — print two)
// =============================================================================
module rack_ear_1u(ear_depth=80, box_h=RACK_U - 2) {
    EAR_W  = 19.0;    // Ear protrusion from box edge to rack hole
    EAR_H  = RACK_U;
    EAR_T  = 3.5;     // Ear thickness

    // Horizontal ear plate
    difference() {
        cube([EAR_W + ear_depth, EAR_T, EAR_H]);

        // Rack mounting holes (M6, 3× per U)
        for (i=[0, 1, 2]) {
            translate([EAR_W/2, 0, RACK_U/2 - RACK_HOLE_SEP + i*RACK_HOLE_SEP])
                rotate([-90, 0, 0]) cylinder(d=RACK_HOLE_D, h=EAR_T + 1);
        }

        // Box attachment holes (to PSU enclosure side)
        for (z=[10, box_h - 10]) {
            translate([EAR_W + 15, 0, z])
                rotate([-90, 0, 0]) cylinder(d=M4_C, h=EAR_T + 1);
            translate([EAR_W + ear_depth - 15, 0, z])
                rotate([-90, 0, 0]) cylinder(d=M4_C, h=EAR_T + 1);
        }

        // Label slot (embossed rack unit ID)
        translate([EAR_W + 20, EAR_T - 0.6, EAR_H/2 - 8]) cube([30, 0.8, 16]);
    }

    // Captive nut pockets (M4, rear face)
    for (z=[10, box_h - 10]) {
        translate([EAR_W + 15, EAR_T - 1, z - 4])
            difference() {
                cube([8, 5, 8]);
                translate([4, 0, 4]) rotate([-90, 0, 0]) cylinder(d=M4_C, h=6);
                translate([0.5, 2.5, 0.5]) cube([7, 3.5, 7]);  // Nut pocket
            }
    }
}

// =============================================================================
// MODULE 2: RACK EAR (2U)
// =============================================================================
module rack_ear_2u(ear_depth=100) {
    scale([1, 1, 2]) rack_ear_1u(ear_depth);
}

// =============================================================================
// MODULE 3: VEHICLE / MOBILE MOUNT — CONSOLE BRACKET
// =============================================================================
// L-bracket that attaches PSU to vehicle console or center hump.
// Lower flange bolts to console; upper flange supports PSU base.
// =============================================================================
module vehicle_console_bracket(
    PSU_W = 160, PSU_D = 110,
    OFFSET_H = 80   // Height from console surface to PSU bottom
) {
    BT = 4.0;  // Bracket thickness

    difference() {
        union() {
            // Horizontal upper plate (PSU base attachment)
            cube([PSU_W, PSU_D, BT]);

            // Vertical riser (one side)
            translate([0, 0, -OFFSET_H]) cube([PSU_W, BT, OFFSET_H]);

            // Gusset triangles for rigidity
            for (x=[20, PSU_W - 30]) {
                translate([x, 0, -OFFSET_H]) {
                    linear_extrude(BT) {
                        polygon([[0,0],[0,OFFSET_H],[OFFSET_H*0.4, 0]]);
                    }
                }
            }
        }

        // PSU mounting holes (upper plate, 4 corners)
        for (x=[15, PSU_W-15]) for (y=[15, PSU_D-15]) {
            translate([x, y, -1]) cylinder(d=M4_C, h=BT + 2);
        }

        // Console mounting slots (elongated for adjustment, lower)
        for (x=[30, PSU_W - 50]) {
            translate([x, 0, -OFFSET_H + 10])
                rotate([-90, 0, 0]) hull() {
                    cylinder(d=M5_C, h=BT + 1);
                    translate([30, 0, 0]) cylinder(d=M5_C, h=BT + 1);
                }
        }
    }
}

// =============================================================================
// MODULE 4: MAST CLAMP — ROUND MAST (40–60mm OD)
// =============================================================================
module mast_clamp_psu(
    mast_d    = 50,
    clamp_w   = 50,
    clamp_h   = 40,
    plate_l   = 80
) {
    HALF_GAP = 1.0;  // Gap between clamp halves

    difference() {
        union() {
            // Cylindrical clamp body
            cylinder(d=mast_d + 2*WALL + 8, h=clamp_h);
            // Mounting plate extension
            translate([-(mast_d/2 + WALL + 4 + plate_l - 2), -(WALL + 4), 0])
                cube([plate_l, (mast_d + 2*WALL + 8) / 2 + WALL + 4, clamp_h]);
        }

        // Mast bore
        translate([0, 0, -1]) cylinder(d=mast_d + HALF_GAP*2, h=clamp_h + 2);

        // Clamp split (saw slot)
        translate([-(mast_d + 20), -HALF_GAP, -1]) cube([mast_d + 20, HALF_GAP*2, clamp_h + 2]);

        // Clamping bolt holes (M6, through split sides)
        translate([mast_d/2 + 4, 0, clamp_h/2])
            rotate([0, 90, 0]) cylinder(d=M6_C, h=20);
        translate([-(mast_d/2 + 4), 0, clamp_h/2])
            rotate([0, 90, 0]) {
                cylinder(d=M6_C, h=20);
                translate([0, 0, 12]) cylinder(d=12, h=8);  // Bolt head recess
            }

        // Plate mounting holes (4× M5)
        for (z=[10, clamp_h-10]) {
            for (px=[
                -(mast_d/2 + WALL + 4 + plate_l - 20),
                -(mast_d/2 + WALL + 4 + 20)
            ]) {
                translate([px, 0, z])
                    rotate([0, 90, 0]) cylinder(d=M5_C, h=plate_l);
            }
        }
    }
}

// =============================================================================
// MODULE 5: DIN RAIL CLIP (35mm TS35)
// =============================================================================
module din_rail_clip(body_w=40, body_h=50, body_d=20) {
    DIN_HOOK_T = 2.5;
    DIN_HOOK_D = 12.0;

    difference() {
        union() {
            // Mounting body
            cube([body_w, body_d, body_h]);

            // Fixed hook (top)
            translate([0, 0, body_h - DIN_HOOK_T - DIN_H])
                cube([body_w, DIN_HOOK_D, DIN_HOOK_T]);
            translate([0, DIN_HOOK_D - DIN_HOOK_T, body_h - DIN_HOOK_T - DIN_H])
                cube([body_w, DIN_HOOK_T, DIN_H + DIN_HOOK_T]);
        }

        // Snap latch slot (bottom — spring clip snaps over DIN lower edge)
        translate([-1, 0, 5]) cube([body_w + 2, DIN_HOOK_D, DIN_HOOK_T + 1]);

        // Device mounting holes (2× M4)
        for (x=[body_w/4, 3*body_w/4]) {
            translate([x, body_d/2, -1]) cylinder(d=M4_C, h=body_h + 2);
        }
    }

    // Snap tongue (flexible; bottom; clips under DIN lower edge)
    translate([5, 0, 0]) {
        difference() {
            cube([body_w - 10, DIN_HOOK_D, DIN_HOOK_T + 2]);
            translate([2, -1, -1]) cube([body_w - 14, DIN_HOOK_D + 2, DIN_HOOK_T + 4]);
        }
    }
}

// =============================================================================
// MODULE 6: ANDERSON POWERPOLE CABLE CLAMP / STRAIN RELIEF
// =============================================================================
// Two-piece snap clamp holds multiple Powerpole cables together at exit point.
// =============================================================================
module pp_cable_clamp(n_cables=4, cable_od=8.0) {
    PITCH = cable_od + 3;
    CW = n_cables * PITCH + 6;
    CH = cable_od + 2*WALL;
    CD = 20;
    HH = CH/2;

    // Lower half
    difference() {
        cube([CW, CD, HH]);
        for (i=[0:n_cables-1]) {
            translate([6 + PITCH/2 + i*PITCH, -1, HH])
                rotate([-90, 0, 0]) cylinder(d=cable_od, h=CD + 2);
        }
        // Screw holes for M3
        for (x=[4, CW-4]) {
            translate([x, CD/2, -1]) cylinder(d=M3_C, h=HH + 2);
        }
    }

    // Upper half (translated for separate print/STL)
    translate([CW + 5, 0, 0]) {
        difference() {
            cube([CW, CD, HH + 2]);  // Slightly taller for snap-over
            for (i=[0:n_cables-1]) {
                translate([6 + PITCH/2 + i*PITCH, -1, 0])
                    rotate([-90, 0, 0]) cylinder(d=cable_od, h=CD + 2);
            }
            for (x=[4, CW-4]) {
                translate([x, CD/2, -1]) {
                    cylinder(d=M3_C, h=HH + 4);
                    translate([0, 0, HH + 1]) cylinder(d=6.5, h=3);  // Head recess
                }
            }
        }
    }
}

// =============================================================================
// MODULE 7: DESKTOP FEET (4 per supply unit)
// =============================================================================
module desktop_foot(foot_d=18, foot_h=8) {
    difference() {
        // Outer rubber-pad holder
        cylinder(d=foot_d, h=foot_h);

        // Central M4 through-hole for mounting screw
        cylinder(d=M4_C, h=foot_h + 1);

        // Rubber pad recess (bottom, 3mm deep)
        translate([0, 0, -1]) cylinder(d=foot_d - 4, h=4);
    }

    // Rubber pad guidance lip
    translate([0, 0, 1]) difference() {
        cylinder(d=foot_d - 3, h=2);
        cylinder(d=foot_d - 7, h=3);
    }
}

// =============================================================================
// MODULE 8: TO-220 HEATSINK INSULATOR CLIP
// =============================================================================
// Clips over TO-220 tab; provides mechanical alignment and insulation.
// Works with mica or Kapton insulator pad between device and heatsink.
// =============================================================================
module to220_clip(heatsink_screw_spacing=25) {
    BW = 15; BD = 8; BH = 18;
    difference() {
        cube([BW, BD, BH]);

        // TO-220 tab slot (10.16mm wide, 1.6mm thick)
        translate([BW/2 - 5.08, BD - 3, -1])
            cube([10.16, 4, 8]);

        // Body recess (package body below tab)
        translate([BW/2 - 4, BD - 6, 7])
            cube([8, 7, BH]);

        // Mounting screw clearance hole
        translate([BW/2, BD/2, 14])
            cylinder(d=M3_C, h=BH);
    }
}

// =============================================================================
// MODULE 9: WIRE LABEL CLIPS (snap onto #12 or #10 AWG wire)
// =============================================================================
module wire_label(wire_od=3.3, label_w=25, label_h=12) {
    LT = 1.5;  // Label thickness
    GAP = 0.5; // Snap gap

    // Label face
    cube([label_w, LT, label_h]);

    // Snap clip around wire
    translate([label_w/2, LT, label_h/2]) {
        difference() {
            cylinder(d=wire_od + 4, h=8, center=true);
            cylinder(d=wire_od + GAP*2, h=10, center=true);
            // Opening for snap-on
            translate([-(wire_od + 4), -1, -5]) cube([wire_od + 4, wire_od + 6, 10]);
        }
    }
}

// =============================================================================
// RENDER CONTROLS — Uncomment to view/export
// =============================================================================

rack_ear_1u();
//translate([30, 0, 0]) rack_ear_2u();

//translate([0, 150, 0]) vehicle_console_bracket();

//translate([200, 0, 0]) mast_clamp_psu(mast_d=50);

//translate([0, 300, 0]) din_rail_clip();

//translate([200, 150, 0]) pp_cable_clamp(n_cables=4);

//for (i=[0:3]) translate([i*25, 400, 0]) desktop_foot();

//translate([0, 500, 0]) to220_clip();

//translate([200, 500, 0]) wire_label(label_w=30);
