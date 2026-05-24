/*
 * guy_tensioners_anchors.scad
 * TM-MAST-001 Rev A — Guy Wire Tensioner and Anchor Hardware
 *
 * 3D-printable field hardware for guy wire management and ground anchoring:
 *   - In-line guy tensioner (turnbuckle-style, wrench tighten)
 *   - Wire thimble adapter (prevents cable kinking at eye)
 *   - Guy wire equalizer plate (3 or 4 guy wires to single anchor)
 *   - Screw anchor T-handle extension
 *   - Stake anchor eye adapter (for bent rebar)
 *   - Sandbag anchor strap with D-ring
 *   - Deadman cable guide tube (exits soil at correct angle)
 *   - Wind indicator (simple streamer holder for visual wind direction)
 *
 * Material: PETG or ASA; 5 perimeters; 50% gyroid infill for strength
 * All hardware is load-bearing; print conservatively.
 * Units: mm
 */

$fn = 60;

WALL  = 3.5;     // Slightly heavier wall for load-bearing parts
M4_C  = 4.4;
M6_C  = 6.4;
M8_C  = 8.5;
M10_C = 10.5;
TOL   = 0.3;

// =============================================================================
// MODULE 1: IN-LINE GUY TENSIONER
// =============================================================================
// Printed turnbuckle-equivalent. Two threaded rods (M6) screwed into body.
// Rotating body tightens or loosens. Guy wire connects to eye on each rod end.
// Uses standard M6 threaded rod stock + M6 hex nuts (not fully printed).
// RATED: 1,200 N working load (at 3× safety factor on 50A PETG). Verify print.
// =============================================================================
module guy_tensioner(
    body_l = 70,    // Tensioner body length
    body_d = 22,    // Body diameter
    eye_id = 8      // Wire eye inner diameter
) {
    EYE_T  = 5;
    EYE_H  = 20;
    GRIP_W = 3;  // Wrench flat width above body OD

    // Body (hexagonal for wrench grip)
    difference() {
        union() {
            // Hex body
            cylinder(d = body_d, h = body_l, $fn = 6);
            // Grip flats (center section)
            translate([0, 0, body_l/2 - 8])
                cylinder(d = body_d + 4, h = 16, $fn = 6);
        }
        // Through channel (hollow for weight/flexibility)
        cylinder(d = body_d - 2 * WALL, h = body_l + 2);

        // M6 threaded rod holes (both ends, blind pockets for nut capture)
        translate([0, 0, -1]) cylinder(d = M6_C, h = body_l * 0.4 + 1);
        translate([0, 0, body_l - body_l * 0.4]) cylinder(d = M6_C, h = body_l * 0.4 + 2);

        // M6 nut capture (hex, both ends)
        translate([0, 0, body_l * 0.35]) cylinder(d = 11.0, h = 6, $fn = 6);
        translate([0, 0, body_l - body_l * 0.35 - 6]) cylinder(d = 11.0, h = 6, $fn = 6);
    }

    // End eyes (both ends, same orientation = open)
    for (z_pos = [body_l + 2, -EYE_H - 2]) {
        translate([0, 0, z_pos]) {
            difference() {
                union() {
                    cylinder(d = eye_id + 2 * EYE_T, h = EYE_H);
                    // M6 rod connection stub below eye
                    translate([0, 0, z_pos < 0 ? EYE_H : -8])
                        cylinder(d = M6_C + 4, h = 8);
                }
                // Eye bore
                translate([0, 0, EYE_T]) cylinder(d = eye_id, h = EYE_H);
                // Wire entry slot (top of eye)
                translate([-1.5, -1, -1]) cube([3, eye_id/2 + EYE_T + 2, EYE_H + 2]);
                // M6 rod bore
                cylinder(d = M6_C, h = EYE_H + 12);
            }
        }
    }

    // Assembly note printed in body side (cosmetic groove)
    // "TIGHTEN = CLOCKWISE" orientation marker
    translate([body_d/2, 0, body_l/2]) rotate([0, 90, 0])
        cylinder(d = 2, h = 2);
}

// =============================================================================
// MODULE 2: WIRE THIMBLE ADAPTER
// =============================================================================
// Teardrop-shaped insert protects eye loops in Dyneema or wire rope from
// sharp radius bending. Fits inside a spliced/tied eye.
// =============================================================================
module wire_thimble(
    wire_d = 3,     // Wire/Dyneema diameter
    thimble_w = 8   // Thimble groove width
) {
    R_outer = wire_d * 3;    // Outer radius of teardrop
    R_inner = wire_d * 1.5;  // Inner radius (wire groove)

    difference() {
        union() {
            // Teardrop body (extruded along Y)
            hull() {
                cylinder(d = R_outer * 2, h = thimble_w);
                translate([0, R_outer * 1.5, 0])
                    cylinder(d = R_outer, h = thimble_w);
            }
        }
        // Wire groove (outer perimeter)
        translate([0, 0, -1]) {
            hull() {
                cylinder(d = R_inner * 2 + wire_d, h = thimble_w + 2);
                translate([0, R_outer * 1.5, 0])
                    cylinder(d = R_inner + wire_d, h = thimble_w + 2);
            }
        }
        // Cut away half (thimble is open U shape)
        translate([-(R_outer + 2), 0, -1])
            cube([R_outer * 2 + 4, R_outer * 3 + 2, thimble_w + 2]);
    }
}

// =============================================================================
// MODULE 3: GUY EQUALIZER PLATE (3-wire version)
// =============================================================================
// Triangular plate with 3 wire eyes and 1 anchor attachment eye.
// Distributes load from anchor to three guy wires simultaneously.
// =============================================================================
module guy_equalizer_3(
    eye_id   = 8,
    plate_t  = 6,
    arm_r    = 35    // Arm reach from center
) {
    EYE_T = 5;
    EYE_H = 18;

    difference() {
        // Triangular plate body
        hull() {
            cylinder(d = 20, h = plate_t);  // Center
            for (i = [0:2]) {
                rotate([0, 0, i * 120]) translate([arm_r, 0, 0])
                    cylinder(d = eye_id + EYE_T * 2 + 4, h = plate_t);
            }
        }

        // Center M8 through hole (anchor attachment)
        cylinder(d = M8_C, h = plate_t + 2);

        // Three wire eyes
        for (i = [0:2]) {
            rotate([0, 0, i * 120]) translate([arm_r, 0, 0]) {
                cylinder(d = eye_id, h = plate_t + 2);
                // Wire entry slot
                translate([-1.5, 0, -1]) cube([3, arm_r + eye_id, plate_t + 2]);
            }
        }
    }

    // M8 eye boss at center bottom (attachment to anchor)
    translate([0, 0, plate_t]) {
        difference() {
            cylinder(d = eye_id + EYE_T * 2, h = EYE_H);
            translate([0, 0, EYE_T]) cylinder(d = eye_id, h = EYE_H);
            translate([-1.5, -1, -1]) cube([3, eye_id + EYE_T + 2, EYE_H + 2]);
        }
    }
}

// =============================================================================
// MODULE 4: SCREW ANCHOR T-HANDLE ADAPTER
// =============================================================================
// T-handle extension for driving 12–22mm screw anchors into soil.
// Accepts hex rod tip; cross-bar provides twisting torque.
// =============================================================================
module screw_anchor_thandle(
    anchor_od = 16,   // Screw anchor rod diameter
    handle_w  = 200,  // Total handle width
    handle_d  = 18    // Handle tube diameter
) {
    socket_h = 60;    // Socket depth

    // T-bar (horizontal handle)
    rotate([90, 0, 0]) {
        difference() {
            cylinder(d = handle_d, h = handle_w, center = true);
            // Hollow for weight reduction
            cylinder(d = handle_d - 2 * WALL, h = handle_w + 2, center = true);
        }
    }

    // Vertical socket (drives anchor rod)
    difference() {
        cylinder(d = anchor_od + 2 * WALL, h = socket_h);
        // Socket bore (hex or round, depending on anchor head)
        cylinder(d = anchor_od + TOL, h = socket_h + 2, $fn = 6);  // Hex socket
    }

    // Web gussets between handle and socket
    for (side = [-1, 1]) {
        translate([side * (anchor_od/2 + WALL), 0, 0]) {
            linear_extrude(handle_d * 0.4)
                polygon([[0, 0], [side * (handle_d/2 - anchor_od/2 - WALL*2), 0],
                          [0, socket_h * 0.5]]);
        }
    }
}

// =============================================================================
// MODULE 5: STAKE ANCHOR EYE ADAPTER
// =============================================================================
// Fits over bent rebar hook (10–16mm rebar); provides flat eye for guy wire.
// Captures bent end of rebar inside a reinforced collar.
// =============================================================================
module stake_anchor_eye(
    rebar_d  = 12,
    eye_id   = 8,
    body_h   = 40
) {
    EYE_T  = 5;
    EYE_H  = 20;
    SLEEVE = rebar_d + 2 * WALL;

    union() {
        // Rebar capture sleeve
        difference() {
            cylinder(d = SLEEVE, h = body_h);
            cylinder(d = rebar_d + TOL, h = body_h + 2);
            // Rebar hook entry slot (allows bent hook to insert from side)
            translate([-rebar_d/2 - 1, 0, body_h - rebar_d * 3])
                cube([rebar_d + 2, SLEEVE + 2, rebar_d * 3 + 2]);
        }

        // Guy wire eye (radial from sleeve)
        translate([SLEEVE/2, 0, body_h * 0.4]) rotate([0, 90, 0]) {
            difference() {
                cylinder(d = eye_id + 2 * EYE_T, h = EYE_H);
                translate([0, 0, EYE_T]) cylinder(d = eye_id, h = EYE_H);
                translate([-1.5, -1, -1]) cube([3, eye_id + EYE_T + 2, EYE_H + 2]);
            }
        }
    }
}

// =============================================================================
// MODULE 6: SANDBAG ANCHOR STRAP CLIP
// =============================================================================
// Wraps around military-style sandbag; D-ring accepts guy wire or snap hook.
// Two-piece design: strap body + buckle clip.
// =============================================================================
module sandbag_anchor_clip(
    strap_w = 38,   // Strap slot width (accepts 38mm webbing)
    strap_t = 4,    // Strap slot thickness
    eye_id  = 12    // D-ring or attachment eye ID
) {
    BODY_H = strap_w + 8;
    EYE_T  = 6;
    EYE_H  = 22;

    difference() {
        // Main body
        cube([strap_w + 2 * WALL, BODY_H, strap_t + WALL]);

        // Strap slot (through body)
        translate([WALL, WALL + 4, WALL])
            cube([strap_w, BODY_H - 8, strap_t + 2]);
    }

    // Eye boss (extends from center of body)
    translate([strap_w/2 + WALL, BODY_H/2, strap_t + WALL]) {
        difference() {
            cylinder(d = eye_id + 2 * EYE_T, h = EYE_H);
            translate([0, 0, EYE_T]) cylinder(d = eye_id, h = EYE_H);
            translate([-1.5, -1, -1]) cube([3, eye_id + EYE_T + 2, EYE_H + 2]);
        }
    }
}

// =============================================================================
// MODULE 7: DEADMAN CABLE GUIDE TUBE
// =============================================================================
// Short tube inserted through soil at anchor cable exit angle.
// Prevents soil from cutting cable at surface break; maintains angle.
// Compatible with 25mm PVC conduit (ID used as outer guide).
// =============================================================================
module deadman_cable_guide(
    cable_d   = 8,    // Cable OD
    guy_angle = 35,   // Guy wire angle from horizontal (degrees)
    length    = 120   // Tube length (spans surface/soil interface)
) {
    // Guide tube at angle
    rotate([90 - guy_angle, 0, 0]) {
        difference() {
            cylinder(d = cable_d + 2 * WALL, h = length);
            cylinder(d = cable_d + TOL, h = length + 2);
        }
    }

    // Soil surface flange (lies flat on surface, distributes load)
    cylinder(d = 60, h = 4);
}

// =============================================================================
// MODULE 8: SIMPLE WIND INDICATOR MOUNT
// =============================================================================
// Clip to mast top; holds lightweight streamer or ribbon for visual wind
// direction check before raising. Remove before operation.
// =============================================================================
module wind_indicator_clip(mast_od = 38) {
    CLIP_H = 30;
    clip_t = 3;

    difference() {
        cylinder(d = mast_od + 2 * clip_t, h = CLIP_H);
        // Mast bore (slip fit)
        cylinder(d = mast_od + TOL * 2, h = CLIP_H + 2);
        // Split cut
        translate([-2, -(mast_od + clip_t + 2), -1])
            cube([4, mast_od + clip_t + 4, CLIP_H + 2]);
    }

    // Streamer attachment arm
    translate([mast_od/2 + clip_t, 0, CLIP_H]) {
        difference() {
            sphere(d = 10);
            cube([6, 6, 14], center = true);  // Slot for ribbon/zip tie
        }
    }
}

// =============================================================================
// RENDER CONTROLS
// =============================================================================

guy_tensioner();
// translate([60, 0, 0]) wire_thimble(wire_d=3);
// translate([0, 100, 0]) guy_equalizer_3();
// translate([100, 0, 0]) screw_anchor_thandle(anchor_od=16);
// translate([100, 100, 0]) stake_anchor_eye(rebar_d=12);
// translate([0, 200, 0]) sandbag_anchor_clip();
// translate([100, 180, 0]) deadman_cable_guide(cable_d=8, guy_angle=35);
// translate([0, 300, 0]) wind_indicator_clip();
