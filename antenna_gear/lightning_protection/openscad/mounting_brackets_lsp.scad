// ============================================================================
// TM-LSP-001  Rev A — Lightning Surge Protection
// Mounting Brackets: Mast/Tower/Wall Installation Hardware
// Print bed: 200 × 200 × 200 mm  |  Material: ASA (UV-stable, outdoor)
// Layer height: 0.2 mm  |  Infill: 40% gyroid  |  Walls: 4 perimeters
// ============================================================================
//
// MODULES IN THIS FILE:
//   1. mast_clamp_bracket(mast_d)      — U-bolt saddle for round mast/pipe
//   2. wall_mount_bracket()            — flat back for wood/metal surface
//   3. tower_leg_bracket(leg_w)        — wraps square tower leg
//   4. entry_panel_mount()             — exterior wall entry panel frame
//   5. ground_strap_guide(strap_w)     — clips to route copper bonding strap
//   6. rod_clamp_cover()               — weatherproof cover over ground rod clamp
//   7. arrestor_enclosure()            — generic weatherproof GDT arrestor box
//   8. strain_relief_coax(coax_od)     — coax bend-radius limiter at entry
//   9. cable_bundle_clamp(n)           — multi-cable bundle at entry panel
//  10. enclosure_lid_lsp()             — lid for arrestor_enclosure()
//
// All hardware: M4 or M6 stainless steel.
// U-bolt: 50mm pipe size (M8, standard plumbing U-bolt).
// ============================================================================

$fn = 64;

// ── Global dimensions ──────────────────────────────────────────────────────
WALL       = 3.0;    // shell wall thickness
BOLT_M4    = 4.2;    // M4 clearance hole
BOLT_M6    = 6.4;    // M6 clearance hole
BOLT_M8    = 8.4;    // M8 clearance hole (U-bolt)
NUT_M4_W   = 7.7;    // M4 hex nut across-flats (width)
NUT_M4_H   = 3.2;    // M4 hex nut height
NUT_M6_W   = 10.0;   // M6 across-flats
NUT_M6_H   = 5.0;    // M6 height
TOL        = 0.3;    // print tolerance (clearance fits)

// ── Render selection ───────────────────────────────────────────────────────
// Set SHOW = 1..10 to render individual parts, or 0 for assembly preview.
SHOW = 0;

// ── Part dispatch ──────────────────────────────────────────────────────────
if (SHOW == 0) {
    // Print layout — all parts spread on bed
    mast_clamp_bracket(mast_d=50);
    translate([120, 0, 0])   wall_mount_bracket();
    translate([0, 100, 0])   tower_leg_bracket(leg_w=40);
    translate([120, 100, 0]) entry_panel_mount();
    translate([0, 0, 0])     color("gray") ground_strap_guide(strap_w=25);
}
if (SHOW == 1)  mast_clamp_bracket(mast_d=50);
if (SHOW == 2)  wall_mount_bracket();
if (SHOW == 3)  tower_leg_bracket(leg_w=40);
if (SHOW == 4)  entry_panel_mount();
if (SHOW == 5)  ground_strap_guide(strap_w=25);
if (SHOW == 6)  rod_clamp_cover();
if (SHOW == 7)  arrestor_enclosure();
if (SHOW == 8)  strain_relief_coax(coax_od=10.3);
if (SHOW == 9)  cable_bundle_clamp(n=4);
if (SHOW == 10) enclosure_lid_lsp();


// ============================================================================
// 1. MAST CLAMP BRACKET
//    Attaches GDT arrestor enclosure to a round mast (antenna mast, pipe).
//    Default 50mm mast; parametric for 25–75mm masts.
//    Two M8 U-bolt holes; two M6 holes to bolt arrestor enclosure.
// ============================================================================
module mast_clamp_bracket(mast_d=50) {
    saddle_w  = mast_d + 2*WALL + 10;
    saddle_h  = 30;              // height of the saddle band
    back_w    = 90;              // width of the back plate
    back_h    = 80;              // height of the back plate
    back_t    = WALL + 2;        // thickness of back plate
    ubolt_sep = mast_d + 10;     // centre-to-centre of U-bolt holes

    difference() {
        union() {
            // Back plate
            cube([back_w, back_t, back_h]);

            // Saddle half-cylinder on front face
            translate([back_w/2, back_t, back_h/2])
            rotate([90, 0, 0])
            saddle_profile(mast_d, saddle_w, saddle_h);
        }

        // U-bolt holes (pair, through saddle and back plate)
        for (dx = [-ubolt_sep/2, ubolt_sep/2])
            translate([back_w/2 + dx, -1, back_h/2])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M8, h=back_t + saddle_h + 2);

        // M6 mount holes for arrestor enclosure (4-hole pattern)
        for (dx = [-28, 28]) for (dz = [-25, 25])
            translate([back_w/2 + dx, -1, back_h/2 + dz])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M6, h=back_t + 2);

        // M6 countersink on back face
        for (dx = [-28, 28]) for (dz = [-25, 25])
            translate([back_w/2 + dx, back_t - 3.5, back_h/2 + dz])
            rotate([-90, 0, 0])
            cylinder(d=12, h=5);

        // Mast bore (removes material from saddle interior)
        translate([back_w/2, back_t - 0.1, back_h/2])
        rotate([90, 0, 0])
        cylinder(d=mast_d + TOL*2, h=saddle_h + 0.2);
    }
}

module saddle_profile(mast_d, w, h) {
    r_out = mast_d/2 + WALL + 5;
    r_in  = mast_d/2 + TOL;
    linear_extrude(height=h, center=true)
    difference() {
        hull() {
            circle(r=r_out);
            translate([-w/2, -r_out]) square([w, 1]);
        }
        // Clearance for mast
        circle(r=r_in);
        // Remove back half
        translate([-w, 0]) square([2*w, r_out + 1]);
    }
}


// ============================================================================
// 2. WALL MOUNT BRACKET
//    Mounts arrestor enclosure or entry panel to exterior wall studs.
//    Four 6mm wall lag-bolt holes; two 6mm arrestor mount holes.
//    Drain slot at bottom centre to prevent water pooling.
// ============================================================================
module wall_mount_bracket() {
    W   = 100;
    H   = 120;
    T   = WALL + 2;
    flange_h = 20;

    difference() {
        union() {
            // Main back plate
            cube([W, T, H]);
            // Top and bottom stiffener flanges
            for (z = [0, H - flange_h])
                translate([0, 0, z])
                cube([W, T + 15, flange_h]);
        }

        // 4× M6 lag-bolt holes (into wall studs, 400mm o.c.)
        for (dx = [15, W-15]) for (dz = [15, H-15])
            translate([dx, -1, dz])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M6, h=T + 2);

        // 4× M6 arrestor-mount holes (56mm × 56mm pattern)
        for (dx = [-28, 28]) for (dz = [-28, 28])
            translate([W/2 + dx, -1, H/2 + dz])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M6, h=T + 2);

        // Drain slot at bottom centre
        translate([W/2 - 5, 0, -1])
        cube([10, T + 16, 12]);

        // Weight-saving pockets (structural ribs remain)
        translate([10, T*0.3, flange_h + 5])
        cube([W - 20, T, H - 2*flange_h - 10]);
    }
}


// ============================================================================
// 3. TOWER LEG BRACKET
//    Wraps a square tower leg (Rohn 25G style, 38mm or 40mm flat).
//    Clamping bolt M8 pulls two halves together.
//    Back face has M6 tapped-pad for arrestor enclosure.
// ============================================================================
module tower_leg_bracket(leg_w=40) {
    body_w = leg_w + 2*WALL + 2*TOL + 20;
    body_h = 80;
    body_d = leg_w/2 + WALL + 8;
    ear_w  = 20;

    difference() {
        union() {
            // Main body (C-channel open toward leg)
            cube([body_w, body_d, body_h]);

            // Side ears for clamping bolt
            for (dx = [0, body_w])
                translate([dx - ear_w*(dx==0?1:0), 0, 0])
                cube([ear_w, body_d + 15, body_h]);
        }

        // Tower leg slot (open to front)
        translate([-1, body_d - leg_w - TOL, (body_h - leg_w*1.5)/2])
        cube([body_w + 2, leg_w + TOL + 1, leg_w*1.5]);

        // M8 clamping bolt holes through ears
        for (dz = [body_h/3, 2*body_h/3])
            translate([-ear_w - 2, body_d + 5, dz])
            rotate([0, 90, 0])
            cylinder(d=BOLT_M8, h=body_w + 2*ear_w + 4);

        // M8 nut traps (hex, one side)
        for (dz = [body_h/3, 2*body_h/3])
            translate([body_w + ear_w - 10, body_d + 5, dz])
            rotate([0, 90, 0]) {
                cylinder(d=NUT_M6_W + 2, h=8, $fn=6);
                translate([0, 0, -1]) cylinder(d=BOLT_M8, h=15);
            }

        // 4× M6 enclosure mount holes on back face
        for (dx = [-20, 20]) for (dz = [-20, 20])
            translate([body_w/2 + dx, -1, body_h/2 + dz])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M6, h=WALL + 2);
    }
}


// ============================================================================
// 4. ENTRY PANEL MOUNT
//    Exterior wall frame for the single-point entry panel (300×150×6mm).
//    Mounts to wall with 4× lag bolts; panel snaps/bolts into recess.
//    Cable exit holes: 3× coax (25mm), 1× conduit (32mm).
// ============================================================================
module entry_panel_mount() {
    // Panel dimensions
    PW = 155;   // panel width  (fits inside frame)
    PH = 105;   // panel height
    PT = 6;     // panel thickness

    // Frame dimensions
    FW = PW + 2*WALL + 2*TOL;
    FH = PH + 2*WALL + 2*TOL;
    FT = PT + WALL*2;
    lip = 8;    // inset lip that captures panel edges

    difference() {
        union() {
            // Frame body
            cube([FW, FT, FH]);
            // 4× wall mount ears
            for (dx = [0, FW]) for (dz = [0, FH])
                translate([dx - (dx>0?20:0), 0, dz - (dz>0?20:0)])
                cube([20, FT, 20]);
        }

        // Panel recess
        translate([WALL, WALL, WALL])
        cube([PW + TOL*2, PT + TOL, PH + TOL*2]);

        // Panel lip cutout (inner; panel edges held by lip perimeter)
        translate([WALL + lip, -1, WALL + lip])
        cube([PW - 2*lip + TOL*2, FT + 2, PH - 2*lip + TOL*2]);

        // 3× coax feedthrough holes (25mm OD weathertight gland)
        for (i = [0:2])
            translate([FW*0.2 + i * FW*0.25, -1, FH/2])
            rotate([-90, 0, 0])
            cylinder(d=25, h=FT + 2);

        // 1× conduit feedthrough (32mm, power/control)
        translate([FW*0.85, -1, FH/2])
        rotate([-90, 0, 0])
        cylinder(d=32, h=FT + 2);

        // 4× M6 lag bolt holes in ears
        for (dx = [10, FW + 10]) for (dz = [10, FH + 10])
            translate([dx, -1, dz])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M6, h=FT + 2);

        // Drain hole at bottom centre
        translate([FW/2 - 3, 0, 2])
        cube([6, FT, 5]);
    }
}


// ============================================================================
// 5. GROUND STRAP GUIDE
//    Clips along the path of a copper bonding strap.
//    Prevents strap from flapping, routes it close to wall surface.
//    Single lag-bolt hole for wood/masonry screw.
// ============================================================================
module ground_strap_guide(strap_w=25) {
    sw   = strap_w + TOL*2;   // strap channel width
    st   = 2.5 + TOL;         // strap thickness clearance
    body_w = sw + 2*WALL;
    body_h = sw + WALL + 5;
    body_d = st + WALL*2;
    len  = 60;                // guide length

    difference() {
        union() {
            // Guide channel body
            cube([len, body_d, body_h]);
            // Mounting tab
            translate([(len - 30)/2, 0, body_h])
            cube([30, body_d + 10, WALL + 3]);
        }

        // Strap slot through body
        translate([(len - sw)/2, WALL, WALL])
        cube([sw + (len - sw), st, body_h]);

        // Open slot on top (strap inserts from top)
        translate([(len - sw)/2, WALL, body_h - st - 1])
        cube([sw, st + WALL + 1, body_h]);

        // Snap tab cutouts (two flexible tabs grip strap)
        for (dx = [(len - sw)/2 - 0.1, (len + sw)/2 - 1.5])
            translate([dx, WALL - 0.1, body_h - st - 4])
            cube([1.5, st + 1, 8]);

        // Mount screw hole
        translate([len/2, body_d + 10 + 1, body_h + WALL/2 + 1.5])
        rotate([90, 0, 0])
        cylinder(d=BOLT_M6, h=body_d + 12);
    }
}


// ============================================================================
// 6. GROUND ROD CLAMP COVER
//    Weatherproof cover over the ground rod → copper strap connection.
//    Snaps over an 8mm strap and a standard 5/8" (16mm) rod.
//    Prevents moisture and galvanic corrosion at the clamp.
// ============================================================================
module rod_clamp_cover() {
    rod_d   = 17;    // 5/8" ground rod OD + clearance
    strap_w = 15;    // narrowest strap the cover must accept
    strap_t = 4;     // strap + clamp assembly height at exit
    h       = 80;    // cover height
    r_out   = rod_d/2 + WALL + 6;
    w_body  = strap_w + 2*WALL + 10;

    difference() {
        union() {
            // Main cylindrical cowl over rod
            cylinder(d=r_out*2, h=h);
            // Side extension for strap exit
            translate([-w_body/2, r_out - WALL, 10])
            cube([w_body, WALL + 20, h - 20]);
        }

        // Rod clearance bore
        translate([0, 0, -1])
        cylinder(d=rod_d, h=h + 2);

        // Strap exit slot
        translate([-strap_w/2 - TOL, r_out - 1, 10 + WALL])
        cube([strap_w + TOL*2, WALL + 22, strap_t + TOL*2]);

        // Open bottom for installation over existing clamp
        cylinder(d=r_out*2 + 2, h=12);

        // 2× mounting screw holes to clamp to rod with stainless zip tie pass-through
        for (a = [60, -60])
            rotate([0, 0, a])
            translate([r_out, 0, h/2])
            rotate([0, 90, 0])
            cylinder(d=4, h=r_out + 5);
    }
}


// ============================================================================
// 7. ARRESTOR ENCLOSURE
//    Weatherproof housing for in-line GDT or multi-stage arrestor PCB.
//    Two SO-239/N-type openings on opposite ends; ground lug post on bottom.
//    IP54 — sealed lid with gasket groove.
//    PCB mounts on 4× M3 standoffs; cover secured with 4× M4 screws.
// ============================================================================
module arrestor_enclosure() {
    EL = 110;    // internal length (connector to connector)
    EW = 55;     // internal width
    EH = 40;     // internal height
    T  = WALL;

    conn_d_so239 = 16.0;   // SO-239 panel hole diameter
    conn_d_ntype = 20.5;   // N-type panel hole diameter
    gnd_post_d   = 10;

    ox = EL + 2*T;
    ow = EW + 2*T;
    oh = EH + T;

    difference() {
        union() {
            // Main box (open top)
            difference() {
                cube([ox, ow, oh]);
                translate([T, T, T])
                cube([EL, EW, EH + 1]);
            }
            // Connector end bosses
            for (x = [-T, ox])
                translate([x == -T ? -4 : ox, ow/2 - 12, 0])
                cube([4, 24, oh]);

            // Corner pillars (internal) for lid screws
            for (cx = [T + 3, ox - T - 3 - 8]) for (cy = [T + 3, ow - T - 3 - 8])
                translate([cx, cy, T])
                cube([8, 8, EH - 2]);
        }

        // SO-239 hole (antenna side)
        translate([-5, ow/2, oh/2])
        rotate([0, 90, 0])
        cylinder(d=conn_d_so239, h=T + 10);

        // SO-239 mounting holes (4× M4)
        for (da = [45, 135, 225, 315])
            translate([-5, ow/2 + cos(da)*14.5, oh/2 + sin(da)*14.5])
            rotate([0, 90, 0])
            cylinder(d=BOLT_M4, h=T + 10);

        // N-type or SO-239 hole (shack side)
        translate([ox - T - 4, ow/2, oh/2])
        rotate([0, 90, 0])
        cylinder(d=conn_d_so239, h=T + 10);

        // N-type mounting holes
        for (da = [45, 135, 225, 315])
            translate([ox - T - 4, ow/2 + cos(da)*14.5, oh/2 + sin(da)*14.5])
            rotate([0, 90, 0])
            cylinder(d=BOLT_M4, h=T + 10);

        // Ground lug stud hole (bottom)
        translate([ox/2, ow/2, -1])
        cylinder(d=gnd_post_d, h=T + 2);

        // Ground lug screw holes (2× M5)
        for (dy = [-10, 10])
            translate([ox/2, ow/2 + dy, -1])
            cylinder(d=5.3, h=T + 2);

        // PCB standoff holes (M3, 4× pattern)
        for (px = [15, ox - 15]) for (py = [12, ow - 12])
            translate([px, py, T - 0.1])
            cylinder(d=3.3, h=6);

        // Lid screw holes (M4, into corner pillars)
        for (cx = [T + 7, ox - T - 7]) for (cy = [T + 7, ow - T - 7])
            translate([cx, cy, oh - 8])
            cylinder(d=BOLT_M4, h=12);

        // Gasket groove on top face
        translate([T/2, T/2, oh - T/2])
        linear_extrude(height=T/2 + 0.1)
        difference() {
            offset(r=2) square([ox - T, ow - T]);
            offset(r=-1) square([ox - T, ow - T]);
        }
    }

    // PCB standoffs (M3 posts, 5mm height)
    for (px = [15, ox - 15]) for (py = [12, ow - 12])
        translate([px, py, T])
        cylinder(d=6, h=5);
}


// ============================================================================
// 8. STRAIN RELIEF — COAX ENTRY
//    Limits bend radius of coax at entry point; prevents kinking.
//    Two-part clamp (print flat, bolt together around coax).
//    Rated for coax OD 6–12 mm; default RG-8X 10.3mm.
// ============================================================================
module strain_relief_coax(coax_od=10.3) {
    r   = coax_od/2 + TOL;
    len = 60;
    h   = coax_od * 1.5 + 2*WALL;
    w   = coax_od + 2*WALL + 4;

    // Curved guide channel half (print two, bolt together)
    difference() {
        cube([len, w, h/2 + WALL]);

        // Coax slot (semicircle cross-section, top half)
        translate([-1, w/2, WALL + r])
        rotate([0, 90, 0])
        cylinder(r=r, h=len + 2);

        // Flare at entry end (15° bend limiter)
        translate([-1, w/2, WALL + r])
        rotate([0, 90, 0])
        cylinder(r1=r + 4, r2=r, h=20);

        // M4 bolt holes (4 along length)
        for (lx = [10, 25, 35, 50])
            translate([lx, -1, h/2 - WALL/2])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M4, h=w + 2);

        // M4 nut traps
        for (lx = [10, 25, 35, 50])
            translate([lx, w - NUT_M4_H - WALL + 0.1, h/2 - WALL/2])
            rotate([-90, 0, 0]) {
                cylinder(d=NUT_M4_W, h=NUT_M4_H + 1, $fn=6);
            }

        // Wall mount hole
        translate([len/2, -1, (h/2 + WALL)/2])
        rotate([-90, 0, 0])
        cylinder(d=BOLT_M6, h=WALL + 2);
    }
}


// ============================================================================
// 9. CABLE BUNDLE CLAMP
//    Holds N coax/control cables together at entry panel.
//    One row of n round slots; wall mount at back; 2× M4 clamping bolts.
//    Uses n=4 default (3× coax + 1 control bundle).
// ============================================================================
module cable_bundle_clamp(n=4, cable_od=12) {
    r    = cable_od/2 + TOL;
    pitch = cable_od + 4;
    total_w = n * pitch + 2*WALL;
    body_h  = cable_od + 2*WALL;
    body_d  = cable_od * 1.2 + WALL;
    bolt_x  = [total_w*0.2, total_w*0.8];

    // Bottom half
    difference() {
        cube([total_w, body_d, body_h/2 + WALL]);

        // Cable slots (semi-circular, bottom half)
        for (i = [0:n-1])
            translate([WALL + pitch/2 + i*pitch, -1, WALL + r])
            rotate([-90, 0, 0])
            cylinder(r=r, h=body_d + 2);

        // 2× M4 clamping bolt holes
        for (bx = bolt_x)
            translate([bx, -1, (body_h/2 + WALL)/2])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M4, h=body_d + 2);

        // Wall mount hole
        translate([total_w/2, body_d + 1, (body_h/2 + WALL)/2])
        rotate([90, 0, 0])
        cylinder(d=BOLT_M6, h=body_d + 2);
    }

    // Top clamp half (printed separately, translated away)
    translate([0, body_d + 10, 0])
    difference() {
        cube([total_w, body_d, body_h/2 + WALL]);

        // Cable slots (top half)
        for (i = [0:n-1])
            translate([WALL + pitch/2 + i*pitch, -1, -r + 0.1])
            rotate([-90, 0, 0])
            cylinder(r=r, h=body_d + 2);

        // M4 holes (top half — clearance only)
        for (bx = bolt_x)
            translate([bx, -1, (body_h/2 + WALL)/2])
            rotate([-90, 0, 0])
            cylinder(d=BOLT_M4, h=body_d + 2);
    }
}


// ============================================================================
// 10. ENCLOSURE LID — LSP
//     Lid for arrestor_enclosure() above.
//     Flat with stiffening ribs; 4× M4 screw holes; gasket contact surface.
// ============================================================================
module enclosure_lid_lsp() {
    EL = 110;
    EW = 55;
    T  = WALL;
    ox = EL + 2*T;
    ow = EW + 2*T;
    lid_t = WALL + 2;

    difference() {
        union() {
            // Main lid plate
            cube([ox, ow, lid_t]);

            // Stiffening ribs (2 across width)
            for (x = [ox*0.3, ox*0.65])
                translate([x, 0, lid_t])
                cube([WALL, ow, 4]);
        }

        // 4× M4 screw holes (into corner pillars of body)
        for (cx = [T + 7, ox - T - 7]) for (cy = [T + 7, ow - T - 7])
            translate([cx, cy, -1])
            cylinder(d=BOLT_M4, h=lid_t + 2);

        // Label recess (for laser engraving or sticker)
        translate([T + 5, T + 5, lid_t - 0.8])
        cube([ox - 2*T - 10, ow - 2*T - 10, 1]);
    }
}

// ============================================================================
// END OF FILE
// ============================================================================
