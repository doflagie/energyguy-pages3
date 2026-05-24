// =============================================================================
// TM-SC-ENC-003 Rev A
// Screwdriver Antenna — Roll-Up Wire Storage Spool
// =============================================================================
// Description : Two-piece split spool for storing the roll-up wire version of
//               the screwdriver antenna field kit. Stores 2.5m of #18 AWG wire.
//               One flange carries 2 rectangular trap housing clips (2m & 6m).
//               The other flange carries 5 circular loading coil module slots
//               (160m / 80m / 60m / 40m / 30m).
//               The two halves are joined by 4× M3 screws through the hub.
//               A handle on the hub periphery has a 10mm hanging hole.
//
// Material    : PETG
// Print bed   : 200 × 200 × 200 mm
//               Each half: 150mm dia × ~30mm tall — fits comfortably flat.
//               Both halves printed side-by-side or sequentially.
// Author      : Generated for Merv Martin
// Date        : 2026-05-06
// =============================================================================

$fn = 64;

// ─── Primary Parameters ──────────────────────────────────────────────────────
flange_od   = 150;    // flange outer diameter
flange_t    = 6;      // flange thickness (each half contributes one flange)
hub_od      = 55;     // hub cylinder outer diameter
hub_h       = 45;     // hub total height (both halves together = full hub)
wall        = 3.0;    // general wall thickness

// ─── Trap Clip Parameters (one flange) ───────────────────────────────────────
n_trap_clips = 2;     // 2m trap and 6m trap positions
trap_clip_w  = 32;    // clip pocket width
trap_clip_d  = 24;    // clip pocket depth (radial)
trap_clip_h  = 4;     // clip pocket depth (axial, into flange)
trap_labels  = ["2m", "6m"];

// ─── Loading Coil Slot Parameters (other flange) ─────────────────────────────
n_coil_slots  = 5;    // 160m, 80m, 60m, 40m, 30m
coil_slot_d   = 32;   // coil module pocket diameter
coil_slot_h   = 4;    // coil module pocket depth (axial, into flange)
coil_labels   = ["160m", "80m", "60m", "40m", "30m"];

// ─── Fastener Pattern ────────────────────────────────────────────────────────
n_screws     = 4;     // M3 screws joining the two halves
screw_d      = 3.4;   // M3 clearance bore
boss_od      = 8.0;   // screw boss OD
screw_r      = hub_od/2 - boss_od/2 - 0.5;  // radial position of screw bosses
nut_trap_d   = 6.2;   // M3 hex nut trap across flats (5.5mm AF + clearance)
nut_trap_h   = 2.8;   // M3 nut height + 0.3 tolerance

// ─── Wire Anchor ─────────────────────────────────────────────────────────────
anchor_w     = 3.5;   // wire anchor notch width (fits #18 AWG doubled)
anchor_h     = 6.0;   // anchor notch depth
anchor_l     = 12.0;  // anchor notch length along hub surface

// ─── Handle ──────────────────────────────────────────────────────────────────
handle_l     = 40;    // handle length (radially outward from hub)
handle_w     = 16;    // handle width
handle_t     = 8;     // handle thickness
hang_hole_d  = 10;    // hanging hole diameter
hang_hole_off = 30;   // hanging hole centre offset from hub surface

// ─── Label Text Engraving Depth ──────────────────────────────────────────────
engrave_d    = 0.6;   // engraving depth into flange surface

// =============================================================================
// MODULE: hub
// Full hub — used by both halves. Each half gets hub_h/2 of this.
// Contains wire anchor notch and M3 screw boss pattern.
// =============================================================================
module hub() {
    half_h = hub_h / 2;

    difference() {
        union() {
            cylinder(d = hub_od, h = half_h);

            // Screw bosses: 4 evenly-spaced around hub interior ring
            for (a = [0 : 360/n_screws : 359])
                rotate([0, 0, a])
                translate([screw_r, 0, 0])
                    cylinder(d = boss_od, h = half_h);
        }

        // Screw clearance bores (through-hole in one half, nut trap in other)
        // Both halves get through-bore; nut trap variant handled in half_spool().
        for (a = [0 : 360/n_screws : 359])
            rotate([0, 0, a])
            translate([screw_r, 0, -0.1])
                cylinder(d = screw_d, h = half_h + 0.2);

        // Wire anchor notch — radial slot at 0° on hub exterior
        translate([hub_od/2 - anchor_l, -anchor_w/2, half_h * 0.4])
            cube([anchor_l + 1, anchor_w, anchor_h]);
    }
}

// =============================================================================
// MODULE: handle
// Integrated handle on the hub perimeter.
// Contains a 10mm hanging hole.
// =============================================================================
module handle() {
    // Handle extends radially outward from hub surface
    translate([hub_od/2, -handle_w/2, -handle_t/2])
    difference() {
        cube([handle_l, handle_w, handle_t]);
        // Hanging hole
        translate([hang_hole_off - hub_od/2, handle_w/2, -0.1])
            cylinder(d = hang_hole_d, h = handle_t + 0.2);
    }
}

// =============================================================================
// MODULE: coil_slot_pattern
// 5 circular pockets on flange face for storing loading coil modules.
// Evenly spaced on a bolt circle at r = (flange_od/2 + hub_od/2) / 2.
// Labels engraved into pocket bottom.
// =============================================================================
module coil_slot_pattern() {
    r_circle = (flange_od/2 + hub_od/2 + coil_slot_d/2) / 2;
    angle_step = 360 / n_coil_slots;

    for (i = [0 : n_coil_slots - 1]) {
        a = i * angle_step;
        rotate([0, 0, a])
        translate([r_circle, 0, 0]) {
            // Circular pocket
            cylinder(d = coil_slot_d, h = coil_slot_h + 0.1);
            // Engraved label at pocket centre
            // Text rendered as a simple linear_extrude negative at pocket floor
            translate([0, 0, coil_slot_h - engrave_d])
            linear_extrude(height = engrave_d + 0.05, center = false)
                text(coil_labels[i],
                     size   = 5,
                     halign = "center",
                     valign = "center",
                     font   = "Liberation Sans:style=Bold");
        }
    }
}

// =============================================================================
// MODULE: trap_clip_pattern
// 2 rectangular clip pockets at 180° apart on trap-side flange.
// Each pocket retains one cylindrical trap housing.
// Labels engraved into pocket bottom.
// =============================================================================
module trap_clip_pattern() {
    angles = [90, 270];   // 180° apart, offset 90° from wire anchor

    for (i = [0 : n_trap_clips - 1]) {
        a = angles[i];
        r_clip = flange_od/2 - trap_clip_d/2 - wall;

        rotate([0, 0, a])
        translate([r_clip, -trap_clip_w/2, 0]) {
            // Rectangular pocket
            cube([trap_clip_d + 0.1, trap_clip_w, trap_clip_h + 0.1]);
            // Engraved label
            translate([trap_clip_d/2, trap_clip_w/2, trap_clip_h - engrave_d])
            linear_extrude(height = engrave_d + 0.05, center = false)
                text(trap_labels[i],
                     size   = 6,
                     halign = "center",
                     valign = "center",
                     font   = "Liberation Sans:style=Bold");
        }
    }
}

// =============================================================================
// MODULE: flange(is_trap_side)
// One flange disk. is_trap_side=true → trap clip pockets.
//                  is_trap_side=false → coil module slots.
// Oriented so flat face sits on the print bed (features face up).
// =============================================================================
module flange(is_trap_side = false) {
    difference() {
        // Flange disk
        cylinder(d = flange_od, h = flange_t);

        // Hub bore
        translate([0, 0, -0.1])
            cylinder(d = hub_od + 0.4, h = flange_t + 0.2);

        // Pockets (removed from top face)
        translate([0, 0, flange_t - (is_trap_side ? trap_clip_h : coil_slot_h)]) {
            if (is_trap_side)
                trap_clip_pattern();
            else
                coil_slot_pattern();
        }
    }
}

// =============================================================================
// MODULE: half_spool(trap_side)
// One complete half of the spool. Combines hub + flange + handle.
// trap_side = true  → this half has the trap clip flange + nut traps.
// trap_side = false → this half has the coil slot flange + through-bores.
//
// Coordinate convention: the mating face of the hub is at Z=0.
// The flange and handle extend in the +Z direction when printed flat.
// =============================================================================
module half_spool(trap_side = false) {
    half_h = hub_h / 2;

    // Flip so mating face is up (glue/screw face visible on top during assembly).
    // For printing, both halves print flat with their outside face down.
    difference() {
        union() {
            // Flange at hub top (away from mating face)
            translate([0, 0, half_h])
                flange(is_trap_side = trap_side);

            // Hub half
            hub();

            // Handle — on hub periphery at 180° from wire anchor
            rotate([0, 0, 180])
            translate([0, 0, half_h/2])
                handle();
        }

        // Nut traps on trap-side half (so nuts are captured inside)
        if (trap_side) {
            for (a = [0 : 360/n_screws : 359])
                rotate([0, 0, a])
                translate([screw_r, 0, 0]) {
                    // Hex nut trap at mating face end (z near 0)
                    translate([0, 0, 1.5])
                        cylinder(d = nut_trap_d, h = nut_trap_h, $fn = 6);
                }
        }
    }
}

// =============================================================================
// RENDER — both halves side by side for print layout
// Both halves print flat on the bed (outside face down).
// After printing, join with 4× M3×(hub_h + 4)mm button-head screws.
// =============================================================================

// Coil-slot half (left)
translate([0, 0, 0])
    half_spool(trap_side = false);

// Trap-clip half (right) — mirrored so mating faces face each other in assembly
// For print layout, both sit outside-face-down, so no mirror needed here.
translate([flange_od + 15, 0, 0])
    half_spool(trap_side = true);

// =============================================================================
// ASSEMBLY NOTES
// 1. Print both halves flat (outside face down). No supports required.
//    Each half: 150mm dia × ~(hub_h/2 + flange_t) = ~28.5mm tall.
// 2. Drop four M3 hex nuts into the nut traps on the trap-side half.
// 3. Seat the two halves together with mating hub faces together.
// 4. Drive 4× M3×50mm button-head screws from the coil-slot side.
// 5. Wire anchor: thread the wire end through the notch on the hub,
//    double it back on itself, and begin winding the 2.5m run onto the hub.
// 6. Trap housings clip into the two rectangular pockets (2m trap, 6m trap).
// 7. Loading coil modules store in the five circular pockets, labelled by band.
// 8. Hang the spool on a pack strap or carabiner via the 10mm handle hole.
//
// Flange pocket assignments:
//   Coil-slot flange: 160m / 80m / 60m / 40m / 30m loading coil modules
//   Trap-clip flange: 2m trap housing / 6m trap housing
// =============================================================================
