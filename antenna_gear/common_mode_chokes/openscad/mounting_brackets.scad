/*
 * mounting_brackets.scad
 * Mounting Brackets for Common-Mode Choke Enclosures
 * TM-CMC-001 Rev A
 * Material: ASA (outdoor) or PETG (indoor only)
 * Print bed: 200 × 200 mm; all parts fit without support
 *
 * Bracket types:
 *   1. U-bolt mast bracket (1"–2.5" mast, adjustable)
 *   2. Flat surface / wall mount bracket with cable routing
 *   3. Tower climbing bracket (clamp-on step bolt style)
 *   4. Coax strain relief clamp (for feedline at choke entry)
 *   5. Ferrite stack retainer clip (keeps stacked toroids aligned)
 *   6. Snap-on bead rail (holds 8–12 beads in line on coax)
 */

$fn = 48;

D_M4   = 4.5;
D_M4_HS= 6.5;
D_M6   = 6.8;
D_M8   = 8.5;

// ── 1. U-BOLT MAST BRACKET ────────────────────────────────────────────────────
// Attaches the choke enclosure (or bare coil) to a round mast.
// Accommodates mast diameter 25mm (1") to 63mm (2.5").
// Two M6 U-bolts thread through slots; tighten with wing nuts for field use.
// Enclosure mounts to top face via 4× M6 bolts matching flange pattern.

module ubolt_mast_bracket(mast_d = 50) {
    PLATE_L = 150;
    PLATE_W = 70;
    PLATE_T = 8;
    SADDLE_H = 20;
    BOLT_PITCH_X = 120;   // spacing between U-bolt legs
    ENC_BOLT_PITCH = 110; // must match enclosure flange M6 pattern

    difference() {
        union() {
            // Flat back plate
            cube([PLATE_L, PLATE_W, PLATE_T]);
            // Mast saddle boss (extends inward)
            translate([PLATE_L/2 - mast_d/2 - 5, PLATE_W/2 - mast_d/2 - 5, PLATE_T])
                cube([mast_d + 10, mast_d + 10, SADDLE_H]);
        }

        // Mast cutout (semi-cylinder into saddle)
        translate([PLATE_L/2, -0.1, PLATE_T + SADDLE_H/2])
            rotate([-90, 0, 0])
            cylinder(d=mast_d + 1.0, h=PLATE_W + 0.2);

        // U-bolt slots (×2, centered on mast center)
        for (dx=[-BOLT_PITCH_X/2, BOLT_PITCH_X/2])
            translate([PLATE_L/2 + dx - D_M6/2, PLATE_W/2 - mast_d/2 - 8, -0.1])
                cube([D_M6, mast_d + 16, PLATE_T + 0.2]);

        // Enclosure mounting holes ×4
        for (pos=[[(PLATE_L-ENC_BOLT_PITCH)/2,    15],
                  [(PLATE_L-ENC_BOLT_PITCH)/2,    PLATE_W-15],
                  [(PLATE_L+ENC_BOLT_PITCH)/2,    15],
                  [(PLATE_L+ENC_BOLT_PITCH)/2,    PLATE_W-15]])
            translate([pos[0], pos[1], -0.1])
                cylinder(d=D_M6, h=PLATE_T+0.2);

        // Lightening holes
        for (x=[15, PLATE_L-15])
            translate([x, PLATE_W/2, -0.1])
                cylinder(d=18, h=PLATE_T+0.2);

        // Corner cable tie slots (route coax along bracket)
        for (y=[5, PLATE_W-5])
            translate([PLATE_L/2-1, y-2, -0.1])
                cube([2, 4, PLATE_T+0.2]);
    }
}

// ── 2. FLAT SURFACE / WALL MOUNT BRACKET ─────────────────────────────────────
// L-bracket for mounting choke enclosure on flat surface (wall, roof penetration,
// equipment rack, or field-day table edge).
// Horizontal arm: attaches to enclosure
// Vertical arm: attaches to wall with M6 lag bolts or anchors

module wall_mount_bracket() {
    H_ARM_L = 100;   // horizontal arm length
    V_ARM_L = 100;   // vertical arm length
    ARM_W   =  70;
    ARM_T   =   8;
    GUSSET  =  30;

    union() {
        // Horizontal arm
        difference() {
            cube([H_ARM_L, ARM_W, ARM_T]);
            // 4× M6 holes for enclosure attachment
            for (pos=[[15,15],[15,ARM_W-15],[H_ARM_L-15,15],[H_ARM_L-15,ARM_W-15]])
                translate([pos[0], pos[1], -0.1]) cylinder(d=D_M6, h=ARM_T+0.2);
        }

        // Vertical arm
        translate([H_ARM_L, 0, 0])
        rotate([0, -90, 0])
        difference() {
            cube([V_ARM_L, ARM_W, ARM_T]);
            // 4× M6 lag holes for wall
            for (pos=[[15,15],[15,ARM_W-15],[V_ARM_L-15,15],[V_ARM_L-15,ARM_W-15]])
                translate([pos[0], pos[1], -0.1]) cylinder(d=D_M6, h=ARM_T+0.2);
        }

        // Diagonal gusset
        translate([H_ARM_L-GUSSET, 0, ARM_T])
            linear_extrude(height=ARM_W)
                polygon([[0,0],[GUSSET,0],[0,GUSSET]]);
    }
}

// ── 3. TOWER CLAMP BRACKET ────────────────────────────────────────────────────
// Sandwiches between tower steps or clamps to tower angle iron leg.
// Designed for standard rohn/DMX tower step spacing.
// Adjustable jaw: opens 25–75mm for various tower leg widths.

module tower_clamp_bracket() {
    CLAMP_L = 120;
    CLAMP_H =  50;
    CLAMP_T =  10;
    JAW_D   =  50;  // maximum tower leg opening

    difference() {
        union() {
            cube([CLAMP_L, CLAMP_H, CLAMP_T]);
            // Enclosure mounting boss (top face)
            translate([CLAMP_L/2-30, CLAMP_H/2-15, CLAMP_T])
                cube([60, 30, 10]);
        }
        // Tower clamp jaw slot (center)
        translate([CLAMP_L/2-5, -0.1, CLAMP_T/2])
            cube([10, CLAMP_H/2+1, CLAMP_T]);

        // Clamping bolt hole M8 (through jaw)
        translate([CLAMP_L/2, -0.1, CLAMP_T/2])
            rotate([-90,0,0]) cylinder(d=D_M8, h=CLAMP_H+0.2);

        // Enclosure mounting ×4 M6
        for (pos=[[CLAMP_L/2-20, CLAMP_H/2-7],
                  [CLAMP_L/2-20, CLAMP_H/2+7],
                  [CLAMP_L/2+20, CLAMP_H/2-7],
                  [CLAMP_L/2+20, CLAMP_H/2+7]])
            translate([pos[0], pos[1], CLAMP_T-0.1]) cylinder(d=D_M6, h=11);

        // Lightening cutouts
        for (x=[10, CLAMP_L-30])
            translate([x, 10, -0.1]) cube([20, CLAMP_H-20, CLAMP_T+0.2]);
    }
}

// ── 4. COAX STRAIN RELIEF CLAMP ───────────────────────────────────────────────
// Clamps coax cable securely at choke enclosure entry port.
// Prevents pulling force on connector from vibration or cable weight.
// Two-piece: lower clamp (prints with enclosure or separately) + upper cap.
// Fits RG-8X (8.4mm OD) or RG-58 (5mm OD).

module strain_relief_lower(coax_d = 8.4) {
    CLAMP_L = 40;
    CLAMP_W = 25;
    CLAMP_H = 12;
    difference() {
        cube([CLAMP_L, CLAMP_W, CLAMP_H]);
        // Coax groove (half-cylinder)
        translate([CLAMP_L/2, -0.1, CLAMP_H/2])
            rotate([-90,0,0]) cylinder(d=coax_d+0.5, h=CLAMP_W+0.2);
        // M4 screw holes ×2
        for (x=[8, CLAMP_L-8])
            translate([x, CLAMP_W/2, -0.1]) cylinder(d=D_M4, h=CLAMP_H+0.2);
    }
}

module strain_relief_upper(coax_d = 8.4) {
    CLAMP_L = 40;
    CLAMP_W = 25;
    CLAMP_H =  8;
    difference() {
        cube([CLAMP_L, CLAMP_W, CLAMP_H]);
        // Coax groove
        translate([CLAMP_L/2, -0.1, 0])
            rotate([-90,0,0]) cylinder(d=coax_d+0.5, h=CLAMP_W+0.2);
        // M4 clearance holes + countersink
        for (x=[8, CLAMP_L-8]) {
            translate([x, CLAMP_W/2, -0.1]) cylinder(d=D_M4, h=CLAMP_H+0.2);
            translate([x, CLAMP_W/2, CLAMP_H-3.5]) cylinder(d1=D_M4, d2=9, h=3.6);
        }
    }
}

// ── 5. FERRITE STACK RETAINER CLIP ────────────────────────────────────────────
// Keeps two stacked FT-240 cores aligned and prevents separation.
// Snaps over both cores; cable tie slot at each end.
// Print 2 per choke (top + bottom of core stack).

module core_retainer_clip() {
    CORE_OD  = 61.5;  // FT-240 OD + 0.5mm clearance
    CLIP_W   = 15;
    CLIP_T   =  4;
    STACK_H  = 28;    // two FT-240 cores stacked (12.7mm × 2 + 2.6mm tape)
    ARM_L    = CORE_OD/2 + CLIP_T + 5;

    union() {
        // Bottom arc (lower core retention)
        rotate_extrude(angle=210)
            translate([CORE_OD/2, 0, 0])
                square([CLIP_T, CLIP_W]);

        // Two arms extending past 210° to form snap-over tabs
        for (a=[0, 210])
            rotate([0, 0, a])
                translate([CORE_OD/2, 0, 0])
                    cube([ARM_L, CLIP_T, CLIP_W]);

        // Cable tie slots at arm ends
        for (a=[0, 210])
            rotate([0, 0, a])
                translate([ARM_L + CORE_OD/2 - 3, CLIP_T/2-1.5, CLIP_W/2-4])
                    cube([3, 3, 8]);
    }
}

// ── 6. SNAP-ON BEAD RAIL ──────────────────────────────────────────────────────
// Holds a stack of snap-on split ferrite beads in a neat row on coax cable.
// Channel accepts beads from above; cable passes through from end.
// Mounts to flat surface or wraps with hook-and-loop strap.

module bead_rail(n_beads=8, bead_w=28.6, bead_h=28.6, coax_d=8.4) {
    RAIL_T = 5;
    RAIL_W = bead_h + 2*RAIL_T;
    RAIL_L = n_beads * (bead_w + 1) + 2*RAIL_T;

    difference() {
        cube([RAIL_L, RAIL_W, RAIL_T + bead_h/2]);

        // Coax channel through rail (lengthwise, centers at mid-height)
        translate([-0.1, RAIL_W/2, RAIL_T + bead_h/2])
            rotate([0, 90, 0])
            cylinder(d=coax_d+1.0, h=RAIL_L+0.2);

        // Bead pockets (top-open channels for each bead)
        for (i=[0:n_beads-1])
            translate([RAIL_T + i*(bead_w+1), RAIL_T, RAIL_T])
                cube([bead_w, bead_h, bead_h/2+0.1]);

        // Mounting slot ×2 (tie-wrap or M4 bolt)
        for (x=[10, RAIL_L-10])
            translate([x-2, RAIL_W/2-2, -0.1])
                cube([4, 4, RAIL_T+0.2]);
    }

    // Retainer lips at top of each bead pocket (flex-snap)
    for (i=[0:n_beads-1]) {
        for (side=[0,1]) {
            translate([RAIL_T + i*(bead_w+1) + side*(bead_w-4),
                       RAIL_T - 1.5, RAIL_T + bead_h/2 - 3])
                cube([4, 1.5, 5]);
        }
    }
}

// ── Render (one item at a time; uncomment to print) ──────────────────────────
ubolt_mast_bracket(mast_d=50);    // 1.97" mast (common); change to mast_d=38 for 1.5"
// translate([160, 0, 0]) wall_mount_bracket();
// translate([0, 160, 0]) tower_clamp_bracket();
// translate([160, 160, 0]) strain_relief_lower(coax_d=8.4);
// translate([160, 140, 15]) strain_relief_upper(coax_d=8.4);
// translate([0, 80, 0]) core_retainer_clip();
// translate([0, 0, 20]) bead_rail(n_beads=8, bead_w=28.6, bead_h=28.6, coax_d=8.4);
