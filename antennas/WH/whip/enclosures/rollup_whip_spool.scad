// ============================================================
// TM-WHP-ENC-003 Rev A
// Roll-up Whip Wire Storage Spool
// 5.6m Telescopic Whip Antenna Project
// ============================================================
// PURPOSE:
//   Two-piece split spool stores ~6 m of #18 AWG insulated wire
//   (approx 1.5 mm OD) plus field accessories:
//     - Trap-side flange  : 6 trap housing clip recesses
//     - Coil-side flange  : 5 loading coil module storage slots
//                           (160m / 80m / 60m / 40m / 30m)
//   The two halves bolt together with M3 × 16 socket-head screws
//   through the hub.  A handle with 10 mm hanging hole is
//   moulded onto the hub OD.
//
// PRINT SETTINGS (PETG):
//   Layer height  : 0.25 mm
//   Perimeters    : 3
//   Infill        : 20 % gyroid
//   Supports      : none required
//   Orientation   : print each half flat (flange face-down)
//   NOTE: Both halves fit comfortably on a 200 × 200 mm bed.
//         Flange OD = 170 mm — orient centrally on bed.
//
// WIRE CAPACITY NOTE:
//   Hub groove volume ≈ π/4 × (hub_od² − spool_core²) × hub_h
//   With hub_od = 65 mm, core ≈ 25 mm, hub_h = 50 mm:
//   ≈ 160 cm³  → comfortably stores ~7 m of #18 AWG (1.5 mm OD
//   with insulation) at 60 % packing density.
//
// HARDWARE:
//   4 × M3 × 16 socket-head cap screws + M3 hex nuts
//   (nuts seat in hex recesses in hub interior faces)
//
// ASSEMBLY:
//   1. Print two halves.
//   2. Wind wire onto hub of one half.
//   3. Mate halves; press M3 hex nuts into recesses.
//   4. Thread M3 × 16 screws through hub flanges and tighten.
//   5. Clip trap housings into trap-side flange recesses.
//   6. Snap coil modules into coil-side slot recesses.
// ============================================================

$fn = 64;

// ── Primary spool dimensions ──────────────────────────────────
flange_od  = 170;   // flange outer diameter
flange_t   = 7;     // flange thickness
hub_od     = 65;    // hub outer diameter (wire wraps here)
hub_h      = 50;    // hub height (= wire winding width)
wall       = 3.0;

// Hub spoke core (axle cylinder inside hub)
core_od    = 25;    // solid inner core of hub

// ── Trap housing clip recesses (trap-side flange) ─────────────
n_trap     = 6;
trap_w     = 35;    // recess X-dimension (trap OD + clearance)
trap_d     = 28;    // recess Y-dimension
trap_h     = 5;     // recess depth

// ── Loading coil module slots (coil-side flange) ─────────────
n_coil     = 5;
coil_d     = 30;    // circular slot diameter (coil OD + clearance)
coil_h     = 5;     // slot depth
coil_labels = ["160m", "80m", "60m", "40m", "30m"];

// ── Handle ────────────────────────────────────────────────────
handle_w       = 60;    // handle length (spans hub OD + overhang)
handle_h       = 25;    // handle height
handle_t       = 8;     // handle thickness
handle_hole_d  = 10;    // hanging hole diameter

// ── M3 hub join bolts ─────────────────────────────────────────
n_bolts        = 4;
bolt_pcd       = (hub_od + core_od) / 2;  // bolt circle PCD
bolt_d         = 3.2;   // M3 clearance
nut_d          = 6.4;   // M3 hex nut flat-to-flat + 0.2 mm
nut_h          = 2.5;   // M3 hex nut height + 0.2 mm

// ── Wire anchor notch ─────────────────────────────────────────
notch_w  = 4;
notch_h  = 8;
notch_d  = 3;

// ── Label emboss ──────────────────────────────────────────────
label_depth = 0.6;
label_size  = 5;

// ==============================================================
// MODULE: hub
//   Solid-walled cylinder forming the wire winding core.
//   Includes M3 bolt holes and one wire anchor notch.
//   This module produces ONE half of the hub (half of hub_h).
// ==============================================================
module hub(nut_side = false) {
    half_h = hub_h / 2;
    difference() {
        // Hub wall
        cylinder(d = hub_od, h = half_h);

        // Hollow interior (thicker wall at core)
        translate([0, 0, wall])
            cylinder(d = hub_od - 2*wall, h = half_h);

        // Core axle bore
        cylinder(d = core_od, h = half_h + 0.1);

        // M3 bolt holes at bolt_pcd
        for (i = [0 : n_bolts - 1]) {
            rotate([0, 0, i * (360 / n_bolts) + 45])
            translate([bolt_pcd / 2, 0, 0]) {
                cylinder(d = bolt_d, h = half_h + 0.1);
                if (nut_side) {
                    // Hex nut recess on mating face
                    translate([0, 0, half_h - nut_h - 0.1])
                        cylinder(d = nut_d, h = nut_h + 0.2, $fn = 6);
                }
            }
        }

        // Wire anchor notch on hub OD
        translate([hub_od/2 - notch_d, -notch_w/2, half_h - notch_h])
            cube([notch_d + 0.1, notch_w, notch_h + 0.1]);
    }
}

// ==============================================================
// MODULE: flange
//   Single flange disc.  trap_side=true adds trap clip recesses
//   on the outer face; trap_side=false adds coil slot recesses.
//   Hub join hole is always present.
// ==============================================================
module flange(trap_side = true) {
    difference() {
        union() {
            // Main disc
            cylinder(d = flange_od, h = flange_t);

            // Peripheral rim lip (keeps wire on spool)
            difference() {
                cylinder(d = flange_od, h = flange_t + 3);
                cylinder(d = flange_od - 4, h = flange_t + 3.1);
            }
        }

        // Hub bore (hub sits flush with inner flange face)
        cylinder(d = hub_od + 0.4, h = flange_t + 0.1);

        // Apply feature recesses to outer face
        if (trap_side) {
            translate([0, 0, flange_t - trap_h])
                trap_clip_pattern();
        } else {
            translate([0, 0, flange_t - coil_h])
                coil_slot_pattern();
        }
    }
}

// ==============================================================
// MODULE: trap_clip_pattern
//   6 equally-spaced rectangular recesses around the flange
//   face, sized for trap housing clips.
// ==============================================================
module trap_clip_pattern() {
    r_centre = flange_od / 2 - trap_w / 2 - 6; // radial position
    for (i = [0 : n_trap - 1]) {
        rotate([0, 0, i * (360 / n_trap)])
        translate([r_centre, -trap_d/2, 0])
            cube([trap_w, trap_d, trap_h + 0.1]);
    }
}

// ==============================================================
// MODULE: coil_slot_pattern
//   5 equally-spaced circular recesses on flange face for
//   loading coil module storage.  Labels embossed between slots.
// ==============================================================
module coil_slot_pattern() {
    r_centre = flange_od / 2 - coil_d / 2 - 8;
    for (i = [0 : n_coil - 1]) {
        angle = i * (360 / n_coil);
        rotate([0, 0, angle])
        translate([r_centre, 0, 0]) {
            // Circular slot
            cylinder(d = coil_d, h = coil_h + 0.1);

            // Label embossed at depth (raised above recess floor)
            translate([0, 0, coil_h - label_depth])
                linear_extrude(height = label_depth + 0.1)
                    text(coil_labels[i],
                         size   = label_size,
                         halign = "center",
                         valign = "center",
                         font   = "Liberation Sans:style=Bold");
        }
    }
}

// ==============================================================
// MODULE: handle
//   Rectangular handle moulded onto hub OD, extending outward.
//   10 mm hanging hole centred in handle.
// ==============================================================
module handle() {
    translate([-handle_w/2, hub_od/2 - 2, 0]) {
        difference() {
            // Handle block
            cube([handle_w, handle_t, handle_h]);

            // Hanging hole
            translate([handle_w/2, handle_t/2, handle_h/2])
                rotate([90, 0, 0])
                    cylinder(d = handle_hole_d, h = handle_t + 0.2);

            // Chamfer edges of hanging hole (cosmetic)
            translate([handle_w/2, handle_t/2, handle_h/2])
                rotate([90, 0, 0])
                    cylinder(d1 = handle_hole_d + 3,
                             d2 = handle_hole_d,
                             h  = 2);
        }
    }
}

// ==============================================================
// MODULE: half_spool
//   One complete half: inner flange + hub half + optional handle.
//   Set trap_side=true for the trap-clip flange half;
//   trap_side=false for the coil-module flange half.
//   The hub half with nut_side=true is the coil side (receives
//   bolt from trap side).
// ==============================================================
module half_spool(trap_side = true) {
    nut_s = !trap_side;  // nut recesses go on coil side

    union() {
        // Flange
        flange(trap_side = trap_side);

        // Hub half (sits on inner face of flange)
        translate([0, 0, flange_t])
            hub(nut_side = nut_s);

        // Handle — only on trap-side half for ergonomics
        if (trap_side) {
            translate([0, 0, flange_t + hub_h/2 - handle_h/2])
                handle();
        }
    }
}

// ==============================================================
// MAIN RENDER
//   Both halves side by side for bed layout preview.
//   Left  = trap-side half  (print flange face-down)
//   Right = coil-side half  (print flange face-down, mirrored)
// ==============================================================

// Trap-side half — left position
translate([-(flange_od/2 + 5), 0, 0])
    half_spool(trap_side = true);

// Coil-side half — right position, mirrored about Z so flange
// is face-down on the print bed without overhanging the hub
translate([flange_od/2 + 5, 0, flange_t + hub_h/2])
    mirror([0, 0, 1])
        half_spool(trap_side = false);

// ── End of TM-WHP-ENC-003 ────────────────────────────────────
