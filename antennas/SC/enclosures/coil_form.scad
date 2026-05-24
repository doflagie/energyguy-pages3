// =============================================================================
// TM-SC-ENC-001 Rev A
// Screwdriver Antenna — Loading Coil Form
// =============================================================================
// Description : Cylindrical spool for the HF screwdriver antenna loading coil.
//               A 1/4-20 threaded rod passes through the center bore. A sliding
//               wiper contact rides in the registration groove along the outside
//               of the wound coil. Wire is wound in the 95mm winding region
//               between the two flanges.
//
// Material    : PETG
// Print bed   : 200 × 200 × 200 mm  (fits upright, longest axis = 105 mm Z)
// Author      : Generated for Merv Martin
// Date        : 2026-05-06
// =============================================================================

$fn = 64;

// ─── Primary Parameters ──────────────────────────────────────────────────────
form_od    = 75;          // outer diameter of winding cylinder
form_id    = 7.5;         // center bore: clearance for 1/4-20 threaded rod
wind_l     = 95;          // winding region length
flange_t   = 5;           // flange thickness
flange_od  = 85;          // flange outer diameter
total_l    = wind_l + 2 * flange_t;  // = 105 mm

// ─── Derived / Detail Parameters ─────────────────────────────────────────────
coil_pitch    = 2.3;      // mm per turn of wound wire (approx #18 AWG close-wound)
groove_w      = 2.0;      // wiper track groove width
groove_d      = 1.0;      // wiper track groove depth
flat_w        = 4.0;      // anti-rotation flat width
flat_d        = 1.5;      // anti-rotation flat depth
light_d       = 10.0;     // lightening hole diameter
wire_slot_w   = 2.2;      // wire-start/end slot width
wire_slot_l   = 8.0;      // wire-start/end slot radial length
tap_hole_d    = 2.5;      // coax tap hole diameter
tap_angle     = 0;        // angular position of tap hole (degrees, 0 = positive X)
wiper_angle   = 90;       // wiper groove is at 90° from tap hole
boss_h        = 2.0;      // turn-mark boss height
boss_d        = 4.0;      // turn-mark boss diameter

// Turn number → Z position on winding cylinder
function turn_z(n) = flange_t + n * coil_pitch;

// =============================================================================
// MODULE: lightening_holes
// Three 10mm through-holes in a flange, evenly spaced at 120°.
// Call with the flange sitting at z=0, oriented along +Z.
// =============================================================================
module lightening_holes(z_start, thickness) {
    r_light = (form_od / 2 + flange_od / 2) / 2;   // radial midpoint of flange ring
    for (a = [0, 120, 240]) {
        rotate([0, 0, a + 60])   // offset 60° so holes don't land on groove/flat
        translate([r_light, 0, z_start - 0.1])
            cylinder(d = light_d, h = thickness + 0.2);
    }
}

// =============================================================================
// MODULE: wire_holes
// A pair of radial slots through each flange for threading wire ends.
// One slot at ~180° (opposite tap hole), one at ~270°.
// =============================================================================
module wire_holes() {
    slot_r = form_od / 2;
    slot_angles = [180, 270];

    // Bottom flange slots (z = 0 to flange_t)
    for (a = slot_angles)
        rotate([0, 0, a])
        translate([slot_r - wire_slot_l / 2, -wire_slot_w / 2, -0.1])
            cube([wire_slot_l + 5, wire_slot_w, flange_t + 0.2]);

    // Top flange slots (z = flange_t + wind_l to total_l)
    for (a = slot_angles)
        rotate([0, 0, a])
        translate([slot_r - wire_slot_l / 2, -wire_slot_w / 2, flange_t + wind_l - 0.1])
            cube([wire_slot_l + 5, wire_slot_w, flange_t + 0.2]);
}

// =============================================================================
// MODULE: turn_mark(turn_num, pitch)
// A small raised boss on the surface of the winding cylinder at the Z position
// corresponding to turn N. Boss is located at 180° (directly opposite wiper
// groove) to keep it out of the winding path.
// =============================================================================
module turn_mark(turn_num, label_comment = "") {
    z = turn_z(turn_num);
    rotate([0, 0, 180])          // opposite side from wiper groove
    translate([form_od / 2, 0, z])
    rotate([0, 90, 0])           // boss points radially outward
        cylinder(d = boss_d, h = boss_h);
}

// =============================================================================
// MODULE: coil_form
// Main body: winding cylinder + two flanges + keyed flat + wiper groove.
// =============================================================================
module coil_form() {
    difference() {
        union() {
            // ── Winding cylinder ────────────────────────────────────────────
            cylinder(d = form_od, h = total_l);

            // ── Bottom flange ────────────────────────────────────────────────
            cylinder(d = flange_od, h = flange_t);

            // ── Top flange ──────────────────────────────────────────────────
            translate([0, 0, flange_t + wind_l])
                cylinder(d = flange_od, h = flange_t);

            // ── Turn mark bosses ─────────────────────────────────────────────
            // Band allocations (comments for reference):
            turn_mark(1);    // reference — 1 turn from bottom
            turn_mark(2);    // reference
            turn_mark(3);    // reference
            turn_mark(4);    // tap point — coax feed at 4 turns
            turn_mark(6);    // 10m band lower limit (approx)
            turn_mark(10);   // 12m / 15m crossover
            turn_mark(15);   // 17m band
            turn_mark(24);   // 20m band
            turn_mark(41);   // 40m band (near top of form)
        }

        // ── Center bore: 1/4-20 rod clearance ───────────────────────────────
        translate([0, 0, -0.1])
            cylinder(d = form_id, h = total_l + 0.2);

        // ── Anti-rotation flat (keyed groove) — full length, at 0° ──────────
        // A flat-bottomed slot running the full Z length of the form.
        translate([form_od / 2 - flat_d, -flat_w / 2, -0.1])
            cube([flat_d + 1, flat_w, total_l + 0.2]);

        // ── Wiper registration groove — full length, at 90° ─────────────────
        rotate([0, 0, wiper_angle])
        translate([form_od / 2 - groove_d, -groove_w / 2, -0.1])
            cube([groove_d + 1, groove_w, total_l + 0.2]);

        // ── Coax tap hole through bottom flange, at tap_angle (0°) ──────────
        rotate([0, 0, tap_angle])
        translate([flange_od / 2 - 12, 0, -0.1])
            cylinder(d = tap_hole_d, h = flange_t + 0.2);

        // ── Lightening holes — bottom flange ────────────────────────────────
        lightening_holes(0, flange_t);

        // ── Lightening holes — top flange ───────────────────────────────────
        lightening_holes(flange_t + wind_l, flange_t);

        // ── Wire routing slots ───────────────────────────────────────────────
        wire_holes();
    }
}

// =============================================================================
// RENDER
// Print orientation: upright (Z axis = long axis of form).
// Fits 105mm tall × 85mm dia on print bed.
// =============================================================================
coil_form();

// =============================================================================
// ASSEMBLY NOTES
// 1. Print upright with supports in bore only (or use a 7.5mm drill to clean).
// 2. Winding starts at bottom flange wire slot. Wind #18 AWG enamel wire
//    close-wound toward top flange. End through top flange wire slot.
// 3. Coax center conductor taps through bottom flange tap hole at turn 4 mark.
// 4. Wiper contact bracket pins into the 90° registration groove.
// 5. Anti-rotation flat at 0° mates with alignment rib inside motor_base.scad.
// 6. 1/4-20 threaded rod passes through 7.5mm center bore.
//    Bore may be chased with 5/16" drill if needed for smooth fit.
// =============================================================================
