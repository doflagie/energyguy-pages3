/*
 * mast_collars.scad
 * TM-MAST-001 Rev A — Mast Collar Hardware
 *
 * 3D-printable collars, guy attachment rings, and clamping hardware
 * for telescoping fiberglass and aluminum antenna masts.
 *
 * Modules:
 *   split_collar()         — Split clamping collar for any mast OD
 *   guy_ring_collar()      — Collar with 3 or 4 guy wire attachment eyes
 *   rotator_saddle_cage()  — Saddle bracket for rotator/mast interface
 *   tube_end_cap()         — Protective end cap (mast section top/bottom)
 *   piston_seal_ring()     — Pneumatic piston ring for air-lift mast
 *   strain_relief_clip()   — Rotator cable strain relief at mast
 *   antenna_mounting_u()   — U-bolt plate for side-mounting vertical antenna
 *   mast_section_stop()    — Positive extension stop ring
 *
 * Material: PETG (outdoor use, 70°C rated)
 *           ASA for >60°C ambient (desert/vehicle roof deployments)
 * Infill: 40% (gyroid or cubic); 4 perimeters; 0.2mm layer
 * Units: mm
 */

$fn = 60;

// ─── Constants ─────────────────────────────────────────────────────────────
WALL  = 3.0;
M4_C  = 4.4;    // M4 clearance
M4_I  = 5.2;    // M4 heat insert OD
M6_C  = 6.4;    // M6 clearance
M8_C  = 8.5;    // M8 clearance
TOL   = 0.3;    // Print tolerance / clearance

// =============================================================================
// MODULE 1: SPLIT COLLAR
// =============================================================================
// Two-piece split collar clamps around mast tube. M4 bolts draw halves together.
// Replaces or supplements hose clamps. Provides flat surface for attachments.
// =============================================================================
module split_collar(
    mast_od   = 50,    // Mast outer diameter
    collar_h  = 25,    // Collar height (axial)
    collar_t  = 8,     // Collar wall thickness
    n_bolts   = 2      // Number of bolt pairs (1 or 2)
) {
    OD = mast_od + 2 * collar_t;
    split_w = OD * 0.15;  // Width of split gap

    // HALF A
    difference() {
        union() {
            // Main collar body (half cylinder)
            difference() {
                cylinder(d=OD, h=collar_h);
                cylinder(d=mast_od + TOL*2, h=collar_h + 2);
                // Split cut (full Y+ half removed)
                translate([-OD, -OD, -1]) cube([OD, OD, collar_h + 2]);
            }
            // Bolt flanges (two ears)
            for (side=[-1, 1]) {
                translate([side * (mast_od/2 + collar_t/2), -collar_t, 0])
                    cube([collar_t, collar_t * 2, collar_h], center=false);
            }
        }
        // Bolt holes through flanges
        for (side=[-1, 1]) {
            for (z = n_bolts == 1 ? [collar_h/2] : [collar_h*0.25, collar_h*0.75]) {
                translate([side * (mast_od/2 + collar_t/2), 0, z])
                    rotate([90, 0, 0]) cylinder(d=M4_C, h=collar_t * 3, center=true);
            }
        }
    }

    // HALF B (mirrored, translated for print layout)
    translate([OD + 10, 0, 0]) mirror([0, 1, 0]) {
        difference() {
            union() {
                difference() {
                    cylinder(d=OD, h=collar_h);
                    cylinder(d=mast_od + TOL*2, h=collar_h + 2);
                    translate([-OD, -OD, -1]) cube([OD, OD, collar_h + 2]);
                }
                for (side=[-1, 1]) {
                    translate([side * (mast_od/2 + collar_t/2), -collar_t, 0])
                        cube([collar_t, collar_t * 2, collar_h]);
                }
            }
            // Captive nut pockets + bolt holes
            for (side=[-1, 1]) {
                for (z = n_bolts == 1 ? [collar_h/2] : [collar_h*0.25, collar_h*0.75]) {
                    translate([side * (mast_od/2 + collar_t/2), 0, z]) {
                        rotate([90, 0, 0]) cylinder(d=M4_C, h=collar_t * 3, center=true);
                        // Captive nut hex pocket
                        translate([0, -(collar_t * 1.5 - 3.5), 0])
                            rotate([90, 0, 0]) cylinder(d=8, h=3.5, $fn=6);
                    }
                }
            }
        }
    }
}

// =============================================================================
// MODULE 2: GUY RING COLLAR
// =============================================================================
// Wide collar with integral eyes for attaching guy wires.
// 3-lug version: 120° spacing; 4-lug version: 90° spacing
// =============================================================================
module guy_ring_collar(
    mast_od = 50,
    n_guys  = 3,     // 3 or 4
    eye_id  = 8,     // Eye inner diameter (accepts 6mm Dyneema loop or snap hook)
    collar_h= 35,
    collar_t= 8
) {
    EYE_T = 5;  // Eye wall thickness
    EYE_L = 18; // Eye lug length (radial)

    difference() {
        union() {
            // Collar ring
            cylinder(d = mast_od + 2 * collar_t, h = collar_h);

            // Guy attachment eyes (radial lugs)
            for (i = [0 : n_guys - 1]) {
                angle = i * 360 / n_guys;
                rotate([0, 0, angle]) {
                    translate([mast_od/2 + collar_t, 0, collar_h/2]) {
                        rotate([0, 90, 0]) {
                            // Lug body
                            difference() {
                                cylinder(d = eye_id + 2 * EYE_T, h = EYE_L);
                                translate([0, 0, EYE_T])
                                    cylinder(d = eye_id, h = EYE_L);
                                // Split wire-entry slot (top of lug)
                                translate([-1.5, 0, -1]) cube([3, eye_id + EYE_T + 2, EYE_L + 2]);
                            }
                            // Lug base gusset
                            translate([-(eye_id/2 + EYE_T), -(eye_id/2 + EYE_T), 0])
                                cube([eye_id + 2 * EYE_T, eye_id/2 + EYE_T, EYE_T]);
                        }
                    }
                }
            }
        }

        // Mast bore
        cylinder(d = mast_od + TOL * 2, h = collar_h + 2);

        // Split cut (collar is a clamping ring — cut and bolt to clamp)
        translate([-2, -mast_od - collar_t - 2, -1])
            cube([4, (mast_od + collar_t) * 2 + 4, collar_h + 2]);

        // Clamping bolt holes
        for (z = [collar_h * 0.25, collar_h * 0.75]) {
            for (side = [-1, 1]) {
                translate([side * (mast_od/2 + collar_t * 0.5), 0, z])
                    rotate([90, 0, 0])
                        cylinder(d = M4_C, h = (mast_od + 2 * collar_t) + 2, center = true);
            }
        }
    }
}

// =============================================================================
// MODULE 3: ROTATOR SADDLE CAGE
// =============================================================================
// Upper support bracket for mast-through rotator mount.
// Clamps to mast below rotator; has U-bolt pattern matching rotator ears.
// =============================================================================
module rotator_saddle_cage(
    mast_od      = 50,
    rotator_w    = 120,   // Rotator body width (G-450A ≈ 110mm; G-800 ≈ 130mm)
    rotator_h    = 60,    // Height of cage body
    ubolt_span   = 80,    // U-bolt mounting hole spacing (center to center)
    ubolt_d      = 8      // U-bolt rod diameter
) {
    cage_t = WALL;
    cage_w = rotator_w + 4;

    difference() {
        union() {
            // Main cage block
            translate([-cage_w/2, -(mast_od/2 + cage_t + 10), 0])
                cube([cage_w, mast_od + 2 * cage_t + 20, rotator_h]);

            // Mast collar bosses (each side)
            for (x = [-cage_w/2 - 8, cage_w/2]) {
                translate([x, -cage_t, 0])
                    cube([8, cage_t * 2, rotator_h]);
            }
        }

        // Mast bore (vertical through center)
        translate([0, 0, -1]) cylinder(d = mast_od + TOL * 2, h = rotator_h + 2);

        // Rotator U-bolt slots (allow U-bolt rod through cage)
        for (x = [-ubolt_span/2, ubolt_span/2]) {
            translate([x, -(mast_od/2 + cage_t + 10) - 1, rotator_h * 0.4])
                cube([ubolt_d + 2, mast_od + 2 * cage_t + 22, rotator_h * 0.25]);
        }

        // Mast clamp bolts (M6, through cage sides)
        for (z = [rotator_h * 0.25, rotator_h * 0.75]) {
            for (x = [-cage_w/2 - 4, cage_w/2 + 4]) {
                translate([x, 0, z])
                    rotate([0, 90, 0]) cylinder(d = M6_C, h = cage_w + 16 + 2, center = true);
            }
        }
    }
}

// =============================================================================
// MODULE 4: TUBE END CAP
// =============================================================================
// Protective cap for open end of mast section. Prevents debris ingress.
// Press fit; printed at +0.3mm of tube ID for friction retention.
// =============================================================================
module tube_end_cap(
    tube_od = 50,
    wall    = 2.0,
    depth   = 12
) {
    id = tube_od - 2 * wall;  // Assumes tube wall
    plug_od = id - TOL;        // Friction fit

    difference() {
        union() {
            // Plug body (inserts into tube)
            cylinder(d = plug_od, h = depth);
            // Flange (rests on tube end face)
            translate([0, 0, depth])
                cylinder(d = tube_od + 2, h = 2);
        }
        // Hollow (reduce material, keep some rigidity)
        translate([0, 0, 2]) cylinder(d = plug_od - 2 * wall, h = depth + 2);
    }
}

// =============================================================================
// MODULE 5: PNEUMATIC PISTON RING
// =============================================================================
// Piston seal ring for air-lift telescoping mast.
// Printed in PETG; lightly lubricated with silicone grease before use.
// =============================================================================
module pneumatic_piston_ring(
    inner_tube_od = 38,   // OD of inner tube (the piston)
    outer_tube_id = 44,   // ID of outer tube (the cylinder)
    ring_h        = 12    // Ring height
) {
    clearance = 0.15;     // Tight clearance for sealing
    seal_od   = outer_tube_id - clearance;
    seal_id   = inner_tube_od + TOL;

    groove_w = 3.0;   // O-ring groove width
    groove_d = 1.8;   // O-ring groove depth (for 3mm Buna-N O-ring)

    difference() {
        cylinder(d = seal_od, h = ring_h);

        // Bore for inner tube
        cylinder(d = seal_id, h = ring_h + 2);

        // O-ring groove (outer surface for cylinder sealing)
        translate([0, 0, ring_h / 2 - groove_w / 2])
            difference() {
                cylinder(d = seal_od + 2, h = groove_w);
                cylinder(d = seal_od - 2 * groove_d, h = groove_w + 2);
            }
    }

    // NOTE: O-ring sits in groove; contact between piston ring OD and cylinder ID.
    // Lubricate outer groove with silicone grease only (not petroleum based).
}

// =============================================================================
// MODULE 6: CABLE STRAIN RELIEF CLIP
// =============================================================================
// Clips to mast collar; provides cable tie anchor and cable routing groove.
// Prevents rotator cable from flapping or chafing on mast hardware.
// =============================================================================
module strain_relief_clip(
    mast_od  = 50,
    cable_d  = 8,    // Rotator cable outer diameter
    n_cables = 1
) {
    CLIP_H = 30;
    clip_t = 4;

    difference() {
        union() {
            // Mast-contact arc (half cylinder)
            difference() {
                cylinder(d = mast_od + clip_t * 2, h = CLIP_H);
                cylinder(d = mast_od + TOL * 2, h = CLIP_H + 2);
                // Remove front half (cable side)
                translate([0, -(mast_od + clip_t * 2), -1])
                    cube([mast_od + clip_t * 2 + 2, mast_od + clip_t * 2, CLIP_H + 2]);
            }

            // Cable guide block
            translate([mast_od/2 + clip_t, -n_cables * (cable_d + 3) / 2, 0])
                cube([cable_d + 4, n_cables * (cable_d + 3) + 3, CLIP_H]);
        }

        // Cable channel(s)
        for (i = [0 : n_cables - 1]) {
            translate([mast_od/2 + clip_t + cable_d/2 + 2,
                       -(n_cables-1) * (cable_d+3)/2 + i * (cable_d + 3),
                       -1])
                cylinder(d = cable_d + TOL * 2, h = CLIP_H + 2);
        }

        // Hose clamp groove (accept 9mm wide clamp)
        translate([-(mast_od/2 + clip_t + 2), -(mast_od/2 + clip_t + 2), CLIP_H/2 - 5])
            cube([mast_od + clip_t * 4 + 4, mast_od/2 + clip_t + 2, 10]);
    }
}

// =============================================================================
// MODULE 7: MAST SECTION EXTENSION STOP
// =============================================================================
// Ring bonded or clamped to outer section; inner section rests on it at full
// extension. Prevents over-extension and provides positive lock position.
// =============================================================================
module mast_section_stop(
    mast_od = 44,    // Outer section inner diameter ≈ inner section OD + clearance
    ring_h  = 8,
    ring_t  = 4
) {
    // Step ring: sits on top of outer tube; inner tube passes through
    difference() {
        cylinder(d = mast_od + 2 * ring_t, h = ring_h);
        // Inner bore just passes inner tube section
        cylinder(d = mast_od - 2.0, h = ring_h + 2);   // inner section OD ≈ mast_od - 6mm
    }

    // Raised stop nubs (contact inner tube lower edge)
    for (i = [0 : 2]) {
        rotate([0, 0, i * 120]) translate([mast_od/2 - 2, 0, ring_h])
            cylinder(d = 4, h = 3);
    }
}

// =============================================================================
// RENDER CONTROLS
// =============================================================================

split_collar(mast_od=50, collar_h=25, collar_t=8, n_bolts=2);
// translate([80, 0, 0]) split_collar(mast_od=38);
// translate([0, 80, 0]) guy_ring_collar(mast_od=50, n_guys=3);
// translate([0, 160, 0]) guy_ring_collar(mast_od=57, n_guys=4);
// translate([100, 0, 0]) rotator_saddle_cage();
// translate([0, 240, 0]) tube_end_cap(tube_od=63);
// translate([100, 80, 0]) pneumatic_piston_ring(inner_tube_od=38, outer_tube_id=44);
// translate([100, 120, 0]) strain_relief_clip(mast_od=50, cable_d=8, n_cables=2);
// translate([100, 160, 0]) mast_section_stop(mast_od=44);
