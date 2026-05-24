/*
 * enclosure_choke_hf.scad
 * Weatherproof Enclosure for HF Toroidal Common-Mode Choke
 * TM-CMC-001 Rev A
 * Material: ASA (UV-stable, outdoor rated) — NEVER PLA outdoors
 * Print bed: 200 × 200 mm; two parts (body + lid); each fits 200×200
 *
 * Fits:
 *   Single FT-240 core (61mm OD) with 10t RG-8X (total coil ~95mm OD)
 *   Double stacked FT-240 (two cores, same coil OD)
 *   10–12 turns RG-8X (8.4mm cable OD)
 *
 * Outer dimensions: 130 × 130 × 80 mm (body) + 15mm lid = 95mm total
 * Coax entry: two 21mm weatherproof ports (N-connector or UHF SO-239)
 * Mounting: 4× M6 bolts on bottom flange (U-bolt mast clamp OR flat surface)
 * Drain: 3mm drain hole at bottom of body
 * Seal: silicone bead in lid groove + 3mm O-ring compressed by lid screws
 *
 * Print orientation:
 *   Body: print open-side UP; no support required
 *   Lid:  print face DOWN (flat side down); no support required
 */

$fn = 48;

// ── Parameters ─────────────────────────────────────────────────────────────────
BOX_L  = 130;
BOX_W  = 130;
BOX_H  = 80;
WALL   = 3.5;    // thicker wall for UV stability and impact resistance
CORN   = 8.0;
LID_H  = 15;

COAX_PORT_D   = 21.0;  // clearance for UHF SO-239 panel mount (20.8mm hex)
N_PORT_D      = 22.5;  // clearance for N-connector panel flange (22.4mm)
PORT_OFFSET_Z = 25;    // height of port centers above box floor

D_M4    = 4.5;   // M4 clearance
D_M4_HS = 6.5;   // M4 heat-set insert bore
D_M6    = 6.8;   // M6 clearance (mounting bolts)
D_DRAIN = 3.5;   // drain hole

ORING_D = 122;   // O-ring groove center diameter
ORING_W = 3.5;   // O-ring cross-section × 1.2 (compressed)
ORING_H = 2.5;   // groove depth

// ── Utilities ──────────────────────────────────────────────────────────────────
module rounded_box(l, w, h, r, fn=48) {
    hull() for (x=[r, l-r]) for (y=[r, w-r])
        translate([x, y, 0]) cylinder(r=r, h=h, $fn=fn);
}

module rounded_shell(l, w, h, r, wall) {
    difference() {
        rounded_box(l, w, h, r);
        translate([wall, wall, wall])
            rounded_box(l-2*wall, w-2*wall, h-wall+0.1, r-wall);
    }
}

// ── Body ───────────────────────────────────────────────────────────────────────
module choke_body() {
    difference() {
        union() {
            // Main box shell
            rounded_shell(BOX_L, BOX_W, BOX_H, CORN, WALL);

            // Mounting flange (bottom, 10mm outboard)
            translate([-10, -10, 0])
                difference() {
                    rounded_box(BOX_L+20, BOX_W+20, WALL, CORN+5);
                    translate([10, 10, -0.1]) rounded_box(BOX_L, BOX_W, WALL+0.2, CORN);
                }

            // Lid seal groove bosses (raised rim at top of box wall)
            translate([0, 0, BOX_H-ORING_H-1])
                difference() {
                    rounded_box(BOX_L, BOX_W, ORING_H+1, CORN);
                    // O-ring groove
                    translate([WALL-1, WALL-1, 0.5])
                        rounded_box(BOX_L-2*WALL+2, BOX_W-2*WALL+2, ORING_H+1, CORN-WALL);
                    // Interior relief
                    translate([WALL+ORING_W, WALL+ORING_W, -0.1])
                        rounded_box(BOX_L-2*(WALL+ORING_W), BOX_W-2*(WALL+ORING_W),
                                    ORING_H+1.5, CORN-WALL-ORING_W);
                }
        }

        // ── Coax ports (sides) ─────────────────────────────────────────────
        // Left side (y=0): IN port
        translate([BOX_L/2, -0.1, PORT_OFFSET_Z])
            rotate([-90, 0, 0])
            cylinder(d=COAX_PORT_D, h=WALL+0.2);

        // Right side (y=BOX_W): OUT port
        translate([BOX_L/2, BOX_W-WALL-0.1, PORT_OFFSET_Z])
            rotate([-90, 0, 0])
            cylinder(d=COAX_PORT_D, h=WALL+0.2);

        // ── Drain hole (bottom center, offset to avoid standoffs) ──────────
        translate([BOX_L/2, BOX_W/2+20, -0.1])
            cylinder(d=D_DRAIN, h=WALL+0.2);

        // ── Lid screw holes (M4, ×4 corners of box top rim) ───────────────
        for (pos=[[10, 10],[10, BOX_W-10],[BOX_L-10, 10],[BOX_L-10, BOX_W-10]])
            translate([pos[0], pos[1], BOX_H-ORING_H-5])
                cylinder(d=D_M4, h=ORING_H+6);

        // ── Mounting flange M6 holes (×4) ─────────────────────────────────
        for (pos=[[-5,-5],[-5, BOX_W+5],[BOX_L+5,-5],[BOX_L+5, BOX_W+5]])
            translate([pos[0], pos[1], -0.1])
                cylinder(d=D_M6, h=WALL+0.2);
    }

    // ── Ferrite core standoff shelf (raised platform for coil to rest on) ──
    // Prevents core-to-floor contact; allows airflow; elevates above drain level
    translate([WALL+5, WALL+5, WALL])
        difference() {
            cube([BOX_L-2*WALL-10, BOX_W-2*WALL-10, 8]);
            // Core through-hole: 68mm dia (FT-240 core OD + 7mm clearance)
            translate([(BOX_L-2*WALL-10)/2, (BOX_W-2*WALL-10)/2, -0.1])
                cylinder(d=68, h=9);
            // Lightening slots (four quadrant cutouts)
            for (a=[0,90,180,270])
                rotate([0,0,a]) translate([10,-5,-0.1]) cube([40,10,9]);
        }

    // ── M4 heat-set boss inserts for lid ──────────────────────────────────
    for (pos=[[10, 10],[10, BOX_W-10],[BOX_L-10, 10],[BOX_L-10, BOX_W-10]])
        translate([pos[0], pos[1], BOX_H-10])
            difference() {
                cylinder(d=10, h=10);
                cylinder(d=D_M4_HS, h=10);
            }
}

// ── Lid ────────────────────────────────────────────────────────────────────────
module choke_lid() {
    difference() {
        union() {
            // Lid plate
            rounded_box(BOX_L, BOX_W, LID_H, CORN);
            // Engagement lip that presses O-ring in body groove
            translate([WALL-0.5, WALL-0.5, -(LID_H*0.4)])
                rounded_box(BOX_L-2*WALL+1, BOX_W-2*WALL+1, LID_H*0.4+1, CORN-WALL);
        }

        // M4 screw clearance holes
        for (pos=[[10, 10],[10, BOX_W-10],[BOX_L-10, 10],[BOX_L-10, BOX_W-10]])
            translate([pos[0], pos[1], -0.1])
                cylinder(d=D_M4, h=LID_H+0.2);

        // M4 countersink (flush head)
        for (pos=[[10, 10],[10, BOX_W-10],[BOX_L-10, 10],[BOX_L-10, BOX_W-10]])
            translate([pos[0], pos[1], LID_H-4])
                cylinder(d1=D_M4, d2=9, h=4.1);

        // Label recess (top of lid, centered)
        translate([BOX_L/2, BOX_W/2, LID_H-0.8])
            hull() for (x=[-40,40]) for (y=[-15,15])
                translate([x,y,0]) cylinder(r=3, h=0.9);

        // Cable tie pass-through slots (×2, for securing coax to lid)
        for (y=[BOX_W/2-20, BOX_W/2+20])
            translate([BOX_L/2-1, y, -0.1])
                cube([2, 8, LID_H+0.2]);
    }
}

// ── Port collar / weatherproof gland (prints in two halves) ───────────────────
module port_gland(coax_d) {
    // Outer gland nut that compresses a rubber seal around the coax
    GLAND_OD = coax_d + 14;
    GLAND_L  = 20;
    difference() {
        cylinder(d=GLAND_OD, h=GLAND_L);
        // Coax bore (slightly oversized for easy threading)
        translate([0, 0, -0.1]) cylinder(d=coax_d+0.6, h=GLAND_L+0.2);
        // Hex flats (spanner wrench, 24mm AF)
        for (a=[0,60,120]) rotate([0,0,a])
            translate([0, GLAND_OD/2-2.5, GLAND_L-6])
                cube([GLAND_OD, 5, 6.1], center=true);
    }
    // Compression cone (inner piece, press-fits gland seal)
    translate([GLAND_OD+5, 0, 0]) {
        difference() {
            cylinder(d1=GLAND_OD, d2=coax_d+4, h=10);
            translate([0,0,-0.1]) cylinder(d=coax_d+0.6, h=10.2);
        }
    }
}

// ── U-bolt mounting saddle (for 1.5" to 2" mast) ─────────────────────────────
module mast_saddle(mast_d=50) {
    SADDLE_W = 70;  SADDLE_H = 20;  SADDLE_T = 8;
    BOLT_PITCH = 50;
    difference() {
        cube([SADDLE_W, mast_d/2+SADDLE_T, SADDLE_H]);
        // Mast radius cutout
        translate([SADDLE_W/2, mast_d/2+SADDLE_T+0.1, SADDLE_H/2])
            rotate([90,0,0]) cylinder(d=mast_d, h=mast_d/2+SADDLE_T+0.2);
        // M6 U-bolt holes
        for (x=[(SADDLE_W-BOLT_PITCH)/2, (SADDLE_W+BOLT_PITCH)/2])
            translate([x, -0.1, SADDLE_H/2])
                rotate([-90,0,0]) cylinder(d=D_M6, h=SADDLE_T+0.2);
    }
}

// ── Render ─────────────────────────────────────────────────────────────────────
choke_body();
// translate([0, BOX_W+20, LID_H]) rotate([180,0,0]) choke_lid();
// translate([BOX_L+20, 0, 0]) port_gland(COAX_PORT_D);
// translate([BOX_L+20, 50, 0]) mast_saddle(50);
