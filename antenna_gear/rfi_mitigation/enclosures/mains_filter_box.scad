/**
 * mains_filter_box.scad — TM-RFI-ENC-001 Rev A
 * AC Mains EMI Line Filter Enclosure
 *
 * SAFETY: This enclosure operates at mains voltage.
 * Print in PETG (heat-resistant, chemical-resistant).
 * Use heavy-walled construction (3mm minimum).
 * All internal surfaces accept copper tape for shielding.
 * Mount on grounded metal surface or install in metal outer box.
 *
 * Features:
 *   - 130×80×55 mm body (fits PCB 120×70 mm)
 *   - IEC C14 inlet cutout (mains input)
 *   - 4× IEC C13 outlet holes (shack equipment)
 *   - Ground terminal hole (chassis earth)
 *   - PCB standoffs for filter PCB
 *   - Solid walls (no vents — mains voltage inside)
 *   - Snap-fit lid + M3 security screws
 *   - Mains voltage warning label recess on top
 */

// ── Parameters ─────────────────────────────────────────────────────────────
box_w   = 180.0;   // X — enough for 4× C13 outlets
box_d   = 90.0;    // Y
box_h   = 60.0;    // Z total
wall    = 3.5;     // heavy wall (mains safety)
floor_h = 4.0;     // thick floor
lid_h   = 12.0;
body_h  = box_h - lid_h;
r = 5.0;
$fn = 32;

// IEC C14 inlet: 48×23 mm opening
iec_w  = 48.0;
iec_h  = 23.0;

// IEC C13 outlet: 25×17 mm opening
c13_w  = 25.0;
c13_h  = 17.0;

// Ground terminal: 6.5mm hole (M6 bolt)
gnd_d  = 6.5;

// PCB dims
pcb_w  = 140.0;
pcb_d  = 70.0;
pcb_so_h = 5.0;
pcb_so_od = 7.0;
pcb_so_id = 2.7;

// Label recess
label_w = 60.0;
label_h = 15.0;
label_depth = 0.8;

// Lid screws
screw_d = 3.3;

module rounded_box(w, d, h, r_) {
    hull()
        for (x=[r_, w-r_]) for (y=[r_, d-r_])
            translate([x, y, 0]) cylinder(h=h, r=r_, $fn=24);
}

module standoff(x, y) {
    translate([x, y, floor_h])
        difference() {
            cylinder(h=pcb_so_h, d=pcb_so_od, $fn=16);
            translate([0,0,-0.5]) cylinder(h=pcb_so_h+1, d=pcb_so_id, $fn=16);
        }
}

module body() {
    difference() {
        rounded_box(box_w, box_d, body_h, r);

        // Inner cavity
        translate([wall, wall, floor_h])
            rounded_box(box_w-2*wall, box_d-2*wall, body_h, r-wall+0.5);

        // ── REAR PANEL (y=0): IEC C14 mains inlet ─────────────────────
        // Centered, at mid-height
        translate([(box_w - iec_w)/2, -0.5, floor_h + (body_h - floor_h - iec_h)/2])
            cube([iec_w, wall+1, iec_h]);

        // ── FRONT PANEL (y=box_d): 4× IEC C13 outlets ─────────────────
        for (i=[0:3])
            translate([12 + i*(c13_w + 8), box_d+0.5,
                       floor_h + (body_h - floor_h - c13_h)/2])
                cube([c13_w, wall+1, c13_h]);

        // ── RIGHT SIDE: Ground terminal ────────────────────────────────
        translate([box_w+0.5, box_d/2, floor_h + 10])
        rotate([0, 90, 0])
            cylinder(h=wall+1, d=gnd_d, $fn=16);

        // ── Lid screw holes ─────────────────────────────────────────────
        for (x=[10, box_w-10]) for (y=[10, box_d-10])
            translate([x, y, body_h-6])
                cylinder(h=7, d=screw_d, $fn=16);
    }

    // PCB standoffs
    ox = (box_w - pcb_w) / 2;
    oy = (box_d - pcb_d) / 2;
    standoff(ox + 4,       oy + 4);
    standoff(ox + pcb_w-4, oy + 4);
    standoff(ox + 4,       oy + pcb_d-4);
    standoff(ox + pcb_w-4, oy + pcb_d-4);
    standoff(ox + pcb_w/2, oy + 4);      // center support
    standoff(ox + pcb_w/2, oy + pcb_d-4);

    // Corner screw bosses
    for (x=[10, box_w-10]) for (y=[10, box_d-10])
        translate([x, y, 0]) cylinder(h=body_h, d=8, $fn=16);
}

module lid() {
    fl_d = 4.0;
    plate_h = lid_h - fl_d;
    difference() {
        union() {
            rounded_box(box_w, box_d, plate_h, r);
            translate([wall+1, wall+1, plate_h])
                rounded_box(box_w-2*(wall+1), box_d-2*(wall+1), fl_d, r-wall);
        }
        // Screws
        for (x=[10, box_w-10]) for (y=[10, box_d-10])
            translate([x, y, -0.5])
                cylinder(h=lid_h+1, d=screw_d, $fn=16);
        // Warning label recess (embossed area)
        translate([(box_w - label_w)/2, (box_d - label_h)/2, -0.5])
            cube([label_w, label_h, label_depth + 0.5]);
    }
    // Text raised inside label recess — print with filament change at layer:
    // "MAINS VOLTAGE - HAZARDOUS"
    translate([(box_w - label_w)/2 + 4, (box_d - label_h)/2 + 3, plate_h - label_depth])
        linear_extrude(label_depth + 0.5)
            text("MAINS VOLTAGE", size=3.5, font="Liberation Sans:style=Bold");
}

// ── Assembly Notes ──────────────────────────────────────────────────────────
// 1. Print body and lid in PETG at 40% infill minimum.
// 2. Line ALL interior surfaces with copper tape (walls, floor, lid interior).
// 3. SAFETY: Install PCB in box. Wire IEC C14 → filter → C13 outlets.
//    All mains connections with properly rated wire and ferrules.
// 4. Earth ground: M6 bolt through ground terminal hole connects to safety earth.
//    Also connect safety earth to copper tape lining via ring terminal.
// 5. Lock lid with M3 screws. Apply "MAINS VOLTAGE" warning sticker.
// 6. Mount finished box on power strip or bench surface.
//    DO NOT OPEN WHILE ENERGIZED.

print_part = "body";  // "body" | "lid"

if (print_part == "body") {
    color("ivory", 0.9) body();
} else {
    color("beige", 0.9) lid();
}
