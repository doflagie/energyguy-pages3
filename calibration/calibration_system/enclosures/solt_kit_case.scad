/**
 * solt_kit_case.scad — TM-CAL-ENC-003 Rev A
 * SOLT Calibration Kit Case
 *
 * Holds:
 *   - 4× SMA(M) calibration standards (SHORT, OPEN, LOAD, THRU)
 *   - 6× SMA inline attenuator pads (3/6/10/20/30/40 dB)
 *   - 2× SMA-SMA barrel adapters
 *   - 2× BNC-SMA adapters
 *   - 3× SMA cables (150mm)
 *   - 1× Laminated calibration reference card
 *
 * Design: hinged clamshell, foam-lined interior, label window on lid
 * Envelope: 160×100×35 mm (two halves: 160×100×17.5 each)
 */

// ── Parameters ──────────────────────────────────────────────────────────────
box_w  = 160.0;
box_d  = 100.0;
half_h = 17.0;   // each half height
wall   = 2.5;
hinge_d = 4.0;
hinge_pin_d = 2.5;
r      = 4.0;
$fn    = 32;

// SMA standard pocket: 14mm dia × 22mm deep
std_d  = 14.5;
std_h  = 23.0;

// Attenuator module pocket: 11mm dia × 18mm deep (SMA-SMA in-line style)
att_d  = 11.5;
att_h  = 18.0;

// Cable coil pocket (Ø30mm × 12mm deep for 150mm cable coiled)
cable_d = 32.0;
cable_h = 13.0;

// Card pocket: 90mm × 65mm × 2mm
card_w  = 92.0;
card_d  = 67.0;
card_h  = 2.5;

// Label window in lid
label_w = 80.0;
label_d = 40.0;

// Latch (simple over-center snap on front edge)
latch_w = 30.0;
latch_h = 6.0;

module rounded_box_2d(w, d, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) circle(r=r_, $fn=16);
}

module half_shell() {
    difference() {
        // Outer shell
        linear_extrude(half_h)
            rounded_box_2d(box_w, box_d, r);
        // Interior pocket
        translate([wall, wall, wall])
            linear_extrude(half_h)
                rounded_box_2d(box_w-2*wall, box_d-2*wall, r-wall+0.5);
    }
}

// ── Bottom Half (contains all component pockets) ───────────────────────────
module bottom_half() {
    difference() {
        union() {
            half_shell();
            // Lip for clamshell alignment
            translate([wall+1, wall+1, half_h])
                linear_extrude(2.5)
                    rounded_box_2d(box_w-2*(wall+1), box_d-2*(wall+1), r-wall);
        }

        // ── SMA STANDARD POCKETS (4× row at back) ─────────────────────
        for (i=[0:3])
            translate([16 + i*36, box_d-30, wall])
                cylinder(h=std_h+1, d=std_d, $fn=24);

        // Label recess inside each standard pocket (engraved marker ring)
        // Colors: RED=SHORT, GREEN=OPEN, BLUE=LOAD, NATURAL=THRU
        // Label text embossed on exterior of pocket lip:
        for (i=[0:3]) {
            translate([16 + i*36, box_d-30, wall + std_h - 2])
                cylinder(h=2.5, d=std_d+3, $fn=24);  // recess ring for ID band
        }

        // ── ATTENUATOR POCKETS (6× row in middle) ─────────────────────
        for (i=[0:5])
            translate([14 + i*25, box_d-60, wall])
                cylinder(h=att_h+1, d=att_d, $fn=24);

        // ── CABLE COIL POCKETS (3× left area) ─────────────────────────
        for (i=[0:2])
            translate([18 + i*36, 20, wall])
                cylinder(h=cable_h+1, d=cable_d, $fn=32);

        // ── ADAPTER POCKET (BNC-SMA, SMA-SMA: 2× each) ────────────────
        translate([130, 15, wall])
            cube([25, 15, 10]);
        translate([130, 35, wall])
            cube([25, 15, 10]);

        // ── CARD SLOT (right side, shallow) ───────────────────────────
        translate([(box_w - card_w)/2, wall + 3, half_h - card_h - 1])
            cube([card_w, card_d, card_h + 1]);
    }
}

// ── Top Half (lid) ─────────────────────────────────────────────────────────
module top_half() {
    difference() {
        half_shell();
        // Receive the lip from bottom half
        translate([wall+1+0.4, wall+1+0.4, 0])
            linear_extrude(2.8)
                rounded_box_2d(box_w-2*(wall+1)-0.8, box_d-2*(wall+1)-0.8, r-wall-0.3);
        // Label window
        translate([(box_w-label_w)/2, (box_d-label_d)/2, half_h-1.5])
            cube([label_w, label_d, 2]);
    }
    // Raised label text inside window
    translate([(box_w-label_w)/2+4, (box_d-label_d)/2+6, half_h-1.0])
        linear_extrude(1.2)
            text("SOLT CAL KIT", size=6, font="Liberation Sans:style=Bold");
    translate([(box_w-label_w)/2+4, (box_d-label_d)/2+16, half_h-1.0])
        linear_extrude(1.2)
            text("SHORT  OPEN  LOAD  THRU", size=4, font="Liberation Mono");
}

// ── Pocket label tabs (print separately; press-fit into pocket lip) ─────────
module std_label(txt, col) {
    // Small disc that sits on top of each pocket
    difference() {
        cylinder(h=3, d=std_d+2, $fn=24);
        cylinder(h=3.5, d=std_d, $fn=24);  // hollow center
    }
    translate([-std_d/2+1, -2, 0])
        linear_extrude(3.5)
            text(txt, size=4, font="Liberation Sans:style=Bold");
}

// ── Render ─────────────────────────────────────────────────────────────────
print_part = "bottom";  // "bottom" | "top" | "labels"

if (print_part == "bottom") {
    color("sandybrown", 0.9) bottom_half();
} else if (print_part == "top") {
    color("peru", 0.9) top_half();
} else {
    // Print 4 label rings (filament change for color would be ideal)
    color("red",   0.9) translate([0, 0, 0])  std_label("S", "red");
    color("green", 0.9) translate([20, 0, 0]) std_label("O", "green");
    color("blue",  0.9) translate([40, 0, 0]) std_label("L", "blue");
    color("gray",  0.9) translate([60, 0, 0]) std_label("T", "gray");
}

// ── Assembly Notes ──────────────────────────────────────────────────────────
// 1. Print bottom_half and top_half in ASA or PETG (durable, field use).
// 2. Cut foam inserts (craft foam or packaging foam):
//    - 3mm base layer for cable coil pockets
//    - Thin ring pads for attenuator pockets
//    Standards pockets and cable pockets do not need foam (snug fit designed in).
// 3. Construct SOLT standards (see TM-CAL-SCH-005 for construction details):
//    SHORT: solder center pin to GND inside SMA(M)
//    OPEN:  leave center pin open; seal with hot glue to prevent contamination
//    LOAD:  2× 100Ω 0402 in parallel between center and GND
//    THRU:  SMA(M)-SMA(M) barrel adapter (purchase)
// 4. Mark each standard with colored dot (nail polish or paint):
//    RED = SHORT, GREEN = OPEN, BLUE = LOAD, no mark = THRU
// 5. Print label rings (labels option) in matching colors if multi-filament
//    printer available; otherwise use adhesive colored labels.
// 6. Laminate CAL_CARD reference sheet (see manual Appendix D) and store in slot.
// 7. Snap lid closed; test latching. Add velcro strip on front edge if snap
//    is insufficient.
